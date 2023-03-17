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

#ifndef PROGRESSMANAGER_H_
#define PROGRESSMANAGER_H_

// Includes standard
#include <string>
#include <map>

/** namespace suri */
namespace suri {
/** Clase que permite registrar una tareas que progresan, muestra el progreso */
/**
 * Administrador del progreso de tareas.
 * Cada proceso se registra y al ejecutar cada ciclo interactua con el
 * ProgressManager que se encarga de reportar al usuario el progreso, y
 * determinar si se debe o no cancelar, etc.
 * Permite registrar tarea e indicar su avance. La tarea tiene un numero de
 * ciclos y una descripcion.
 * Tiene metodos estaticos que permiten crear y obtener una instancia del
 * ProgressManager
 * Solo tiene funcionalidad basica, las acciones que se toman cuando se registra
 * una tarea se deberian definir en clases hijo.
 */
class ProgressManager {
   /** Ctor. de Copia. */
   ProgressManager(const ProgressManager &ProgressManager);

public:
   /** Tipo para la funcion que crea PMs */
   typedef ProgressManager* (*CreatorFunction)(void);
   /** Ctor */
   ProgressManager() :
         nextId_(0) {
   }
   /** Dtor */
   virtual ~ProgressManager() {
   }

   /** Trae ProgressManger (singleton) */
   static ProgressManager* GetInstance();
   /** Guarda la funcion que se usa para crear ProgressManager la primera vez que se usa */
   static bool SetCreateFunction(CreatorFunction pFunction);
   /** Registra una tarea */
   virtual int GetProgressId(int Cycles, const std::string &Description);
   /** Libera tarea registrada */
   virtual bool ReleaseProgressId(int ProgressId);
   /** Avanza una tarea registrada */
   bool Tick(int ProgressId);
   /** Indica si un Id de progreso es valido */
   virtual bool IsValid(int ProgressId) const;
   /** Indica si se debe enviar un Update al Id de progreso */
   virtual bool SendViewerUpdate(int ProgressId) const;
   /** Indica si se debe abortar la operacion cuyo progreso tiene el Id pasado */
   virtual bool ShouldAbort(int ProgressId) const;
   /** Actualiza el progreso para el Id */
   virtual void Update(int ProgressId) const;
protected:
private:
   int nextId_; /*! valor del ultimo progressId otorgado */
   /** Clase que guarda el numero de cyclos y la descripcion de una tarea */
   struct ProgressData {
      /** Ctor */
      /**
       * @param[in] Cycles numero de ciclos que requiere la tarea
       * @param[in] Description descripcion de la tarea
       */
      ProgressData(int Cycles, const std::string &Description) :
            cycles_(Cycles), desc_(Description) {
      }
      int cycles_; /*! cantidad de ciclos en una tarea */
      std::string desc_; /*! descripcion de la tarea */
   };
protected:
   std::map<int, ProgressData> progressQueue_; /*! info de cada tarea registrada */
private:
   static CreatorFunction pFunction_; /*! Funcion que crea primer progressManager */
   static ProgressManager* pProgressManager_; /*! ProgresManager del singleton */
};

ProgressManager *WxProgressManager();

/** Funcion externa que se usa para inicializar ProgressManager */
extern ProgressManager::CreatorFunction NullProgressManager;
}

#endif /*PROGRESSMANAGER_H_*/
