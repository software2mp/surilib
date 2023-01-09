/** \file TaskManager.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <set>
#include <vector>
#include <map>

// Includes suri
#include "suri/TaskManager.h"

namespace suri {

/** Ctor */
TaskManager::TaskManager() {
}

/** Dtor */
TaskManager::~TaskManager() {
}

/** Registra un tipo de tarea en manager */
bool TaskManager::AddTaskBuilder(const std::string &TaskName,
                                       TaskConfigBuilder* pBuilder) {
   if (!pBuilder || taskBuilders_.find(TaskName) != taskBuilders_.end())
      return false;

   taskBuilders_[TaskName] = pBuilder;
   return true;
}

/** Usa parametros para construir TaskConfig */
TaskConfig* TaskManager::GetTaskConfig(const std::string &TaskName,
                                       const ParameterCollection& Params) {
   std::map<std::string, TaskConfigBuilder*>::iterator it =
                                             taskBuilders_.find(TaskName);
   if (it == taskBuilders_.end())
      return NULL;
   return (it->second)->Build(Params);
}

/** Agrega una tarea a manager */
bool TaskManager::AppendTask(TaskInterface* pTask) {
   if (!pTask || GetTaskIterator(pTask->GetId()) != tasks_.end())
      return false;
   tasks_.push_back(pTask);
   return true;
}

/** Elimina una tarea de manager */
bool TaskManager::DeleteTask(const SuriObject::UuidType &TaskId) {
   std::vector<TaskInterface*>::iterator it = GetTaskIterator(TaskId);
   if (it == tasks_.end())
      return false;
   delete *it;
   tasks_.erase(it);
   return true;
}

/** Retorna una tarea */
TaskInterface* TaskManager::GetTask(const SuriObject::UuidType &TaskId) {
   std::vector<TaskInterface*>::iterator it = GetTaskIterator(TaskId);
   return (it != tasks_.end()) ? *it : NULL;
}

/** Retorna el id de las tareas */
std::set<SuriObject::UuidType> TaskManager::GetTaskIds() {
   std::set<SuriObject::UuidType> ids;
   std::vector<TaskInterface*>::iterator it = tasks_.begin();
   for (; it != tasks_.end(); ++it)
      ids.insert((*it)->GetId());

   return ids;
}

/** Retorna el id de las tareas activas */
std::set<SuriObject::UuidType> TaskManager::GetActiveTaskIds() {
   std::set<SuriObject::UuidType> ids;
   std::vector<TaskInterface*>::iterator it = tasks_.begin();
   for (; it != tasks_.end(); ++it)
      if ((*it)->IsActive())
         ids.insert((*it)->GetId());

   return ids;
}

/** Retorna un iterador que apunta a la tarea*/
std::vector<TaskInterface*>::iterator TaskManager::GetTaskIterator(
                                    const SuriObject::UuidType &TaskId) {
   std::vector<TaskInterface*>::iterator it = tasks_.begin();
   for (; it != tasks_.end() && (*it)->GetId() != TaskId; ++it);
   return it;
}

} /* namespace suri */
