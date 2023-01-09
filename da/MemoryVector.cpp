/*! \file MemoryVector.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MemoryVector.h"

// Includes standards
#include <sstream>

// Includes suri
#include "logmacros.h"

// Includes OGR
#include "ogrsf_frmts.h"

// Defines


/** namespace suri */
namespace suri {
/** Mapa que relaciona nombre de vectores con sus datasources */
MemoryVector::DatasourceCollectionType MemoryVector::loadedDataSources_;

/** Ctor */
MemoryVector::MemoryVector() {
}

/** Dtor */
MemoryVector::~MemoryVector() {
   ReleaseDataSource();
}

/**
 * Salva el vector en un formato dado con las opciones dadas.
 *
 * Si se trata de un archivo en memoria, realiza la operacion de copiado
 * sale, en caso contrario intenta con el metodo de la clase base.
 * @param[in] Filename Nombre de archivo sobre el cual salvar el vector.
 * @param[in] Format Formato a salvar.
 * @param[in] WriteOptions Opciones de formato.
 */
bool MemoryVector::Save(const std::string &Filename, const std::string &Format,
                        const OptionsMapType &WriteOptions) {
   if (!IS_MEMORY_VECTOR(Filename)) {
      return Vector::Save(Filename, Format, WriteOptions);
   }
   Vector *pvector = Vector::Open(Filename);
   // copio los datos locales al vector en memoria
   if (this->Copy(pvector)) {
      return true;
   }
   Vector::Close(pvector);
   return false;
}

/**
 * Inspecciona el archivo, para saber si es del formato que maneja
 *  Para que este metodo acepte manejar el vector, el nombre tiene que venir
 * prependeado por shpmemory: y terminado en un :, seguido por el WKT que
 * representa la referencia espacial en la que se manejara el vector.
 * Por ejemplo :
 *
 * shpmemory:/tmp/archivo_en_memoria.shp:[WGS84]
 *
 * @param[in] Filename nombre del vector
 * @param[in] VectorAccess tipo de acceso al vector
 * @param[in] Layers cantidad de capas en vector
 * @param[in] Type tipo de vector
 * @return Informa si MemoryVector puede ocuparse del vector
 */
bool MemoryVector::Inspect(const std::string &Filename, VectorAccessType VectorAccess,
                           int Layers, VectorType Type) {
   bool ismemory = IS_MEMORY_VECTOR(Filename);
   if (!ismemory) {
      return false;
   }

   std::string filename = GET_NAME(Filename);
   bool isloaded = loadedDataSources_.find(filename) != loadedDataSources_.end();

   // esta cargado y estoy tratando de crear => fallo
   if (isloaded && Layers != 0 && Type != Undefined) {
      return false;
   }
// comento las verificaciones porque el subsistema de creacion y edicion
// de vectores crea automaticamente las capas.
#ifdef __UNUSED_CODE__
   // si no esta cargado y no estoy tratando de crear => fallo
   if ( !isloaded && Layers==0 && Type==Undefined ) {
      return false;
   }
   // no esta cargado, indico las bandas, el tipo y voy a escribir
   if ( !isloaded && Layers>0 && Type != Undefined && VectorAccess!=ReadOnly ) {
      return true;
   }
#endif
   return true;
}

/**
 * Carga el vector
 * @param[in] Filename nombre del vector
 * @param[in] VectorAccess tipo de acceso al vector
 * @param[in] Layers cantidad de capas en vector
 * @param[in] Type tipo de vector
 * @return this o NULL si no pudo crear datasource
 * \todo cambiar por lectura de las opciones los nombres de las capas
 */
Vector* MemoryVector::Load(const std::string &Filename, VectorAccessType VectorAccess,
                           int Layers, VectorType Type) {
// comento las verificaciones porque el subsistema de creacion y edicion
// de vectores crea automaticamente las capas.
#ifdef __UNUSED_CODE__
   if ( (Layers<1 || Type == Undefined) && VectorAccess==WriteOnly ) {
      return this;
   }
#endif
   std::string spatialrefwkt = GET_SPATIAL_REF(Filename);
   DatasourceCollectionType::iterator it;
   std::string memoryname = GET_NAME(Filename);
   it = loadedDataSources_.find(memoryname);
   // ya existia
   if (it != loadedDataSources_.end()) {
      SetDataSource(it->second.second);
      REPORT_DEBUG("D:Creacion de vector en memoria con %d referencias",
                   loadedDataSources_[memoryName_].first);
      return this;
   }
   OGRSFDriver *pdriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
         "Memory");
   if (!pdriver) {
      return this;
   }
   // asigno el datasource creado al del vector
   OGRDataSource * pvectordatasource = pdriver->CreateDataSource(memoryname.c_str());
   if (pvectordatasource) {
      pvectordatasource->SetDriver(pdriver);
// comento las verificaciones porque el subsistema de creacion y edicion
// de vectores crea automaticamente las capas.
#ifdef __UNUSED_CODE__
      OGRwkbGeometryType type = wkbUnknown;
      switch ( Type ) {
         case Polygon:
         type = wkbPolygon;
         break;
         case Line:
         type = wkbLineString;
         break;
         case Point:
         type = wkbPoint;
         break;
         default:
         break;
      }
      for ( int i=0; i<Layers; i++ )
      {
         // \todo cambiar por lectura de las opciones los nombres de las capas
         std::stringstream ss;
         ss << "MemLayer_" << i;
         OGRSpatialReference *pspatialref =
         new OGRSpatialReference(spatialrefwkt.c_str() );
         pVectorDataSource_->CreateLayer(ss.str().c_str(), pspatialref,
               type, NULL);
         pspatialref->Release();
      }
#endif
   } else {
      REPORT_AND_FAIL_VALUE("D:No se pudo crear pVectorDataSource_", NULL);
   }
   SetDataSource(pvectordatasource);
   SetOption(Description, "Vector en memoria");
   SetOption("Filename", memoryName_);
   return this;
}

