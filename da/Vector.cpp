/*! \file Vector.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes standard
#include <cstring>

// Includes suri
#include "suri/Vector.h"
#include "VectorFactory.h"
#include "logmacros.h"
#include "suri/Wkt.h"
#include "suri/CoordinatesTransform.h"
#include "suri/Subset.h"
#include "suri/Extent.h"
#include "SpatialReference.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/ExactCoordinatesTransformation.h"

// Includes otros
#include "ogrsf_frmts.h"
#include "ogr_featurestyle.h"
#include "ogr_feature.h"

// Defines
/** Cantidad de tipos de vectores diferentes */
#define VECTOR_TYPES_COUNT 5
/** String con los tipos de vectores */
std::string VectorTypes[] = { message_UNDEFINED, message_POINT, message_LINE,
                              message_POLYGON, message_GEOM_COLLECTION };  // No
// se
// traducen,
// elemento
// de
// xml
#define BUFFER_ORIGIN_FIELD_NAME "Origen"
#define BUFFER_DISTANCE_FIELD_NAME "DistBufer"
#define BUFFER_ID_FIELD_NAME "ID"

/** namespace suri */
namespace suri {
/** propiedades basicas */
Vector::OptionsIdMapType Vector::optionsIds_;

// ------------------------------------------------------------------------------
/** Inicializa las clases derivadas */
/**
 *  Inicializa las factorias de bandas e imagenes
 */
void InitializeVectors() {
   // Garantizo una sola inicializacion
   static bool isinitialized = false;
   if (isinitialized) {
      return;
   }
   isinitialized = true;
   // Registra todas las clases imagen
   RegisterAllVectors();
}

// ------------------------------------------------------------------------------
/** Ctor */
Vector::Vector() :
      pVectorDataSource_(NULL) {
}

// ------------------------------------------------------------------------------
/** Dtor */
Vector::~Vector() {
   for (std::set<OGRLayer *>::iterator it = createdSqlLayers_.begin();
         it != createdSqlLayers_.end(); it++)
      pVectorDataSource_->ReleaseResultSet(*it);

   OGRDataSource::DestroyDataSource(pVectorDataSource_);
   pVectorDataSource_ = NULL;
}

// ------------------------------------------------------------------------------
// ------------------------ METODOS STATICOS ------------------------
/**
 * Funcion que permite registrar una nueva clase vector
 * @param[in] ClassId id de la nueva clase de vector
 * @param[in] CreateFunction funcion que se usa para crear el nuevo tipo de
 * vector
 */
void Vector::RegisterVector(std::string ClassId, Vector* (*CreateFunction)()) {
   VectorFactory &factory = TheVectorFactory::Instance();
   factory.RegisterClass(ClassId, CreateFunction);
   Vector *pvector = CreateFunction();
   pvector->InitializeClass();
   delete pvector;
}

// ------------------------------------------------------------------------------
/**
 * @param[in] Filename ruta del vector que se quiere abrir
 * @param[in] VectorAccess tipo de acceso que se espera del vector. Por default
 * es ReadOnly
 * @param[in] Layers cantidad de capas
 * @param[in] Type tipo de vector
 * \todo Cargar tabla de estilos con el VectorStyleManager
 * \todo Definir en algun lado las constantes hardcodeadas
 */
Vector* Vector::Open(const std::string &Filename, VectorAccessType VectorAccess,
                     int Layers, VectorType Type) {
   VectorFactory &factory = TheVectorFactory::Instance();
   VectorFactory::IdentifierVector::iterator it = factory.GetClassIds().begin();
   while (it != factory.GetClassIds().end()) {
      // no deberia tirar ninguna excepcion ya que usa los strings propios
      Vector *pvector = TheVectorFactory::Instance().CreateClass(*it);

      // guarda el nombre del archivo
      pvector->SetOption(Vector::Filename, Filename);
      if (pvector->Inspect(Filename, VectorAccess, Layers, Type)) {
         pvector = pvector->Load(Filename, VectorAccess, Layers, Type);
         if (!pvector || !pvector->GetVectorDataSource()) {
            Close(pvector);
            return pvector;
         }
         // Intenta cargar el tipo de archivo
         if (pvector->GetVectorDataSource()->GetDriver()) {
            pvector->SetOption(Format,
                               pvector->GetVectorDataSource()->GetDriver()->GetName());
         }

         // Intenta cargar la tablas de estilos provenientes de archivo
         if (pvector->GetVectorDataSource()->GetStyleTable()) {
            // OGRStyleTable * pstyletable = pvector->GetVectorDataSource()->GetStyleTable();
            // \todo Cargar tabla de estilos con el VectorStyleManager
         }

         // Determina las capacidades de la fuente de datos vectoriales
         // \todo Definir en algun lado las constantes hardcodeadas
         suri::Wkt *pcapswkt = suri::Wkt::Create("CAPS[\"Vector Source Capabilites\"]");
         // Creacion de capas nuevas
         if (VectorAccess != ReadOnly) {
            if (pvector->GetVectorDataSource()->TestCapability(ODsCCreateLayer)) {
               pcapswkt->InsertValue("New Layers Creation Alowed", "CAPS|CREATELAYER");
            }
         }
         pvector->SetOption(Capabilities, pcapswkt->GetWktString());
         suri::Wkt::Destroy(pcapswkt);
         return pvector;
      }
      Close(pvector);
      it++;
   }
   REPORT_AND_FAIL_VALUE("D:No se pudo identificar el formato del vector (%s).", NULL,
                         Filename.c_str());
}

// ------------------------------------------------------------------------------
/**
 * Elimina el vector y deja la referencia en NULL.
 * @param[in] pVector vector que se desea eliminar
 */
void Vector::Close(Vector* &pVector) {
   delete pVector;
   pVector = NULL;
}

/**
 * Inicializa las clases
 * @return informa si tubo exito.
 */
bool Vector::Init() {
   InitializeVectors();
   return true;
}

// ------------------------------------------------------------------------------
// ------------------- ESCRITURA A VECTOR EXTERNO -------------------
/** Salva el vector en un formato dado con las opciones dadas */
/**
 * @param[in] Filename ruta donde se quiere salvar el vector
 * @param[in] Format formato del archivo de salida
 * @param[in] WriteOptions opciones.
 * \TODO(Javier - Sin TCK): "Inspirarse" en ogr2ogr.cpp para salvar con todos los chiches
 */
bool Vector::Save(const std::string &Filename, const std::string &Format,
                  const OptionsMapType &WriteOptions) {
   if (Filename == GetOption(Vector::Filename)) {
      return false;
   }
   OGRSFDriver *pdriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
         Format.c_str());
   if (!pdriver->TestCapability(ODrCCreateDataSource)) {
      return false;
   }

