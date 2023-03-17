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

#ifndef GUIINTERFACE_H_
#define GUIINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Command.h"

// Includes Wx
#include "wx/wx.h"
// Defines
// forwards

namespace suri {

// forwards
class Widget;
class ToolInterface;

/*!
 * Interfaz que representa el comportamiento comun
 * a todos los administradores de la aplicacion
 * (a nivel GUI)
 */
class GuiInterface {
public:
   GuiInterface(Widget *pWidget) {
   }
   virtual ~GuiInterface() {
   }
   /** Agrega la herramienta en la ubicacion de los Commands asociados**/
   virtual bool AddTool(ToolInterface* pTool) = 0;
   /** Elimina la herramienta de las ubicaciones del os Commandas asociados **/
   virtual bool RemoveTool(ToolInterface* pTool) = 0;
   virtual void AddToolbar(Widget *pWidget)=0;
   /** Elimina el Toolbar en la ubicacion indicada por parametro**/
   virtual void RemoveToolbar(const std::string& LocationId)=0;
protected:
   /** Metodo que agrega un menu en la ubicacion descripta por el command**/
   virtual void AddMenu(Command *pCommand, const std::string& MenuLocation)=0;
   /** Elimina un menu **/
   virtual void RemoveMenu(const std::string& MenuLocation)=0;
   /** Agrega widget **/
   virtual void AddWidget(Widget *pWidget)=0;
   /** Elimina widget **/
   virtual void RemoveWidget()=0;

private:
};

} /* namespace suri */

#endif /* GUIINTERFACE_H_ */
