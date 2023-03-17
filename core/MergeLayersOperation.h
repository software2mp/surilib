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

#ifndef MERGELAYERSOPERATION_H_
#define MERGELAYERSOPERATION_H_

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards
class OGRFeatureDefn;

namespace suri {

/**
 * Procesa las fuentes de datos que recibe generando una nueva fuente de datos con los campos
 * seleccionados. Solo aplica en fuentes de datos del mismo tipo.
 */
class MergeLayersOperation : public suri::VectorOperation {
public:
   /** Constructor */
   MergeLayersOperation(std::multimap<std::string, std::string> FieldsMultipmap);
   /** Destructor */
   virtual ~MergeLayersOperation();
private:
   /**
    * Procesa las fuentes de datos de entrada generando la union de ambas
    * en una nueva fuente de datos
    */
   virtual VectorDatasource* DoProcessDatasource(VectorOperation::OperationType& Operation,
                                                 std::vector<DatasourceInterface*>& Datasources);
   /** Crea los campos seleccionados de todas las fuentes de datos en la capa destino */
   void MergeFields(std::vector<DatasourceInterface*>& Datasources, OGRLayer* pVectorLayer);
   /**
    * Recorre todos los features de todas las fuentes de datos
    * en busca de campos seleccionados en la capa destino
    */
   void MergeFeatures(std::vector<DatasourceInterface*>& Datasources, OGRLayer* pVectorLayer);
   /**
    * Setea los valores de los campos seleccionados de todas las fuentesde datos
    * en la capa destino y crea el feature
    */
   bool CreateFeature(OGRFeature* pFeature, OGRFeature* pNewFeature,
                      std::string FieldName,
                      OGRFeatureDefn* pOriginFeatureDef,
                      OGRFeatureDefn* pDestFeatureDef);
   /**
    * Contiene los nombres de los campos seleccionados para la capa destino y
    * el id de la fuente de datos a la cual corresponden
    */
   std::multimap<std::string, std::string> fieldsmultipmap_;
};

} /** namespace suri */

#endif /* MERGELAYERSOPERATION_H_ */