   // Si el vector ya existe hay que borrarlo, caso contrario falla CopyDataSource
   OGRDataSource *ptempdatasource = OGRSFDriverRegistrar::Open(Filename.c_str());
   if (ptempdatasource) {
      OGRDataSource::DestroyDataSource(ptempdatasource);
      if (!pdriver->TestCapability(ODrCDeleteDataSource)) {
         return false;
      }
      if (pdriver->DeleteDataSource(Filename.c_str()) != OGRERR_NONE) {
         return false;
      }
   }

   OGRDataSource *pdatasource = pdriver->CopyDataSource(pVectorDataSource_,
                                                        Filename.c_str());
   if (!pdatasource) {
      return false;
   }
   bool retval = true;
   if (pdatasource->SyncToDisk() != OGRERR_NONE) {
      retval = false;
   }
   OGRDataSource::DestroyDataSource(pdatasource);
   return retval;
}

/**
 * Copia los datos al vector
 * @param[in] pTarget vector donde se quiere compiar los datos.
 * @return true La copia ha resultado exitosa
 * @return false Hubo un error al copiar.
 */
bool Vector::Copy(Vector *pTarget) {
   if (!pTarget || !pTarget->GetVectorDataSource()) {
      return false;
   }
   // obtengo el driver del destino
   OGRSFDriver *pdriver = pTarget->GetVectorDataSource()->GetDriver();
   if (!pdriver || !pdriver->TestCapability(ODrCCreateDataSource)) {
      return false;
   }
   // copio el datasource local usando el driver destino
   OGRDataSource *pdatasource = pdriver->CopyDataSource(
         pVectorDataSource_, pTarget->GetVectorDataSource()->GetName());
   // si no pudo copiar o flushear
   if (!pdatasource) {
      return false;
   }
   // asigno el driver del datasource
   pdatasource->SetDriver(pdriver);
   bool retval = true;
   if (pdatasource->SyncToDisk() != OGRERR_NONE) {
      retval = false;
   }
   pTarget->SetDataSource(pdatasource);
   return retval && pdatasource == pTarget->GetVectorDataSource();
}

