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
