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

#ifndef LIBRARYITEMCATALOG_H_
#define LIBRARYITEMCATALOG_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/SuriObject.h"
#include "LibraryItemOrigin.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class LibraryItemAttribute;

/*!
 * Clase que representa la informacion que brinda un item de biblioteca en el catalogo.
 */
class LibraryItemCatalog {
   /** ctor copia **/
   LibraryItemCatalog(const LibraryItemCatalog&);

public:
   /** ctor **/
   LibraryItemCatalog(const SuriObject::UuidType& ItemId,
                      const LibraryItemAttribute* pPrincipalAttribute,
                      const LibraryItemAttribute* pExtendedAttribute = NULL,
                      const SuriObject::UuidType& OriginId = SuriObject::NullUuid);
   /** dtor */
   virtual ~LibraryItemCatalog();
   /**
    * Devuelve el nombre atributo principal
    * del item para la muestra del catalogo.
    * @return string con el nombre del atributo principal
    */
   std::string GetPrincipalAttributeName() const;
   /**
    * Devuelve el nombre del atributo extendido (atributo descriptivo del atributo principal)
    *  del item para la muestra del catalogo.
    * @return string con el nombre del atributo extendido
    */
   std::string GetExtendedAttributeName() const;
   /**
    * Devuelve el valor atributo extendido (atributo descriptivo del atributo principal)
    *  del item para la muestra del catalogo.
    * @return string con el valor del atributo extendido
    */
   std::string GetExtendedAttributeValue() const;
   /**
    * Devuelve el valor del atributo principal
    *  del item para la muestra del catalogo.
    * @return string con el nombre del atributo extendido
    */
   std::string GetPrincipalAttributeValue() const;
   /**
    * Devuelve el id del item asociado
    * Precondicion: Siempre se tiene id asociado a un item
    * @return string con id del item asociado
    */
   SuriObject::UuidType GetAssociatedItemId() const;
   /** Obtiene el id del origen que creo el catalogo **/
   SuriObject::UuidType GetOriginId() const;
   /** Indica que el item al que corresponde el catalog
    *  forma parte de los favoritos
    */
   void SetFavoriteFlag(bool Favorite);
   /** Indica si el item fue marcado como favorito  */
   bool IsFavorite() const;

private:
   SuriObject::UuidType itemId_;
   /** Atributo principal del item**/
   const LibraryItemAttribute* pPrincipalAttribute_;
   /** Atributo descriptivo del atributo principal **/
   const LibraryItemAttribute* pExtendedAttribute_;
   /** id del origen en el que se encuentra el item*/
   SuriObject::UuidType originId_;
   bool favorite_;
};

} /** namespace suri */

#endif /* LIBRARYITEMCATALOG_H_ */
