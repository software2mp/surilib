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

#include "suri/SourceSupervisor.h"

/** namespace suri */
namespace suri {
/**
 *  En el destructor se toman las fuentes supervisadas que quedan y se les
 * elimina el supervisor.
 *
 *  Esto deberia considerarse un error, por no hacer Image::PopSource() antes
 * de destruir la imagen. Generar una excepcion? Mensaje de advertencia?
 *
 *  Se debe definir
 */
SourceSupervisor::~SourceSupervisor() {
// __STRICT_SOURCE_SUPERVISOR__ determina si al destruir el SourceSupervisor
// se "desregistra" de las fuentes manejadas para asi evitar que se lo llame
// desde una fuente.
#if __STRIC_SOURCE_SUPERVISOR__ != 1
   // Si quedan fuentes supervisadas les saca el supervisor
   SourceSetType::iterator it = childSources_.begin();
   while (it != childSources_.end()) {
      (*it)->pSupervisor_ = NULL;
      it++;
   }
#endif   // __STRIC_SOURCE_SUPERVISOR__
}

/**
 * crea una nueva instancia de la fuente manejada
 * @return puntero a RasterSource creado
 */
RasterSource *SourceSupervisor::CreateSource() {
   RasterSource *psource = Create();
   AddSource(psource);
   return psource;
}

/**
 * Elimina una fuente
 * @param[in] pSource RasterSource que se quiere eliminar
 */
void SourceSupervisor::DelSource(RasterSource *pSource) {
   childSources_.erase(pSource);
}

/**
 * Agrega una fuente
 * @param[in] pNewSource nuevo RasterSource
 */
void SourceSupervisor::AddSource(RasterSource *pNewSource) {
   childSources_.insert(pNewSource);
}
}  // namespace suri

