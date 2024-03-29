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

#ifndef FILELIBRARYITEMORIGIN_H_
#define FILELIBRARYITEMORIGIN_H_



// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/LibraryItemOrigin.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un origen asociado a un archivo de texto plano.
 * La representacion de los items de este tipo de origen esta dado
 * por un item por linea en el archivo y una serie de atributos separados por un token
 * Ademas uno de esos atributos actuara como identificar de ese
 * item.
 * Este tipo de origen supone que el archivo que posee la informacion de los items
 * son todos pertenecientes a la misma biblioteca.
 * 1 archivo -> 1 biblioteca
 */
class TextFileLibraryItemOrigin : public suri::LibraryItemOrigin {
public:
   /** Constructor **/
   TextFileLibraryItemOrigin(const SuriObject::UuidType& LibraryId,
                             std::vector<LibraryItemOrigin::CapabilityType> Capabilities,
                             const std::string& FileName,
                             OriginCategoryType Category = LibraryItemOrigin::USER,
                             const std::string& FavoritesIndexFileName = "",
                             const std::string& AttributeToken = ";",
                             const std::string& AttributeNameValueToken = "=");

   /** Constructor que indica el nombre del origen **/
   TextFileLibraryItemOrigin(const SuriObject::UuidType& LibraryId,
                             std::vector<LibraryItemOrigin::CapabilityType> Capabilities,
                             const std::string& FileName,
                             const std::string& Name,
                             OriginCategoryType Category = LibraryItemOrigin::USER,
                             const std::string& FavoritesIndexFileName = "",
                             const std::string& AttributeToken = ";",
                             const std::string& AttributeNameValueToken = "=");
   /** Destructor **/
   virtual ~TextFileLibraryItemOrigin();

private:
   friend class DensitySliceLibraryOrigin;
   /** Agrega el item pasado por parametro a la fuente de datos del origen.
    *  Metodo que tiene un comportamiento particular para cada tipo de origen.
    *  (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual bool AddItemToSource(LibraryItem* pItem);
   /** Elimina un item de la fuente de datos  */
   virtual bool RemoveItemFromSourceByCode(const std::string& Code);
   /**
    * Elimina el item de la fuente de datos del origen correspondiente al id
    * que se pasa por parametro
    * Metodo que tiene un comportamiento particular para cada tipo de origen.
    * (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual bool RemoveItemFromSource(const SuriObject::UuidType& ItemId);
   /** Obtiene el item pasado por parametro a la fuente de datos del origen
    *  Metodo que tiene un comportamiento particular para cada tipo de origen.
    *  (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual const LibraryItem* GetItemByCodeFromSource(const std::string& Code);
   /** Reemplaza un item de la fuente de datos del origen
    *  Metodo que tiene un comportamiento particular para cada tipo de origen.
    *  (una clase concreta de origen tiene que reimplementar este metodo)
    */
   virtual bool ReplaceItemFromSource(LibraryItem* pItem);
   /** Carga los items del origen que se encuentra en el archivo**/
   void LoadItems();
   /** Metodo auxiliar para parsear una linea de un archivo y generar un
    *  item para la bilbioteca de acuerdo a los atributos indicados**/
   LibraryItem* CreateItemFromLine(const std::string& Line) const;
   /** Metodo auxiliar crea una linea con todos los atributos del item**/
   std::string CreateLineFromItem(const LibraryItem* pItem) const;
   /** Metodo auxiliar que remplaza el archivo del origen eliminando o reemplazando
    *  la linea correspondiente al item pasado por parametro
    */
   bool ReplaceorDeleteItemFromTextFile(const LibraryItem* pItem, bool Replace = true);

   /** Path/Nombre del archivo al que apunta el origen **/
   std::string fileName_;
   /** Token que separa los atributos del item dentro de la linea **/
   std::string attributeToken_;
   /** Token que separa el nombre de un atributo y su valor*/
   std::string attributeNameValueToken_;
   int filePosition_;
};

} /** namespace suri */

#endif /* FILELIBRARYITEMORIGIN_H_ */
