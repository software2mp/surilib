/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

// Includes standard
#include <string>
#include <algorithm>

// Includes suri
#include "suri/VectorEditor.h"
#include "OgrGeometryEditor.h"
#include "logmacros.h"
#include "suri/DataTypes.h"
#include "suri/XmlElement.h"

// Includes Wx
#include "wx/wx.h"

// Includes OGR
#include "ogrsf_frmts.h"
#include "ogr_featurestyle.h"

#define DEFAULT_PRECISION 15

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @return instancia de la clase VectorEditor
 */
VectorEditor::VectorEditor() :
      pCurrentVector_(NULL), pReadOnlyVector_(NULL), pReadWriteVector_(NULL),
      currentLayerIndex_(-1), currentFeatureId_(-1), pCurrentFeature_(NULL),
      pCurrentFeatureDefn_(NULL), pCurrentLayer_(NULL), pGeometryEditor_(NULL),
      dirtyFeatureIds_(true), dirty_(false) {
}

/**
 * Destructor.
 */
VectorEditor::~VectorEditor() {
   CloseVector();
}

/**
 * Abre un vector, si no existe lo crea. OGR genera el directorio y por cada
 * capa crea un shp. Si el Filename contiene un .shp lo elimina.
 * @param[in] Filename nombre del directorio que contendra los shapefile de
 * cada capa.
 * @return bool que indica si la operacion tuvo exito
 */
bool VectorEditor::OpenVector(const std::string &Filename) {
   // Cierro el vector abierto
   CloseVector();
   // Elimino extension de archivo
   wxString filename(Filename);
   filename.Trim();
   // filename.Remove(filename.find(".shp"));
   // lo intento abrir sin crear, por si es un vector existente
   pReadWriteVector_ = Vector::Open(filename.c_str(), Vector::ReadWrite);
   pReadOnlyVector_ = Vector::Open(filename.c_str());

   if (!pReadWriteVector_ && !pReadOnlyVector_) {
      REPORT_DEBUG("D:El vector no se pudo crear");
      Vector::Close(pReadWriteVector_);
      Vector::Close(pReadOnlyVector_);
      pReadWriteVector_ = NULL;
      pReadOnlyVector_ = NULL;
      return false;
   }
   Vector* pvector = pReadOnlyVector_ ? pReadOnlyVector_ : pReadWriteVector_;
   fileName_ = pvector->GetOption(Vector::Filename);
   return true;
}

/**
 * Cierra el vector abierto
 */
void VectorEditor::CloseVector() {
   // cierro capa
   CloseLayer();
   if (pReadWriteVector_) {
      if (pReadWriteVector_->GetVectorDataSource()) {
         pReadWriteVector_->GetVectorDataSource()->SyncToDisk();
      }
      // cierro vector
      Vector::Close(pReadWriteVector_);
      pReadWriteVector_ = NULL;
   }
   Vector::Close(pReadOnlyVector_);
   pReadOnlyVector_ = NULL;

   // Este codigo es para ejecutar el REPACK sobre el vector cuando se termina de utilizar.
   // Es necesario para cuando se realizan operacion de borrado sobre el mismo. Se utiliza
   // aca en lugar del metodo DeleteFeature ya que para que surta efecto (al menos en Windows)
   // es necesario que este cerrado, abrirlo, ejecutar la consulta y cerrar.
   SanitizeVector();
}

/**
 * Abre capa en posicion LayerIndex, si no existe falla.
 * @param[in] LayerIndex nro de capa que se desea abrir
 * @return bool que indica si la operacion tuvo exito
 */
bool VectorEditor::OpenLayer(int LayerIndex) {
   if (!pReadWriteVector_ && !pReadOnlyVector_) {
      REPORT_DEBUG("D:El vector no esta abierto");
      return false;
   }
   // cierro capa anterior
   CloseLayer();

   // abro capa
   currentLayerIndex_ = LayerIndex;

   // Deacuerdo a las propiedades del vector lo abro.
   if (CanWrite() || !pReadOnlyVector_) {
      pCurrentVector_ = pReadWriteVector_;
   } else if (pReadOnlyVector_) {
      pCurrentVector_ = pReadOnlyVector_;
   }
   pCurrentLayer_ = pCurrentVector_->GetLayer(currentLayerIndex_);
   if (!pCurrentLayer_) {
      currentLayerIndex_ = -1;
      return false;
   }

   return true;
}

/**
 * Abre capa de nombre LayerName, si no existe la crea.
 * @param[in] LayerName nombre de la capa que se quiere abrir
 * @param[in] SpatialReference referencia espacial de la capa que se quiere
 * abrir. Si la capa no existe es un campo obligatorio
 * @param[in] VecType tipo de vectores que contendra la capa, si la capa
 * no existe es un campo obligatorio
 * @return int indice de la capa que se creo
 */
