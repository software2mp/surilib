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

#ifndef VECTOREDITIONCONTROLLERCOMMAND_H_
#define VECTOREDITIONCONTROLLERCOMMAND_H_

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "suri/Command.h"
// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class VectorEditorButton;

/**
 * Clase encargada de sincronizar los estados de los botones de edicion
 * vectorial
 */
class VectorEditionControllerCommand : public suri::Command,
                                          public ObserverInterface {
   /** constructor copia **/
   VectorEditionControllerCommand(const VectorEditionControllerCommand&);

public:
   /** ctor **/
   VectorEditionControllerCommand();
   /** dtor **/
   virtual ~VectorEditionControllerCommand();
   /**
    * Metodo que se llama para realizar la accion. Debe redefinirse en las
    * clases hijas.
    */
   virtual bool Execute();
   /** Obtiene el id del comando **/
   virtual CommandIdType GetId() const;
   /** Obtiene el id del grupo **/
   virtual CommandExclusionGroupType GetGroupId() const;
   /** Obtiene el tipo de comando **/
   virtual CommandType GetType() const;
   /** Obtiene la descripcion del comando **/
   virtual const std::string GetDescription() const;
   /** */
   virtual const std::string GetDecoration() const;
   /** Obtiene la ubicacion **/
   virtual const std::string GetLocation() const;
   /** Configura el id al comando **/
   virtual void SetId(const CommandIdType &Id);
   /** Configura el group id **/
   virtual void SetGroupId(const CommandExclusionGroupType &GroupId);
   /** Configura el tipo de comando **/
   virtual void SetType(const CommandType &Type);
   /** Configura la descripcion **/
   virtual void SetDescription(const std::string &Description);
   /** Configura la decoracion al comando (ej. el bitmap asociado) **/
   virtual void SetDecoration(const std::string &Decoration);
   /** Configra la ubicacion del comando */
   virtual void SetLocation(const std::string &Location);
   /** Indica si el comando se encuentra activo **/
   virtual bool IsActive() const;
   /** Activa/desactiva el comando **/
   virtual void SetActive(bool Active = true);
   /** Indica si el comando se encuenta habilitado **/
   virtual bool IsEnabled() const;
   /** Habilita/deshabilita un comando **/
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
   /**
    * Agrega el boton al grupo de botones que se habilitan cuando se
    * inicia la edicion vectorial.
    * **/
   void AddButtonToActivateGroup(VectorEditorButton* pButton);
   /**
    * Agrega el boton al grupo de botones que se habilitan cuando se
    * finaliza la edicion vectorial.
    * **/
   void AddButtonToDeactivateGroup(VectorEditorButton* pButton);
   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

protected:
   /** Metodo que se llama cuando inicia/finaliza la edicion vectorial.**/
   void VectorEditionStateChanged(bool Started = true);
   ParameterCollection* pParams_;
   std::vector<VectorEditorButton*> activeToolGroup_;
   std::vector<VectorEditorButton*> deactiveToolGroup_;
   bool active_;
   bool enable_;
};

} /** namespace suri */

#endif /* VECTOREDITIONCONTROLLERCOMMAND_H_ */