/** Copia los datos al vector aplicando un filtro espacial dado por el subset que se pasa
 *  por parametro
 *  @param[out] pTarget vector donde se quiere compiar los datos.
 *  @param[in] SpatialFilter subset que funciona como filtro espacial
 *  @param[in] SpatialReference sistema de referencia de la capa copia
 *  @param[in] LayerIndex indice de la capa
 *  @return true La copia ha resultado exitosa
 *  @return false Hubo un error al copiar.*/
bool Vector::Copy(Vector *pTarget, const Subset& SpatialFilter,
                const std::string& SpatialReference, int LayerIndex) {
   if (!pTarget || !pTarget->GetVectorDataSource()) {
         return false;
   }
   OGRLayer* player = GetLayer(LayerIndex);
   if (!player)
      return false;
   OGRLayer* pdestlayer = pTarget->GetLayer(LayerIndex);
   if (pdestlayer) {
      pTarget->DeleteLayer(LayerIndex);
   }
   pdestlayer = pTarget->CreateLayer(player->GetName(),
                                     SpatialReference.c_str(),
                                     GetLayerType(LayerIndex));

   ParameterCollection params;
   char* psrwkt;
   player->GetSpatialRef()->exportToWkt(&psrwkt);
   std::string srin = psrwkt;
   OGRFree(psrwkt);
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,  srin);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, SpatialReference);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (pct == NULL) {
      REPORT_DEBUG("D:Bounding box imposible de calcular en coordenadas de mundo.");
      return false ;
   }
   OGRGeometry * pfilter = player->GetSpatialFilter();

#ifdef __SPATIAL_FILTER_ENABLE__
   Extent extent(SpatialFilter);
   /** TODO(Gabriel - TCK #7096): se comenta ya que si se utiliza el extent
    *  de la capa vectorial que se quiere exportar a veces el filtro espacial
    *  hace no se obtenga ninguna geometria
    */
   // TODO(Gabriel - TCK #8172): se desactiva el filtro espacial ya que hacia
   // que pueda encontrar las geometrias a exportar una capa vectorial
   OGRGeometry * pfilter = player->GetSpatialFilter();
   Extent extent(SpatialFilter);
   player->SetSpatialFilterRect(extent.min_.x_, extent.min_.y_, extent.max_.x_,
                                extent.max_.y_);
