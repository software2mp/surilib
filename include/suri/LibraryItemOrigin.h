/*! \file LibraryItemOrigin.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIBRARYITEMORIGIN_H_
#define LIBRARYITEMORIGIN_H_

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "suri/SuriObject.h"

namespace suri {

class LibraryItem;
class LibraryItemAttribute;
class LibraryItemCatalog;

/*!
 * Clase padre que representa un origen asociado a un items de biblioteca.
 * Es quien realmente posee la informacion de los  items.
 * Posee el manejo de items en comun para todos los origenes.
 */
class LibraryItemOrigin : public suri::SuriObject {
   LibraryItemOrigin(const LibraryItemOrigin&);

public:
   /** enum con los tipos de origenes **/
   typedef enum {
      NONE = -1, SYSTEM = 0, USER, PROJECT
   } OriginCategoryType;
   /** enum con los tipos de capacidads para origenes
    * ver si vale la pena incluir WRITEONLY **/
   typedef enum {
      CAP_NONE = -1, READ = 0, WRITE, APPEND
   } CapabilityType;
   /** constructor **/
   LibraryItemOrigin(const SuriObject::UuidType& LibraryId, OriginCategoryType OriginCategory,
                     std::vector<LibraryItemOrigin::CapabilityType> Capabilities);
   /** constructor **/
   LibraryItemOrigin(const SuriObject::UuidType& LibraryId, OriginCategoryType OriginCategory,
                     std::vector<LibraryItemOrigin::CapabilityType> Capabilities,
                     const std::string& Name);
   /** destructor **/
   virtual ~LibraryItemOrigin();
   /**
    * Agrega el item pasado por parametro a la origen.
    * @param[in] pItem referencia del item a agregar
    * @return true en caso de agregarlo, false en caso contrario
    */
   bool AddItem(LibraryItem* pItem);
   /**
    * Elimina el item de la origen correspondiente al Id pasado por parametro
    * @param[in] ItemId id del item a eliminar
    * @return true en caso de eliminarlo, false en caso contrario
    */
   bool RemoveItem(const SuriObject::UuidType& ItemId);
   /**
    * Obtiene el item pasado por parametro a la origen
    * @param[in] ItemId id del item a obtener
    * @return LibraryItem* referencia al item solicitado
    * @return NULL en caso de no encontrar el item solicitado
    */
   const LibraryItem* GetItem(const SuriObject::UuidType& ItemId) const;
   /**
    * Obtiene el item pasado por parametro a la origen
    * @param[in] Code codigo del item a obtener
    * @return LibraryItem* referencia al item solicitado
    * @return NULL en caso de no encontrar el item solicitado
    */
   const LibraryItem* GetItemByCode(const std::string& Code);
   /**
    * Reemplaza un item de la origen
    * @param[in] pItem referencia del item a reemplazar
    * @return true en caso de poder reemplazarlo, false en caso contrario
    */
   bool ReplaceItem(LibraryItem* pItem);
   /**
    * Reemplaza los atributos de un item
    * @param ItemId Id del item a reemplazar
    * @param pAttributesVector atributos a reemplazar/agregar
    * @param Add boolean que indica si se agrega el atributo en caso de no existir
    * @return true en caso de reemplazar los atributos en el item
    * @return false en caso contrario
    */
   bool ReplaceAttributesFromItem(const SuriObject::UuidType& ItemId,
                                  std::vector<LibraryItemAttribute*>* pAttributesVector,
                                  bool Add = true);


   /**
    * Elimina los atributos de un item.
    */
   bool RemoveAttributesFromItem(const SuriObject::UuidType& ItemId,
                                 std::vector<LibraryItemAttribute*>* pAttributesVector);

