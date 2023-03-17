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

#ifndef VECTOROPERATION_H_
#define VECTOROPERATION_H_

// Includes Estandar
#include <stddef.h>
#include "suri/DatasourceInterface.h"
// Includes Suri
// Includes Wx
// Defines
// forwards
class OGRFeature;

namespace suri {
// forwards
class VectorDatasource;

class VectorOperation {
public:
   /** Tipos de operaciones vectoriales */
   typedef enum {
      Union = 0, Intersection = 1, Trim = 2, Buffer = 3, MergeLayers = 4
   } OperationType;
   static const char* OperationString[];
   /** Constructor */
   VectorOperation();
   /** Destructor */
   virtual ~VectorOperation();
   /** Intenta procesar las fuentes de datos en las clases que implementan DoProcessDatasource */
   VectorDatasource* ProcessDatasource(OperationType& OpType,
                                       std::vector<DatasourceInterface*>& Datasources) {
      VectorDatasource* pds = DoProcessDatasource(OpType, Datasources);
      if (!pds && pSucesor_) {
         pds = pSucesor_->ProcessDatasource(OpType, Datasources);
      }
      return pds;
   }

   void AddSucesor(VectorOperation* pOperation) {
      if (pSucesor_ == NULL) {
         pSucesor_ = pOperation;
         return;
      }
      VectorOperation* pnext = pSucesor_;
      while (pnext->pSucesor_) {
         pnext = pnext->pSucesor_;
      }
      pnext->pSucesor_ = pOperation;
   }

protected:
   /** Genera un archivo shp en el directorio temporal del sistema */
   std::string GetTempFile();
   /** Procesa las fuentes de datos de entrada generando la operacion correspondiente
    * de ambas en una nueva fuente de datos */
   virtual VectorDatasource* DoProcessDatasource(
         OperationType& OpType, std::vector<DatasourceInterface*>& Datasources)=0;
   /** Copia los campos de una capa origen a una capa destino */
   void CreateFields(OGRLayer* pLayer, OGRLayer* pDestLayer);
   /** Copia el contenido de los campos de un feature origen a los campos correspondientes del
    * feature destino */
   void CopyFieldsFromOrigin(OGRFeature *pFeature, OGRFeature *pDestFeature, int FieldCount,
                             int Field = 0);
   /**
    * Verifica que todas las capas de las fuentes de datos vectoriales sean del mismo tipo
    * @return true si todas las capas son del mismo tipo, caso contrario false
    */
   bool VerifyLayersType(std::vector<DatasourceInterface*> Datasources);
   VectorOperation* pSucesor_;
};

} /** namespace suri */

#endif /* VECTOROPERATION_H_ */