/**
 * Asigna el datasource asociado
 * @param[in] pVectorDataSource nuevo datasource
 */
void MemoryVector::SetDataSource(OGRDataSource *pVectorDataSource) {
   // si es si mismo no hace nada
   // tienen que existir
   if (!pVectorDataSource) {
      return;
   }
   // si no tengo driver o son el mismo driver, actualizo
   if (!pVectorDataSource_
         || (pVectorDataSource->GetDriver() && pVectorDataSource_->GetDriver()
               && pVectorDataSource->GetDriver()->GetName()
                     == pVectorDataSource_->GetDriver()->GetName())) {
      ReleaseDataSource();
      DatasourceCollectionType::iterator it = loadedDataSources_.begin();
      bool usedname = false;
      while (it != loadedDataSources_.end()) {
         // si lo encuentro por puntero, le incremento la cuenta
         if (it->second.second == pVectorDataSource) {
            memoryName_ = it->first;
            // incremento la cuenta de referencias
            it->second.first++;
            // asigno el datasource guardado al de la instancia
            pVectorDataSource_ = it->second.second;
            return;
         }
         if (memoryName_.empty() && pVectorDataSource->GetName() == it->first) {
            usedname = true;
         }
         it++;
      }
      // me protejo de nombres repetidos
      memoryName_ = pVectorDataSource->GetName();
      if (usedname) {
         memoryName_ = "/" + memoryName_;
      }

      SetOption("Filename", memoryName_);
      // asigno el datasource guardado al de la instancia
      pVectorDataSource_ = pVectorDataSource;
      // lo guardo en el mapa con cuenta == 1
      loadedDataSources_.insert(
            std::make_pair(memoryName_, std::make_pair(1, pVectorDataSource_)));
   }
}

/** Libera datasource */
void MemoryVector::ReleaseDataSource() {
   if (!memoryName_.empty()) {
      REPORT_DEBUG("D:Quedan %d referencias al vector shpmemory:%s",
                   loadedDataSources_[memoryName_].first, memoryName_.c_str());
      // cierro si la cuenta de referencias llega a 0
      if (--(loadedDataSources_[memoryName_].first) == 0) {
         // No se hace un DestroyDataSource, porque en el ultimo
         // MemoryVector que queda, se hace en el dtor de Vector
         loadedDataSources_.erase(memoryName_);
         REPORT_DEBUG("D:Eliminando shpmemory:%s", memoryName_.c_str());
      } else {
         memoryName_.clear();
         pVectorDataSource_ = NULL;  // para que suri::Vector::Close() no me cierre algo en uso
      }
   }
}

/** Inicializacion en las clases derivadas, se llama al registrarlas */
void MemoryVector::InitializeClass() {
   // Depende de Vector, la inicializacion se hace ahi.
   // al menos trato de registrar el driver de memoria
   RegisterOGRMEM();
}
}
