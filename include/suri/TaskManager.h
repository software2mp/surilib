/** \file TaskManager.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