int VectorEditor::OpenLayer(const std::string &LayerName,
                            const std::string &SpatialReference,
                            const Vector::VectorType VecType) {
   // Para que no rompa en algunos lugares del codigo se pone el or en vez de and
   if (!pReadWriteVector_ && !pReadOnlyVector_) {
      REPORT_DEBUG("D:El vector no esta abierto");
      return -1;
   }
   CloseLayer();

   // Busco la capa en el datasource
   pCurrentLayer_ = CreateLayer(LayerName, SpatialReference, VecType);

   if (pCurrentLayer_) {
      Vector* pvector = pReadOnlyVector_ ? pReadOnlyVector_ : pReadWriteVector_;
      currentLayerIndex_ = pvector->GetLayerIndex(pCurrentLayer_);
      // Si no puedo abrirla falla
      if (CanWrite() || !pReadOnlyVector_) {
         pCurrentVector_ = pReadWriteVector_;
      } else {
         pCurrentVector_ = pReadOnlyVector_;
      }
   }

   return currentLayerIndex_;
}

/**
 * Crea capa de nombre LayerName.
 * @param[in] LayerName nombre de la capa que se quiere crear
 * @param[in] SpatialReference referencia espacial de la capa que se quiere
 * crear.
 * @param[in] VecType tipo de vectores que contendra la capa
 * @return puntero a capa creada. Si la capa ya existia retorna un puntero
 * a la misma.
 */
OGRLayer* VectorEditor::CreateLayer(const std::string &LayerName,
                                    const std::string &SpatialReference,
                                    Vector::VectorType VecType) {
   if (!CanCreateLayer()) {
      return NULL;
   }

   // Busco la capa en el datasource
   OGRLayer* newlayer = pReadWriteVector_->GetLayer(LayerName);
   // Si no existe la creo
   if (!newlayer) {
      newlayer = pReadWriteVector_->CreateLayer(LayerName.c_str(),
                                                SpatialReference.c_str(), VecType);
   }

   return newlayer;
}

/**
 * Verifica si el vector esta abierto y si el formato del vector permite que
 * se agregen nuevas capas.
 * @return bool resultado de la comprobacion
 */
bool VectorEditor::CanCreateLayer() const {
   if (!pReadWriteVector_ || !pReadWriteVector_->GetVectorDataSource()) {
      REPORT_DEBUG("D:El vector no esta abierto");
      return false;
   }
   if (!pReadWriteVector_->GetVectorDataSource()->TestCapability(ODsCCreateLayer)) {
      REPORT_DEBUG("D:El formato del vector no permite que se agregen nuevas capas");
      return false;
   }
   return true;
}

/**
 * Elimina la capa en la posicion indicada
 * @param[in] LayerIndex nro de capa a eliminar
 */
void VectorEditor::DeleteLayer(int LayerIndex) {
   if (!CanDeleteLayer()) {
      return;
   }

   if (currentLayerIndex_ == LayerIndex) {
      REPORT_DEBUG("D:Error. No se puede eliminar capa abierta");
      return;
   }
   if (!pReadWriteVector_->DeleteLayer(LayerIndex)) {
      REPORT_DEBUG("D:Error. No se pudo eliminar capa");
      return;
   }
}

/**
 * Elimina la capa de nombre indicado
 * @param[in] LayerName nombre de la capa que se quiere eliminar
 */
void VectorEditor::DeleteLayer(const std::string &LayerName) {
   if (!CanDeleteLayer()) {
      return;
   }

   // Busco la capa en el datasource
   OGRLayer* layer = pReadWriteVector_->GetLayer(LayerName);
   if (!layer) {
      REPORT_DEBUG("D:Error. No existe una capa con este nombre");
      return;
   }
   // Busco el indice de la capa
   int index = pReadWriteVector_->GetLayerIndex(layer);
   DeleteLayer(index);
}

/**
 * Verifica si el vector esta abierto y si el formato del vector permite que
 * se eliminen capas.
 * @return bool resultado de la comprobacion
 */
bool VectorEditor::CanDeleteLayer() const {
   if (!pReadWriteVector_ || !pReadWriteVector_->GetVectorDataSource()) {
      REPORT_DEBUG("D:El vector no esta abierto");
      return false;
   }
   if (!pReadWriteVector_->GetVectorDataSource()->TestCapability(ODsCDeleteLayer)) {
      REPORT_DEBUG("D:El formato del vector no permite que se eliminen capas");
      return false;
   }
   return true;
}

/**
 * Actualiza los cambios al disco.
 * \attention Es necesario cerrar y abrir el vector porque SyncToDisk de OGR no
 * actualiza siempre los datos en disco.
 */
void VectorEditor::SyncToDisk() {
   int currentlayer = currentLayerIndex_;
   long currentfeature = currentFeatureId_;

   // Abro vector temporal porque si no quedan referencias a vector en memoria
   // se elimina
   Vector* ptempvector = Vector::Open(fileName_);
   CloseVector();
   OpenVector(fileName_);
   Vector::Close(ptempvector);

   // Solo abro la capa y el feature si ya existian.(Evita que las crea vacias)
   if (currentlayer >= 0) {
      OpenLayer(currentlayer);
      if (currentfeature >= 0) {
         OpenFeature(currentfeature);
      }
   }
}

/**
 * Cierra la capa abierta.
 */
void VectorEditor::CloseLayer() {
   if (currentLayerIndex_ >= 0) {
      // cierro feature abierto
      CloseFeature();
      // cierro capa
      pCurrentLayer_->SyncToDisk();
      currentLayerIndex_ = -1;
      pCurrentLayer_ = NULL;
   }
   dirtyFeatureIds_ = true;
}

/**
 * Verifica si se puede escribir el archivo
 * @return true si se puede escribir el archivo
 * @return false si no se puede escribir el archivo
 */
