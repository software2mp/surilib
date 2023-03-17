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

#ifndef COLORTABLECATEGORY_H_
#define COLORTABLECATEGORY_H_

// Includes standar
#include <map>
#include <list>

// Includes suri
#include "ColorTable.h"

/** namespace suri */
namespace suri {

// Forwards
class LibraryManager;

/**
 * Agrupa las tablas de colores en categorias. Permite buscar una
 * tabla de colores sin nombre recorriendo todas las tablas registradas y
 * comparando el contenido(IsLoaded).
 */
class ColorTableCategory {
public:
   /** Ctor */
   ColorTableCategory();
   /** Dtor */
   ~ColorTableCategory();
   /** retorna los nombres de las tablas de colores en la categoria */
   std::list<std::string> GetColorTableNames();
   /** Busca tabla de colores Name en el mapa. Retorna puntero interno */
   bool GetColorTable(const std::string &Name, const ColorTable* &pColorTable);
   /** Agrega tabla de colores a la categoria.El puntero pasa a ser de la clase */
   bool AddColorTable(ColorTable* pColorTable);
   /** retorna los nombres de las tablas de colores en la categoria */
   std::list<std::string> GetColorTableNames(LibraryManager* pManager);
   /** Busca tabla de colores Name en el mapa. Retorna puntero interno */
   bool GetColorTable(LibraryManager* pManager, const std::string &Name, const ColorTable* &pColorTable);
   /** Elimina tabla de colores de la categoria */
   bool DeleteColorTable(std::string Name);
   /** Busca la tabla de colores primero por nombre y despues por contenido. */
   bool IsLoaded(const ColorTable* &UnknowColorTable, std::string &ColorTableName);

private:

   std::map<std::string, ColorTable*> loadedColorTables_; /*! Tablas de colores */
   /* cargadas en la categoria */
};
}

#endif /* COLORTABLECATEGORY_H_ */
