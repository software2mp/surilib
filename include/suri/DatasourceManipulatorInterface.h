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

#ifndef DATASOURCEMANIPULATOR_H_
#define DATASOURCEMANIPULATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "DatasourceInterface.h"
#include "suri/Option.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * La clase Manipulator provee una interfaz que permite manipular diferentes tipos de objetos
 * de manera prolija y con abstraccion de cada tipo en particular
 */
class DatasourceManipulatorInterface {
public:
   /** Constructor */
   DatasourceManipulatorInterface() {}
   /** Destructor */
   virtual ~DatasourceManipulatorInterface() {}
   /** Crea una nueva fuente de datos que cumple con los requisitos de validacion de Enclosure */
   virtual DatasourceInterface* Create(const std::string& FileName,
                                       const Option& option) const = 0;
   /** Efectua la logica de agregado para un Datasource validado */
   virtual bool AddNature(DatasourceInterface* pDatasource) const = 0;
   /** Efectua la logica de limpieza para un Datasource validado */
   virtual bool CleanNature(DatasourceInterface* pDatasource) const = 0;
   /** Efectua la logica de copia de un Datasource a otro */
   virtual bool CopyNature(DatasourceInterface* pSourceDatasource,
                 DatasourceInterface* pDestinationDatasource) const = 0;
};

} /** namespace suri */

#endif /* DATASOURCEMANIPULATOR_H_ */