#endif
   // Copia la definicion de los campos a la capa destino
   OGRFeatureDefn* pfeaturedef = player->GetLayerDefn();
   for (int i = 0; i < pfeaturedef->GetFieldCount(); ++i) {
      pdestlayer->CreateField(pfeaturedef->GetFieldDefn(i));
   }
   player->ResetReading();
   OGRFeature *pfeature = player->GetNextFeature(); // Apunta al primer feature
   ExactCoordinatesTransformation* pctcoord =
         dynamic_cast<ExactCoordinatesTransformation*>(pct);
   while (pfeature) {
      OGRFeature* pdestfeature = pfeature->Clone();
      if (pctcoord != NULL && !pctcoord->IsIdentity() &&
            pctcoord->GetOGRCT(true) != NULL) {
         // si el sistema de salida es diferente al de entrada aplico la transformacion
         OGRGeometry* pgeom = pfeature->GetGeometryRef()->clone();
         pgeom->transform(pctcoord->GetOGRCT());
         pdestfeature->SetGeometryDirectly(pgeom);
      }
      pdestlayer->CreateFeature(pdestfeature);
      pfeature = player->GetNextFeature();
   }
   player->SetSpatialFilter(NULL);
   player->SetSpatialFilter(pfilter);
   return true;
}

OGRLayer* Vector::CopyLayer (int LayerIndex, const std::string& name) {
   OGRLayer* src = pVectorDataSource_->GetLayer(LayerIndex);
   OGRLayer* copy = pVectorDataSource_->CopyLayer(src, name.c_str());
   return copy;
}


// ------------------------------------------------------------------------------
// ---------------------------- METODOS NO ESTATICOS ---------------------------
/**
 * Acceso al DataSource de OGR
 * @return OGRDataSource interno de la clase
 * \attention El puntero retornado es responsabilidad de esta clase,no eliminar.
 */
OGRDataSource *Vector::GetVectorDataSource() const {
   return pVectorDataSource_;
}

// ------------------------------------------------------------------------------
/**
 * Obtiene la cuenta de capas
 * @return cantidad de capas del vector
 */
int Vector::GetLayerCount() const {
   return pVectorDataSource_->GetLayerCount();
}

// ------------------------------------------------------------------------------
/**
 * Acceso a la capa por indice
 * @param[in] LayerIndex indice de la capa
 * @return OGRLayer interno de vector
 * \attention El puntero retornado es responsabilidad de esta clase,no eliminar.
 */
OGRLayer *Vector::GetLayer(int LayerIndex) const {
   return pVectorDataSource_->GetLayer(LayerIndex);
}

// ------------------------------------------------------------------------------
/**
 * Devuelve el indice a la capa por nombre (-1 si falla)
 * @param[in] LayerName nombre de la capa
 * @return indice de la capa, -1 si no la encuentra
 */

int Vector::GetLayerIndex(const std::string &LayerName) const {
   OGRLayer * player = pVectorDataSource_->GetLayerByName(LayerName.c_str());
   if (player == NULL) {
      return -1;
   }
   for (int i = 0; i < GetLayerCount(); i++)
      if (player == GetLayer(i)) {
         return i;
      }
   return -1;
}

// ------------------------------------------------------------------------------
/**
 * Indica el tipo de capa
 * @param[in] LayerIndex indice de la capa
 * @return tipo de vectores que contiene la capa
 */
Vector::VectorType Vector::GetLayerType(const int LayerIndex) const {
   OGRLayer* player = GetLayer(LayerIndex);
   if (player != NULL) {
      OGRwkbGeometryType geomtype = player->GetLayerDefn()->GetGeomType();
      if (geomtype != wkbUnknown) {
         return TranslateType(geomtype);
      } else {
         return TranslateType(GuessGeomType(player));
      }
   } else {
      REPORT_ERROR("D:Vector: capa inexistente.");
   }
   return Vector::Undefined;
}

// ------------------------------------------------------------------------------
/**
 * Indica el Sistema de Referencia de la capa
 * @param[in] LayerIndex indice de la capa
 * @return El wkt que representa la referencia espacial del vector.
 * @return En caso de no tener, un wkt que representa coordenadas raster.
 */