   /* Reemplaza todos los atributos de un item
    * @param ItemId Id del item a reemplazar
    * @param pAttributesVector atributos a reemplazar/agregar **/
   bool SetAttributesFromItem(const SuriObject::UuidType& ItemId,
                              std::vector<LibraryItemAttribute*>* pAttributesVector);
   /**
    * Reemplaza los atributos de un item
    * @param Principal Atributo principal del item a reemplazar
    * @param pAttributesVector atributos a reemplazar/agregar
    * @param Add boolean que indica si se agrega el atributo en caso de no existir
    * @return true en caso de reemplazar los atributos en el item
    * @return false en caso contrario
    */
   bool ReplaceAttributesFromItemByPrincipal(const SuriObject::UuidType& Principal,
                                        std::vector<LibraryItemAttribute*>* pAttributesVector,
                                        bool Add = true);
   /**
    * Devuelve el catalogo (un resumen de todos los items) del origen.
    * @return vector de items de catalogo de la biblioteca
    */
   std::vector<LibraryItemCatalog*> GetCatalog(bool FilterFavorites = false) const;
   /** Agrega una capacidad al origen*/
   bool AddCapability(CapabilityType Capability);
   /**
    * Devuelve las capacidades del origen **/
   std::vector<CapabilityType> GetCapabilities() const;
   /** Pregunta por una capacidad puntual del origen**/
   bool HasCapability(CapabilityType Capability) const;
   /**
    * Devuelve el tipo de origen
    * @return Tipo de origen
    */
   OriginCategoryType GetCategory() const;

#ifdef __DEBUG__
   /** Devuelve la lista en un string con (id, principal) por cada item **/
   std::string GetListAsString() const;
#endif
   /** Devuelve el nombre del origen **/
   std::string GetName() const;
   /** Remueve todos los elementos de la lista **/
   void RemoveAllItems();
   /** Indica cual es el archivo correspondiente al indices de favoritos */
   void SetFavoritesIndexFile(const std::string& FileName);
   /** Obtiene el nombre del archivo de indices de favoritos */
   std::string GetFavoritesIndexFileName() const;
   /** Metodo que verifica si un item fue marcado como favorito **/
   bool IsFavorite(const SuriObject::UuidType& ItemId) const;
   /** Agrega un item a favoritos **/
   bool AddItemToFavorites(const SuriObject::UuidType& ItemId);
   /** Elimina un item de favoritos **/
   bool RemoveItemFromFavorites(const SuriObject::UuidType& ItemId);

protected:
   /**
    * Agrega el item pasado por parametro a la fuente de datos del origen.
    * Metodo que tiene un comportamiento particular para cada tipo de origen.
    * (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual bool AddItemToSource(LibraryItem* pItem);
   /**
    * Elimina el item de la fuente de datos del origen correspondiente al codigo
    * pasado por parametro
    * Metodo que tiene un comportamiento particular para cada tipo de origen.
    * (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual bool RemoveItemFromSourceByCode(const std::string& Code);
   /**
    * Elimina el item de la fuente de datos del origen correspondiente al id
    * que se pasa por parametro
    * Metodo que tiene un comportamiento particular para cada tipo de origen.
    * (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual bool RemoveItemFromSource(const SuriObject::UuidType& ItemId);
   /**
    * Obtiene el item pasado por parametro a la fuente de datos del origen
    * Metodo que tiene un comportamiento particular para cada tipo de origen.
    * (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual const LibraryItem* GetItemByCodeFromSource(const std::string& Code);
   /**
    * Reemplaza un item de la fuente de datos del origen
    * Metodo que tiene un comportamiento particular para cada tipo de origen.
    * (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual bool ReplaceItemFromSource(LibraryItem* pItem);
   /**
    * Permite actualizar el item en memoria en caso que este incompleto o
    * haya quedado desactualizado con el origen de datos concreto.
    * (una clase concreta de origen tiene que reimplementar este metodo)
    * @param[in] pItem item que quiero actualizar
    * @return indica si se pudo actualizar el item.
    */
   virtual bool UpdateItem(LibraryItem* pItem) const;
   /** Metodo auxiliar que se encarga de levantar el indice de favoritos que se encuentra
    * configurado
    * Precondicion: el archivo de indices se encuentra bien configurado. Esto quiere
    * decir que se encuentra un id por linea del archivo y que ademas ese
    * id existe dentro de la lista de items disponibles en la biblioteca
    */
   void LoadFavoritesIndex();
   /** Metodo auxiliar que se encarga de cargar los items del origen **/
   /** Id de la biblioteca que corresponde el origen **/
   SuriObject::UuidType libraryId_;
   /** Tipo de origen, Sistema, Usuario,... */
   OriginCategoryType originCategory_;
   /** Tipo de capacidad requerido por el origen **/
   std::vector<CapabilityType> capabilites_;
   /** Items asociados al origen **/
   typedef std::list<LibraryItem*> LibraryItemList;
   LibraryItemList items_;
   /** Asociacion Id-Codigo para items **/
   typedef std::map<std::string, SuriObject::UuidType> IdCodeMap;
   IdCodeMap idCodeMap_;
   std::string name_;
   std::string favoritesIndexFileName_;
   /** conjunto de items que pertenecen a favoritos **/
   std::set<std::string> favoritesIndex_;
};

}  // namespace suri

#endif /* LIBRARYITEMORIGIN_H_ */
