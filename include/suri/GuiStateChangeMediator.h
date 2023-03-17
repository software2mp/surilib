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

#ifndef GUISTATECHANGEMEDIATOR_H_
#define GUISTATECHANGEMEDIATOR_H_

// Includes stadard
#include <map>
#include <string>

// Includes suri
#include "suri/GuiStateInterface.h"
#include "suri/ObserverInterface.h"
#include "suri/Command.h"

namespace suri {

class Widget;

/**
 * Recibe eventos y modifica el estado de la aplicacion
 */
class GuiStateChangeMediator : public ObserverInterface {
public:
   /** Ctor */
   GuiStateChangeMediator(Widget *pMainWidget);
   /** Dtor */
   virtual ~GuiStateChangeMediator();

   /** Configura el estado del mediador */
   void SetState(GuiStateInterface* pGuiStateInterface);

   /** Elimina todos los commands registrados. */
   void ResetCommandList();
   /** Elimina todos los commands registrados. */
   void RemoveCommand(const std::string &CommandId);
   /** Registra un comando en el tool indicado. */
   void RegisterCommand(const std::string &CommandGuiId, Command* pCommand);
   /** Actualiza estado de la aplicacion a partir del evento */
   virtual bool StateChangeEvent(const std::string EventId,
                                       const std::string SourceId);

   /** Modifica el estado de habilitacion de la herramienta */
   virtual bool EnableCommand(const std::string CommandId, bool NewState);
   /** Modifica el estado de activacion de la herramienta */
   virtual bool ActivateCommand(const std::string CommandId, bool NewState);


   // --- Metodos auxiliares - mantienen la interfaz grafica consistente ---
   /** Actualiza el estado de activacion de la herramienta en la interfaz grafica */
   virtual bool UpdateCommandGuiState(const std::string CommandId);
   /** Modifica el estado de activacion de la herramienta */
   virtual bool DesactivateConflictingCommands(const std::string CommandId);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

   // Eventos
   static const std::string StartTask;
   static const std::string EndTask;
   static const std::string Initialize;
   static const std::string Activate;
   static const std::string Deactivate;
   static const std::string DeleteCommand;

   // Sources
   static const std::string VectorEditionTask;
   static const std::string ClassificationTask;
   static const std::string CoregisterTask;
   static const std::string GeoreferenceTask;
   static const std::string AnimationTask;
   static const std::string MapEditionTask;

private:
   /** Modifica estado del item en menu en caso de existir. */
   virtual bool EnableCommandMenuItem(const std::string CommandLocations,
                                                             bool NewState);
   /** Modifica estado del item en menu en caso de existir. */
   virtual bool ActivateCommandMenuItem(const std::string CommandLocations,
                                                             bool NewState);

   /** Mapa con los comandos registrados */
   typedef std::map<std::string, Command*> CommandMap;
   CommandMap commands_;
   /** Estado de la interfaz. Clase con logica que responde a un evento */
   GuiStateInterface* pGuiStateInterface_;
   /** Widget de la ventana principal */
   Widget *pMainWidget_;
};

} /* namespace suri */
#endif /* GUISTATECHANGEMEDIATOR_H_ */