std::string Vector::GetLayerSR(const int LayerIndex) const {
   OGRLayer * player = GetLayer(LayerIndex);
   if (player == NULL) {
      REPORT_ERROR("D:Vector: capa inexistente.");
      return std::string();
   }
   std::string spatialreference;
   if (player->GetSpatialRef() != NULL) {
      char *psrwkt;
      player->GetSpatialRef()->exportToWkt(&psrwkt);
      spatialreference = psrwkt;
      OGRFree(psrwkt);
   }
   if (spatialreference.empty()) {
      return std::string(LATLON_SPATIAL_REFERENCE);
   }
   return spatialreference;
}

// ------------------------------------------------------------------------------
/**
 * Crea una capa mediante comando SQL
 * @param[in] SQLStatement comando sql que se va a ejecutar
 * @param[in] pFilterGeometry geometria que representa un filtro espacial
 * @param[in] SQLDialect permite control del dialecto del comando
 * @return capa que contiene el resultado de la consulta
 */
OGRLayer *Vector::CreateSqlLayer(const std::string &SQLStatement,
                                 OGRGeometry *pFilterGeometry,
                                 const std::string &SQLDialect) {
   OGRLayer *player;
   if (SQLDialect.empty()) {
   player = pVectorDataSource_->ExecuteSQL(SQLStatement.c_str(), pFilterGeometry,
                                              NULL);
   } else {
      player = pVectorDataSource_->ExecuteSQL(SQLStatement.c_str(), pFilterGeometry,
                                              SQLDialect.c_str());
   }

   if (player != NULL) {
      createdSqlLayers_.insert(player);
   }

   return player;
}

// ------------------------------------------------------------------------------
/**
 * Destruye una capa mediante comando SQL
 * @param[in] pSQLLayer capa SQL
 * @return informa si pudo eliminar la capa
 */
bool Vector::DestroySqlLayer(OGRLayer * &pSQLLayer) {
   if (createdSqlLayers_.find(pSQLLayer) == createdSqlLayers_.end()) {
      return false;
   }

   pVectorDataSource_->ReleaseResultSet(pSQLLayer);
   createdSqlLayers_.erase(pSQLLayer);
   pSQLLayer = NULL;
   return true;
}

// ------------------------------------------------------------------------------
/**
 * Devuelve el tipo de Vector en String.
 * @param[in] Type tipo de vector que quiero retornar
 * @return string que representa a Type
 */
std::string Vector::GetVectorTypeAsString(const int Type) {
   if (Type >= 0 && Type < VECTOR_TYPES_COUNT) {
      return VectorTypes[Type];
   }
   return wxT("");
}

/**
 * Elimina la capa en la posicion indicada
 * @param[in] LayerName nombre de la capa que se quiere obtener
 * @return bool que informa resultado de operacion
 */
OGRLayer *Vector::GetLayer(const std::string &LayerName) const {
   return pVectorDataSource_->GetLayerByName(LayerName.c_str());
}

/**
 * Crea capa con nombre, ref espacial y tipo de vector indicados
 * @param[in] LayerName nombre de la capa que se quiere abrir
 * @param[in] SpatialReference referencia espacial de la capa que se quiere
 * abrir.
 * @param[in] VecType tipo de vectores que contendra la capa
 * @return int indice de la capa que se creo
 */
