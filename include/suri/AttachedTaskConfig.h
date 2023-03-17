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

#ifndef ATTACHEDTASKCONFIG_H_
#define ATTACHEDTASKCONFIG_H_

#include <cstddef>

#include "suri/TaskConfig.h"

namespace suri {

/**
 * Contiene una referencia a un objeto de configuracion (el objeto al que se le
 * agrega una configuracion extra). Este mismo, puede ser un agregado a otra
 * configuracion o no (en este caso se estaria tratando con una configuracion
 * principal)
 */
class AttachedTaskConfig : public TaskConfig {
public:
   /**
    * Constructor.
    */
   explicit AttachedTaskConfig(TaskConfig* pTaskConfigP);

   /**
    * Destructor.
    */
   virtual ~AttachedTaskConfig();

   /**
    * Metodo a implementar por la configuracion concreta que se esta
    * agregando que contiene el procedimiento de configuracion en si.
    */
   virtual int Configure();

   /**
    * Metodo a implementar por la configuracion concreta que contiene
    * el procedimiento para configurar el entorno una vez finalizada
    * la tarea.
    */
   virtual int Clear();

   /**
    * Verifica si la configuracion tiene lo que necesita para seguir adelante.
    */
   virtual int Verify() const;

private:
   TaskConfig* pTaskConfig_;  // Configuracion a la que se le agrega responsabilidad.
};

}  // namespace suri

#endif  // ATTACHEDTASKCONFIG_H_
