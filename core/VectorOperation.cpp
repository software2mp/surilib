/*! \file VectorOperation.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards
// Includes otros
#include "ogrsf_frmts.h"

namespace suri {

const char* VectorOperation::OperationString[] =
                  { "Union", "Interseccion", "Recorte", "Buffer", "Fusion de capas" };

/** Constructor */
VectorOperation::VectorOperation() : pSucesor_(NULL) {
}

/** Destructor */
VectorOperation::~VectorOperation() {
}

/**
 * Objetivo: Genera un archivo shp en el directorio temporal del sistema
 * @param[in]  HtmlSrc: contenido del archivo a generar.
 * @return Ok? path del archivo salvado : cadena vacia.
 */
std::string VectorOperation::GetTempFile() {
   //genero archivo temporal
   std::string pathtemp = wxFileName::CreateTempFileName("").c_str();
   pathtemp += "sur.shp";
   return pathtemp;
}

/**
 * Copia los campos de una capa origen a una capa destino
 * @param[in] pLayer: capa origen
 * @param[in] pDestLayer: capa destino
 * */
void VectorOperation::CreateFields(OGRLayer* pLayer, OGRLayer* pDestLayer) {
   OGRFeatureDefn* pfeaturedef = pLayer->GetLayerDefn();
   int fieldcount = pfeaturedef->GetFieldCount();
   for (int i = 0; i < fieldcount; ++i)
      pDestLayer->CreateField(pfeaturedef->GetFieldDefn(i));
}

/** Copia el contenido de los campos de un feature origen a
 *  los campos correspondientes del feature destino
 *  @param[in] pFeature: feature origen
 *  @param[in] pDestFeature: feature destino
 *  @param[in] FieldCount: cantidad de campos del feature destino
 *  @param[in] Field: posicion donde comienza el feature origen desde del destino
 *   */
void VectorOperation::CopyFieldsFromOrigin(OGRFeature *pFeature, OGRFeature *pDestFeature,
                                           int FieldCount, int Field) {
   for (int Index = 0; Field < FieldCount; ++Index, ++Field) {
      OGRField* pfield = pFeature->GetRawFieldRef(Index);
      pDestFeature->SetField(Field, pfield);
   }
}

/**
 * Verifica que todas las capas de las fuentes de datos vectoriales sean del mismo tipo
 * @return true si todas las capas son del mismo tipo, caso contrario false
 */
bool VectorOperation::VerifyLayersType(std::vector<DatasourceInterface*> Datasources) {
   if (Datasources.size() < 2)
      return false;
   // Obtengo el tipo de capa de la primer fuente vectorial
   Vector* pdatasource = Datasources[0]->GetDatasourceAsVector();
   Vector::VectorType type = pdatasource->GetLayerType(0);
   // Recorro el resto de las fuentes vectoriales y comparo
   std::vector<DatasourceInterface*>::iterator it = Datasources.begin();
   for (int i = 0; it != Datasources.end(); ++i, ++it) {
      pdatasource = Datasources[i]->GetDatasourceAsVector();
      if (type != pdatasource->GetLayerType(0))
         return false;
   }
   delete pdatasource;
   return true;
}

} /** namespace suri */
