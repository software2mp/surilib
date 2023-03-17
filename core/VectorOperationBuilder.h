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

#ifndef VECTOROPERATIONBUILDER_H_
#define VECTOROPERATIONBUILDER_H_

// Includes Estandar
#include <vector>
#include <string>
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class VectorOperationBuilder {
public:
   /** Constructor */
   VectorOperationBuilder(Option& Parameters);
   /** Destructor */
   virtual ~VectorOperationBuilder();
   /** Obtiene una instancia de la clase correspondiente a la operacion que quiera realizarse */
   VectorOperation* GetOperation(VectorOperation::OperationType Operation);
   static const std::string DistanceFieldKey; /** Clave de campo de distancia de buffer */
   static const std::string DistanceValueKey; /** Clave de distancia de buffer */
   static const std::string NRingsKey; /** Clave de numero de anillos concentricos */
   static const std::string ConversionFactorKey; /** Clave de factor de conversion */
   static const std::string MergeFieldsMapKey; /** Clave de campos a fusionar */

private:
   double distance_; /** Distancia de buffer */
   int nrings_; /** Numero de anillos concentricos */
   std::string distancefield_; /** Campo de distancia de buffer */
   double conversionfactor_; /** Factor de conversion */
   VectorOperation::OperationType operation_; /** Vector de operaciones */
   Option operationOptions_;
};

} /** namespace suri */

#endif /* VECTOROPERATIONBUILDER_H_ */
