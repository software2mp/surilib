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
#include "suri/LibraryItemCatalog.h"
#include "suri/LibraryItemAttribute.h"
#include "suri/LibraryItemOrigin.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** constructor **/
LibraryItemCatalog::LibraryItemCatalog(const SuriObject::UuidType& ItemId,
                                       const LibraryItemAttribute* pPrincipalAttribute,
                                       const LibraryItemAttribute* pExtendedAttribute,
                                       const SuriObject::UuidType& OriginId) :
      itemId_(ItemId), pPrincipalAttribute_(pPrincipalAttribute),
      pExtendedAttribute_(pExtendedAttribute), originId_(OriginId),
      favorite_(false) {
}

/** destructor **/
LibraryItemCatalog::~LibraryItemCatalog() {
}

/**
 * Devuelve el nombre atributo principal
 * del item para la muestra del catalogo.
 * @return string con el nombre del atributo principal
 */
std::string LibraryItemCatalog::GetPrincipalAttributeName() const {
   return pPrincipalAttribute_->GetName();
}
/**
 * Devuelve el nombre del atributo extendido (atributo descriptivo del atributo principal)
 *  del item para la muestra del catalogo.
 * @return string con el nombre del atributo extendido
 */
std::string LibraryItemCatalog::GetExtendedAttributeName() const {
   std::string name;
   if (pExtendedAttribute_)
      name = pExtendedAttribute_->GetName();
   return name;
}
/**
 * Devuelve el valor atributo extendido (atributo descriptivo del atributo principal)
 *  del item para la muestra del catalogo.
 * @return string con el valor del atributo extendido
 */
std::string LibraryItemCatalog::GetExtendedAttributeValue() const {
   std::string value;
   if (pExtendedAttribute_)
      value = pExtendedAttribute_->GetValue();
   return value;
}
/**
 * Devuelve el valor del atributo principal
 *  del item para la muestra del catalogo.
 * @return string con el nombre del atributo extendido
 */
std::string LibraryItemCatalog::GetPrincipalAttributeValue() const {
   return pPrincipalAttribute_->GetValue();
}
/**
 * Devuelve el id del item asociado
 * Precondicion: Siempre se tiene id asociado a un item
 * @return string con id del item asociado
 */
SuriObject::UuidType LibraryItemCatalog::GetAssociatedItemId() const {
   return itemId_;
}

/** Obtiene el id del origen que creo el catalogo **/
SuriObject::UuidType LibraryItemCatalog::GetOriginId() const {
   return originId_;
}

/** Indica que el item al que corresponde el catalog
 *  forma parte de los favoritos
 */
void LibraryItemCatalog::SetFavoriteFlag(bool Favorite) {
   favorite_ = Favorite;
}
/** Indica si el item fue marcado como favorito  */
bool LibraryItemCatalog::IsFavorite() const {
   return favorite_;
}
} /** namespace suri */
