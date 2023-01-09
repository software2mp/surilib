/** \file CompositeCommand.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>

// Includes suri
#include "CompositeCommand.h"
#include "logmacros.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/GuiStateChangeMediator.h"
#include "suri/StateChangeNotification.h"

namespace suri {

/** Ctor */
CompositeCommand::CompositeCommand(const CommandIdType &Id) :
    active_(true), enabled_(false){
   id_ = Id;
   groupId_ = 0;
   pInputParams_ = NULL;
   SetActive(false);
   SetEnabled(true);
}

/** Dtor */
CompositeCommand::~CompositeCommand() {
   std::string commandid = IntToString(GetId());
   suri::StateChangeNotification notif(GuiStateChangeMediator::DeleteCommand, commandid);
   Notify(&notif);
   ClearCommands();
}

/** 
 * Agrega un command al composite 
 * @param[in] pCommand comando que se desea agregar
 */
void CompositeCommand::AppendCommand(Command* pCommand) {
   pCommands_.push_back(pCommand);
}

/** Limpia los commands */
void CompositeCommand::ClearCommands() {
   std::vector<Command*>::iterator it = pCommands_.begin();
   for (; it != pCommands_.end(); ++it)
      delete *it;
   pCommands_.clear();
}

/** 
 * Ejecuta el comando 
 * @return true si todos los comando se ejecutaron exitosamente
 *         false en otro caso.
 */
bool CompositeCommand::Execute() {
   bool result = true;
   std::vector<Command*>::iterator it = pCommands_.begin();
   for (; it != pCommands_.end(); ++it)
      result = result && (*it)->Execute();
   return result;
}

/** 
 * Retorna el id del comando 
 * @return id de comando 
 */
Command::CommandIdType CompositeCommand::GetId() const {
   return id_;
}

/** 
 * Retorna el id del grupo 
 * @return id de grupo
 */
Command::CommandExclusionGroupType CompositeCommand::GetGroupId() const {
   Command::CommandExclusionGroupType groupid = groupId_;
   std::vector<Command*>::const_iterator it = pCommands_.begin();
   for (; it != pCommands_.end(); ++it)
      groupid = groupid | (*it)->GetGroupId();
   return groupid;
}

/** 
 * Retorna el tipo de commando 
 * @return tipo de comando
 */
Command::CommandType CompositeCommand::GetType() const {
   return type_;
}

/** 
 * Tooltip / Help 
 * @return string con la descripcion
 */
const std::string CompositeCommand::GetDescription() const {
   return description_;
}
/** 
 * Decoracion del boton - bitmap asociado 
 * @return string con la decoracion
 */
const std::string CompositeCommand::GetDecoration() const {
   return decoration_;
}

/** ruta (xpath, ramas, etc) del menu donde aparece, hasta el texto del item. */
const std::string CompositeCommand::GetLocation() const {
   return location_;
}

/** 
 * Configura el id del comando 
 * @param[in] Id id de comando
 */
void CompositeCommand::SetId(const CommandIdType &Id) {
   id_ = Id;
   std::vector<Command*>::const_iterator it = pCommands_.begin();
   for (; it != pCommands_.end(); ++it)
      (*it)->SetId(id_);
}

/** 
 * Configura el id de grupo 
 * @param[in] GroupId id de grupo
 */
void CompositeCommand::SetGroupId(const CommandExclusionGroupType &GroupId) {
   groupId_ = GroupId;
}

/** 
 * Configura el tipo del commando 
 * @param[in] Type tipo de comando
 */
void CompositeCommand::SetType(const CommandType &Type) {
   type_ = Type;
}

/** 
 * Configura la descipcion - Tooltip / Help 
 * @param[in] Location nuevo valor de la descripcion
 */
void CompositeCommand::SetDescription(const std::string &Description) {
   description_ = Description;
}

/** 
 * Configura la decoracion - bitmap asociado 
 * @param[in] Location nuevo valor de la decoracion
 */
void CompositeCommand::SetDecoration(const std::string &Decoration) {
   decoration_ = Decoration;
}

/** 
 * Configura la locacion 
 * @param[in] Location nuevo valor de la locacion.
 */
void CompositeCommand::SetLocation(const std::string &Location) {
   location_ = Location;
}

/** 
 * Indica si esta activo 
 * @return bool que indica si esta activo
 */
bool CompositeCommand::IsActive() const {
   return active_;
}

/** 
 * Configura si esta activo 
 * @param[in] Enable bool que indica si esta activo
 */
void CompositeCommand::SetActive(bool Active) {
   if (active_ != Active) {
      std::vector<Command*>::iterator it = pCommands_.begin();
      for (; it != pCommands_.end(); ++it)
         (*it)->SetActive(Active);
      active_ = Active;

      std::string commandid = IntToString(GetId());
      std::string action = active_ ? suri::GuiStateChangeMediator::Activate :
                                       suri::GuiStateChangeMediator::Deactivate;
      suri::StateChangeNotification notif(action, commandid);
      Notify(&notif);

      std::string assoctaskid = GetAssociatedTaskId();
      if (!assoctaskid.empty()) {
         std::string assocaction =
               active_ ? suri::GuiStateChangeMediator::StartTask :
                         suri::GuiStateChangeMediator::EndTask;
         suri::StateChangeNotification notif(assocaction, assoctaskid);
         Notify(&notif);
      }
   }
}

/** 
 * Indica si esta habilitado 
 * @return bool que indica si esta habilitado
 */
bool CompositeCommand::IsEnabled() const {
   return enabled_;
}

/** 
 * Configura si esta habilitado 
 * @param[in] Enable bool que indica si esta habilitado
 */
void CompositeCommand::SetEnabled(bool Enable) {
   std::vector<Command*>::iterator it = pCommands_.begin();
   for (; it != pCommands_.end(); ++it)
      (*it)->SetEnabled(Enable);
   enabled_ = Enable;
}

/** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
void CompositeCommand::SetInputParameters(ParameterCollection* pParameters) {
   pInputParams_ = pParameters;
}

/** Obtiene los parametros de entrada del command*/
ParameterCollection* CompositeCommand::GetInputParameters() const {
   return pInputParams_;
}

/** Indica los parametros de salida al command (ej. seleccion de un choice)*/
void CompositeCommand::SetOutputParameters(ParameterCollection* pParameters) {
}

/** Obtiene los parametros de salida del command */
ParameterCollection* CompositeCommand::GetOutputParameters() const {
   return NULL;
}

/**
 * Obtiene el nombre de la tarea asociada (si existe alguna).
 */
std::string CompositeCommand::GetAssociatedTaskId() const {
   return associatedTaskId_;
}

/**
 * Establece el nombre de la tarea asociada (si existe alguna)
 */
void CompositeCommand::SetAssociatedTaskId(std::string TaskId) {
   associatedTaskId_ = TaskId;
}

} /* namespace suri */
