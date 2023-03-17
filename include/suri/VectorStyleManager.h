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

#ifndef VECTORSTYLEMANAGER_H_
#define VECTORSTYLEMANAGER_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/VectorStyleTable.h"

/** Defines */
/** namespace suri */
namespace suri {
/** Clase que maneja las tablas de estilos vectoriales */
/**
 *  Esta clase maneja las tablas de estilos vectoriales
 *  Brinda estilos por defecto
 *  Maneja las tablas de estilos
 *  \todo Implementacion tipo singleton???
 */
class VectorStyleManager {
   /** Constructor */
   VectorStyleManager();
   /** Destructor */
   ~VectorStyleManager();
   /** Constructor de copia */
   VectorStyleManager(const VectorStyleManager &pVStyleMgr);
public:
   /** Devuelve una instancia de la clase. */
   static VectorStyleManager& Instance();
// ------------------- METODOS DE CONSULTA ------------------------------
   /** Devuelve la cantidad de total tablas de estilos */
   size_t GetTablesCount();
   /** Devuelve los nombres de las tablas de estilos */
   std::vector<std::string> GetTablesNames();
   /** Devuelve una tabla de estilo */
   VectorStyleTable * GetTable(const size_t StyleTableIndex);
   /** Devuelve una tabla de estilo */
   VectorStyleTable * GetTable(const std::string &CategoryName,
                               Vector::VectorType Type);
   /** Devuelve una tabla de estilo */
   VectorStyleTable * GetTable(Vector::VectorType Type);
   /** Agrega una tabla de estilos y toma posesion de la mismo */
   bool AddTable(const std::string &CategoryName, VectorStyleTable *pVStyleTable);
   /** Agrega una tabla de estilos y toma posesion de la mismo */
   bool AddTable(VectorStyleTable * pVStyleTable);
   /** Borra una tabla de estilos */
   bool DeleteTable(const size_t StyleTableIndex);
   /** Borra una tabla de estilos */
   bool DeleteTable(const std::string &CategoryName, Vector::VectorType Type);
   /** Borra una tabla de estilos */
   bool DeleteTable(Vector::VectorType Type);
   /** Devuelve un estilo default */
   VectorStyle * GetDefaultStyle(const std::string &CategoryName,
                                 Vector::VectorType Type);
   /** Devuelve un estilo default */
   VectorStyle * GetDefaultStyle(Vector::VectorType Type);
   /** Devuelve un estilo particular. */
   VectorStyle * GetStyle(const std::string &CategoryName,
                          Vector::VectorType VectorType, size_t StyleId);


protected:
   /** Carga los estilos del sistema */
   void LoadDefaultStyles();

   std::string defaultCategory_; /*! categoria default (layer) */
   std::multimap<std::string, VectorStyleTable *> styleTablesMap_; /*! Tablas */
   /* de estilos */
private:
};
} /*namespace suri*/

#endif /*VECTORSTYLEMANAGER_H_*/
