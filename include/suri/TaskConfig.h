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

#ifndef TASKCONFIG_H_
#define TASKCONFIG_H_

#define TC_OK                 0
#define TC_ERROR              1
#define TC_UNEXPECTED_ERROR   2
#define TC_BASE_USER          1024

namespace suri {

/**
 * Define la interfaz para la configuracion principal y para las
 * configuraciones que pueden ser agragadas dinamicamente a la principal.
 */
class TaskConfig {
public:
   /** dtor */
   virtual ~TaskConfig() {}
   /**
    * Metodo a implementar por la configuracion concreta que contiene
    * el procedimiento de configuracion en si.
    */
   virtual int Configure() = 0;

   /**
    * Metodo a implementar por la configuracion concreta que contiene
    * el procedimiento para configurar el entorno una vez finalizada
    * la tarea.
    */
   virtual int Clear() = 0;

   /**
    * Verifica si la configuracion tiene lo que necesita para seguir adelante.
    */
   virtual int Verify() const = 0;
};

}  // namespace suri

#endif  // TASKCONFIG_H_
