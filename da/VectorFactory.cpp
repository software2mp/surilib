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

#include "VectorFactory.h"

// Includes suri
#include "suri/Vector.h"
#include "MemoryVector.h"

/** namespace suri */
namespace suri {
/** Registra las distintas clases de vectores */
/**
 *  Registra las clases derivadas de Vector que manejan los distintos formatos.
 * Utiliza los metodos estaticos de las clases derivadas ::GetClassId() y
 * ::Create() para agregar a la factoria.
 *
 *  El orden en que se registran las clases es relevante ya que se prueba FIFO
 * cada clase para identificar los archivos, es decir las de arriba tienen
 * precedencia sobre las de abajo.
 */
void RegisterAllVectors() {
#ifdef __GDAL__
   Vector::RegisterVector(MemoryVector::GetClassId(), MemoryVector::Create);
   Vector::RegisterVector(Vector::GetClassId(), Vector::Create);
#endif
}
}  // namespace suri
