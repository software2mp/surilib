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

#include <string>

#include "LibraryConfigDefinitionOrigin.h"
#include "LibraryConfigDefinitionOriginSource.h"

namespace suri {

/**
 * Ctor.
 */
LibraryConfigDefinitionOrigin::LibraryConfigDefinitionOrigin() {

}

/**
 * Dtor.
 */
LibraryConfigDefinitionOrigin::~LibraryConfigDefinitionOrigin() {

}

/**
 * Devuelve el nombre del origen.
 */
std::string LibraryConfigDefinitionOrigin::GetName() const {
   return name_;
}

/**
 * Establece el nombre del origen.
 */
void LibraryConfigDefinitionOrigin::SetName(const std::string& Name) {
   name_ = Name;
}

/**
 * Devuelve los permisos disponibles sobre el origen.
 */
int LibraryConfigDefinitionOrigin::GetCapabilities() const {
   return capabilities_;
}

/**
 * Establece los permisos sobre el origen.
 */
void LibraryConfigDefinitionOrigin::SetCapabilities(int Capabilities) {
   capabilities_ = Capabilities;
}

/**
 * Devuelve el nivel del origen.
 */
int LibraryConfigDefinitionOrigin::GetLevel() const {
   return level_;
}

/**
 * Establece el nivel del origen.
 */
void LibraryConfigDefinitionOrigin::SetLevel(int Level) {
   level_ = Level;
}

/**
 * Devuelve la fuente del origen.
 */
LibraryConfigDefinitionOriginSource* LibraryConfigDefinitionOrigin::GetSource() const {
   return pSource_;
}

/**
 * Establece la fuente del origen.
 */
void LibraryConfigDefinitionOrigin::SetSource(LibraryConfigDefinitionOriginSource* pSource) {
   pSource_ = pSource;
}

}  // namespace suri
