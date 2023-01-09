/** \file AttachedTaskConfig.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/AttachedTaskConfig.h"

namespace suri {

/**
 * Constructor.
 */
AttachedTaskConfig::AttachedTaskConfig(TaskConfig* pTaskConfigP) {
   pTaskConfig_ = pTaskConfigP;
}

/**
 * Destructor.
 */
AttachedTaskConfig::~AttachedTaskConfig() {
   if (pTaskConfig_ != NULL) {
      delete pTaskConfig_;
      pTaskConfig_ = NULL;
   }
}

/**
 * Metodo a implementar por la configuracion concreta que se esta
 * agregando que contiene el procedimiento de configuracion en si.
 *
 * @return TC_OK si se puedo realizar la configuracion indicando que
 * el proceso puede continuar.
 */
int AttachedTaskConfig::Configure() {
   if (pTaskConfig_ != NULL) {
      return pTaskConfig_->Configure();
   }
   return TC_OK;
}

/**
 * Metodo a implementar por la configuracion concreta que contiene
 * el procedimiento para configurar el entorno una vez finalizada
 * la tarea.
 *
 * @return TC_OK si se puedo realizar la configuracion indicando que
 * el proceso puede continuar.
 */
int AttachedTaskConfig::Clear() {
   if (pTaskConfig_ != NULL) {
      return pTaskConfig_->Clear();
   }
   return TC_OK;
}

/**
 * Verifica si la configuracion tiene lo que necesita para seguir adelante.
 */
int AttachedTaskConfig::Verify() const {
   if (pTaskConfig_ != NULL) {
      return pTaskConfig_->Verify();
   }
   return TC_OK;
}

}  // namespace suri