bool VectorEditor::CanWrite() const {
   // Pregunto por currentLayerIndex en vez de la clase porque cuando abro el
   // vector todavia no se inicializo pCurrentLayer_.
   if (currentLayerIndex_ < 0) {
      REPORT_DEBUG("D: No hay una capa abierta");
      return false;
   }
   Vector* pvector = pReadWriteVector_ ? pReadWriteVector_ : pReadOnlyVector_;

   OGRLayer* player = pvector ? pvector->GetLayer(currentLayerIndex_) : NULL;
   if (!player)
      return false;
   if (!player->TestCapability(OLCRandomWrite))
      return false;

   // Verifico si la capa es ReadOnly
   // Se verifica esto ya que hay casos en los cuales se abre el archivo
   // por otro lado a la vez y se necesita hacer este chequeo
   if (!pvector || !pvector->GetLayer(currentLayerIndex_)
         || !pvector->GetLayer(currentLayerIndex_)->GetLayerDefn()) {
      REPORT_DEBUG("D:Error al abrir los vectores");
      return false;
   }

   // Si la capa no se pudo abrir como ReadWrite abro como read only
   if (pvector->GetLayer(currentLayerIndex_)->GetLayerDefn()->GetFieldCount()
         != pvector->GetLayer(currentLayerIndex_)->GetLayerDefn()->GetFieldCount()) {
      REPORT_DEBUG("D:El vector solo se puede ver en modo lectura");
      return false;
   }

   return true;
}

/**
 * Verifica si se puede leer de la tabla
 * @return true si se puede leer de la tabla
 * @return false si no se puede leer de la tabla
 */
bool VectorEditor::CanRead() const {
   // Pregunto por currentLayerIndex en vez de la clase porque cuando abro el
   // vector todavia no se inicializo pCurrentLayer_.
   if (!pCurrentVector_ || !pCurrentLayer_) {
      REPORT_DEBUG("D: No hay una capa abierta");
      return false;
   }
   /** No es determinante que no pueda leer con acceso random. Hay fuentes de datos
    * vectoriales que tienen acceso secuencial (ej. archivos csv)
    **/

   if (!pCurrentLayer_->TestCapability(OLCRandomRead)) {
      REPORT_DEBUG("D:No se puede leer el vector con acceso random");
      //return false;
   }

   return true;
}

/**
 * Crea campo de nombre y tipo indicados en capa abierta
 * \pre No debe exisitir un feature abierto
 * @param[in] Name nombre del campo que se quiere crear
 * @param[in] DataType tipo de dato que contendra el campo
 * @param[in] Length longitud del campo
 * @return bool que informa el resultado de la operacion
 *
 * \todo si el nombre se repite o es muy largo falla
 *
 * \todo (danieltesta-#3189): Verificar en todos los lugares donde se usen
 * DataInfo<float> o DataInfo<double> para unificar ya que en algunos lugares
 * se estaba utilizando DataInfo<float> cuando en realidad no estaba soportado.
 */
bool VectorEditor::CreateField(const std::string &Name, const std::string &DataType,
                               int Length) {
   // Si no hay capa abierta falla o hay un feature abierto
   if (!CanCreateField()  || GetOpenFeature() >= 0)
      return false;

   // Segun el tipo de dato creo campo de tipo diferente
   switch (Type2Id(DataType)) {
      case DataInfo<int>::Id: {
         OGRFieldDefn fieldint(Name.c_str(), OFTInteger);
         REPORT_DEBUG(fieldint.GetNameRef());
         pCurrentLayer_->CreateField(&fieldint, FALSE);
         break;
      }
      case DataInfo<double>::Id: {
         OGRFieldDefn fielddouble(Name.c_str(), OFTReal);
         fielddouble.SetPrecision(DEFAULT_PRECISION);
         pCurrentLayer_->CreateField(&fielddouble, FALSE);
         break;
      }
      case DataInfo<float>::Id: {
         OGRFieldDefn fielddouble(Name.c_str(), OFTReal);
         fielddouble.SetPrecision(DEFAULT_PRECISION);
         pCurrentLayer_->CreateField(&fielddouble, FALSE);
         break;
      }
      case DataInfo<unsigned char>::Id: {
         OGRFieldDefn fieldchar(Name.c_str(), OFTString);
         if (Length > 0) {
            fieldchar.SetWidth(Length);
         } else {
            fieldchar.SetWidth(512);
         }
         pCurrentLayer_->CreateField(&fieldchar, FALSE);
         break;
      }
      default:
         return false;
         break;
   }

   SyncToDisk();
   return true;
}

/**
 * Analiza si puede crear un campo en capa abierta
 * @return true si puede crear un campo en capa abierta
 * @return false si no puede crear un campo en capa abierta
 */
bool VectorEditor::CanCreateField() const {
   if (!CanWrite()) {
      REPORT_DEBUG("D: No hay una capa abierta");
      return false;
   }
   if (pCurrentLayer_->TestCapability(OLCCreateField) == FALSE) {
      REPORT_DEBUG("D:El formato del vector no permite que se agregen columnas");
      return false;
   }
   return true;
}

/**
 * Elimina un campo de la capa abierta.
 * \pre No debe exisitir un feature abierto
 * @param[in] Name nombre del campo que se quiere eliminar
 */
