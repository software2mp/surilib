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

#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <list>
#include <set>
#include <string>
#include <vector>

#include "suri/LibraryItem.h"
#include "suri/LibraryItemCatalog.h"
#include "suri/LibraryItemOrigin.h"
#include "suri/SuriObject.h"

namespace suri {

/*!
 * Clase que representa una biblioteca.
 */
class Library : public suri::SuriObject {
   /** ctor copia **/
   Library(const Library&);

public:
   /** constructor **/
   explicit Library(const std::string& LibraryCode = SuriObject::NullUuid);
   /** destructor **/
   virtual ~Library();
   /**
    * Agrega el item pasado por parametro a la biblioteca
    * @param[in] pItem referencia del item a agregar
    * @return true en caso de agregarlo, false en caso contrario
    */
   bool AddItem(LibraryItem* pItem);
   /** Agrega un item a un origen en particular de la biblioteca
    *  @param[in] OriginId id del origen sobre el cual se agrega
    *  @param[in] pItem referencia al item que se desea agregar
    *  @return true en caso de poder agregar el item, false en caso contrario*/
   bool AddItemToLibraryOrigin(const SuriObject::UuidType& OriginId,
                               LibraryItem* pItem);

   /**
    * Elimina el item de la biblioteca correspondiente al Id pasado por parametro
    * @param[in] ItemId id del item a eliminar
    * @return true en caso de eliminarlo, false en caso contrario
    */
   bool RemoveItem(const SuriObject::UuidType& ItemId);
   /**
    * Obtiene el item pasado por parametro a la biblioteca
    * @param[in] ItemId id del item a obtener
    * @return LibraryItem* referencia al item solicitado
    * @return NULL en caso de no encontrar el item solicitado
    */
   const LibraryItem* GetItem(const SuriObject::UuidType& ItemId) const;
   /** Obtiene el item correspondiente al atributo principal pasado por parametro*/
   const LibraryItem* GetItemByPrincipal(const std::string& PrincipalValue) const;
   /**
    * Reemplaza un item de la bilbioteca
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
    * @param Principal Id del item a reemplazar
    * @param pAttributesVector atributos a reemplazar/agregar
    * @param Add boolean que indica si se agrega el atributo en caso de no existir
    * @return true en caso de reemplazar los atributos en el item
    * @return false en caso contrario
    */
   bool ReplaceAttributesFromItemByPrincipal(const SuriObject::UuidType& Principal,
                                        std::vector<LibraryItemAttribute*>* pAttributesVector,
                                        bool Add = true);
   /**
    * Devuelve los origenes de la biblioteca que corresponden
    * a la categoria pasad apor parametro.
    * Es decir, la lista de los items que contiene
    * @return vector con los origenes que corresponden a la categoria
    */
   std::vector<const LibraryItemOrigin*> GetOriginsByCategory(
                           LibraryItemOrigin::OriginCategoryType OriginCategory) const;

   /** Devuelve los tipos de origenes que posee la bibloteca */
   std::set<LibraryItemOrigin::OriginCategoryType> GetOriginCategory() const;

   /**
    * Devuelve el catalogo entero de la biblioteca (la composicion de todos los origenes)
    * @return
    */
   std::vector<LibraryItemCatalog*> GetCatalog(bool FilterFavorites = false)const;
   /** Elimina los origenes configurados */
   void Reset();
   /** Agrega un origen a la biblioteca */
   void AddOrigin(LibraryItemOrigin* pOrigin);
   /** Retorna el codigo que identifica a la biblioteca */
   std::string GetLibraryCode() const;
   /** Obtiene el id del primer origen que posee la capability indicada
    *  por parametro*/
   SuriObject::UuidType GetOriginIdPrecedenceByAccess(
         LibraryItemOrigin::CapabilityType Capability) const;

   /** Indica si el origen posee la capability indicada por parametro **/
   bool HasOriginCapability(const SuriObject::UuidType& OriginId,
                            LibraryItemOrigin::CapabilityType Capability) const;
   /** Metodo que verifica si un item fue marcado como favorito
   *  @param[in] Principal valor del atributo principal del item a consultar
   *  @return true en caso de que el item aparezca en el listado de favoritos
   *  @return false en caso contrario **/
   bool IsFavorite(const SuriObject::UuidType& ItemId) const;
   /** Agrega un item a favoritos **/
   bool AddItemToFavorites(const SuriObject::UuidType& ItemId);
   /** Elimina un item de favoritos **/
   bool RemoveItemFromFavorites(const SuriObject::UuidType& ItemId);

private:
   /** Metodo auxiliar para obtener el origen por precedencia **/
   std::vector<LibraryItemOrigin*> FindPrecedenceByAccess(
         LibraryItemOrigin::CapabilityType Capability) const;
   /** Metodo auxiliar que inserta de ordenada los origenes por precedencia.
    *  Utilizando la secuencia de origenes brindada por enum OriginType. De
    *  forma que un nuevo origen se inserta como de mayor precedencia para el
    *  permiso asociado al mismo. Ejemplo: Si se inserta el origen "Proyecto", que
    *  requiere acceso "APPEND" es insertado por encima de los otros origenes
    *  existentes con acceso "Append".  **/
   void InsertOrderedOrigin(LibraryItemOrigin* pOrigin);
   /** Obtiene el origen al id que se pasa por parametro
    *  @return puntero al origen, null en caso contrario
    */
   LibraryItemOrigin* GetOrigin(const SuriObject::UuidType& OriginId);
   typedef std::vector<LibraryItemOrigin*> LibraryOriginList;
   LibraryOriginList libraryOriginList_;
   /** Identificador alternativo para la biblioteca **/
   std::string libraryCode_;
};

}  // namespace suri

#endif /* LIBRARY_H_ */
