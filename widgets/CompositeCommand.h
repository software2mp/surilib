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

#ifndef COMPOSITECOMMAND_H_
#define COMPOSITECOMMAND_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/Command.h"
#include "suri/SuriObject.h"

namespace suri {

class ParameterCollection;

/**
 * Command que permite agrupar otros commands.
 * Todos los comandos que se agregan comparten el id. Caso contrario no
 * funcionan los ToolCommand contenidos.
 */
class CompositeCommand : public Command {
public:
   /** Ctor */
   explicit CompositeCommand(const CommandIdType &Id = SuriObject::NullIntUuid);
   /** Dtor */
   virtual ~CompositeCommand();

   /** Agrega un command al composite */
   void AppendCommand(Command* pCommand);
   /** Limpia los commands */
   void ClearCommands();

   /** Ejecuta el comando */
   virtual bool Execute();

   /** Retorna el id del comando */
   virtual CommandIdType GetId() const;
   /** Retorna el id del grupo */
   virtual CommandExclusionGroupType GetGroupId() const;
   /** Retorna el tipo de commando */
   virtual CommandType GetType() const;
   /** Tooltip / Help */
   virtual const std::string GetDescription() const;
   /** bitmap asociado */
   virtual const std::string GetDecoration() const;
   /** ruta (xpath, ramas, etc) del menu donde aparece, hasta el texto del item. */
   virtual const std::string GetLocation() const;

   /** Configura el id del comando */
   virtual void SetId(const CommandIdType &Id);
   /** Configura el id de grupo */
   virtual void SetGroupId(const CommandExclusionGroupType &GroupId);
   /** Configura el tipo del commando */
   virtual void SetType(const CommandType &Type);
   /** Configura la descipcion - Tooltip / Help */
   virtual void SetDescription(const std::string &Description);
   /** Configura la decoracion - bitmap asociado */
   virtual void SetDecoration(const std::string &Decoration);
   /** Configura la locacion */
   virtual void SetLocation(const std::string &Location);

   /** Indica si esta activo */
   virtual bool IsActive() const;
   /** Configura si esta activo */
   virtual void SetActive(bool Active = true);
   /** Indica si esta habilitado */
   virtual bool IsEnabled() const;
   /** Configura si esta habilitado */
   virtual void SetEnabled(bool Enable = true);
   /** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
   virtual void SetInputParameters(ParameterCollection* pParameters);
   /** Obtiene los parametros de entrada del command*/
   ParameterCollection* GetInputParameters() const;
   /** Indica los parametros de salida al command (ej. seleccion de un choice)*/
   virtual void SetOutputParameters(ParameterCollection* pParameters);
   /** Obtiene los parametros de salida del command */
   ParameterCollection* GetOutputParameters() const;

   /** Obtiene el nombre de la tarea asociada (si existe alguna) */
   virtual std::string GetAssociatedTaskId() const;

   /** Establece el nombre de la tarea asociada (si existe alguna) */
   virtual void SetAssociatedTaskId(std::string TaskId);

private:
   CommandExclusionGroupType groupId_;
   CommandIdType id_;
   CommandType type_;
   std::string decoration_;
   std::string description_;
   std::string location_;
   bool active_;
   bool enabled_;
   /** Comandos que agrupa este comando */
   std::vector<Command*> pCommands_;
   std::string associatedTaskId_;
   ParameterCollection* pInputParams_;
};

} /* namespace suri */
#endif /* COMPOSITECOMMAND_H_ */