bool VectorEditor::DeleteField(const std::string &Name) {
   int position = GetFieldPosition(Name);
   if (!CanDeleteField() || GetOpenFeature() >= 0 || position < 0)
      return false;

   OGRErr error = pCurrentLayer_->DeleteField(position);

   SyncToDisk();
   return (error == OGRERR_NONE);
}

/**
 * Analiza si puede eliminar un campo en capa abierta
 * @return true si puede eliminar un campo en capa abierta
 * @return false si no puede eliminar un campo en capa abierta
 */
bool VectorEditor::CanDeleteField() const {
   if (!CanWrite()) {
      REPORT_DEBUG("D: No hay una capa abierta");
      return false;
   }
   if (pCurrentLayer_->TestCapability(OLCDeleteField) == FALSE) {
      REPORT_DEBUG("D:El formato del vector no permite que se agregen columnas");
      return false;
   }
   return true;
}

/**
 * Busca el id del feature en posicion indicada.
 * @param[in] FeaturePosition posicion del feature(0 a n)
 * @return id del feature en caso de exito
 * @return -1 en caso de error.
 */
long VectorEditor::GetFeatureId(int FeaturePosition) {
   long returnvalue = -1;

   if (!CanRead()) {
      return returnvalue;
   }

   // Si dirtyFeatureIds_==true recargo el listado de feature ids;
   if (dirtyFeatureIds_) {
      featuresIds_.clear();
      pCurrentLayer_->ResetReading();
      OGRFeature* pfeature = pCurrentLayer_->GetNextFeature();
      for (int i = 0; pfeature != NULL; i++) {
         featuresIds_.push_back(pfeature->GetFID());
         OGRFeature::DestroyFeature(pfeature);
         pfeature = pCurrentLayer_->GetNextFeature();
      }
      dirtyFeatureIds_ = false;
   }

   if (featuresIds_.size() == 0) {
      return returnvalue;
   }
   returnvalue = featuresIds_[std::min(featuresIds_.size(), (size_t) FeaturePosition)];
   return returnvalue;
}

/**
 * Retorna la referencia espacial de la capa abierta
 * @return Referencia espacial de la capa abierta
 */
std::string VectorEditor::GetLayerSpatialReference() const {
   if (!CanRead()) {
      return std::string("");
   }

   return pCurrentVector_->GetLayerSR(currentLayerIndex_);
}

/**
 * Abre Feature del identificador FeatureId, si no existe crea un feature.
 * Si existe el campo Clockwise y es falso se invierten los puntos.
 * @param[in] FeatureId identificador del feature que se quiere abrir
 * @param[in] LoadGeometry si este campo es false no se carga la geometria.
 * No se podra agregar ni sacar puntos pero se podran modificar los campos.
 * @return identificador del feature abierto
 */
long VectorEditor::OpenFeature(long FeatureId, bool LoadGeometry) {
   CloseFeature();
   // Si no hay capa abierta falla
   if (!CanRead()) {
      return -1;
   }

   if ( FeatureId != -1 )
   pCurrentFeature_ = pCurrentLayer_->GetFeature(FeatureId);

   if (!pCurrentFeature_) {
      // Si el feature no existe lo crea
      pCurrentFeature_ = OGRFeature::CreateFeature(pCurrentLayer_->GetLayerDefn());
      if (!pCurrentFeature_) {
         REPORT_DEBUG("D:Error al crear el Feature");
         return -1;
      }
      // Crea GeometryEditor con tipo de vector permitido por la capa
      pCurrentLayer_->CreateFeature(pCurrentFeature_);
      currentFeatureId_ = pCurrentFeature_->GetFID();
      pGeometryEditor_ = new OgrGeometryEditor(
            pCurrentVector_->GetLayerSR(currentLayerIndex_),
            pCurrentVector_->GetLayerType(currentLayerIndex_));
      dirtyFeatureIds_ = true;
   } else {
      // Si el feature existe crea GeometryEditor con su contenido
      currentFeatureId_ = FeatureId;
      if (LoadGeometry) {
         if (pCurrentFeature_->GetGeometryRef()) {
            pGeometryEditor_ = new OgrGeometryEditor(
                  pCurrentLayer_->GetFeature(FeatureId)->StealGeometry());

            // Invierto el sentido de los puntos para contrarrestar el cambio que hace OGR
            int clockwise = 0;
            if (pCurrentLayer_->GetLayerDefn()->GetFieldIndex("clockwise") >= 0) {
               GetField("clockwise", clockwise);
               if ((pGeometryEditor_) && (clockwise == 0)) {
                  REPORT_DEBUG("D:No clockwise");
                  // Se crea nuevo geometryeditor
                  OgrGeometryEditor* pgeometryeditor = new OgrGeometryEditor(
                        pCurrentVector_->GetLayerSR(currentLayerIndex_),
                        pCurrentVector_->GetLayerType(currentLayerIndex_));
                  // Se guardan puntos en sentido inverso
                  Coordinates coord;
                  for (int i = pGeometryEditor_->GetPointCount() - 1; i >= 0; i--) {
                     pGeometryEditor_->GetPoint(coord, i);
                     pgeometryeditor->AddPoint(coord);
                  }
                  // Se remplaza geometria leida con la nueva
                  delete pGeometryEditor_;
                  pGeometryEditor_ = pgeometryeditor;
               }
            }
         } else {
            // Si no se pudo leeer geometryeditor carga uno vacio.
            pGeometryEditor_ = new OgrGeometryEditor(
                  pCurrentVector_->GetLayerSR(currentLayerIndex_),
                  pCurrentVector_->GetLayerType(currentLayerIndex_));
         }
      }
   }
   pCurrentFeatureDefn_ = NULL;
   if (pCurrentFeature_ && pCurrentFeature_->GetDefnRef()) {
      pCurrentFeatureDefn_ = pCurrentFeature_->GetDefnRef()->Clone();
   }
   return currentFeatureId_;
}

