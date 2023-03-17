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
#include <vector>

#include "suri/LibraryClient.h"

namespace suri {

/**
 * Ctor.
 */
LibraryClient::LibraryClient(LibraryManager* pLibraryManager,
                             LibraryManager::AccessType Access) :
      accessType_(Access), pManager_(pLibraryManager), registrationNumber_(0) {
   if (pManager_) {
      registrationNumber_ = pManager_->RegisterClient(Access);
      accessKey_ = pManager_->GenerateAccessKey(registrationNumber_);
   }
}

/**
 * Dtor.
 */
LibraryClient::~LibraryClient() {
   if (pManager_ != NULL) pManager_->UnregisterClient(registrationNumber_);
}

/**
 * Obtiene la bibliotecacon el Id pasado por parametro
 * @param[in] LibraryId id de la biblioteca
 * @return referencia a la biblioteca buscada
 * @return NULL en caso de no existir la biblioteca
 */
const Library* LibraryClient::GetLibrary(const SuriObject::UuidType& LibraryId) const {
   LibraryManager::AccessKey accesskey = accessKey_;
   if (!accessKey_.CanRead() || accessKey_.GetDescriptionValue() < 0) {
      /**
       * Mini-fix para cuando se desconfigura el access key. Se reinicia el accesskey
       * Si el access es invalido no se puede obtener la biblioteca. Esto hace que
       * a veces la aplicacion no muestre los items de biblioteca en el CGU
       */
      int newregistrationnumber = pManager_->RegisterClient(accessType_);
      accesskey = pManager_->GenerateAccessKey(newregistrationnumber);
   }
   return pManager_ ? pManager_->GetLibrary(LibraryId, accesskey) : NULL;
}
/**
 * Agrega una biblioteca que se pasa por parametro.
 * @param[in] pLibrary referencia a la biblioteca a agregar
 * @return true en caso de poder agregar la biblioteca, false en caso contrario
 */
bool LibraryClient::AddLibrary(Library* pLibrary) {
   return pManager_ ? pManager_->AddLibrary(pLibrary, accessKey_) : false;
}
/**
 * Elimina la biblioteca correspondiente al id que se pasa por parametro
 * @param[in] LibraryId Id de la biblioteca
 * @return true en caso de poder eliminar la biblioteca, false en caso contrario
 */
bool LibraryClient::RemoveLibrary(const SuriObject::UuidType& LibraryId) {
   return pManager_ ? pManager_->RemoveLibrary(LibraryId, accessKey_) : false;
}
/**
 * Agrega un item a una biblioteca
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] pItem referencia al item que se desea agregar
 * @return true en caso de poder agregar el item, false en caso contrario
 */
bool LibraryClient::AddItemToLibrary(const SuriObject::UuidType& LibraryId,
                                     LibraryItem* pItem) {
   return pManager_ ? pManager_->AddItemToLibrary(LibraryId, pItem, accessKey_) : false;
}

/**
 * Agrega un item a una biblioteca en un origen determinado
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] OriginId id del origen al cual se agrega el item
 * @param[in] pItem referencia al item que se desea agregar
 * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
 * sea el adecuado para la ejecucion del mismo
 * @return true en caso de poder agregar el item, false en caso contrario
 */
bool LibraryClient::AddItemToLibraryOrigin(const SuriObject::UuidType& LibraryId,
                                           const SuriObject::UuidType& OriginId,
                                           LibraryItem* pItem) {
   return
         pManager_ ? pManager_->AddItemToLibraryOrigin(LibraryId, OriginId, pItem,
                                                       accessKey_) :
                     false;
}
/**
 * Elimina un item de la biblioteca correspondiente al id que se pasa por parametro
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] ItemId Id del item a eliminar
 * @return true en caso de poder eliminar el item, false en caso contrario
 */
bool LibraryClient::RemoveItemFromLibrary(const SuriObject::UuidType& LibraryId,
                                          const SuriObject::UuidType& ItemId) {
   return
         pManager_ ? pManager_->RemoveItemFromLibrary(LibraryId, ItemId, accessKey_) :
                     false;
}
/**
 * Reemplaza un item de la biblioteca correpespondiente al Id pasado por parametro.
 * La instancia anterior del atributo es eliminada
 * y reemplazada por la nueva instancia.
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] pItem referencia del item con el que se va a reemplazar.
 * @return true en caso de poder reemplazar el atributo, false en caso contrario
 */
bool LibraryClient::ReplaceItemFromLibrary(const SuriObject::UuidType& LibraryId,
                                           LibraryItem* pItem) {
   return
         pManager_ ? pManager_->ReplaceItemFromLibrary(LibraryId, pItem, accessKey_) :
                     false;
}

/**
 * Obtiene un item de la biblioteca correspondiente al id que se pasa por parametro
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] ItemId Id del item a obtener
 * @return referencia al item de biblioteca con el id pasado por parametro
 * @return NULL en caso contrario
 */
const LibraryItem* LibraryClient::GetItemFromLibrary(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& ItemId) const {
   return
         pManager_ ? pManager_->GetItemFromLibrary(LibraryId, ItemId, accessKey_) : NULL;
}

/**
 * Agrega un item a favoritos
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] ItemId id del item que se quiere agregar a favoritos
 * @return true en caso de agregar el item a favoritos de forma correcta.
 * @return false en caso contrario (id inexistente).
 */
bool LibraryClient::AddItemToFavorites(const SuriObject::UuidType& LibraryId,
                                       const SuriObject::UuidType& ItemId) {
   return pManager_ ? pManager_->AddItemToFavorites(LibraryId, ItemId) : false;
}

/**
 * Elimina un item a favoritos
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] ItemId id del item que se quiere sacar de favoritos
 * @return true en caso de sacar el item de favoritos de forma correcta.
 * @return false en caso contrario (id inexistente).
 */
bool LibraryClient::RemoveItemFromFavorites(const SuriObject::UuidType& LibraryId,
                                            const SuriObject::UuidType& ItemId) {
   return pManager_ ? pManager_->RemoveItemFromFavorites(LibraryId, ItemId) : false;
}

/**
 * Devuelve los origenes de la biblioteca que corresponden a la categoria asociada.
 * @param[in] LibraryId Id de la libreria que se quiere obtener el catalogo
 * @param[in] OriginCategory categoria de origen
 * @return vector con los origenes correspondientes a la categoria solicitada
 */
std::vector<const LibraryItemOrigin*> LibraryClient::GetOriginsByCategory(
      const SuriObject::UuidType& LibraryId,
      LibraryItemOrigin::OriginCategoryType OriginCategory) const {
   if (pManager_)
      return pManager_->GetOriginsByCategory(LibraryId, accessKey_, OriginCategory);
   std::vector<const LibraryItemOrigin*> origins;
   return origins;
}

/**
 * Devuelve el catalogo de un origen de la biblioteca,filtrado o no por favoritos,
 * del origen indicado por parametro.
 * Es decir, la lista de los items que contiene
 * @return vector con los ids de los items contenidos
 */
std::vector<LibraryItemCatalog*> LibraryClient::GetCatalog(
      const SuriObject::UuidType& LibraryId, bool FilterFavorites) const {
   if (pManager_) return pManager_->GetCatalog(LibraryId, accessKey_, FilterFavorites);
   std::vector<LibraryItemCatalog*> catalogs;
   return catalogs;
}

/**
 * Metodo que se encarga de reemplzar los atributos de un item de una biblioteca
 * @param LibraryId Id de la biblioteca a la cual pertenece al item
 * @param ItemId id del item al cual reemplazar los atributos
 * @param pAttributesVector vector con los atributos
 * @param Add boolean que indica si se agrega el attributo en caso de no existir
 * @return true en caso de poder reemplazar los atributos
 * @return false en caso contrario
 */
bool LibraryClient::ReplaceAttributesFromItem(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector, bool Add) {
   return
         pManager_ ? pManager_->ReplaceAttributesFromItem(LibraryId, ItemId,
                                                          pAttributesVector, accessKey_,
                                                          Add) :
                     false;
}

/**
 * Metodo que se encarga de eliminar los atributos de un item de una biblioteca.
 */
bool LibraryClient::RemoveAttributesFromItem(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector) {
   bool retval = false;

   if (pManager_ != NULL) {
      retval = pManager_->RemoveAttributesFromItem(LibraryId, ItemId,
                                                    pAttributesVector, accessKey_);
   }

   return retval;
}

/** Metodo que se encarga de reemplzar los atributos de un item de una biblioteca **/
bool LibraryClient::ReplaceAttributesFromItemByPrincipal(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& Principal,
      std::vector<LibraryItemAttribute*>* pAttributesVector, bool Add) {
   return
         pManager_ ? pManager_->ReplaceAttributesFromItemByPrincipal(LibraryId,
                                                                     Principal,
                                                                     pAttributesVector,
                                                                     accessKey_, Add) :
                     false;
}

/**
 * Obtiene la biblioteca correspondiente al codigo pasada por paramtero
 * @param LibraryCode codigo la biblioteca
 * @return puntero const a la biblioteca correspondiente al codigo solicitado
 * @return NULL en caso de no existir/encontrar la biblioteca
 */
const Library* LibraryClient::GetLibraryByCode(const std::string& LibraryCode) const {
   LibraryManager::AccessKey accesskey = accessKey_;
   if (!accessKey_.CanRead() || accessKey_.GetDescriptionValue() < 0) {
      /**
       * Mini-fix para cuando se desconfigura el access key. Se reinicia el accesskey
       * Si el access es invalido no se puede obtener la biblioteca. Esto hace que
       * a veces la aplicacion no muestre los items de biblioteca en el CGU
       */
      int newregistrationnumber = pManager_->RegisterClient(accessType_);
      accesskey = pManager_->GenerateAccessKey(newregistrationnumber);
   }
   return pManager_ ? pManager_->GetLibraryByCode(LibraryCode, accesskey) : NULL;
}

/* Reemplaza todos los atributos de un item
 * @param ItemId Id del item a reemplazar
 * @param pAttributesVector atributos a reemplazar/agregar **/
bool LibraryClient::SetAttributesFromItem(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector) {
   return
         pManager_ ? pManager_->SetAttributesFromItem(LibraryId, ItemId,
                                                      pAttributesVector, accessKey_) :
                     false;
}

}  // namespace suri
