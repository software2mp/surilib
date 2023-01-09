/*! \file LibraryManager.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYMANAGER_H_
#define LIBRARYMANAGER_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "Library.h"
#include "suri/ObserverInterface.h"
#include "suri/SuriObject.h"

#define LM_ID_TYPE   long long

namespace suri {

/*!
 * Es la clase encargada de administrar el manejo de bibliotecas.
 * Funciona como intermediaria entre el cliente de la biblioteca y la biblioteca que se
 * solicita. Centraliza el manejo de las bibliotecas en todo el sistema.
 */
class LibraryManager : public ObserverInterface {
   /** ctor copia **/
   LibraryManager(const LibraryManager&);

public:
   /**Constructor **/
   LibraryManager();
   /** Descturctor **/
   virtual ~LibraryManager();
   /** Clase que representa la descripcion del tipo de acceso poseido.**/
   class AccessKey {
   public:
      AccessKey(bool Read = true, bool Write = false, bool Append = false, LM_ID_TYPE Value =
            -1) : read_(Read), write_(Read), append_(Append), value_(Value) {
      }

      bool CanWrite() const { return write_; }

      bool CanRead() const { return read_; }

      bool CanAppend() const { return append_; }

      LM_ID_TYPE GetDescriptionValue() const { return value_; }

   private:
      bool read_;
      bool write_;
      bool append_;
      LM_ID_TYPE value_;
   };

   typedef enum {
      READONLY = 0, WRITEONLY = 1, READWRITE = 2, APPEND = 3, ALL = 4
   } AccessType;

   /**
    * Obtiene la bibliotecacon el Id pasado por parametro
    * @param[in] LibraryId id de la biblioteca
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return referencia a la biblioteca buscada
    * @return NULL en caso de no existir la biblioteca o de no poseer los
    * permisos necesarios.
    */
   const Library* GetLibrary(const SuriObject::UuidType& LibraryId,
                             const AccessKey& Key) const;
   /**
    * Agrega una biblioteca que se pasa por parametro.
    * @param[in] pLibrary referencia a la biblioteca a agregar
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return true en caso de poder agregar la biblioteca, false en caso contrario
    * (ej. no poseer los permisos necesarios).
    */
   bool AddLibrary(Library* pLibrary, const AccessKey& Key);
   /**
    * Elimina la biblioteca correspondiente al id que se pasa por parametro
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return true en caso de poder eliminar la biblioteca, false en caso contrario
    * (ej. no poseer los permisos necesarios).
    */
   bool RemoveLibrary(const SuriObject::UuidType& LibraryId, const AccessKey& Key);
   /**
    * Agrega un item a una biblioteca
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] pItem referencia al item que se desea agregar
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return true en caso de poder agregar el item, false en caso contrario
    * (ej. no poseer los permisos necesarios).
    */
   bool AddItemToLibrary(const SuriObject::UuidType& LibraryId, LibraryItem* pItem,
                         const AccessKey& Key);
   /**
    * Agrega un item a una biblioteca en un origen determinado
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] OriginId id del origen al cual se agrega el item
    * @param[in] pItem referencia al item que se desea agregar
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return true en caso de poder agregar el item, false en caso contrario
    */
   bool AddItemToLibraryOrigin(const SuriObject::UuidType& LibraryId,
                               const SuriObject::UuidType& OriginId,
                               LibraryItem* pItem,
                               const AccessKey& Key);
   /**
    * Elimina un item de la biblioteca correspondiente al id que se pasa por parametro
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] ItemId Id del item a eliminar
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return true en caso de poder eliminar el item, false en caso contrario
    * (ej. no poseer los permisos necesarios).
    */
   bool RemoveItemFromLibrary(const SuriObject::UuidType& LibraryId,
                              const SuriObject::UuidType& ItemId, const AccessKey& Key);
   /**
    * Reemplaza un item de la biblioteca correpespondiente al Id pasado por parametro.
    * La instancia anterior del atributo es eliminada
    * y reemplazada por la nueva instancia.
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] pItem referencia del item con el que se va a reemplazar.
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return true en caso de poder reemplazar el atributo, false en caso contrario
    * (ej. no poseer los permisos necesarios).
    */
   bool ReplaceItemFromLibrary(const SuriObject::UuidType& LibraryId,
                               LibraryItem* pItem, const AccessKey& Key);

   /**
    * Obtiene un item de la biblioteca correspondiente al id que se pasa por parametro
    * @param[in] LibraryId Id de la biblioteca
    * @param[in] ItemId Id del item a obtener
    * @param[in] AccessKey descripcion del accesso. El metodo verifica que el accesso
    * sea el adecuado para la ejecucion del mismo
    * @return referencia al item de biblioteca con el id pasado por parametro
    * @return NULL en caso contrario (ej. no poseer los permisos necesarios)
    */
   const LibraryItem* GetItemFromLibrary(const SuriObject::UuidType& LibraryId,
                                         const SuriObject::UuidType& ItemId,
                                         const AccessKey& Key) const;

   /** Metodo que se encarga de reemplzar los atributos de un item de una biblioteca
    * @param LibraryId Id de la biblioteca a la cual pertenece al item
    * @param ItemId id del item al cual reemplazar los atributos
    * @param pAttributesVector vector con los atributos
    * @param Add boolean que indica si se agrega el attributo en caso de no existir
    * @return true en caso de poder reemplazar los atributos
    * @return false en caso contrario
    */
   bool ReplaceAttributesFromItem(const SuriObject::UuidType& LibraryId,
                                  const SuriObject::UuidType& ItemId,
                                  std::vector<LibraryItemAttribute*>* pAttributesVector,
                                  const AccessKey& Key, bool Add = true);