/**
 * Verifica si un campo con este nombre existe y retorna la posicion
 * @param[in] FieldName nobre del campo que se quiere buscar
 * @return posicion del campo con este nombre. Retorna -1 si el campo
 * no existe.
 */
int VectorEditor::GetFieldPosition(const std::string& FieldName) const {
   if (!CanRead()) {
      return -1;
   }

   return pCurrentLayer_->GetLayerDefn()->GetFieldIndex(FieldName.c_str());
}

/**
 * Retorna el tipo de dato en el campo
 * @param[in] Position posicion del campo
 * @return tipo de dato
 */
std::string VectorEditor::GetFieldType(int Position) const {
   if (!CanRead() || !pCurrentLayer_->GetLayerDefn()
         || !pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)) {
      return DataInfo<void>::Name;
   }

   if (pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetType()
         == OFTInteger) {
      return DataInfo<int>::Name;
   }
   if (pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetType() == OFTReal) {
      // Se castean a int porque al copiar una tabla los campos int se transforman
      // a double con precision=0.
      if (pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetPrecision() == 0) {
         return DataInfo<int>::Name;
      } else {
         return DataInfo<double>::Name;
      }
   }
   if (pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetType() == OFTString) {
      return DataInfo<unsigned char>::Name;
   }
   return DataInfo<void>::Name;
}

/**
 * Retorna la longitud del campo para string o la precision en otro caso
 * @param[in] Position posicion del campo
 * @return ancho/cantidad de digitos de precision del campo
 */
int VectorEditor::GetFieldLength(int Position) const {
   if (!CanRead() || !pCurrentLayer_->GetLayerDefn()
         || !pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)) {
      return -1;
   }

   if (pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetType() == OFTString) {
      return pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetWidth();
   }

   return pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetPrecision();
}

/**
 * Elimina el feature indicado, debe estar cerrado
 * @param[in] FeatureId identificador del Feature abierto
 * @return informa si pudo eliminar el campo
 */
bool VectorEditor::DeleteFeature(long FeatureId) {
   if (!CanDeleteFeature()) {
      return false;
   }

   // Si el feature se esta editando falla
   if (FeatureId == currentFeatureId_) {
      REPORT_DEBUG("D:No se puede eliminar feature abierto");
      return false;
   }

   // Si el feature no existe falla
   if (!pCurrentLayer_->GetFeature(FeatureId)) {
      REPORT_DEBUG("D:No se puede eliminar, el feature no existe");
      return false;
   }

   if (pCurrentLayer_->DeleteFeature(FeatureId) == OGRERR_UNSUPPORTED_OPERATION) {
      REPORT_DEBUG("D:Operacion no soportada");
      return false;
   }

   dirtyFeatureIds_ = true;

   return true;
}

/*
 * Informa si se pueden agregar features a una capa
 * @return true indicando que se pueden agregar features.
 * @return false indicando que no se pueden agregar features.
 */
bool VectorEditor::CanCreateFeature() const {
   if (!CanWrite()) {
      return false;
   }
   return true;
}


/*
 * Informa si se pueden eliminar features de una capa
 * @return true indicando que se pueden eliminar features.
 * @return false indicando que no se pueden eliminar features.
 */
bool VectorEditor::CanDeleteFeature() const {
   if (!CanWrite()) {
      return false;
   }

   if (!pCurrentLayer_->TestCapability(OLCDeleteFeature)) {
      REPORT_DEBUG("D:El formato del vector no permite que se eliminen capas");
      return false;
   }
   return true;
}

/*
 * Retorna la cantidad de features abiertas
 * @return  cantidad de features abiertas
 */
int VectorEditor::GetFeatureCount() const {
   if (!CanRead()) {
      return -1;
   }

   return pCurrentLayer_->GetFeatureCount();
}

/* Retorna la cantidad de campos en tabla de capa abierta
 * @return cantidad de campos en tabla de capa abierta
 */
int VectorEditor::GetFieldCount() const {
   if (!CanRead() || !pCurrentLayer_->GetLayerDefn()) {
      REPORT_DEBUG("D: Error al abrir capa");
      return -1;
   }
   return pCurrentLayer_->GetLayerDefn()->GetFieldCount();
}

/* Retorna el nombre del campo en la posicion indicada.
 * @param[in] Position indice del campo (entre 0 y GetFieldCount()-1).
 * @return string con el nombre del campo
 */
