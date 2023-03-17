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

#include "LibraryConfigDefinitionOriginSource.h"

namespace suri {

const std::string LibraryConfigDefinitionOriginSource::kTypeTextfile = "textfile";
const std::string LibraryConfigDefinitionOriginSource::kTypeGdal = "gdal" ;

/**
 * Ctor.
 */
LibraryConfigDefinitionOriginSource::LibraryConfigDefinitionOriginSource() {

}

/**
 * Dtor.
 */
LibraryConfigDefinitionOriginSource::~LibraryConfigDefinitionOriginSource() {

}

/**
 * Devuelve el nombre de la fuente de datos. Ej: de ser un archivos
 * de texto plano devolveria el nombre del mismo.
 */
std::string LibraryConfigDefinitionOriginSource::GetName() const {
   return name_;
}

/**
 * Establece el nombre del origen de datos.
 */
void LibraryConfigDefinitionOriginSource::SetName(const std::string& Name) {
   name_ = Name;
}

/**
 * Devuelve el tipo de la fuente de datos.
 */
std::string LibraryConfigDefinitionOriginSource::GetType() const {
   return type_;
}

/**
 * Establece el tipo del origen de datos.
 */
void LibraryConfigDefinitionOriginSource::SetType(const std::string& Type) {
   type_ = Type;
}

/**
 * Devuelve la ruta de la fuente de datos. Ej: de ser un archivo
 * de texto plano seria la ruta en el sistema de archivos.
 */
std::string LibraryConfigDefinitionOriginSource::GetPath() const {
   return path_;
}

/**
 * Establece la ruta del origen de datos.
 */
void LibraryConfigDefinitionOriginSource::SetPath(const std::string& Path) {
   path_ = Path;
}

/**
 * Devuelve el tipo de item. Ej: si es sist. georreferenciado,
 * proyectado, datum o elipsoide.
 */
int LibraryConfigDefinitionOriginSource::GetItemType() const {
   return itemType_;
}

/**
 * Establece el tipo de item.
 */
void LibraryConfigDefinitionOriginSource::SetItemType(int ItemType) {
   itemType_ = ItemType;
}

}  // namespace suri
