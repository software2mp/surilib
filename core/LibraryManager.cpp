/*! \file LibraryManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryManager.h"

namespace suri {

/** Constructor **/
LibraryManager::LibraryManager() : lastRegistrarionNumber_(0) {

}

/** Desctructor: Elimina todas las bibliotecas **/
LibraryManager::~LibraryManager() {
   delete_all(libraries_);
}

/**
 * Obtiene la bibliotecacon el Id pasado por parametro
 * @param[in] LibraryId id de la biblioteca.
 * @param[in] Key descripcion del accesso. El metodo verifica que el accesso
 * sea el adecuado para la ejecucion del mismo.
 * @return referencia a la biblioteca buscada
 * @return NULL en caso de no existir la biblioteca o de no poseer los
 * permisos necesarios.
 */
const Library* LibraryManager::GetLibrary(const SuriObject::UuidType& LibraryId,
                                          const AccessKey& Key) const {
   if (!Key.CanRead() || !IsValidKey(Key))
      return NULL;
   const Library* plibrary = GetLibrary(LibraryId);
   return plibrary;
}

/** Metodo auxiliar para obtener una biblioteca dado un Id **/
Library* LibraryManager::GetLibraryRef(const SuriObject::UuidType& LibraryId) const {
   Library* plibrary = NULL;
   LibraryList::const_iterator findit = libraries_.begin();
   for (; findit != libraries_.end(); ++findit) {
      if ((*findit)->GetId().compare(LibraryId) == 0) {
         plibrary = *findit;
      }
   }
   return plibrary;
}

/**
 * Agrega una biblioteca que se pasa por parametro.
 * @param[in] pLibrary referencia a la biblioteca a agregar
 * @param[in] Key descripcion del accesso. El metodo verifica que
 * el accesso sea el adecuado para la ejecucion del mismo
 * @return true en caso de poder agregar la biblioteca, false en caso contrario
 * (ej. no poseer los permisos necesarios).
 */
bool LibraryManager::AddLibrary(Library* pLibrary,
                                const AccessKey& Key) {
   if (!Key.CanAppend() || !IsValidKey(Key))
      return false;
   bool added = false;
   if (!GetLibraryRef(pLibrary->GetId())) {
      added = true;
      libraries_.push_back(pLibrary);
   }
   return added;
}

/**
 * Elimina la biblioteca correspondiente al id que se pasa por parametro
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] Key descripcion del accesso. El metodo verifica que
 * el accesso sea el adecuado para la ejecucion del mismo
 * @return true en caso de poder eliminar la biblioteca, false en caso contrario
 * (ej. no poseer los permisos necesarios).
 */
bool LibraryManager::RemoveLibrary(const SuriObject::UuidType& LibraryId,
                                   const AccessKey& Key) {
   if (!Key.CanWrite() || !IsValidKey(Key))
      return false;
   bool removed = false;
   LibraryList::iterator findit = libraries_.begin();
   for (; !removed && findit != libraries_.end(); ++findit) {
      if ((*findit)->GetId().compare(LibraryId) == 0) {
         removed = true;
         Library* plibrary = *findit;
         libraries_.erase(findit);
         delete plibrary;
      }
   }
   return removed;
}

/**
 * Agrega un item a una biblioteca
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] pItem referencia al item que se desea agregar
 * @param[in] Key descripcion del accesso. El metodo verifica que
 * el accesso sea el adecuado para la ejecucion del mismo
 * @return true en caso de poder agregar el item, false en caso contrario
 * (ej. no poseer los permisos necesarios).
 */
