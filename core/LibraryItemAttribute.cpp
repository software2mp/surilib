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
#include <string>

// Includes Suri
#include "suri/LibraryItemAttribute.h"

// Includes Wx

// Defines

// forwards

namespace suri {


const std::string LibraryItemAttribute::PrincipalAttrName = "Principal";
const std::string LibraryItemAttribute::ExtendedAttrName = "Extended";


/** Constructor por defecto de la instancia
 * @param[in] Name nombre del atributo
 * @param[in] Value valor del atributo
 * @param[in] AttributeType tipo del atributo
 **/
LibraryItemAttribute::LibraryItemAttribute(const std::string& Name,
                                           const std::string& Value,
                                           AttributeTypeEnum AttributeType) :
      name_(Name), type_(AttributeType), value_(Value) {
}

/** Destructor **/
LibraryItemAttribute::~LibraryItemAttribute() {
}

/**
 * Devuelve el nombre asociado al atributo
 * @return string con el nombre del atributo
 */
std::string LibraryItemAttribute::GetName() const {
   return name_;
}

/**
 * Devuelve el tipo asociado al atributo
 * @return string con el tipo del atributo
 */
LibraryItemAttribute::AttributeTypeEnum LibraryItemAttribute::GetType() const {
   return type_;
}

/**
 * Devuelve el valor asociado al atributo
 * @return string con el valor del atributo
 */
std::string LibraryItemAttribute::GetValue() const {
   return value_;
}

/** Asocia un valor al atributo */
void LibraryItemAttribute::SetValue(std::string Value) {
   value_ = Value;
}

/** Compara si dos atributos son iguales **/
bool LibraryItemAttribute::Equals(const LibraryItemAttribute& Attribute) const {
   return value_.compare(Attribute.GetValue()) == 0;
}

} /** namespace suri */
