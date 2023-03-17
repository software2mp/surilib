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

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "LibraryConfigDefinition.h"
#include "LibraryConfigDefinitionOrigin.h"

namespace suri {

/**
 * Ctor.
 */
LibraryConfigDefinition::LibraryConfigDefinition(const std::string& Name) :
      name_(Name) {
}

/**
 * Dtor.
 */
LibraryConfigDefinition::~LibraryConfigDefinition() {

}

/**
 * Agrega un origen a la definicion.
 */
void LibraryConfigDefinition::AddOrigin(LibraryConfigDefinitionOrigin* pOrigin) {
   origins_.push_back(pOrigin);
}

/**
 * Devuelve todos los origenes disponibles.
 */
std::vector<LibraryConfigDefinitionOrigin*> LibraryConfigDefinition::GetOrigins() const {
   return origins_;
}

/**
 * Devuelve todos los origenes disponibles para un determinado nivel.
 */
std::vector<LibraryConfigDefinitionOrigin*> LibraryConfigDefinition::GetOriginsByLevel(
      int Level) const {
   std::vector<LibraryConfigDefinitionOrigin*> retvec;
   for (int ix = 0, lenix = origins_.size(); ix < lenix; ++ix) {
      if (origins_[ix]->GetLevel() == Level) {
         retvec.push_back(origins_[ix]);
      }
   }
   return retvec;
}

/**
 * Devuelve el nombre de la definicion.
 */
std::string LibraryConfigDefinition::GetName() const {
   return name_;
}
/** Configura el archivo de indice de favoritos */
void LibraryConfigDefinition::SetFavoritesIndexFileName(const std::string& FileName) {
   favoritesIndexFileName_ = FileName;
}

/** Obtiene el filename del indice de favoritos*/
std::string LibraryConfigDefinition::GetFavoritesIndexFileName() const {
   return favoritesIndexFileName_;
}
} // namespace suri