bool LibraryManager::AddItemToLibrary(const SuriObject::UuidType& LibraryId,
                                      LibraryItem* pItem,
                                      const AccessKey& Key) {
   if (!Key.CanAppend() || !IsValidKey(Key))
      return false;
   bool itemadded = false;
   Library* plibrary = GetLibraryRef(LibraryId);
   if (plibrary) {
      itemadded = plibrary->AddItem(pItem);
   }
   return itemadded;
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
bool LibraryManager::AddItemToLibraryOrigin(const SuriObject::UuidType& LibraryId,
                            const SuriObject::UuidType& OriginId,
                            LibraryItem* pItem,
                            const AccessKey& Key) {
   if (!Key.CanAppend() || !IsValidKey(Key))
      return false;
   Library* plibrary = GetLibraryRef(LibraryId);
   if (!plibrary)
      return false;
   return plibrary->AddItemToLibraryOrigin(OriginId, pItem);
}

/**
 * Elimina un item de la biblioteca correspondiente al id que se pasa por parametro
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] ItemId Id del item a eliminar
 * @param[in] Key descripcion del accesso. El metodo verifica que
 * el accesso sea el adecuado para la ejecucion del mismo
 * @return true en caso de poder eliminar el item, false en caso contrario
 * (ej. no poseer los permisos necesarios).
 */
bool LibraryManager::RemoveItemFromLibrary(const SuriObject::UuidType& LibraryId,
                                           const SuriObject::UuidType& ItemId,
                                           const AccessKey& Key) {
   if (!Key.CanWrite() || !IsValidKey(Key))
      return false;
   bool itemremoved = false;
   Library* plibrary = GetLibraryRef(LibraryId);
   if (plibrary) {
      itemremoved = plibrary->RemoveItem(ItemId);
   }
   return itemremoved;
}

/**
 * Reemplaza un item de la biblioteca correpespondiente al Id pasado por parametro.
 * La instancia anterior del atributo es eliminada
 * y reemplazada por la nueva instancia.
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] pItem referencia del item con el que se va a reemplazar.
 * @param[in] Key descripcion del accesso. El metodo verifica
 * que el accesso sea el adecuado para la ejecucion del mismo
 * @return true en caso de poder reemplazar el atributo, false en caso contrario
 * (ej. no poseer los permisos necesarios).
 */
bool LibraryManager::ReplaceItemFromLibrary(const SuriObject::UuidType& LibraryId,
                                            LibraryItem* pItem,
                                            const AccessKey& Key) {
   if (!Key.CanWrite() || !IsValidKey(Key))
      return false;
   bool itemreplaced = false;
   Library* plibrary = GetLibraryRef(LibraryId);
   if (plibrary) {
      itemreplaced = plibrary->ReplaceItem(pItem);
   }
   return itemreplaced;
}

/**
 * Obtiene un item de la biblioteca correspondiente al id que se pasa por parametro
 * @param[in] LibraryId Id de la biblioteca
 * @param[in] ItemId Id del item a obtener
 * @param[in] Key descripcion del accesso. El metodo verifica
 * que el accesso sea el adecuado para la ejecucion del mismo
 * @return referencia al item de biblioteca con el id pasado por parametro
 * @return NULL en caso contrario (ej. no poseer los permisos necesarios)
 */
const LibraryItem* LibraryManager::GetItemFromLibrary(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& ItemId,
      const AccessKey& Key) const {
   if (!Key.CanRead() || !IsValidKey(Key))
      return NULL;
   const LibraryItem* pitem = NULL;
   const Library* plibrary = GetLibrary(LibraryId);
   if (plibrary) {
      pitem = plibrary->GetItem(ItemId);
   }
   return pitem;
}


/** Metodo que se encarga de reemplzar los atributos de un item de una biblioteca
 * @param LibraryId Id de la biblioteca a la cual pertenece al item
 * @param ItemId id del item al cual reemplazar los atributos
 * @param pAttributesVector vector con los atributos
 * @param Add boolean que indica si se agrega el attributo en caso de no existir
 * @return true en caso de poder reemplazar los atributos
 * @return false en caso contrario
 */
bool LibraryManager::ReplaceAttributesFromItem(const SuriObject::UuidType& LibraryId,
                               const SuriObject::UuidType& ItemId,
                               std::vector<LibraryItemAttribute*>* pAttributesVector,
                               const AccessKey& Key, bool Add) {
   if (!Key.CanWrite() || !IsValidKey(Key))
        return false;
   bool attributesreplaced = false;
   Library* plibrary = GetLibraryRef(LibraryId);
   if (plibrary) {
      attributesreplaced =
            plibrary->ReplaceAttributesFromItem(ItemId, pAttributesVector, Add);
   }
   return attributesreplaced;
}

/**
 * Metodo que se encarga de eliminar los atributos de un item de una biblioteca.
 */
bool LibraryManager::RemoveAttributesFromItem(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& ItemId,
      std::vector<LibraryItemAttribute*>* pAttributesVector, const AccessKey& Key) {
   if (!Key.CanWrite() || !IsValidKey(Key))
      return false;

   bool attributesreplaced = false;

   Library* plibrary = GetLibraryRef(LibraryId);
   if (plibrary != NULL) {
      attributesreplaced = plibrary->RemoveAttributesFromItem(ItemId,
                                                              pAttributesVector);
   }
   return attributesreplaced;
}

/**
 * Metodo que se encarga de configurar los atributos de un item de una biblioteca
 * pisando los que tiene
 */
bool LibraryManager::SetAttributesFromItem(const SuriObject::UuidType& LibraryId,
                           const SuriObject::UuidType& ItemId,
                           std::vector<LibraryItemAttribute*>* pAttributesVector,
                           const AccessKey& Key) {
   if (!Key.CanWrite() || !IsValidKey(Key))
        return false;
   bool attributesreplaced = false;
   Library* plibrary = GetLibraryRef(LibraryId);
   if (plibrary) {
      attributesreplaced =
            plibrary->SetAttributesFromItem(ItemId, pAttributesVector);
   }
   return attributesreplaced;
}

/** Metodo que se encarga de reemplzar los atributos de un item de una biblioteca
 * @param LibraryId Id de la biblioteca a la cual pertenece al item
 * @param Principal atributo principal del item al cual reemplazar los atributos
 * @param pAttributesVector vector con los atributos
 * @param Add boolean que indica si se agrega el attributo en caso de no existir
 * @return true en caso de poder reemplazar los atributos
 * @return false en caso contrario
 */
bool LibraryManager::ReplaceAttributesFromItemByPrincipal(
      const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& Principal,
      std::vector<LibraryItemAttribute*>* pAttributesVector, const AccessKey& Key,
      bool Add) {
   if (!Key.CanWrite() || !IsValidKey(Key))
        return false;
   bool attributesreplaced = false;
   Library* plibrary = GetLibraryRef(LibraryId);
   if (plibrary) {
      attributesreplaced =
            plibrary->ReplaceAttributesFromItemByPrincipal(Principal, pAttributesVector, Add);
   }
   return attributesreplaced;
}

/**
 * Obtiene una llave de acceso para el tipo de cliente solicitado.
 * @param[in] RegistrationNumber numero de registracion asociado al cliente
 * @return AccessKey llave de acceso con los permisos correspondientes.
 * Si el cliente no se encuentra registrado recibe una llave que no posee ningun
 * tipo de permiso.
 * (ej. el numero de registracion es invalido).
 */
LibraryManager::AccessKey LibraryManager::GenerateAccessKey(
      LM_ID_TYPE RegistrationNumber) const {
   bool read = false, write = false, append = false;
   LM_ID_TYPE value = -1;
   ClientAccessMap::const_iterator findit = clientAccessMap_.find(RegistrationNumber);
   if (findit != clientAccessMap_.end()) {
      value = RegistrationNumber;
      switch (findit->second) {
         case READONLY:
            read = true;
            break;
         case WRITEONLY:
            write = true;
            break;
         case READWRITE:
            read = true;
            write = true;
            break;
         case APPEND:
            append = true;
            break;
         case ALL:
            append = true;
            read = true;
            write = true;
            break;
         default:
            break;
      }
   }
   return AccessKey(read, write, append, value);
}

/**
 * Registra un cliente de tipo y acceso pasado por parametro
 * @param[in] AccessTypeRequired tipo de acceso requerido del cliente que se quiere
 * registrar.
 * @return identificador de la registracion del cliente (asociacion cliente-acceso)
 * @return -1 en caso de no poder registrar el cliente
 */
LM_ID_TYPE LibraryManager::RegisterClient(LibraryManager::AccessType AccessTypeRequired) {
   LM_ID_TYPE registrationnumber = GenerateRegistrationNumber();
   if (registrationnumber > -1) {
      std::pair<ClientAccessMap::iterator, bool> resultpair = clientAccessMap_.insert(
            std::make_pair(registrationnumber, AccessTypeRequired));

      if (resultpair.second == false) return -1;
   }
   return registrationnumber;
}

/**
 * Modifica el acceso para un tipo de cliente. Para la modificacion del acceso
 * es necesario saber el id de registro del cliente que se quiere modificar.
 * Postcondicion: Para actualizar los permisos la llave debe ser regenerada
 * @param[in] RegistrationNumber identificador obtenido durante la registracion
 * del cliente.
 * @param[in] AccessTypeRequired tipo de acceso requerido del cliente que se quiere
 * registrar.
 * @return true en caso de registrar correctamente al cliente, false en caso contrario
 * (ej. cliente ya existente)
 */
bool LibraryManager::ChangeAccessFromClient(LM_ID_TYPE RegistrationNumber,
                            LibraryManager::AccessType AccessTypeRequired) {
   bool accesschanged = false;
   ClientAccessMap::iterator findit = clientAccessMap_.find(RegistrationNumber);
   if (findit != clientAccessMap_.end()) {
      LM_ID_TYPE registrationnumber = findit->first;
      clientAccessMap_.erase(findit);
      clientAccessMap_.insert(std::make_pair(registrationnumber, AccessTypeRequired));
      accesschanged = true;
   }
   return accesschanged;
}

/**
 * Metodo auxiliar que calcula el numero de registracion para un nuevo cliente.
 * (9223372036854775807 ids deberian alcanzar)
 *
 * @return entero que representa el numero de registracion del nuevo cliente
 * @return -1 en caso de que no se pueda registrar al cliente
 */
LM_ID_TYPE LibraryManager::GenerateRegistrationNumber() {
   if (lastRegistrarionNumber_ == std::numeric_limits<LM_ID_TYPE>::max())
      return -1;

   return ++lastRegistrarionNumber_;
}

/** Metodo auxiliar para la validacion de una AccessKey
 * Realiza la verificacion del valor descriptivo de la llave
 * (especie de CheckSum)
 * @return true en caso de que la llave sea valida
 * @return false en caso de llave invalida
 * **/
bool LibraryManager::IsValidKey(const AccessKey& Key) const {
   ClientAccessMap::const_iterator findit =
         clientAccessMap_.find(Key.GetDescriptionValue());
   return (findit != clientAccessMap_.end());
}

/**
 * Agrega un item a favoritos
 * @param[in] ItemId id del item que se quiere agregar a favoritos
 * @return true en caso de agregar el item a favoritos de forma correcta.
 * @return false en caso contrario (id inexistente).
 */
bool LibraryManager::AddItemToFavorites(const SuriObject::UuidType& LibraryId,
                                        const SuriObject::UuidType& ItemId) {
   Library* plibrary = GetLibraryRef(LibraryId);
   return plibrary->AddItemToFavorites(ItemId);
}

/**
 * Elimina un item a favoritos
 * @param[in] ItemId id del item que se quiere sacar de favoritos
 * @return true en caso de sacar el item de favoritos de forma correcta.
 * @return false en caso contrario (id inexistente).
 */
bool LibraryManager::RemoveItemFromFavorites(const SuriObject::UuidType& LibraryId,
                                             const SuriObject::UuidType& ItemId) {
   Library* plibrary = GetLibraryRef(LibraryId);
   return plibrary->RemoveItemFromFavorites(ItemId);
}

/**
 * Devuelve los origenes de la biblioteca que corresponden a la categoria asociada.
 * @param[in] LibraryId Id de la libreria que se quiere obtener el catalogo
 * @param[in] Key descripcion del accesso que posee el cliente del metodo.
 * @param[in] OriginCategory categoria de origen
 * @return vector con los origenes correspondientes a la categoria solicitada
 */
std::vector<const LibraryItemOrigin*> LibraryManager::GetOriginsByCategory(
      const SuriObject::UuidType& LibraryId, const AccessKey& Key,
      LibraryItemOrigin::OriginCategoryType OriginCategory) const {
   std::vector<const LibraryItemOrigin*> origins;
   if (!Key.CanRead() || !IsValidKey(Key))
      return origins;
   const Library* plibrary = GetLibrary(LibraryId);
   if (plibrary) {
      origins = plibrary->GetOriginsByCategory(OriginCategory);
   }
   return origins;
}

/**
 * Devuelve el catalogo de un origen de la biblioteca,filtrado o no por favoritos,
 * del origen indicado por parametro.
 * Es decir, la lista de los items que contiene
 * @return vector con los ids de los items contenidos
 */
std::vector<LibraryItemCatalog*> LibraryManager::GetCatalog(
      const SuriObject::UuidType& LibraryId, const AccessKey& Key,
      bool FilterFavorites) const {
   std::vector<LibraryItemCatalog*> catalog;
   if (!Key.CanRead() || !IsValidKey(Key))
      return catalog;
   const Library* plibrary = GetLibrary(LibraryId);
   if (plibrary) {
      catalog = plibrary->GetCatalog(FilterFavorites);
   }
   return catalog;
}


/**
 * Obtiene la biblioteca con el codigo pasado por parametro
 * @param[in] Code codigo de la biblioteca
 * @param[in] Key descripcion del accesso. El metodo verifica que el accesso
 * sea el adecuado para la ejecucion del mismo
 * @return referencia a la biblioteca buscada
 * @return NULL en caso de no existir la biblioteca o de no poseer los
 * permisos necesarios.
 */
Library* LibraryManager::GetLibraryByCode(const std::string &Code,
                                                  const AccessKey& Key) const {
   Library* plibrary = NULL;
   LibraryList::const_iterator findit = libraries_.begin();
   for (; findit != libraries_.end(); ++findit) {
      if ((*findit)->GetLibraryCode().compare(Code) == 0)
         plibrary = *findit;
   }
   return plibrary;
}

/**
 * Desregistra el cliente con el numero especificado.
 */
bool LibraryManager::UnregisterClient(LM_ID_TYPE RegistrationNumber) {
   bool retval = false;
   ClientAccessMap::iterator findit = clientAccessMap_.find(RegistrationNumber);
   if (findit != clientAccessMap_.end()) {
      clientAccessMap_.erase(findit);
      retval = true;
   }
   return retval;
}

/** Metodo auxiliar para obtener una biblioteca dado un Id **/
const Library* LibraryManager::GetLibrary(const SuriObject::UuidType& LibraryId) const {
   const Library* plibrary = NULL;
   LibraryList::const_iterator findit = libraries_.begin();
   for (; findit != libraries_.end(); ++findit) {
      if ((*findit)->GetId().compare(LibraryId) == 0) {
         plibrary = *findit;
      }
   }
   return plibrary;
}

/** Metodo auxiliar que dado un catalogo filtra el mismo por favoritos **/
void LibraryManager::FilterCatalogByFavorites(
      std::vector<LibraryItemCatalog*>* pCatalog) const {
}

/**Metodo que indica si un item es favorito
 * @return true en caso de que el item se encuentra en la lista de favoritos
 * @return false en caso de que no este marcado como favorito */
bool LibraryManager::IsFavorite(const SuriObject::UuidType& LibraryId,
                                const SuriObject::UuidType& ItemId) const {
   Library* plibrary = GetLibraryRef(LibraryId);
    return plibrary->IsFavorite(ItemId);
}

}  // namespace suri
