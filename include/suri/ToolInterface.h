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

#ifndef TOOLINTERFACE_H_
#define TOOLINTERFACE_H_

// Includes Estandar
#include <string>
#include <list>
#include <map>

// Includes Suri
#include "suri/Command.h"
#include "suri/MouseEventInterface.h"
#include "suri/PaintEventInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class Widget;

class ToolInterface {
   ToolInterface(const ToolInterface&);
protected:
   ToolInterface() {
   }
public:
   virtual ~ToolInterface() {
   }

   virtual bool IsActive(const Command *pToolCommand) const=0;
   virtual void SetActive(Command *pToolCommand, bool Active = true)=0;
   virtual bool IsEnabled(const Command *pToolCommand) const=0;
   virtual void SetEnabled(Command *pToolCommand, bool Enable = true)=0;

   virtual void Execute(const Command *pToolCommand)=0;

   virtual std::list<Command*> GetCommands() const=0;
   virtual Command* GetCommand(const Command::CommandIdType &Id) const=0;
   virtual std::list<MouseEventInterface*> GetMouseEvents() const=0;
   virtual std::list<PaintEventInterface*> GetPaintEvents() const=0;
   virtual std::multimap<std::string, Widget*> GetWidgets() const=0;

   /** Retorna el id del comando en la interfaz grafica */
   virtual std::string GetCommandGuiId(const Command* pCommand) const = 0;
};

} /* namespace suri */

#endif /* TOOLINTERFACE_H_ */