std::string VectorEditor::GetFieldName(int Position) const {
   if (!CanRead() || !pCurrentLayer_->GetLayerDefn()) {
      REPORT_DEBUG("D: Error al abrir capa");
      return "";
   }
   if (!pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)) {
      REPORT_DEBUG("D: Posicion incorrecta");
      return "";
   }
   return pCurrentLayer_->GetLayerDefn()->GetFieldDefn(Position)->GetNameRef();
}

/**
 * Obtiene identificador del Feature
 * @return identificador de feature abierto. -1 si no hay ninguno
 */
long VectorEditor::GetOpenFeature() {
   return currentFeatureId_;
}

/**
 * Cierra el Feature abierto. Si existe el campo clockwise setea su valor.
 * Si el campo no existe y clockwise es falso crea el campo y lo setea a false.
 */
void VectorEditor::CloseFeature() {
   if (CanWrite() && pCurrentFeature_) {
      // Verifico que el feature definition de la capa no se halla modificado
      if (dirty_) {
         pCurrentLayer_->SyncToDisk();
         dirty_ = false;
      }
      OGRFeatureDefn* playerfeaturedefinition = pCurrentLayer_->GetLayerDefn();
      bool validfeature;

#ifndef __ERROR_FROM_OGR_SOLVED__
      // Solo actualizo el vector si no se modifico el feature definition
      // caso contrario OGR genera sigsegv
      validfeature = Compare(playerfeaturedefinition, pCurrentFeatureDefn_);
      if (validfeature) {
#endif

         // Guardo geometria (saca copia de pGeometryEditor)
         if (pGeometryEditor_
               && pCurrentFeature_->SetGeometry(pGeometryEditor_->GetOgrGeometry())!=
               OGRERR_NONE) {
            REPORT_DEBUG("D:No se pudo escribir el punto");
         }
         // Guardo feature actualizado
         if (pCurrentLayer_->SetFeature(pCurrentFeature_) != OGRERR_NONE) {
            SHOW_ERROR(message_LAYER_MODIFIED_CLOSE_ERROR);
            validfeature = false;
         }

#ifndef __ERROR_FROM_OGR_SOLVED__
      } else {
         // Se genera memory leak porque OGR produce SIGSEV si el
         // feature definition no coincide con el de la capa. Inclusive
         // en destroy.
         SHOW_ERROR(message_LAYER_MODIFIED_CLOSE_ERROR);
         pCurrentFeature_ = NULL;
      }
#endif
      pCurrentFeatureDefn_->Release();
      pCurrentFeatureDefn_ = NULL;

      // Destruyo la caracteristica
      OGRFeature::DestroyFeature(pCurrentFeature_);
      pCurrentFeature_ = NULL;
// Con edicion en memoria eso genera problemas
#ifdef __UNUSED_CODE__
      // Para los poligonos busca si existe campo y lo actualiza. Si no existe y no es clockwise lo
      // crea
      if (pGeometryEditor_->GetGeometryType() == Vector::Polygon) {
         if ( pOpenVector_->GetLayer(currentLayerIndex_Index_)->
               GetLayerDefn()->GetFieldIndex("clockwise")>=0 ) {
            // Si el campo existe setea clockwise a valor correcto
            OGRPolygon *ppolyref = dynamic_cast<OGRPolygon*>(
                  pfeature->GetGeometryRef() );
            if ( ppolyref && ppolyref->getExteriorRing()->isClockwise() ) {
               SetField("clockwise", 1);
            } else {
               SetField("clockwise", 0);
            }
         } else {
            // Cuando el sentido se invierte este flag se pasa a true
            CreateField("clockwise", DataInfo<int>::Name);
            SetField("clockwise", 0);
         }
      }
#endif

      // Elimino GeometryEditor del feature
      delete pGeometryEditor_;
      pGeometryEditor_ = NULL;

      if (!validfeature) {
         int featureid = currentFeatureId_;
         currentFeatureId_ = -1;
         DeleteFeature(featureid);
      }
   }

   // Si al entrar a este metodo dirty_ es true entonces deberia llegar a este
   // punto con false.
   // Se hace esta verificacion ya que en modo de edicion vectorial este llamado
   // a SyncToDisk hace que la aplicacion de vuelva extremadamente lenta si se
   // trabajando con un vector en disco. Esto sucede incluso si no se hace ninguna
   // modificacion.
   if (dirty_)
      pCurrentLayer_->SyncToDisk();
   currentFeatureId_ = -1;

}

/**
 * Guarda el valor en el campo indicado. El campo debe ser int
 * @param[in] Name nombre del campo al que se le quiere asignar valor
 * @param[in] Value int que se quiere asignar al campo para el feature abierto
 * @return true si pudo guardar el valor en el campo.
 * @return false si no pudo guardar el valor en el campo.
 */
bool VectorEditor::SetField(const std::string &Name, int Value) {
   if (!CanWrite() || !pCurrentFeature_) {
      REPORT_DEBUG("D:Error al setear campo, no hay un feature abierto");
      return false;
   }
   pCurrentFeature_->SetField(Name.c_str(), Value);
   dirty_ = true;
   return (pCurrentLayer_->SetFeature(pCurrentFeature_) == OGRERR_NONE);
}

/**
 * Guarda el valor en el campo indicado. El campo debe ser double
 * @param[in] Name nombre del campo al que se le quiere asignar valor
 * @param[in] Value Double que se asignara al campo para el feature abierto
 * @return true si pudo guardar el valor en el campo.
 * @return false si no pudo guardar el valor en el campo.
 */
