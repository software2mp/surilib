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

// Includes Estandar
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "suri/TextFileLibraryItemOrigin.h"

#include "FavoritesIndexFile.h"
#include "LibraryUtils.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryItem.h"
#include "suri/LibraryItemAttribute.h"

#include "wx/filename.h"

#define ITEM_LIST_BUFFER_SIZE 30

namespace suri {

/**
 * Constructor
 * @param[in] LibraryId id de la biblioteca a la que pertenece el origen
 * @param[in] Capabilities capacidades que poseera el origen
 * @param[in] FileName archivo al que apunta el origen
 * @param[in] Category categoria del origen (usuario, proyecto, sistema)
 * @param[in] FavoritesIndexFileName archivo de indice de favoritos
 * @param[in] AttributeToken token que separa los atributos
 * @param[in] AttributeNameValueToken token que separa atributo-valor
 */
TextFileLibraryItemOrigin::TextFileLibraryItemOrigin(
      const SuriObject::UuidType& LibraryId,
      std::vector<LibraryItemOrigin::CapabilityType> Capabilities,
      const std::string& FileName,
      LibraryItemOrigin::OriginCategoryType Category,
      const std::string& FavoritesIndexFileName,
      const std::string& AttributeToken, const std::string& AttributeNameValueToken) :
      LibraryItemOrigin(LibraryId, Category, Capabilities), fileName_(FileName),
      attributeToken_(AttributeToken),
      attributeNameValueToken_(AttributeNameValueToken), filePosition_(0) {
   favoritesIndexFileName_ = FavoritesIndexFileName;
   EnsureFilePath(fileName_);
   LoadItems();
}

/** Constructor que indica el nombre del origen **/
TextFileLibraryItemOrigin::TextFileLibraryItemOrigin(
      const SuriObject::UuidType& LibraryId,
      std::vector<LibraryItemOrigin::CapabilityType> Capabilities,
      const std::string& FileName, const std::string& Name, OriginCategoryType Category,
      const std::string& FavoritesIndexFileName,
      const std::string& AttributeToken, const std::string& AttributeNameValueToken) :
      LibraryItemOrigin(LibraryId, Category, Capabilities, Name), fileName_(FileName),
      attributeToken_(AttributeToken),
      attributeNameValueToken_(AttributeNameValueToken), filePosition_(0) {
   favoritesIndexFileName_ = FavoritesIndexFileName;
   EnsureFilePath(fileName_);
   LoadItems();
}

/** Destructor **/
TextFileLibraryItemOrigin::~TextFileLibraryItemOrigin() {
}

/** Metodo auxiliar que se encarga de cargar los items del origen **/
void TextFileLibraryItemOrigin::LoadItems() {
   RemoveAllItems();
   filePosition_ = 0;
   std::string line;
   std::ifstream file(fileName_.c_str());
   if (file.is_open()) {
      for (int linenumber = 0; file.good() && linenumber <= ITEM_LIST_BUFFER_SIZE;
            ++linenumber) {
         getline(file, line);
         LibraryItem* pitem = CreateItemFromLine(line);
         if (pitem) {
            items_.push_back(pitem);
            const LibraryItemAttribute* pattr = pitem->GetPrincipalAttribute();
            std::string codevalue;
            if (pattr)
               codevalue = pattr->GetValue();
            else
               codevalue = pitem->GetId();
            idCodeMap_.insert(std::make_pair(codevalue, pitem->GetId()));
         }
      }
      filePosition_ = file.tellg();
      file.close();
   }
   if (!favoritesIndexFileName_.empty()) {
      LoadFavoritesIndex();
   }
}

/** Metodo auxiliar para parsear una linea de un archivo y generar un
 *  item para la bilbioteca de acuerdo a los atributos indicados
 *  @return item correspondiente a la linea del archivo pasada por parametro
 *  @return NULL si no es posible crear el item asociado a la linea**/
LibraryItem* TextFileLibraryItemOrigin::CreateItemFromLine(
      const std::string& Line) const {
   std::vector < std::string > attributes = tokenizer(Line, attributeToken_);
   std::vector<std::string>::iterator it = attributes.begin();
   std::string codevalue;
   std::vector<LibraryItemAttribute*> attributesvalues;
   for (; it != attributes.end(); ++it) {
      std::vector < std::string > attribute = tokenizer(*it, attributeNameValueToken_);
      if (attribute.size() == 2) {
         if (attribute[0].compare(LibraryItemAttribute::PrincipalAttrName) == 0) {
            codevalue = attribute[1];
         } else {
              attributesvalues.push_back(
                  new LibraryItemAttribute(attribute[0], attribute[1]));
         }
      } else {
		       /** si es diferente de 2 quiere decir que el valor del atributo
             *  contiene algun caracter con '=' y esto hace que de un
             *  un size mas grande. Por esto, se toma como clave 
                attribute[0] y como valor el resto del string 
				  **/
				size_t pos = (*it).find(attributeNameValueToken_);
			  	if (pos != std::string::npos) {
					std::string attvalue = (*it).substr(pos+1);
					attributesvalues.push_back(
                  new LibraryItemAttribute(attribute[0], attvalue));
				}
		}
   }
   LibraryItem* pitem = NULL;
   if (!codevalue.empty()) {
      pitem = new LibraryItem(libraryId_, codevalue);
      pitem->ConfigureOriginId(GetId());
      pitem->AddAttributes(attributesvalues);
   }
   return pitem;
}

/** Metodo auxiliar crea una linea con todos los atributos del item**/
std::string TextFileLibraryItemOrigin::CreateLineFromItem(
      const LibraryItem* pItem) const {
   std::string itemline;
   LibraryItem::MapAttribute attributes = pItem->GetAttributes();
   LibraryItem::MapAttribute::const_iterator constit = attributes.begin();
   for (; constit != attributes.end(); ++constit) {
      itemline.append(constit->second->GetName());
      itemline.append(attributeNameValueToken_);
      itemline.append(constit->second->GetValue());
      ++constit;
      if (constit != attributes.end())
         itemline.append(attributeToken_);
      --constit;
   }
   return itemline;
}

/** Agrega el item pasado por parametro a la fuente de datos origen.
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool TextFileLibraryItemOrigin::AddItemToSource(LibraryItem* pItem) {
   bool added = false;
   std::ofstream file(fileName_.c_str(), std::ofstream::app);
   if (file.is_open()) {
      std::string itemline = CreateLineFromItem(pItem);
      file << itemline << std::endl;
      added = true;
      file.close();
   }
   return added;
}

/** Elimina el item de la fuente de datos del origen correspondiente al Id pasado por parametro
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool TextFileLibraryItemOrigin::RemoveItemFromSourceByCode(const std::string& Code) {
   const LibraryItem* pitem = GetItemByCode(Code);
   if (!pitem)
      return false;
   return ReplaceorDeleteItemFromTextFile(pitem, false);
}

/** Elimina el item de la fuente de datos del origen correspondiente al Id pasado por parametro
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool TextFileLibraryItemOrigin::RemoveItemFromSource(const SuriObject::UuidType& ItemId) {
   const LibraryItem* pitem = GetItem(ItemId);
   if (!pitem)
      return false;
   return ReplaceorDeleteItemFromTextFile(pitem, false);
}

/** Obtiene el item pasado por parametro a la fuente de datos del origen
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
const LibraryItem* TextFileLibraryItemOrigin::GetItemByCodeFromSource(
      const std::string& Code) {
   LibraryItem* pitem = NULL;
   std::ifstream file(fileName_.c_str());
   if (file.is_open()) {
      std::string line;
      file.seekg(filePosition_, std::ios_base::beg);
      while (!pitem && file.good()) {
         getline(file, line);
         LibraryItem* plineitem = CreateItemFromLine(line);
         if (plineitem) {
            std::string principalvalue = plineitem->GetPrincipalAttribute()->GetValue();
            if (principalvalue.compare(Code) == 0) {
               pitem = plineitem;
               // Ya que recorro el archivo lo agrego a la lista
               items_.push_back(plineitem);
               idCodeMap_.insert(std::make_pair(principalvalue, plineitem->GetId()));
            }
         }
      }
      filePosition_ = file.tellg();
      file.close();
   }
   return pitem;
}

/** Reemplaza un item de la fuente de datos del origen
 *  Para reemplazar un item de un archivo de texto
 *  se utiliza un archivo temporal en el que se deja el resultado del
 *  archivo y luego se renombra el archivo al original
 *  Metodo que tiene un comportamiento particular para cada tipo de origen.
 *  (una clase concreta de origen tiene que reimplementar este metodo)
 */
bool TextFileLibraryItemOrigin::ReplaceItemFromSource(LibraryItem* pItem) {
   return ReplaceorDeleteItemFromTextFile(pItem);
}

/** Metodo auxiliar que remplaza el archivo del origen eliminando o reemplazando
 *  la linea correspondiente al item pasado por parametro
 */
bool TextFileLibraryItemOrigin::ReplaceorDeleteItemFromTextFile(
      const LibraryItem* pItem, bool Replace) {
   std::ifstream filein(fileName_.c_str());
   std::string filetempout = fileName_ + ".tmp";
   filePosition_ = 0;
   // archivo temporal
   std::ofstream fileout(filetempout.c_str());
   if (filein.is_open()) {
      std::string line;
      while (filein.good()) {
         getline(filein, line);
         LibraryItem* pitem = CreateItemFromLine(line);
         if (pitem) {
            bool found = pitem->GetPrincipalAttribute()->GetValue().compare(
                  pItem->GetPrincipalAttribute()->GetValue()) == 0;
            if (!found || (found && Replace)) {
               if (found && Replace)
                  line = CreateLineFromItem(pItem);
               line += "\n";
               fileout << line;
            }
            delete pitem;
         }
      }
      filein.close();
      fileout.close();
      // Borro el archivo original
      remove(fileName_.c_str());
      // Renombro el archivo de salida
      rename(filetempout.c_str(), fileName_.c_str());
   }
   return true;
}

} /** namespace suri */
