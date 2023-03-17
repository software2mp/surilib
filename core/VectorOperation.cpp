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
