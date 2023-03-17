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

#ifndef VECTOROPERATIONPROCESS_H_
#define VECTOROPERATIONPROCESS_H_

// Includes Estandar
// Includes Suri
#include "FileVectorExporterProcess.h"
#include "suri/ProcessAdaptLayer.h"
#include "VectorOperation.h"
// Includes Wx
// Defines

namespace suri {
// forwards

class VectorOperationProcess : public suri::FileVectorExporterProcess {
public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Constructor */
   VectorOperationProcess(std::vector<DatasourceInterface*>& InputDatasources,
                          const Subset &ViewerSubset, Element* &pOutputElement,
                          DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~VectorOperationProcess();
private:
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
    **/
   std::vector<wxXmlNode*> DoGetXmlParameters() const;
   /** Obtiene la lista de elementos a renderizar en el proceso */
   DatasourceContainerInterface* GetRenderizationList();
   /** Crea una fuente de datos vectorial con el buffer
    *  basandose en una copia de la fuente original */
   VectorDatasource* GetProcessedDatasource();
   /** Genera un archivo shp en el directorio temporal del sistema */
   std::string GetTempFile();
   VectorOperation::OperationType operation_; /** Tipo de operacion */
   std::vector<DatasourceInterface*> datasources_; /** Fuentes de datos para procesar */
   double distancevalue_; /** Distancia de buffer */
   std::string distancevaluestr_; /** String con distancia de buffer */
   std::string bufferfield_; /** Campo de distancia de buffer */
   int nrings_; /** Numero de anillos concentricos */
   std::string nringsstr_; /** String con numero de anillos concentricos */
   double conversionfactor_; /** Factor de conversion */
   std::string conversionfactorstr_; /** String con factor de conversion */
   std::string datasourcefields_; /** String con campo y fuente de datos a la que corresponden */
};

} /** namespace suri */

#endif /* VECTOROPERATIONPROCESS_H_ */
