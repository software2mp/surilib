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
