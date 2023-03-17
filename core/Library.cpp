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

#include <set>
#include <string>
#include <vector>

#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"
#include "suri/Library.h"
#include "suri/LibraryItemAttribute.h"
#include "suri/LibraryItemOrigin.h"
#include "suri/TextFileLibraryItemOrigin.h"

namespace suri {

/** Constructor **/
Library::Library(const std::string& LibraryCode): libraryCode_(LibraryCode) {
   if (LibraryCode == "") {
      std::vector<LibraryItemOrigin::CapabilityType> capabilites;
      capabilites.push_back(LibraryItemOrigin::READ);
      capabilites.push_back(LibraryItemOrigin::WRITE);
      capabilites.push_back(LibraryItemOrigin::APPEND);

      std::string path = Configuration::GetParameter("app_data_dir", wxT("./"));
      path.append("/system.lbr");
      LibraryItemOrigin* porigin = new TextFileLibraryItemOrigin(GetId(),
                                                           capabilites, path);

      InsertOrderedOrigin(porigin);
   }
}

/** Destructor: elimina todos los origenes **/
Library::~Library() {
   delete_all(libraryOriginList_);
}

/**
 * Agrega el item pasado por parametro a la biblioteca
 * @param[in] pItem referencia del item a agregar
 * @return true en caso de agregarlo, false en caso contrario
 */
bool Library::AddItem(LibraryItem* pItem) {
   std::vector<LibraryItemOrigin*> origins = FindPrecedenceByAccess(
         LibraryItemOrigin::APPEND);
   if (origins.empty())
      return false;
   return origins[0]->AddItem(pItem);
}

/** Agrega un item a un origen en particular de la biblioteca
 *  @param[in] OriginId id del origen sobre el cual se agrega
 *  @param[in] pItem referencia al item que se desea agregar
 *  @return true en caso de poder agregar el item, false en caso contrario*/
bool Library::AddItemToLibraryOrigin(const SuriObject::UuidType& OriginId,
                            LibraryItem* pItem) {
   LibraryItemOrigin* porig = GetOrigin(OriginId);
   return porig ? porig->AddItem(pItem) : false;
}

/**
 * Elimina el item de la biblioteca correspondiente al Id pasado por parametro
 * @param[in] ItemId id del item a eliminar
 * @return true en caso de eliminarlo, false en caso contrario
 */
bool Library::RemoveItem(const SuriObject::UuidType& ItemId) {
   // Debe buscar en todos los origenes hasta que pueda borrar el item
   LibraryOriginList::iterator it = libraryOriginList_.begin();
   bool removed = false;
   for (; !removed && it != libraryOriginList_.end() ; ++it) {
      if ((*it)->HasCapability(LibraryItemOrigin::WRITE)) {
         removed = (*it)->RemoveItem(ItemId);
      }
   }
   return removed;
}

/**
 * Obtiene el item pasado por parametro a la biblioteca
 * @param[in] ItemId id del item a obtener
 * @return LibraryItem* referencia al item solicitado
 * @return NULL en caso de no encontrar el item solicitado
 */
const LibraryItem* Library::GetItem(const SuriObject::UuidType& ItemId) const {
   LibraryOriginList::const_iterator consit = libraryOriginList_.begin();
   const LibraryItem* pitem = NULL;
   for (; !pitem && consit != libraryOriginList_.end() ; ++consit) {
      LibraryItemOrigin* porigin = *consit;
      if (porigin->HasCapability(LibraryItemOrigin::READ)) {
         pitem = porigin->GetItem(ItemId);
      }
   }
   return pitem;
}

/** Obtiene el item correspondiente al atributo principal pasado por parametro*/
const LibraryItem* Library::GetItemByPrincipal(const std::string& PrincipalValue) const {
   LibraryOriginList::const_iterator consit = libraryOriginList_.begin();
   const LibraryItem* pitem = NULL;
   for (; !pitem && consit != libraryOriginList_.end() ; ++consit) {
      LibraryItemOrigin* porigin = *consit;
      if (porigin->HasCapability(LibraryItemOrigin::READ)) {
         pitem = porigin->GetItemByCode(PrincipalValue);
      }
   }
   return pitem;
}

/**
 * Reemplaza un item de la bilbioteca y elimina la instancia anterior
 * @param[in] pItem referencia del item a reemplazar
 * @return true en caso de poder reemplazarlo, false en caso contrario
 */
bool Library::ReplaceItem(LibraryItem* pItem) {
   std::vector<LibraryItemOrigin*> origins = FindPrecedenceByAccess(LibraryItemOrigin::WRITE);
   if (origins.empty())
      return false;
   bool replaced = false;
   std::vector<LibraryItemOrigin*>::iterator it = origins.begin();
   for (; !replaced && it != origins.end(); ++it) {
      replaced = (*it)->ReplaceItem(pItem);
   }
   return replaced;
}


/**
 * Reemplaza los atributos de un item
 * @param ItemId Id del item a reemplazar
 * @param pAttributesVector atributos a reemplazar/agregar
 * @param Add boolean que indica si se agrega el atributo en caso de no existir
 * @return true en caso de reemplazar los atributos en el item
 * @return false en caso contrario
 */
bool Library::ReplaceAttributesFromItem(const SuriObject::UuidType& ItemId,
                               std::vector<LibraryItemAttribute*>* pAttributesVector,
                               bool Add) {
   std::vector<LibraryItemOrigin*> origins = FindPrecedenceByAccess(LibraryItemOrigin::WRITE);
   if (origins.empty())
      return false;
   bool replaced = false;
   std::vector<LibraryItemOrigin*>::iterator it = origins.begin();
   for (; !replaced && it != origins.end(); ++it) {
      replaced = (*it)->ReplaceAttributesFromItem(ItemId, pAttributesVector, Add);
   }
   return replaced;
}

/**
 * Elimina los atributos de un item.
 */
bool Library::RemoveAttributesFromItem(
      const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector) {
   std::vector<LibraryItemOrigin*> origins = FindPrecedenceByAccess(LibraryItemOrigin::WRITE);
   if (origins.empty())
      return false;

   bool removed = false;
   std::vector<LibraryItemOrigin*>::iterator it = origins.begin();
   for (; !removed && it != origins.end(); ++it) {
      removed = (*it)->RemoveAttributesFromItem(ItemId, pAttributesVector);
   }
   return removed;
}

/* Reemplaza todos los atributos de un item
 * @param ItemId Id del item a reemplazar
 * @param pAttributesVector atributos a reemplazar/agregar **/
bool Library::SetAttributesFromItem(
      const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector) {
   std::vector<LibraryItemOrigin*> origins = FindPrecedenceByAccess(
         LibraryItemOrigin::WRITE);
   if (origins.empty()) return false;
   bool replaced = false;
   std::vector<LibraryItemOrigin*>::iterator it = origins.begin();
   for (; !replaced && it != origins.end(); ++it) {
      replaced = (*it)->SetAttributesFromItem(ItemId, pAttributesVector);
   }
   return replaced;
}

/**
 * Reemplaza los atributos de un item
 * @param Principal Id del item a reemplazar
 * @param pAttributesVector atributos a reemplazar/agregar
 * @param Add boolean que indica si se agrega el atributo en caso de no existir
 * @return true en caso de reemplazar los atributos en el item
 * @return false en caso contrario
 */
bool Library::ReplaceAttributesFromItemByPrincipal(
      const SuriObject::UuidType& Principal,
      std::vector<LibraryItemAttribute*>* pAttributesVector, bool Add) {
   std::vector<LibraryItemOrigin*> origins = FindPrecedenceByAccess(
         LibraryItemOrigin::WRITE);
   if (origins.empty())
      return false;
   bool replaced = false;
   std::vector<LibraryItemOrigin*>::iterator it = origins.begin();
   for (; !replaced && it != origins.end(); ++it) {
      replaced = (*it)->ReplaceAttributesFromItemByPrincipal(Principal,
                                                             pAttributesVector, Add);
   }
   return replaced;
}

/**
   * Devuelve el catalogo de los origenes de la biblioteca que corresponden
   * a la categoria pasad apor parametro.
   * Es decir, la lista de los items que contiene
   * @return vector con los origenes que corresponden a la categoria
   */

/**
 * Devuelve los origenes biblioteca que corresponde a la categoria pasada por parametro
  * Solamente carga los items que tenga cargados los atributos
 * principal (y que puedan tener el atributo extendido)
 * @param[in] OriginCategory categoria de origen del cual se solicita el catalog
 * @return vector con los origenes que corresponden a la categoria. En caso de
 * no existir ningun origen de esa categoria se devuelve un vector vacio
 */
std::vector<const LibraryItemOrigin*> Library::GetOriginsByCategory(
                        LibraryItemOrigin::OriginCategoryType OriginCategory) const {
   LibraryOriginList::const_iterator findit = libraryOriginList_.begin();
   std::vector<const LibraryItemOrigin*> origins;
   for (; findit != libraryOriginList_.end() ; ++findit) {
      if ((*findit)->GetCategory() == OriginCategory) {
         origins.push_back(*findit);
      }
   }
   return origins;
}

/**
 * Devuelve el catalogo entero de la biblioteca (la composicion de todos los origenes)
 * @return
 */
std::vector<LibraryItemCatalog*> Library::GetCatalog(bool FilterFavorites) const {
   std::vector<LibraryItemCatalog*> catalogvector;
   LibraryOriginList::const_iterator originit = libraryOriginList_.begin();
   for (; originit != libraryOriginList_.end() ; ++originit) {
      std::vector<LibraryItemCatalog*> origincatalog = (*originit)->GetCatalog(FilterFavorites);
      std::vector<LibraryItemCatalog*>::iterator it = origincatalog.begin();
      for (; it != origincatalog.end(); ++it)
         catalogvector.push_back(*it);
   }
   return catalogvector;
}

/** Elimina los origenes configurado */
void Library::Reset() {
   libraryOriginList_.clear();
}

/**
 * Agrega un origen a la biblioteca
 * @param[in] pOrigin Origen que quiero agregar
 */
void Library::AddOrigin(LibraryItemOrigin* pOrigin) {
   InsertOrderedOrigin(pOrigin);
}

/**
 * Retorna el codigo que identifica a la biblioteca
 * @return codigo de la biblioteca
 */
std::string Library::GetLibraryCode() const {
   return libraryCode_;
}

/**
 * Metodo auxiliar para obtener el origen por precedencia
 * @param[in] Capability tipo de acceso que se quiere buscar precedencia en
 * los origenes
 * @return referencia a origen que tiene la mayor precedencia sobre el tipo de acceso
 * pasado por parametro.
 * @return NULL en caso de que ningun origen tenga el tipo de accesso requerido
 */
std::vector<LibraryItemOrigin*> Library::FindPrecedenceByAccess(
      LibraryItemOrigin::CapabilityType Capability) const {
   std::vector<LibraryItemOrigin*> origins;
   LibraryOriginList::const_iterator findit = libraryOriginList_.begin();
   for (; findit != libraryOriginList_.end() ; ++findit) {
      if ((*findit)->HasCapability(Capability)) {
         origins.push_back(*findit);
      }
   }
   return origins;
}

/** Metodo auxiliar que inserta de ordenada los origenes por precedencia.
 *  Utilizando la secuencia de origenes brindada por enum OriginType. De
 *  forma que un nuevo origen se inserta como de mayor precedencia para el
 *  permiso asociado al mismo. Ejemplo: Si se inserta el origen "Proyecto", que
 *  requiere acceso "APPEND" es insertado por encima de los otros origenes
 *  existentes con acceso "Append".  **/
void Library::InsertOrderedOrigin(LibraryItemOrigin* pOrigin) {
   std::vector<LibraryItemOrigin::CapabilityType> capabilities =
                                                pOrigin->GetCapabilities();
   LibraryOriginList::iterator orderit = libraryOriginList_.begin();
   bool foundposition = false;
   for (; !foundposition && orderit != libraryOriginList_.end(); ++orderit) {
      std::vector<LibraryItemOrigin::CapabilityType>::const_iterator capit =
                                                         capabilities.begin();
      for ( ; !foundposition && capit != capabilities.end(); ++capit) {
         foundposition = ((*orderit)->HasCapability(*capit));
      }
   }
   libraryOriginList_.insert(orderit, pOrigin);
}


/** Devuelve los tipos de origenes que posee la bibloteca */
std::set<LibraryItemOrigin::OriginCategoryType> Library::GetOriginCategory() const {
   std::set<LibraryItemOrigin::OriginCategoryType> originscategories;
   LibraryOriginList::const_iterator orderit = libraryOriginList_.begin();
   for (; orderit != libraryOriginList_.end(); ++orderit) {
      originscategories.insert((*orderit)->GetCategory());
   }
   return originscategories;
}

/** Obtiene el id del primer origen que posee la capability indicada
 *  por parametro*/
SuriObject::UuidType Library::GetOriginIdPrecedenceByAccess(
      LibraryItemOrigin::CapabilityType Capability) const {
   std::vector<LibraryItemOrigin*> origins = FindPrecedenceByAccess(
         LibraryItemOrigin::APPEND);
   if (origins.empty())
      return SuriObject::NullUuid;
   return origins[0]->GetId();
}

/** Indica si el origen posee la capability indicada por parametro **/
bool Library::HasOriginCapability(const SuriObject::UuidType& OriginId,
                         LibraryItemOrigin::CapabilityType Capability) const {
   LibraryOriginList::const_iterator orderit = libraryOriginList_.begin();
   bool hascapability = true;
   for (; orderit != libraryOriginList_.end(); ++orderit) {
      if ((*orderit)->GetId().compare(OriginId) == 0) {
         hascapability = (*orderit)->HasCapability(Capability);
         break;
      }
   }
   return hascapability;
}

/** Obtiene el origen al id que se pasa por parametro
 *  @return puntero al origen, null en caso contrario
 */
LibraryItemOrigin* Library::GetOrigin(const SuriObject::UuidType& OriginId) {
   LibraryOriginList::const_iterator orderit = libraryOriginList_.begin();
   LibraryItemOrigin* porig = NULL;
   for (; orderit != libraryOriginList_.end() && !porig; ++orderit) {
      if ((*orderit)->GetId().compare(OriginId) == 0) {
         porig = *orderit;
      }
   }
   return porig;
}
/** Metodo que verifica si un item fue marcado como favorito
 *  @param[in] Principal valor del atributo principal del item a consultar
 *  @return true en caso de que el item aparezca en el listado de favoritos
 *  @return false en caso contrario **/
bool Library::IsFavorite(const SuriObject::UuidType& Principal) const {
   LibraryOriginList::const_iterator orderit = libraryOriginList_.begin();
   bool favorite = false;
   for (; !favorite && orderit != libraryOriginList_.end(); ++orderit) {
      favorite = (*orderit)->IsFavorite(Principal);
   }
   return favorite;
}

/** Agrega un item a favoritos **/
bool Library::AddItemToFavorites(const SuriObject::UuidType& ItemId) {
   LibraryOriginList::const_iterator orderit = libraryOriginList_.begin();
   bool favoriteadded = false;
   for (; orderit != libraryOriginList_.end(); ++orderit) {
      favoriteadded |= (*orderit)->AddItemToFavorites(ItemId);
   }
   return favoriteadded;
}

/** Elimina un item de favoritos **/
bool Library::RemoveItemFromFavorites(const SuriObject::UuidType& ItemId) {
   LibraryOriginList::const_iterator orderit = libraryOriginList_.begin();
   bool removefavorite = false;
   for (; orderit != libraryOriginList_.end(); ++orderit) {
      removefavorite |= (*orderit)->RemoveItemFromFavorites(ItemId);
   }
   return removefavorite;
}

}  // namespace suri
