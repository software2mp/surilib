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

#ifndef QUICKMEASSURECOMMANDEXECUTIONHANDLER_H_
#define QUICKMEASSURECOMMANDEXECUTIONHANDLER_H_

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;
class MeassureWidget;

namespace core {

/** Clase encargada de administrar la ejecucion del comando QuickMeassure **/
class QuickMeassureCommandExecutionHandler : public CommandExecutionHandlerInterface {
   /** Constructor Copia **/
   QuickMeassureCommandExecutionHandler(const QuickMeassureCommandExecutionHandler&);
public:
   /** Constructor **/
   QuickMeassureCommandExecutionHandler(
         DataViewManager* pDataView, suri::MeassureWidget* pQuickMeasure);
   /** Destructor **/
   virtual ~QuickMeassureCommandExecutionHandler();
   /**
    * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
    * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    */
   virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
   /** Puntero a DataViewManager */
   DataViewManager* pDataView_;
   /** Puntero a MeassureWidget */
   suri::MeassureWidget* pQuickMeasure_;
};

} /** namespace core */
} /** namespace suri */

#endif /* QUICKMEASSURECOMMANDEXECUTIONHANDLER_H_ */
