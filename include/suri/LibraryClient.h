/*! \file LibraryClient.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYCLIENT_H_
#define LIBRARYCLIENT_H_

#include <list>
#include <string>
#include <vector>

#include "suri/LibraryItemCatalog.h"
#include "suri/LibraryManager.h"

namespace suri {

/*!
 * Clase padre para todos los clientes de biblioteca.
 * Es la clase encargada de encapsular el manejo de accesos a LibraryManager y el uso
 * de la llave de acceso al mismo. Ademas es el encargado de manejar el acceso a favoritos
 */
class LibraryClient {
   /** constructor copia **/
   LibraryClient(const LibraryClient&);

public:
   /** Constructor **/
   LibraryClient(LibraryManager* pLibraryManager,
                          LibraryManager::AccessType Access = LibraryManager::READONLY);
   /** Destructor **/
   virtual ~LibraryClient();
   /**
    * Obtiene la bibliotecacon el Id pasado por parametro
    * @param[in] LibraryId id de la biblioteca
    * @return referencia a la biblioteca buscada
    * @return NULL en caso de no existir la biblioteca
    */
   const Library* GetLibrary(const SuriObject::UuidType& LibraryId) const;
   /**
    * Obtiene la biblioteca correspondiente al codigo pasada por paramtero
    * @param LibraryCode codigo la biblioteca
    * @return puntero const a la biblioteca correspondiente al codigo solicitado
    * @return NULL en caso de no existir/encontrar la biblioteca
    */
   const Library* GetLibraryByCode(const std::string& LibraryCode) const;
   /**
    * Agrega una biblioteca que se pasa por parametro.
    * @param[in] pLibrary referencia a la biblioteca a agregar
    * @return true en caso de poder agregar la biblioteca, false en caso contrario
    */
   bool AddLibrary(Library* pLibrary);
   /**
    * Elimina la biblioteca correspondiente al id que se pasa por parametro
    * @param[in] LibraryId Id de la biblioteca
    * @return true en caso de poder eliminar la biblioteca, false en caso contrario
    */
   bool RemoveLibrary(const SuriObject::UuidType& LibraryId);
   /**
    * Agrega un item a una biblioteca
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] pItem referencia al item que se desea agregar
    * @return true en caso de poder agregar el item, false en caso contrario
    */
   bool AddItemToLibrary(const SuriObject::UuidType& LibraryId, LibraryItem* pItem);
   /**
    * Agrega un item a una biblioteca en un origen determinado
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] OriginId id del origen al cual se agrega el item
    * @param[in] pItem referencia al item que se desea agregar
    * @return true en caso de poder agregar el item, false en caso contrario
    */
   bool AddItemToLibraryOrigin(const SuriObject::UuidType& LibraryId,
                               const SuriObject::UuidType& OriginId,
                               LibraryItem* pItem);
   /**
    * Elimina un item de la biblioteca correspondiente al id que se pasa por parametro
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] ItemId Id del item a eliminar
    * @return true en caso de poder eliminar el item, false en caso contrario
    */
   bool RemoveItemFromLibrary(const SuriObject::UuidType& LibraryId,
                              const SuriObject::UuidType& ItemId);
   /**
    * Reemplaza un item de la biblioteca correpespondiente al Id pasado por parametro.
    * La instancia anterior del atributo es eliminada
    * y reemplazada por la nueva instancia.
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] pItem referencia del item con el que se va a reemplazar.
    * @return true en caso de poder reemplazar el atributo, false en caso contrario
    */
   bool ReplaceItemFromLibrary(const SuriObject::UuidType& LibraryId,
                               LibraryItem* pItem);

   /**
    * Obtiene un item de la biblioteca correspondiente al id que se pasa por parametro
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] ItemId Id del item a obtener
    * @return referencia al item de biblioteca con el id pasado por parametro
    * @return NULL en caso contrario
    */
   const LibraryItem* GetItemFromLibrary(const SuriObject::UuidType& LibraryId,
                                         const SuriObject::UuidType& ItemId) const;
   /**
    * Agrega un item a favoritos
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] ItemId id del item que se quiere agregar a favoritos
    * @return true en caso de agregar el item a favoritos de forma correcta.
    * @return false en caso contrario (id inexistente).
    */
   bool AddItemToFavorites(const SuriObject::UuidType& LibraryId,
                           const SuriObject::UuidType& ItemId);
   /**
    * Elimina un item a favoritos
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] ItemId id del item que se quiere sacar de favoritos
    * @return true en caso de sacar el item de favoritos de forma correcta.
    * @return false en caso contrario (id inexistente).
    */
   bool RemoveItemFromFavorites(const SuriObject::UuidType& LibraryId,
                                const SuriObject::UuidType& ItemId);
   /**
    *    Devuelve el catalogo de un origen de la biblioteca del origen
    *    indicado por parametro.
    */
   std::vector<const LibraryItemOrigin*> GetOriginsByCategory(
         const SuriObject::UuidType& LibraryId,
         LibraryItemOrigin::OriginCategoryType OriginCategory =
               LibraryItemOrigin::SYSTEM) const;
   /**
    * Devuelve el catalogo de un origen de la biblioteca,filtrado o no por favoritos,
    * del origen indicado por parametro.
    * Es decir, la lista de los items que contiene
    * @return vector con los ids de los items contenidos
    */
   std::vector<LibraryItemCatalog*> GetCatalog(const SuriObject::UuidType& LibraryId,
                                               bool FilterFavorites = false) const;

   /** Metodo que se encarga de reemplzar los atributos de un item de una biblioteca **/
   bool ReplaceAttributesFromItem(
         const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& ItemId,
         std::vector<LibraryItemAttribute*>* pAttributesVector, bool Add = true);

   /** Metodo que se encarga de eliminar los atributos de un item de una biblioteca. */
   bool RemoveAttributesFromItem(const SuriObject::UuidType& LibraryId,
                                 const SuriObject::UuidType& ItemId,
                                 std::vector<LibraryItemAttribute*>* pAttributesVector);

   /* Reemplaza todos los atributos de un item
    * @param ItemId Id del item a reemplazar
    * @param pAttributesVector atributos a reemplazar/agregar **/
   bool SetAttributesFromItem(const SuriObject::UuidType& LibraryId,
                              const SuriObject::UuidType& ItemId,
                              std::vector<LibraryItemAttribute*>* pAttributesVector);
   /** Metodo que se encarga de reemplzar los atributos de un item de una biblioteca **/
   bool ReplaceAttributesFromItemByPrincipal(
         const SuriObject::UuidType& LibraryId, const SuriObject::UuidType& Principal,
         std::vector<LibraryItemAttribute*>* pAttributesVector, bool Add = true);

private:
   LibraryManager::AccessType accessType_;
   LibraryManager::AccessKey accessKey_;
   LibraryManager* pManager_;
   LM_ID_TYPE registrationNumber_;
};

}  // namespace suri

#endif /* LIBRARYCLIENT_H_ */
