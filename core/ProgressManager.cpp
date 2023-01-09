/*! \file ProgressManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
