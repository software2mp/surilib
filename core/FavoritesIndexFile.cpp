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
#include <fstream>

// Includes Suri
#include "FavoritesIndexFile.h"
#include "logmacros.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor
 *  @param[in] FileName archivo en donde se encuentra el indice de favoritos */
FavoritesIndexFile::FavoritesIndexFile(const std::string FileName) :
      filename_(FileName) {
}

FavoritesIndexFile::~FavoritesIndexFile() {
}
/** Carga los indices en el set pasado por parametro **/
bool FavoritesIndexFile::LoadFavoritesIndex(std::set<std::string>& FavoritesIndex) {
   if (filename_.empty())
      return false;
   std::string line;
   std::ifstream file(filename_.c_str());
   if (file.is_open()) {
      while (file.good()) {
         getline(file, line);
         if (!line.empty()) {
            FavoritesIndex.insert(line);
         }
      }
      file.close();
   }
   return true;
}

/** Elimina el item con id pasado por parametro del archivo de favoritos **/
bool FavoritesIndexFile::RemoveItemFromFavorite(const std::string& ItemId) {
   if (filename_.empty())
      return false;
   std::ifstream filein(filename_.c_str());
   std::string filetempout = filename_ + ".tmp";
   // archivo temporal
   std::ofstream fileout(filetempout.c_str());
   if (filein.is_open()) {
      std::string line;
      while (filein.good()) {
         getline(filein, line);
         bool found = ItemId.compare(line) == 0;
         if (!found) {
            line += "\n";
            fileout << line;
         }
      }
      filein.close();
      fileout.close();
      // Borro el archivo original
      remove(filename_.c_str());
      // Renombro el archivo de salida
      rename(filetempout.c_str(), filename_.c_str());
   }
   return true;
}

/** Agrega el item con id pasado por parametro al archivo de favoritos al final
 *  del archivo
 * Precondicion: El ItemId debe existir y ser valido **/
bool FavoritesIndexFile::AddItemFromFavorite(const std::string& ItemId) {
   if (filename_.empty())
      return false;
   bool added = false;
   std::ofstream file(filename_.c_str(), std::ofstream::app);
   if (file.is_open()) {
      file << ItemId << std::endl;
      added = true;
      file.close();
   }
   return added;
}

/** Reemplaza el contenido del archivo por el indice pasado por parametro **/
bool FavoritesIndexFile::ReplaceIndex(std::set<std::string>& FavoritesIndex) {
   return false;
}
} /** namespace suri */
