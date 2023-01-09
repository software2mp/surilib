/*! \file VectorEditionControllerCommand.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "VectorEditionControllerCommand.h"
#include "suri/messages.h"
#include "resources.h"
#include "VectorEditorButton.h"
#include "suri/StateChangeNotification.h"

// Includes Wx
// Defines

// forwards

namespace suri {

VectorEditionControllerCommand::VectorEditionControllerCommand() :
      pParams_(NULL), active_(false), enable_(true) {
}

VectorEditionControllerCommand::~VectorEditionControllerCommand() {
}

/**
 * Metodo que se llama para realizar la accion. Debe redefinirse en las
 * clases hijas.
 */
bool VectorEditionControllerCommand::Execute() {
   std::pair<std::multimap<std::string, VectorEditorButton*>::iterator,
         std::multimap<std::string, VectorEditorButton*>::iterator> iterator;
   // recorro los grupos de botones para actualizar su estado
   for (size_t i = 0; i < activeToolGroup_.size(); ++i) {
      if (active_)
         activeToolGroup_[i]->Enable();
      else
         activeToolGroup_[i]->Disable();
   }

   for (size_t i = 0; i < deactiveToolGroup_.size(); ++i) {
      if (!active_)
         deactiveToolGroup_[i]->Enable();
      else
         deactiveToolGroup_[i]->Disable();
   }

   VectorEditionStateChanged(active_);
   return true;
}

/** Obtiene el id del comando **/
Command::CommandIdType VectorEditionControllerCommand::GetId() const {
   return -1;
}

/** Obtiene el id del grupo **/
Command::CommandExclusionGroupType VectorEditionControllerCommand::GetGroupId() const {
   return 0;
}

/** Obtiene el tipo de comando **/
Command::CommandType VectorEditionControllerCommand::GetType() const {
   return "null";
}

/** Obtiene la descripcion del comando **/
const std::string VectorEditionControllerCommand::GetDescription() const {
   return tooltip_TASK_MENU;
}

/** */
const std::string VectorEditionControllerCommand::GetDecoration() const {
   return button_START_ANIMATION_SRC;
}

/** Obtiene la ubicacion **/
const std::string VectorEditionControllerCommand::GetLocation() const {
   return "";
}

/** Configura el id al comando **/
void VectorEditionControllerCommand::SetId(const CommandIdType &Id) {
}

/** Configura el group id **/
void VectorEditionControllerCommand::SetGroupId(
      const CommandExclusionGroupType &GroupId) {
}

/** Configura el tipo de comando **/
void VectorEditionControllerCommand::SetType(const CommandType &Type) {
}

/** Configura la descripcion **/
void VectorEditionControllerCommand::SetDescription(const std::string &Description) {
}

/** Configura la decoracion al comando (ej. el bitmap asociado) **/
void VectorEditionControllerCommand::SetDecoration(const std::string &Decoration) {
}

/** Configra la ubicacion del comando */
void VectorEditionControllerCommand::SetLocation(const std::string &Location) {
}

/** Indica si el comando se encuentra activo **/
bool VectorEditionControllerCommand::IsActive() const {
   return active_;
}

/** Activa/desactiva el comando **/
void VectorEditionControllerCommand::SetActive(bool Active) {
   active_ = Active;

}

/** Indica si el comando se encuenta habilitado **/
bool VectorEditionControllerCommand::IsEnabled() const {
   return enable_;
}

/** Habilita/deshabilita un comando **/
void VectorEditionControllerCommand::SetEnabled(bool Enable) {
   enable_ = Enable;
}

/** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
void VectorEditionControllerCommand::SetInputParameters(
      ParameterCollection* pParameters) {
   pParams_ = pParameters;
}
/** Obtiene los parametros de entrada del command*/
ParameterCollection* VectorEditionControllerCommand::GetInputParameters() const {
   return pParams_;
}
/** Indica los parametros de salida al command (ej. seleccion de un choice)*/
void VectorEditionControllerCommand::SetOutputParameters(
      ParameterCollection* pParameters) {
}

/** Obtiene los parametros de salida del command */
ParameterCollection* VectorEditionControllerCommand::GetOutputParameters() const {
   return NULL;
}

/** Obtiene el nombre de la tarea asociada (si existe alguna) */
std::string VectorEditionControllerCommand::GetAssociatedTaskId() const {
   return "null";
}

/** Establece el nombre de la tarea asociada (si existe alguna) */
void VectorEditionControllerCommand::SetAssociatedTaskId(std::string TaskId) {

}

void VectorEditionControllerCommand::AddButtonToActivateGroup(
      VectorEditorButton* pButton) {
   activeToolGroup_.push_back(pButton);
}

void VectorEditionControllerCommand::AddButtonToDeactivateGroup(
      VectorEditorButton* pButton) {
   deactiveToolGroup_.push_back(pButton);
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void VectorEditionControllerCommand::Update(NotificationInterface* pNotification) {
   StateChangeNotification* pnotif =
         dynamic_cast<StateChangeNotification*>(pNotification);
   if (pnotif) {
      active_ = !active_;
      Execute();
   }
}

/** Metodo que se llama cuando inicia/finaliza la edicion vectorial.**/
void VectorEditionControllerCommand::VectorEditionStateChanged(bool Started) {
   for (size_t i = 0; i < activeToolGroup_.size(); ++i) {
      if (Started)
         activeToolGroup_[i]->OnVectorEditionStarted();
      else
         activeToolGroup_[i]->OnVectorEditionFinished();
   }
   for (size_t i = 0; i < deactiveToolGroup_.size(); ++i) {
      if (Started)
         deactiveToolGroup_[i]->OnVectorEditionStarted();
      else
         deactiveToolGroup_[i]->OnVectorEditionFinished();
   }
}

} /** namespace suri */