bool VectorEditor::SetField(const std::string &Name, double Value) {
   if (!CanWrite() || !pCurrentFeature_) {
      REPORT_DEBUG("D:Error al setear campo, no hay un feature abierto");
      return false;
   }
   pCurrentFeature_->SetField(Name.c_str(), Value);
   dirty_ = true;
   return (pCurrentLayer_->SetFeature(pCurrentFeature_) == OGRERR_NONE);
}

/**
 * Guarda el valor en el campo indicado. El campo debe ser char*
 * @param[in] Name nombre del campo al que se le quiere asignar valor
 * @param[in] Value Char que se quiere asignar al campo para el feature abierto
 * @return true si pudo guardar el valor en el campo.
 * @return false si no pudo guardar el valor en el campo.
 */
bool VectorEditor::SetField(const std::string &Name, const std::string &Value) {
   if (!CanWrite() || !pCurrentFeature_) {
      REPORT_DEBUG("D:Error al setear campo, no hay un feature abierto");
      return false;
   }
#ifdef __WINDOWS__
   std::string value =
         wxString(wxConvUTF8.cMB2WC(Value.c_str()),
                  wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
   pCurrentFeature_->SetField(Name.c_str(), value.c_str());
#else
   pCurrentFeature_->SetField(Name.c_str(), Value.c_str());
#endif  // __WINDOWS__
   dirty_ = true;
   return (pCurrentLayer_->SetFeature(pCurrentFeature_) == OGRERR_NONE);
}

/**
 * Obtiene el valor del campo indicado. El campo debe ser int
 * @param[in] Name nombre del campo del que se quiere obtener el valor
 * @param[out] Value valor del campo para el feature abierto
 */
void VectorEditor::GetField(const std::string &Name, int &Value) const {
   if (!pCurrentFeature_) {
      REPORT_DEBUG("D:Error obtener campo, no hay un feature abierto");
      return;
   }

   int index = pCurrentFeature_->GetFieldIndex(Name.c_str());
   Value = pCurrentFeature_->GetFieldAsInteger(index);
}

/**
 * Obtiene el valor del campo indicado. El campo debe ser double
 * @param[in] Name nombre del campo del que se quiere obtener el valor
 * @param[out] Value valor del campo para el feature abierto
 */
void VectorEditor::GetField(const std::string &Name, double &Value) const {
   if (!pCurrentFeature_) {
      REPORT_DEBUG("D:Error obtener campo, no hay un feature abierto");
      return;
   }

   int index = pCurrentFeature_->GetFieldIndex(Name.c_str());
   Value = pCurrentFeature_->GetFieldAsDouble(index);
}

/**
 * Obtiene el valor del campo indicado. El campo debe ser char*
 * @param[in] Name nombre del campo del que se quiere obtener el valor
 * @param[out] Value valor del campo para el feature abierto
 */
void VectorEditor::GetField(const std::string &Name, std::string& Value) const {
   if (!pCurrentFeature_) {
      REPORT_DEBUG("D:Error obtener campo, no hay un feature abierto");
      return;
   }

   int index = pCurrentFeature_->GetFieldIndex(Name.c_str());
   Value = pCurrentFeature_->GetFieldAsString(index);
#ifdef __WINDOWS__
   Value = wxString(wxConvUTF8.cMB2WC(Value.c_str()),
                    wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#endif
}

/**
 * Modifica un punto a la geometria. Utiliza GeometryEditor para modificarlo.
 * Si no se pasa Posicion se agrega al final.
 * En caso del punto se ignora la posicion.
 * @param[in] Position posicion dentro del vector donde se quiere agregar punto
 * @param[in] Point Coordenadas del nuevo punto en sistema de coordenadas del vector
 * @return bool que informa resultado de operacion
 */
bool VectorEditor::SetPoint(const Coordinates &Point, int Position) {
   if (!pGeometryEditor_) {
      REPORT_DEBUG("D:No se puede agregar punto, no hay un feature abierto");
      return false;
   }
   return pGeometryEditor_->SetPoint(Point, Position);
}

/**
 * Agrega un punto a la linea. Utiliza GeometryEditor para agregarlos.
 * Si no se pasa Posicion se agrega al final.
 * En caso del punto se ignora la posicion.
 * @param[in] Position posicion dentro del vector donde se quiere agregar punto
 * -1 significa al final
 * @param[in] Point Coordenadas del nuevo punto en sistema de coordenadas del vector
 * @return bool que informa resultado de operacion
 */
bool VectorEditor::AddPoint(const Coordinates &Point, int Position) {
   if (!pGeometryEditor_) {
      REPORT_DEBUG("D:No se puede agregar punto, no hay un feature abierto");
      return false;
   }
   return pGeometryEditor_->AddPoint(Point, Position);
}

/**
 * Obtiene las coordenadas de un punto de la linea
 * @param[in] Position posicion dentro del vector del punto.
 * -1 significa el ultimo
 * @param[out] Point Coordenadas del punto en sistema de coordenadas del vector
 * @return bool que informa resultado de operacion
 */
bool VectorEditor::GetPoint(Coordinates &Point, int Position) {
   if (!pGeometryEditor_) {
      REPORT_DEBUG("D:No se puede obtener punto, no hay un feature abierto");
      return false;
   }
   return pGeometryEditor_->GetPoint(Point, Position);
}

/**
 * Elimina un punto de la linea
 * @param[in] Position posicion del punto que se quiere eliminar
 * -1 significa el ultimo
 * @return bool que informa resultado de operacion
 */
bool VectorEditor::DeletePoint(int Position) {
   if (!pGeometryEditor_) {
      REPORT_DEBUG("D:No se puede eliminar punto, no hay un feature abierto");
      return false;
   }
   return pGeometryEditor_->DeletePoint(Position);
}

/**
 * Retorna la cantidad de puntos en el feature abierto
 * @return cantidad de puntos en el feature abierto
 */
int VectorEditor::GetPointCount() const {
   if (!pGeometryEditor_) {
      REPORT_DEBUG("D:No se puede contar puntos, no hay un feature abierto");
      return -1;
   }
   return pGeometryEditor_->GetPointCount();
}

/**
 * Comparo dos FeatureDefinitions. Compara el nombre y el nombre/tipo de dato
 * de los campos
 * @param[in] pFeature1 primer feature definition que se quiere comparar
 * @param[in] pFeature2 segundo feature definition que se quiere comparar
 * @return resultado de la comparacion
 */
bool VectorEditor::Compare(OGRFeatureDefn *pFeature1, OGRFeatureDefn *pFeature2) {
   // Comparo nombre del fueature definition y el numero de campos
   if (strcmp(pFeature1->GetName(), pFeature2->GetName()) != 0 ||
         pFeature1->GetFieldCount() != pFeature2->GetFieldCount()) {
      return false;
   }

   int fieldcount = pFeature1->GetFieldCount();
   for (int ix = 0; ix < fieldcount; ++ix) {
      // Comparo para cada campo el nombre y el tipo.
      OGRFieldDefn* pfielddef1 = pFeature1->GetFieldDefn(ix);
      OGRFieldDefn* pfielddef2 = pFeature2->GetFieldDefn(ix);

      if (strcmp(pfielddef1->GetNameRef(), pfielddef2->GetNameRef()) != 0 ||
            pfielddef1->GetType() != pfielddef2->GetType()) {
         return false;
      }
   }
   return true;
}

/**
 * Devuelve el nombre de la capa abierta
 * @return string con el nombre de la capa abierta. Si no hay capa abierta devuelve string vacio*/
std::string VectorEditor::GetOpenLayerName() const {
   std::string layername;
   if (currentLayerIndex_ != -1 && pCurrentLayer_) {
      layername = pCurrentLayer_->GetName();
   }
   return layername;
}

/** Indica al feature con el id que se pasa por parametro la geometria que le corresponde. **/
bool VectorEditor::SetGeometryByWkt(long FeatureId, const std::string& GeomtryWkt) {
   if (OpenFeature(FeatureId) < 0 || GeomtryWkt.empty())
      return false;
   return pGeometryEditor_->SetGeometryFromWkt(GeomtryWkt, pCurrentLayer_->GetSpatialRef());
}

/** Obtiene el wkt de una geometria asociada a un feature **/
std::string VectorEditor::GetGeometryWkt(long FeatureId) const {
   OGRGeometry* pgeometry = NULL;
   if (pCurrentFeature_ && pCurrentFeature_->GetFID() == FeatureId) {
      pgeometry = pCurrentFeature_->GetGeometryRef();
   } else if (pCurrentLayer_) {
      OGRFeature* pfeature = pCurrentLayer_->GetFeature(FeatureId);
      if (pfeature)
         pgeometry =  pfeature->GetGeometryRef();
   }
   std::string wkt;
   char* pwkt = NULL;
   if(pgeometry && pgeometry->exportToWkt(&pwkt) == OGRERR_NONE) {
      wkt = pwkt;
      OGRFree(pwkt);
   }
   return wkt;
}

/**
 * Ejecuta la operacion REPACK en el vector.
 */
void VectorEditor::SanitizeVector() {
   OGRLayer* poresultset = NULL;
   OGRDataSource* pods = NULL;
   pods = OGRSFDriverRegistrar::Open(fileName_.c_str(), TRUE);
   if (pods) {
      for (int ix = 0, lenix = pods->GetLayerCount(); ix < lenix; ++ix) {
         OGRLayer* player = pods->GetLayer(ix);
         if (player) {
            std::string stt = "REPACK ";
            stt.append(player->GetName());
            poresultset = pods->ExecuteSQL(stt.c_str(), NULL, NULL);
            if (poresultset) pods->ReleaseResultSet(poresultset);
         }
      }
      OGRDataSource::DestroyDataSource(pods);
   }
}

/** Crea una capa mediante comando SQL */
OGRLayer *VectorEditor::CreateSqlLayer(const std::string &SQLStatement,
                                    OGRGeometry *pFilterGeometry,
                                    const std::string &SQLDialect) {
	OGRLayer* player = NULL;
	if (pReadOnlyVector_)
		player = pReadOnlyVector_->CreateSqlLayer(SQLStatement, pFilterGeometry, SQLDialect);
	else if (pReadWriteVector_)
		player =pReadWriteVector_->CreateSqlLayer(SQLStatement, pFilterGeometry, SQLDialect);
	return player;
}
}  /** namespace suri */
