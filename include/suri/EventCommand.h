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

#ifndef EVENTCOMMAND_H_
#define EVENTCOMMAND_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Command.h"
#include "suri/MouseEventInterface.h"
#include "suri/PaintEventInterface.h"
#include "suri/SuriObject.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *
 */
class EventCommand : public Command {
public:
   EventCommand(MouseEventInterface *pMouseEvent, PaintEventInterface *pPaintEvent,
                const CommandIdType &Id = SuriObject::NullIntUuid,
                const CommandType &Type = "None", const std::string &Decoration = "",
                const std::string &Description = "", const std::string &Location = "");
   virtual ~EventCommand();

   virtual bool Execute();

   virtual CommandIdType GetId() const;
   virtual CommandExclusionGroupType GetGroupId() const;
   virtual CommandType GetType() const;

   /** Tooltip / Help */
   virtual const std::string GetDescription() const;
   /** bitmap asociado */
   virtual const std::string GetDecoration() const;
   /** ruta (xpath, ramas, etc) del menu donde aparece, hasta el texto del item. */
   virtual const std::string GetLocation() const;

   virtual void SetId(const CommandIdType &Id);
   virtual void SetGroupId(const CommandExclusionGroupType &GroupId);
   virtual void SetType(const CommandType &Type);
   virtual void SetDescription(const std::string &Description);
   virtual void SetDecoration(const std::string &Decoration);
   virtual void SetLocation(const std::string &Location);

   virtual bool IsActive() const;
   virtual void SetActive(bool Active = true);
   virtual bool IsEnabled() const;
   virtual void SetEnabled(bool Enable = true);
   /** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
   virtual void SetInputParameters(ParameterCollection* pParameters);
   /** Obtiene los parametros de entrada del command*/
   virtual ParameterCollection* GetInputParameters() const;
   /** Indica los parametros de salida al command (ej. seleccion de un choice)*/
   virtual void SetOutputParameters(ParameterCollection* pParameters);
   /** Obtiene los parametros de salida del command */
   virtual ParameterCollection* GetOutputParameters() const;

   /** Obtiene el nombre de la tarea asociada (si existe alguna) */
   virtual std::string GetAssociatedTaskId() const;

   /** Establece el nombre de la tarea asociada (si existe alguna) */
   virtual void SetAssociatedTaskId(std::string TaskId);

private:
   CommandIdType id_;
   CommandType type_;
   std::string decoration_;
   std::string description_;
   std::string location_;
   MouseEventInterface *pMouseEvent_;
   PaintEventInterface *pPaintEvent_;
   bool active_;
   bool enabled_;
   std::string associatedTaskId_;
};

} /* namespace suri */

#endif /* EVENTCOMMAND_H_ */
