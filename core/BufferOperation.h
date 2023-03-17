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

#ifndef BUFFEROPERATION_H_
#define BUFFEROPERATION_H_

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
#include "VectorDatasource.h"
// Includes Wx
// Defines

namespace suri {
// forwards
class VectorDatasource;

class BufferOperation : public suri::VectorOperation {
public:
   /** Constructor */
   BufferOperation(const std::string& FieldName, int Nrings, double Distance,
                   double ConversionFactor);
   /** Destructor */
   virtual ~BufferOperation();

protected:
   /** Retorna el valor de la distancia */
   double GetDistance(double Distance, char* pWkt);
   /** Procesa la fuente de datos de entrada generando un buffer en una nueva fuente de datos */
   virtual VectorDatasource* DoProcessDatasource(VectorOperation::OperationType& Operation,
                                                 std::vector<DatasourceInterface*>& Datasources);
   std::string bufferfield_; /** Nombre del campo que posee la distancia del buffer */
   double distancevalue_; /** Valor de la distancia del buffer */
   int nrings_; /** Numero de anillos concentricos */
   double conversionfactor_; /** Factor de conversion */
};

} /** namespace suri */

#endif /* BUFFEROPERATION_H_ */
