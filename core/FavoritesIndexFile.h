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

#ifndef FAVORITESINDEXFILE_H_
#define FAVORITESINDEXFILE_H_

// Includes Estandar
#include <string>
#include <set>

// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Clase que representa el archivo de indice de favoritos
 * Permite cargar el indice de favoritos
 * Es la clase encargada de administrar el archivo (alta, eliminacion)
 */
class FavoritesIndexFile {
public:
   /** Constructor **/
   FavoritesIndexFile(const std::string FileName);
   virtual ~FavoritesIndexFile();
   /** Carga los indices en el set pasado por parametro **/
   bool LoadFavoritesIndex(std::set<std::string>& FavoritesIndex);
   /** Elimina el item con id pasado por parametro del archivo de favoritos **/
   bool RemoveItemFromFavorite(const std::string& ItemId);
   /** Agrega el item con id pasado por parametro al archivo de favoritos **/
   bool AddItemFromFavorite(const std::string& ItemId);
   /** Reemplaza el contenido del archivo por el indice pasado por parametro **/
   bool ReplaceIndex(std::set<std::string>& FavoritesIndex);

private:
   std::string filename_;
};

} /** namespace suri */

#endif /* FAVORITESINDEXFILE_H_ */
