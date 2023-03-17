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

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "FavoritesIndexFile.h"

#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryItem.h"
#include "suri/LibraryItemAttribute.h"
#include "suri/LibraryItemCatalog.h"
#include "suri/LibraryItemOrigin.h"

namespace suri {

/** Constructor por defecto */
LibraryItemOrigin::LibraryItemOrigin(
      const SuriObject::UuidType& LibraryId, OriginCategoryType OriginCategory,
      std::vector<LibraryItemOrigin::CapabilityType> Capabilities) :
      libraryId_(LibraryId), originCategory_(OriginCategory), capabilites_(Capabilities) {
}

/** Constructor que se indica el nombre del origen **/
LibraryItemOrigin::LibraryItemOrigin(
      const SuriObject::UuidType& LibraryId, OriginCategoryType OriginCategory,
      std::vector<LibraryItemOrigin::CapabilityType> Capabilities,
      const std::string& Name) :
      libraryId_(LibraryId), originCategory_(OriginCategory),
      capabilites_(Capabilities), name_(Name) {
}

/**
 * Destructor
 * elimina todos los items en memoria
 */
LibraryItemOrigin::~LibraryItemOrigin() {
   delete_all(items_);
}

/**
 * Agrega el item pasado por parametro a la biblioteca
 * @param[in] pItem referencia del item a agregar
 * @return true en caso de agregarlo, false en caso contrario
 */
bool LibraryItemOrigin::AddItem(LibraryItem* pItem) {
   bool added = false;
   bool exists = false;
   LibraryItemList::const_iterator it = items_.begin();
   for (; !exists && it != items_.end(); ++it)
      exists = ((*it)->GetId().compare(pItem->GetId()) == 0);

   // Si no existe, lo agrega en el contenedor del origen particular.
   if (!exists) {
      added = AddItemToSource(pItem);
      items_.push_back(pItem);
      const LibraryItemAttribute* pattr = pItem->GetPrincipalAttribute();
      std::string codevalue;
      if (pattr)
         codevalue = pattr->GetValue();
      else
         codevalue = pItem->GetId();
      idCodeMap_.insert(std::make_pair(codevalue, pItem->GetId()));
   }
   pItem->ConfigureOriginId(GetId());
   return added;
}

/**
 * Elimina el item de la biblioteca correspondiente al Id pasado por parametro
 * @param[in] ItemId id del item a eliminar
 * @return true en caso de eliminarlo, false en caso contrario
 */
bool LibraryItemOrigin::RemoveItem(const SuriObject::UuidType& ItemId) {
   // Una vez eliminado el item en la lista elimina adicionalmente
   // en el contenedor  del origen en particular
   bool itemremoved = false;
   itemremoved = RemoveItemFromSource(ItemId);
   const LibraryItem* pitem = NULL;
   std::string principal;
   LibraryItemList::iterator it = items_.begin();
   for (; it != items_.end(); ++it) {
      pitem = ((*it)->GetId().compare(ItemId) == 0)? *it : NULL;
      if (pitem) {
         principal = pitem->GetPrincipalAttribute()->GetValue();
         delete *it;
         items_.erase(it);
         break;
      }
   }
   if (!principal.empty()) {
      idCodeMap_.erase(principal);
   }
   return itemremoved;
}

/**
 * Obtiene el item pasado por parametro a la biblioteca
 * @param[in] ItemId id del item a obtener
 * @return LibraryItem* referencia al item solicitado
 * @return NULL en caso de no encontrar el item solicitado
 */
const LibraryItem* LibraryItemOrigin::GetItem(
      const SuriObject::UuidType& ItemId) const {
   LibraryItemList::const_iterator findit = items_.begin();
   LibraryItem* pitem = NULL;
   for (; findit != items_.end(); ++findit) {
      LibraryItem* pauxitem = *findit;
      SuriObject::UuidType id = pauxitem->GetId();
      if (id.compare(ItemId) == 0) {
         pitem = *findit;
         break;
      }
   }

   UpdateItem(pitem);
   return pitem;
}

/**
 *
 *
 * .
 * .
 *
 *
 *
 *
 *
 * Obtiene el item pasado por parametro a la fuente de datos del origen
 * @param[in] Code codigo del item a obtener
 * @return LibraryItem* referencia al item solicitado
 * @return NULL en caso de no encontrar el item solicitado
 */
const LibraryItem* LibraryItemOrigin::GetItemByCode(const std::string& Code) {
   IdCodeMap::const_iterator findit = idCodeMap_.find(Code);
   const LibraryItem* pitem = NULL;
   if (findit != idCodeMap_.end())
      pitem = GetItem(findit->second);

   // En caso de no encontrar el item en la lista intenta una busqueda adicional
   // con el metodo particular del origen
   if (!pitem)
      pitem = GetItemByCodeFromSource(Code);
   return pitem;
}

/**
 * Reemplaza un item de la bilbioteca y elimina la instancia anterior
 * @param[in] pItem referencia del item a reemplazar
 * @return true en caso de poder reemplazarlo, false en caso contrario
 */
bool LibraryItemOrigin::ReplaceItem(LibraryItem* pItem) {
   // Remplaza el item en el el origen de datos particular
   if (!ReplaceItemFromSource(pItem))
      return false;

   // Si tuvo exito lo remplaza en listado interno
   bool itemreplaced = false;
   LibraryItemList::iterator it = items_.begin();
   for (; !itemreplaced && it != items_.end(); ++it) {
      bool sameprincipal = (pItem->GetPrincipalAttribute()->GetValue().compare(
            (*it)->GetPrincipalAttribute()->GetValue()) == 0);
      if ((*it)->GetId().compare(pItem->GetId()) == 0 || sameprincipal) {
         itemreplaced = true;
         idCodeMap_.erase((*it)->GetPrincipalAttribute()->GetValue());
         LibraryItem* pitem = *it;
         items_.erase(it);
         delete pitem;
      }
   }
   if (itemreplaced) {
      items_.push_back(pItem);
      idCodeMap_.insert(
            std::make_pair(pItem->GetPrincipalAttribute()->GetValue(), pItem->GetId()));
   }
   return itemreplaced;
}

/**
 * Reemplaza los atributos de un item
 * @param ItemId Id del item a reemplazar
 * @param pAttributesVector atributos a reemplazar/agregar
 * @param Add boolean que indica si se agrega el atributo en caso de no existir
 * @return true en caso de reemplazar los atributos en el item
 * @return false en caso contrario
 */
bool LibraryItemOrigin::ReplaceAttributesFromItem(
      const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector, bool Add) {
   LibraryItemList::iterator it = items_.begin();
   LibraryItem* pitem = NULL;
   for (; !pitem && it != items_.end(); ++it) {
      if ((*it)->GetId().compare(ItemId) == 0) {
         pitem = *it;
      }
   }
   if (!pitem)
      return false;
   bool replaced = pitem->ReplaceAttributes(pAttributesVector, Add);
   if (replaced)
      replaced = ReplaceItemFromSource(pitem);
   return replaced;
}

/**
 * Elimina los atributos de un item.
 */
bool LibraryItemOrigin::RemoveAttributesFromItem(
      const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector) {
   LibraryItemList::iterator it = items_.begin();
   LibraryItem* pitem = NULL;
   for (; !pitem && it != items_.end(); ++it) {
      if ((*it)->GetId().compare(ItemId) == 0) {
         pitem = *it;
      }
   }
   if (pitem == NULL)
      return false;
   bool removed = pitem->RemoveAttributes(pAttributesVector);
   if (removed)
      removed = ReplaceItemFromSource(pitem);
   return removed;
}

/* Reemplaza todos los atributos de un item
 * @param ItemId Id del item a reemplazar
 * @param pAttributesVector atributos a reemplazar/agregar **/
bool LibraryItemOrigin::SetAttributesFromItem(const SuriObject::UuidType& ItemId,
                           std::vector<LibraryItemAttribute*>* pAttributesVector) {
   LibraryItemList::iterator it = items_.begin();
   LibraryItem* pitem = NULL;
   for (; !pitem && it != items_.end(); ++it) {
      if ((*it)->GetId().compare(ItemId) == 0) {
         pitem = *it;
      }
   }
   if (!pitem) return false;
   pitem->SetAttributes(*pAttributesVector);
   return ReplaceItemFromSource(pitem);
}

/**
 * Reemplaza los atributos de un item
 * @param Principal Atributo principal del item a reemplazar
 * @param pAttributesVector atributos a reemplazar/agregar
 * @param Add boolean que indica si se agrega el atributo en caso de no existir
 * @return true en caso de reemplazar los atributos en el item
 * @return false en caso contrario
 */
bool LibraryItemOrigin::ReplaceAttributesFromItemByPrincipal(
      const SuriObject::UuidType& Principal,
      std::vector<LibraryItemAttribute*>* pAttributesVector, bool Add) {
   LibraryItem* pitem = const_cast<LibraryItem*>(GetItemByCode(Principal));
   return pitem ? pitem->ReplaceAttributes(pAttributesVector, Add) : false;
}

/**
 * Devuelve el catalogo (un resumen de todos los items) del origen.
 * @return vector de items de catalogo de la biblioteca
 */
std::vector<LibraryItemCatalog*> LibraryItemOrigin::GetCatalog(
      bool FilterFavorites) const {
   LibraryItemList::const_iterator it = items_.begin();
   std::vector<LibraryItemCatalog*> catalogvector;
   for (; it != items_.end(); ++it) {
      LibraryItem* plibraryitem = *it;
      LibraryItemCatalog* plibraryitemcatalog = NULL;
      std::string principal = plibraryitem->GetPrincipalAttribute()->GetValue();
      if (FilterFavorites && !favoritesIndexFileName_.empty() && IsFavorite(principal)) {
            plibraryitemcatalog = plibraryitem->GetCatalogInformation();
            plibraryitemcatalog->SetFavoriteFlag(true);
      } else if (!FilterFavorites || favoritesIndexFileName_.empty()){
         plibraryitemcatalog = plibraryitem->GetCatalogInformation();
      }
      if (plibraryitemcatalog) {
         catalogvector.push_back(plibraryitemcatalog);
      }
   }
   return catalogvector;
}

/**
 * Agrega una capacidad al origen
 * @param Capability
 * @return true si se pudo agregar correctamente
 * @return false en caso contrario
 */


bool LibraryItemOrigin::AddCapability(CapabilityType Capability) {
   capabilites_.push_back(Capability);
   return true;
}

/**
 * Devuelve las capacidad del origen
 * @return vector con todas las capacidad del origen
 */
std::vector<LibraryItemOrigin::CapabilityType> LibraryItemOrigin::GetCapabilities() const {
   return capabilites_;
}
/**
 *
 * @return true en caso de poseer la capacidad
 * @return false en caso contrario
 *
 */
bool LibraryItemOrigin::HasCapability(CapabilityType Capability) const {
   std::vector<CapabilityType>::const_iterator it = capabilites_.begin();
   bool found = false;
   for (; !found && it != capabilites_.end() ; ++it) {
      found = ((*it) == Capability);
   }
   return found;
}

/**
 * Devuelve el tipo de origen
 * @return Tipo de origen
 */
LibraryItemOrigin::OriginCategoryType LibraryItemOrigin::GetCategory() const {
   return originCategory_;
}

/** Agrega el item pasado por parametro a la fuente de datos del origen.
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool LibraryItemOrigin::AddItemToSource(LibraryItem* pItem) {
   return true;
}

/** Elimina el item de la fuente de datos del origen correspondiente al codigo
 *  pasado por parametro
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool LibraryItemOrigin::RemoveItemFromSourceByCode(const std::string& Code) {
   return true;
}

/**
 * Elimina el item de la fuente de datos del origen correspondiente al id
 * que se pasa por parametro
 * Metodo que tiene un comportamiento particular para cada tipo de origen.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool LibraryItemOrigin::RemoveItemFromSource(const SuriObject::UuidType& ItemId) {
   return true;
}

/** Obtiene el item pasado por parametro a la fuente de datos del origen
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
const LibraryItem* LibraryItemOrigin::GetItemByCodeFromSource(
      const std::string& Code) {
   return NULL;
}

/** Reemplaza un item de la fuente de datos del origen
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool LibraryItemOrigin::ReplaceItemFromSource(LibraryItem* pItem) {
   return true;
}

/**
 * Permite actualizar el item en memoria en caso que este incompleto o
 * haya quedado desactualizado con el origen de datos concreto.
 * (una clase concreta de origen tiene que reimplementar este metodo)
 * @param[in] pItem item que quiero actualizar
 * @return indica si se pudo actualizar el item.
 */
bool LibraryItemOrigin::UpdateItem(LibraryItem* pItem) const {
   return true;
}

#ifdef __DEBUG__
/** Devuelve la lista en un string con (id, principal) por cada item **/
std::string LibraryItemOrigin::GetListAsString() const {
   std::string liststring = "[";
   LibraryItemList::const_iterator it = items_.begin();
   for (; it != items_.end(); ++it) {
      liststring.append("(");
      liststring.append((*it)->GetId());
      liststring.append(", ");
      liststring.append((*it)->GetPrincipalAttribute()->GetValue());
      liststring.append(")");
      ++it;
      if (it != items_.end())
         liststring.append(";");
      --it;
   }
   liststring.append("]");
   return liststring;
}
#endif

/** Devuelve el nombre del origen **/
std::string LibraryItemOrigin::GetName() const {
   return name_;
}

/** Remueve todos los elementos de la lista **/
void LibraryItemOrigin::RemoveAllItems() {
   delete_all(items_);
   items_.clear();
}

/** Indica cual es el archivo correspondiente al indices de favoritos */
void LibraryItemOrigin::SetFavoritesIndexFile(const std::string& FileName) {
   if (FileName.compare(favoritesIndexFileName_)) {
      favoritesIndexFileName_ = FileName;
      LoadFavoritesIndex();
   }
}
/** Obtiene el nombre del archivo de indices de favoritos */
std::string LibraryItemOrigin::GetFavoritesIndexFileName() const {
   return favoritesIndexFileName_;
}

/** Metodo auxiliar que se encarga de levantar el indice de favoritos que se encuentra
 * configurado
 * Precondicion: el archivo de indices se encuentra bien configurado. Esto quiere
 * decir que se encuentra un id por linea del archivo y que ademas ese
 * id existe dentro de la lista de items disponibles en la biblioteca
 */
void LibraryItemOrigin::LoadFavoritesIndex() {
   FavoritesIndexFile favfile(favoritesIndexFileName_);
   favoritesIndex_.clear();
   favfile.LoadFavoritesIndex(favoritesIndex_);
}
/** Metodo que verifica si un item fue marcado como favorito **/
bool LibraryItemOrigin::IsFavorite(const SuriObject::UuidType& ItemId) const {
   return favoritesIndex_.find(ItemId) != favoritesIndex_.end();
}

/** Agrega un item a favoritos **/
bool LibraryItemOrigin::AddItemToFavorites(const SuriObject::UuidType& ItemId) {
   if (!IsFavorite(ItemId)) {
      FavoritesIndexFile favfile(favoritesIndexFileName_);
      if (favfile.AddItemFromFavorite(ItemId)) {
         favoritesIndex_.insert(ItemId);
         return true;
      }
      return false;
   }
   return true;
}
/** Elimina un item de favoritos **/
bool LibraryItemOrigin::RemoveItemFromFavorites(const SuriObject::UuidType& ItemId) {
   if (IsFavorite(ItemId)) {
      FavoritesIndexFile favfile(favoritesIndexFileName_);
      if (favfile.RemoveItemFromFavorite(ItemId)) {
         favoritesIndex_.erase(ItemId);
         return true;
      }
      return false;
   }
   return true;
}

}  // namespace suri