OGRLayer* Vector::CreateLayer(const std::string &LayerName,
                              const std::string &SpatialReference,
                              const Vector::VectorType VecType) {
   // Utilizo el VectorType para definir que tipo de vectores contendra la capa
   OGRwkbGeometryType ogrgeomtype = wkbPoint;
   switch (VecType) {
      case Point:
         ogrgeomtype = wkbPoint;
         break;
      case Line:
         ogrgeomtype = wkbLineString;
         break;
      case Polygon:
         ogrgeomtype = wkbPolygon;
         break;
      case GeometryCollection:
         ogrgeomtype = wkbGeometryCollection;
         break;
      case Undefined:
         ogrgeomtype = wkbUnknown;
         break;
      default:
         return NULL;
   }
   // Veo si la capa ya existe
   OGRLayer* newlayer = pVectorDataSource_->GetLayerByName(LayerName.c_str());
   OGRSpatialReference *pspatialref = new OGRSpatialReference(SpatialReference.c_str());
   if (newlayer) {
      // si la capa existe verifico que sus caracteristicas coincidan con los parametros
      if ((newlayer->GetSpatialRef())
            && (newlayer->GetSpatialRef()->IsSameGeogCS(pspatialref))
            && (newlayer->GetLayerDefn())
            && (newlayer->GetLayerDefn()->GetGeomType() == ogrgeomtype)) {
         REPORT_DEBUG("D: Ya existe una capa con este nombre");
      } else {
         newlayer = NULL;
         REPORT_DEBUG(
               "D: Ya existe una capa con este nombre y no coincide con los datos pasados");
      }
   } else {
      // Si no existe la creo
      newlayer = pVectorDataSource_->CreateLayer(LayerName.c_str(), pspatialref,
                                                 ogrgeomtype);
   }
   pspatialref->Release();
   return newlayer;
}

/**
 * Elimina la capa en la posicion indicada
 * @param[in] LayerIndex indice de la capa que se quiere eliminar
 * @return bool que informa resultado de operacion
 */
bool Vector::DeleteLayer(int LayerIndex) {
   return (pVectorDataSource_->DeleteLayer(LayerIndex) == OGRERR_NONE);
}

/**
 * Obtiene el indice de una capa
 * @param[in] pLayer referencia a la capa que se quiere encontrar
 * @return indice de la capa
 */
int Vector::GetLayerIndex(OGRLayer* pLayer) {
   if (!pLayer) {
      return -1;
   }
   // Busco el indice de la capa
   int layercount = pVectorDataSource_->GetLayerCount();
   int i = 0;
   int layer = -1;
   while ((layer < 0) && (i < layercount)) {
      OGRLayer* pogrlayer = GetLayer(i);
      if (strcmp(pogrlayer->GetName(), pLayer->GetName()) == 0) {
         layer = i;
      }
      i++;
   }
   return layer;
}

// ------------------------------------------------------------------------------
/**
 * Agrega o cambia opciones
 * @param[in] Id id de la opcion que quiero modificar
 * @param[in] Value nuevo valor
 */
void Vector::SetOption(const VectorOptionIdType Id, const std::string &Value) {
   SetOption(optionsIds_[Id], Value);
}

// ------------------------------------------------------------------------------
/**
 * Retorna el valor de una opcion
 * @param[in] Id id de la opcion que quiero consultar
 * @return valor que tiene la opcion
 */
std::string Vector::GetOption(const VectorOptionIdType Id) const {
   return GetOption(optionsIds_[Id]);
}

// ------------------------------------------------------------------------------

int Vector::GetFeatureCount(int LayerIndex, const std::string& filter) const {
   OGRLayer* pOgrLayer = GetLayer(LayerIndex);
   pOgrLayer->GetFeaturesRead();
   pOgrLayer->SetAttributeFilter(filter.c_str());
   int result = pOgrLayer->GetFeatureCount();
   pOgrLayer->ResetReading();
   return result;
}


/**
 * Inspecciona el archivo, para saber si es del formato que maneja
 * @param[in] Filename nombre del archivo
 * @param[in] VectorAccess tipo de acceso que quiero
 * @param[in] Layers cantidad de capas
 * @param[in] Type tipo de vector
 * @return informa si lo soporta
 */
bool Vector::Inspect(const std::string &Filename, VectorAccessType VectorAccess,
                     int Layers, VectorType Type) {
   bool retval = true;
   // trato de cargar el vector (asumo que OGRSFDriverRegistrar::Open retorna
   //  NULL en caso de falla)
   Load(Filename, VectorAccess, Layers, Type);
   // No puede manejar el vector o no tiene capas
   if (!pVectorDataSource_) {
      retval = false;
   }
   // destruyo
   OGRDataSource::DestroyDataSource(pVectorDataSource_);
   return retval;
}

