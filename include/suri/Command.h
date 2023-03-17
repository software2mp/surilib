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

#ifndef COMMAND_H_
#define COMMAND_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Subject.h"

// Includes Wx

// Defines

// forwards

namespace suri {
// forwards
class ParameterCollection;

/**
 * Interfaz que permite realizar una accion sin saber la implementacion de
 * la misma.
 */
class Command : public Subject {
   /** Constructor copia **/
   Command(const Command&);

protected:
   /** Constructor **/
   Command() {
   }

public:
   typedef int CommandIdType;
   typedef int CommandExclusionGroupType;
   typedef std::string CommandType;
   /*! Tipo de comando que no interfiere con otros */
   static const CommandExclusionGroupType Standalone;
   /** Destructor **/
   virtual ~Command() {
   }

   /**
    * Metodo que se llama para realizar la accion. Debe redefinirse en las
    * clases hijas.
    */
   virtual bool Execute()=0;
   /** Obtiene el id del comando **/
   virtual CommandIdType GetId() const=0;
   /** Obtiene el id del grupo **/
   virtual CommandExclusionGroupType GetGroupId() const=0;
   /** Obtiene el tipo de comando **/
   virtual CommandType GetType() const=0;
   /** Obtiene la descripcion del comando **/
   virtual const std::string GetDescription() const=0;
   /** */
   virtual const std::string GetDecoration() const=0;
   /** Obtiene la ubicacion **/
   virtual const std::string GetLocation() const=0;
   /** Configura el id al comando **/
   virtual void SetId(const CommandIdType &Id)=0;
   /** Configura el group id **/
   virtual void SetGroupId(const CommandExclusionGroupType &GroupId)=0;
   /** Configura el tipo de comando **/
   virtual void SetType(const CommandType &Type)=0;
   /** Configura la descripcion **/
   virtual void SetDescription(const std::string &Description)=0;
   /** Configura la decoracion al comando (ej. el bitmap asociado) **/
   virtual void SetDecoration(const std::string &Decoration)=0;
   /** Configra la ubicacion del comando */
   virtual void SetLocation(const std::string &Location)=0;
   /** Indica si el comando se encuentra activo **/
   virtual bool IsActive() const=0;
   /** Activa/desactiva el comando **/
   virtual void SetActive(bool Active = true)=0;
   /** Indica si el comando se encuenta habilitado **/
   virtual bool IsEnabled() const=0;
   /** Habilita/deshabilita un comando **/
   virtual void SetEnabled(bool Enable = true)=0;
   /** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
   virtual void SetInputParameters(ParameterCollection* pParameters)=0;
   /** Obtiene los parametros de entrada del command*/
   virtual ParameterCollection* GetInputParameters()const=0;
   /** Indica los parametros de salida al command (ej. seleccion de un choice)*/
   virtual void SetOutputParameters(ParameterCollection* pParameters)=0;
   /** Obtiene los parametros de salida del command */
   virtual ParameterCollection* GetOutputParameters()const=0;

   /** Obtiene el nombre de la tarea asociada (si existe alguna) */
   virtual std::string GetAssociatedTaskId() const = 0;

   /** Establece el nombre de la tarea asociada (si existe alguna) */
   virtual void SetAssociatedTaskId(std::string TaskId) = 0;

protected:
private:
};

} /* namespace suri */

#endif /* COMMAND_H_ */
