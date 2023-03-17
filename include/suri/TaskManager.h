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

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

// Includes standard
#include <string>
#include <set>
#include <vector>
#include <map>

// Includes suri
#include "suri/TaskInterface.h"
#include "suri/TaskConfig.h"
#include "suri/TaskConfigBuilder.h"

namespace suri {

/**
 * Clase encargada de administrar las tareas en ejecucion.
 * Se encarga de construir los TaskConfig que configuran el Layout.
 *
 * \attention Usar AddTaskBuilder solo desde srs, de esta forma se puede
 * soportar distintos Layout dependiendo de la aplicacion ejecutada.
 */
class TaskManager {
public:
   /** Ctor */
   TaskManager();
   /** Dtor */
   virtual ~TaskManager();

   /** Registra un tipo de tarea en manager */
   bool AddTaskBuilder(const std::string &TaskName, TaskConfigBuilder* pBuilder);
   /** Usa parametros para construir TaskConfig */
   TaskConfig* GetTaskConfig(const std::string &TaskName,
                              const ParameterCollection& Params);

   /** Agrega una tarea a manager */
   bool AppendTask(TaskInterface* pTask);
   /** Elimina una tarea de manager */
   bool DeleteTask(const SuriObject::UuidType &TaskId);
   /** Retorna una tarea */
   TaskInterface* GetTask(const SuriObject::UuidType &TaskId);
   /** Retorna el id de las tareas */
   std::set<SuriObject::UuidType> GetTaskIds();
   /** Retorna el id de las tareas activas */
   std::set<SuriObject::UuidType> GetActiveTaskIds();

private:
   /** Retorna un iterador que apunta a la tarea*/
   std::vector<TaskInterface*>::iterator GetTaskIterator(
                                 const SuriObject::UuidType &TaskId);

   /** Mapa con las tareas registradas */
   std::map<std::string, TaskConfigBuilder*> taskBuilders_;
   /** Vector con las tareas en ejecucion */
   std::vector<TaskInterface*> tasks_;
};

} /* namespace suri */
#endif /* TASKMANAGER_H_ */
