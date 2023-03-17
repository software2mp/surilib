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

#include "suri/ProgressManager.h"

// Includes suri
#include "logmacros.h"

/** namespace suri */
namespace suri {
ProgressManager::CreatorFunction ProgressManager::pFunction_ = WxProgressManager;
ProgressManager* ProgressManager::pProgressManager_ = 0;
ProgressManager::CreatorFunction NullProgressManager = NULL;

/** Trae ProgressManger (singleton) */
/**
 * \attention el ProgressManager retornado es responsabilidad de esta clase(No debe eliminarse)
 */
ProgressManager* ProgressManager::GetInstance() {
   // TODO: agregar mutex
   if (!pProgressManager_){
      pProgressManager_ = new ProgressManager();
   }
   return pProgressManager_;
}

/** Guarda la funcion que se usa para crear ProgressManager la primera vez que se usa */
bool ProgressManager::SetCreateFunction(CreatorFunction pFunction) {
   pFunction_ = pFunction;
   return true;
}
/** Registra una tarea */
/**
 * @param[in] Cycles Cantidad de ciclos que necesita la tarea
 * @param[in] Description Descipcion de la tarea
 * @return identificador asignado a la tarea
 */
int ProgressManager::GetProgressId(int Cycles, const std::string &Description) {
   REPORT_DEBUG("D:Creando nuevo progreso \"%s\" (#%d)", Description.c_str(), nextId_);
   progressQueue_.insert(progressQueue_.begin(),
                         std::make_pair(nextId_, ProgressData(Cycles, Description)));
   return nextId_++;
}

/**
 * Libera tarea registrada
 * @param[in] ProgressId identificador de la tarea
 * @return informa si pudo eliminarla
 */
bool ProgressManager::ReleaseProgressId(int ProgressId) {
   size_t size = progressQueue_.size();
   progressQueue_.erase(ProgressId);
   return progressQueue_.size() < size;
}

/**
 * Avanza una tarea registrada
 * @param[in] ProgressId id de la tarea
 * @return informa si tuvo exito
 */
bool ProgressManager::Tick(int ProgressId) {
   if (IsValid(ProgressId) && SendViewerUpdate(ProgressId)) {
      Update(ProgressId);
      return ShouldAbort(ProgressId);
   }
   return false;
}

/**
 * @param[in] ProgressId id de la tarea
 */
bool ProgressManager::IsValid(int ProgressId) const {
   return true;
}

/**
 * @param[in] ProgressId id de la tarea
 */
bool ProgressManager::SendViewerUpdate(int ProgressId) const {
   return true;
}

/**
 * @param[in] ProgressId id de la tarea
 */
bool ProgressManager::ShouldAbort(int ProgressId) const {
   return false;
}

/**
 * @param[in] ProgressId id de la tarea
 */
void ProgressManager::Update(int ProgressId) const {
   REPORT_DEBUG("D:Updating");
}
}