// ------------------------------------------------------------------------------
/**
 * Carga el vector
 * @param[in] Filename nombre del archivo
 * @param[in] VectorAccess tipo de acceso que quiero
 * @param[in] Layers cantidad de capas
 * @param[in] Type tipo de vector
 * @return this
 */
Vector* Vector::Load(const std::string &Filename, VectorAccessType VectorAccess,
                     int Layers, VectorType Type) {
   // Abre segun el modo
   if (VectorAccess == ReadOnly) {
      pVectorDataSource_ = OGRSFDriverRegistrar::Open(Filename.c_str(), FALSE);
   } else {
      pVectorDataSource_ = OGRSFDriverRegistrar::Open(Filename.c_str(), TRUE);
      if (!pVectorDataSource_) {
         // Si no pudo abrir el archivo ve si lo puede crear
         OGRSFDriver *pdriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
               "ESRI Shapefile");
         pVectorDataSource_ = pdriver->CreateDataSource(Filename.c_str());
         if (pVectorDataSource_) {
            pVectorDataSource_->SetDriver(pdriver);
         }
      } else {
         REPORT_DEBUG("D: El archivo ya existia");
      }
   }
   return this;
}

/**
 * Asigna el datasource asociado
 * @param[in] pVectorDataSource nuevo data source
 */
void Vector::SetDataSource(OGRDataSource *pVectorDataSource) {
   if (pVectorDataSource_) {
      OGRDataSource::DestroyDataSource(pVectorDataSource_);
   }

   pVectorDataSource_ = pVectorDataSource;
}

// ------------------------------------------------------------------------------
/** Inicializacion en las clases derivadas, se llama al registrarlas */
void Vector::InitializeClass() {
   OGRRegisterAll();
   optionsIds_.insert(std::make_pair(Format, "Format"));
   optionsIds_.insert(std::make_pair(FormatDescription, "FormatDescription"));
   optionsIds_.insert(std::make_pair(StyleTable, "StyleTable"));
   optionsIds_.insert(std::make_pair(Capabilities, "Capabilities"));
}

/**
 * Trata de obtener el tipo de geometria de la capa basandose
 * en las caracteristica disponibles.
 */
OGRwkbGeometryType Vector::GuessGeomType(OGRLayer* pOgrLayer) const {
   OGRwkbGeometryType retval = wkbUnknown;
   OGRFeatureDefn* pfeaturedefn = pOgrLayer->GetLayerDefn();
   retval = pfeaturedefn->GetGeomType();
   if (retval == wkbUnknown) {
      retval = wkbNone;
      bool hasPoint = this->GetFeatureCount(0, "OGR_GEOMETRY='Point'");
      bool hasLineString = this->GetFeatureCount(0, "OGR_GEOMETRY='LineString'");
      bool hasPolygon = this->GetFeatureCount(0, "OGR_GEOMETRY='Polygon'");
      
      bool geomFound = hasPoint || hasLineString || hasPolygon;
      if (geomFound){
         retval = wkbGeometryCollection;
      }
   }
   return wkbFlatten(retval);
}

/**
 * Convierte el identificador de tipo de geometria de ORG al de SuriLib.
 */
Vector::VectorType Vector::TranslateType(OGRwkbGeometryType OrgType) {
   switch (OrgType) {
      case wkbMultiPolygon: case wkbPolygon: {
         return Vector::Polygon;
      }
      case wkbMultiLineString: case wkbLineString: case wkbLineString25D: {
         return Vector::Line;
      }
      case wkbMultiPoint: case wkbPoint: {
         return Vector::Point;
      }    
      case wkbGeometryCollection:{
         return Vector::GeometryCollection;
      }
      default: {
         REPORT_ERROR("D:Geometria desconocida. (%s).",
                      OGRGeometryTypeToName(static_cast<OGRwkbGeometryType>(OrgType)));
         break;
      }
   }
   return Vector::Undefined;
}

}