   /**
    * Metodo que se encarga de eliminar los atributos de un item de una biblioteca.
    */
   bool RemoveAttributesFromItem(const SuriObject::UuidType& LibraryId,
                                 const SuriObject::UuidType& ItemId,
                                 std::vector<LibraryItemAttribute*>* pAttributesVector,
                                 const AccessKey& Key);


   /** Metodo que se encarga de configurar los atributos de un item de una biblioteca
    * pisando los que tiene
    * @param LibraryId Id de la biblioteca a la cual pertenece al item
    * @param ItemId id del item al cual reemplazar los atributos
    * @param pAttributesVector vector con los atributos
    * @param Add boolean que indica si se agrega el attributo en caso de no existir
    * @return true en caso de poder configurar los atributos
    * @return false en caso contrario
    */
   bool SetAttributesFromItem(const SuriObject::UuidType& LibraryId,
                              const SuriObject::UuidType& ItemId,
                              std::vector<LibraryItemAttribute*>* pAttributesVector,
                              const AccessKey& Key);
   /** Metodo que se encarga de reemplzar los atributos de un item de una biblioteca
    * @param LibraryId Id de la biblioteca a la cual pertenece al item
    * @param Principal atributo principal del item al cual reemplazar los atributos
    * @param pAttributesVector vector con los atributos
    * @param Add boolean que indica si se agrega el attributo en caso de no existir
    * @return true en caso de poder reemplazar los atributos
    * @return false en caso contrario
    */
   bool ReplaceAttributesFromItemByPrincipal(const SuriObject::UuidType& LibraryId,
                                  const SuriObject::UuidType& Principal,
                                  std::vector<LibraryItemAttribute*>* pAttributesVector,
                                  const AccessKey& Key, bool Add = true);
   /**
    * Obtiene una llave de acceso para el tipo de cliente solicitado.
    * @param[in] RegistrationNumber numero de registracin asociado al cliente
    * @return AccessKey llave de acceso con los permisos correspondientes.
    * Si el cliente no se encuentra registrado recibe una llave que no posee ningun
    * tipo de permiso.
    * (ej. el numero de registracion es invalido).
    */
   AccessKey GenerateAccessKey(LM_ID_TYPE RegistrationNumber) const;
   /**
    * Regista un cliente de tipo y acceso pasado por parametro
    * @param[in] AccessTypeRequired tipo de acceso requerido del cliente que se quiere
    * registrar.
    * @return identificador de la registracion del cliente (asociacion cliente-acceso)
    * @return -1 en caso de no poder registrar el cliente
    */
   LM_ID_TYPE RegisterClient(AccessType AccessTypeRequired);
   /**
    * Modifica el acceso para un tipo de cliente. Para la modificacion del acceso
    * es necesario saber el id de registro del cliente que se quiere modificar.
    * @param[in] ClientAssociationNumber identificador obtenido durante la registracion
    * del cliente.
    * @param[in] AccessTypeRequired tipo de acceso requerido del cliente que se quiere
    * registrar.
    * @return true en caso de registrar correctamente al cliente, false en caso contrario
    * (ej. cliente ya existente)
    */
   bool ChangeAccessFromClient(LM_ID_TYPE RegistrationNumber, AccessType AccessTypeRequired);
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
   /**Metodo que indica si un item es favorito */
   bool IsFavorite(const SuriObject::UuidType& LibraryId,
                   const SuriObject::UuidType& ItemId) const;
   /**
    * Devuelve el catalogo de un origen de la biblioteca del origen indicado por parametro.
    */
   std::vector<const LibraryItemOrigin*> GetOriginsByCategory(
         const SuriObject::UuidType& LibraryId, const AccessKey& Key,
         LibraryItemOrigin::OriginCategoryType OriginCategory =
               LibraryItemOrigin::SYSTEM) const;
   /**
    * Devuelve el catalogo de un origen de la biblioteca,filtrado o no por favoritos,
    * del origen indicado por parametro.
    * Es decir, la lista de los items que contiene
    * @return vector con los ids de los items contenidos
    */
   std::vector<LibraryItemCatalog*> GetCatalog(const SuriObject::UuidType& LibraryId,
                                               const AccessKey& Key,
                                               bool FilterFavorites = false) const;

   /** Obtiene la biblioteca con el codigo pasado por parametro */
   Library* GetLibraryByCode(const std::string &Code,
                                           const AccessKey& Key) const;
   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification) {
      pNotification->ApplyNotification(this);
   }

   /**
    * Desregistra el cliente con el numero especificado.
    */
   bool UnregisterClient(LM_ID_TYPE RegistrationNumber);

private:
   /** Metodo auxiliar para obtener una biblioteca dado un Id **/
   const Library* GetLibrary(const SuriObject::UuidType& LibraryId)const;
   /** Metodo auxiliar para obtener una biblioteca dado un Id **/
   Library* GetLibraryRef(const SuriObject::UuidType& LibraryId) const;
   /** Metodo auxiliar que dado un catalogo filtra el mismo por favoritos **/
   void FilterCatalogByFavorites(std::vector<LibraryItemCatalog*>* pCatalog) const;
   /** Metodo auxiliar que calcula el numero de registracion para un nuevo cliente*/
   LM_ID_TYPE GenerateRegistrationNumber();
   /** Metodo auxiliar para la validacion de una AccessKey **/
   bool IsValidKey(const AccessKey& Key) const;
   typedef std::list<Library*> LibraryList;
   LibraryList libraries_;
   typedef std::map<LM_ID_TYPE, AccessType> ClientAccessMap;
   ClientAccessMap clientAccessMap_;
   LM_ID_TYPE lastRegistrarionNumber_;
};

}  // namespace suri

#endif /* LIBRARYMANAGER_H_ */
