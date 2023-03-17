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

// Includes standard
#include <utility>
#include <string>
#include <vector>

// Includees suri
#include "suri/GuiStateChangeMediator.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Widget.h"

// Includes Wx
#include "wx/menuitem.h"
#include "wx/xrc/xmlres.h"
#include "wx/window.h"
#include "wx/wx.h"

namespace suri {

// Eventos
const std::string GuiStateChangeMediator::StartTask = "StartTask";
const std::string GuiStateChangeMediator::EndTask = "EndTask";
const std::string GuiStateChangeMediator::Initialize = "Initialize";
const std::string GuiStateChangeMediator::Activate = "Activate";
const std::string GuiStateChangeMediator::Deactivate = "Deactivate";
const std::string GuiStateChangeMediator::DeleteCommand = "DeleteCommand";

// Sources
const std::string GuiStateChangeMediator::VectorEditionTask = "VectorEditionTask";
const std::string GuiStateChangeMediator::ClassificationTask = "ClassificationTask";
const std::string GuiStateChangeMediator::CoregisterTask = "CoregisterTask";
const std::string GuiStateChangeMediator::GeoreferenceTask = "GeoreferenceTask";
const std::string GuiStateChangeMediator::AnimationTask = "AnimationTask";
const std::string GuiStateChangeMediator::MapEditionTask = "MapEditionTask";

/** Ctor */
GuiStateChangeMediator::GuiStateChangeMediator(Widget *pMainWidget) {
   pGuiStateInterface_ = NULL;
   pMainWidget_ = pMainWidget;
}

/** Dtor */
GuiStateChangeMediator::~GuiStateChangeMediator() {
   delete pGuiStateInterface_;
}

/**
 * Configura el estado del mediador
 * @param[in] pGuiStateInterface nuevo estado de la Gui
 */
void GuiStateChangeMediator::SetState(GuiStateInterface* pGuiStateInterface) {
   pGuiStateInterface_ = pGuiStateInterface;
}

/** Elimina todos los commands registrados. */
void GuiStateChangeMediator::ResetCommandList() {
   commands_.clear();
}

/**
 * Elimina todos los commands registrados.
 * @param[in] CommandId id del comando a eliminar. 
 * Puede ser la posicion del comando en interfaz grafica. Esta formado
 * <nombre tool>|<nombre comando> o el id del comando en forma de string.
 */
void GuiStateChangeMediator::RemoveCommand(const std::string &CommandId) {
   CommandMap::iterator it = commands_.find(CommandId);
   if (it == commands_.end())
      return;
   Command* pcommand = it->second;

   it = commands_.begin();
   while (it != commands_.end()) {
      if (it->second == pcommand)
         commands_.erase(it++);
      else
         ++it;
   }
}

/**
 * Registra un comando para que pueda modificar el estado en el futuro
 * @param[in] CommandGuiId posicion del comando en interfaz grafica. Esta formado
 * por <nombre tool>|<nombre comando>
 * @param[in] pCommand comando a registrar
 */
void GuiStateChangeMediator::RegisterCommand(
               const std::string &CommandGuiId, Command* pCommand) {
   CommandMap::iterator it = commands_.find(CommandGuiId);
   if (it == commands_.end()) {
      commands_.insert(std::pair<std::string, Command*>(CommandGuiId, pCommand));
      std::string idasstring = IntToString(pCommand->GetId());
      commands_.insert(std::pair<std::string, Command*>(idasstring, pCommand));
   }
}

/**
 * Actualiza estado de la aplicacion a partir del evento
 * @param[in] EventId id del evento que ocurrio
 * @param[in] SourceId id del componente/entidad que genero el evento.
 * @return bool que indica si pudo cambiar estado
 */
bool GuiStateChangeMediator::StateChangeEvent(const std::string EventId,
                                                const std::string SourceId) {
   return pGuiStateInterface_ ?
            pGuiStateInterface_->StateChangeEvent(EventId, SourceId) : false;
}

/**
 * Determina si un command esta habilitado o no. Modifica estado del boton e
 * item en menu en caso de existir.
 * @param[in] CommandId id del comando a habilitar
 * @param[in] NewState nuevo estado del comando.
 * @return bool que indica si pudo habilitar/deshabilitar el comando.
 */
bool GuiStateChangeMediator::EnableCommand(const std::string CommandId, bool NewState) {
   CommandMap::iterator it = commands_.find(CommandId);
   if (it != commands_.end()) {
      it->second->SetEnabled(NewState);
      /**
       * \todo (ramirogonzalez-#3860): En vez de actualizar item directamente,
       * usar notificacion disparada por command(Idem ActivateCommand).
       * De esta forma el item se actualiza aunque no se cambie el estado
       * de habilitacion usando GuiStateChangeMediator
       */
      EnableCommandMenuItem(it->second->GetLocation(), NewState);
   }
   return it != commands_.end();
}

/** 
 * Modifica el estado de activacion de la herramienta 
 * @param[in] CommandId id del comando a activar
 * @param[in] NewState nuevo estado de activacion
 * @return bool que indica si pudo activar el comando
 */
bool GuiStateChangeMediator::ActivateCommand(const std::string CommandId, bool NewState) {
   CommandMap::iterator it = commands_.find(CommandId);
   if (it != commands_.end())
      it->second->SetActive(NewState);

   return it != commands_.end();
}

/**
 * Modifica el estado de activacion de la herramienta en GUI
 * @param[in] CommandId id del comando a activar
 * @return bool que indica si pudo encontrar el comando
 */
bool GuiStateChangeMediator::UpdateCommandGuiState(const std::string CommandId) {
   CommandMap::iterator it = commands_.find(CommandId);
   if (it != commands_.end())
      ActivateCommandMenuItem(it->second->GetLocation(), it->second->IsActive());

   return it != commands_.end();
}

/** 
 * Desactiva las herramientas activas que tiene conflictos con el comando. Por
 * ejemplo si ambas herramientas usan el mouse.
 * @param[in] CommandId id del comando
 * @return bool que indica si pudo encontrar el comando
 */
bool GuiStateChangeMediator::DesactivateConflictingCommands(
                                               const std::string CommandId) {
   CommandMap::iterator it = commands_.find(CommandId);
   if (it == commands_.end())
      return false;
   
   Command* pcommand = it->second;
   if (pcommand->IsActive()) {
      CommandMap::iterator it2 = commands_.begin();
      for (; it2 != commands_.end(); ++it2) {
         std::string commandid = it2->first;
         Command* pothercommand = it2->second;
         if (pothercommand->IsActive() && pothercommand != pcommand) {
            if (pcommand->GetGroupId() > 0 && pothercommand->GetGroupId() > 0 &&
                  (pothercommand->GetGroupId() & pcommand->GetGroupId()) != 0)
               pothercommand->SetActive(false);
            if (pcommand->GetGroupId() < 0 &&
                  pothercommand->GetGroupId() == pcommand->GetGroupId())
               pothercommand->SetActive(false);
         }
      }
   }

   return true;
}

/**
 * Modifica estado del item en menu en caso de existir.
 * @param[in] CommandId id del comando a habilitar
 * @param[in] NewState nuevo estado del comando.
 * @return bool que indica si pudo habilitar/deshabilitar el comando.
 */
bool GuiStateChangeMediator::EnableCommandMenuItem(
                  const std::string CommandLocations, bool NewState) {
   wxFrame *pframe = dynamic_cast<wxFrame*>(pMainWidget_->GetFrame());
   wxMenuBar *pmenubar = pframe->GetMenuBar();
   wxMenuItem *pitem = NULL;

   std::vector<std::string> location = tokenizer(CommandLocations, "|");
   std::vector<std::string>::iterator it = location.begin();
   for (; it != location.end() && !pitem; ++it) {
      int location = wxXmlResource::Get()->GetXRCID(it->c_str(), -2);
      pitem = location > 0 ? pmenubar->FindItem(location) : NULL;
      if (pitem)
         pitem->Enable(NewState);
   }
   return pitem != NULL;
}

/**
 * Modifica activacion del item en menu en caso de existir.
 * @param[in] CommandId id del comando a habilitar
 * @param[in] NewState nuevo estado del comando.
 * @return bool que indica si pudo habilitar/deshabilitar el comando.
 */
bool GuiStateChangeMediator::ActivateCommandMenuItem(
                  const std::string CommandLocations, bool NewState) {
   wxFrame *pframe = dynamic_cast<wxFrame*>(pMainWidget_->GetFrame());
   wxMenuBar *pmenubar = pframe->GetMenuBar();
   wxMenuItem *pitem = NULL;

   std::vector<std::string> location = tokenizer(CommandLocations, "|");
   std::vector<std::string>::iterator it = location.begin();
   for (; it != location.end() && !pitem; ++it) {
      int location = wxXmlResource::Get()->GetXRCID(it->c_str(), -2);
      pitem = location > 0 ? pmenubar->FindItem(location) : NULL;
      if (pitem && pitem->IsCheckable())
         pitem->Check(NewState);
   }
   return pitem != NULL;
}


/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void GuiStateChangeMediator::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

} /* namespace suri */
