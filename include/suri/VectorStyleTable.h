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

#ifndef VECTORSTYLETABLE_H_
#define VECTORSTYLETABLE_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "suri/VectorStyle.h"

/** namespace suri */
namespace suri {
/** Clase que implementa una tabla de estilos */
/**
 *  Esta clase implementa una tabla de estilos y los busca
 *  por nombre o por indice.
 *  \todo Metodos de lectura/escritura a disco
 */
class VectorStyleTable {
public:
   /** Constructor */
   VectorStyleTable(const Vector::VectorType &VType, const std::string &CategoryName =
         "");

   /** Constructor */
   VectorStyleTable(const Vector::VectorType &VType, const std::string &CategoryName,
                    const std::vector<std::string> &Styles);

   /** Destructor */
   ~VectorStyleTable();

   /** Devuelve el nombre de la tabla de estilos */
   std::string GetCategoryName() const;

   /** Devuelve el tipo de vector de la tabla de estilos */
   Vector::VectorType GetVectorType() const;

   /** Cambia el nombre de la tabla */
   void ChangeCategoryName(const std::string &NewCategoryName);

   /** Devuelve la cantidad de estilos */
   size_t GetStyleCount() const;

   /** Devuelve los nombres de los estilos */
   std::vector<std::string> GetStyleNames() const;

   /** Devuelve un estilo */
   VectorStyle * GetStyle(const size_t Index) const;

   /** Devuelve un estilo por el nombre */
   VectorStyle * GetStyle(const std::string &StyleName) const;

   /** Devuelve el siguiente estilo a utilizar */
   VectorStyle * GetDefaultStyle();

   /** Agrega un estilo y toma posesion del mismo */
   bool AddStyle(VectorStyle * pVStyle);

   /** Borra un estilo */
   bool DeleteStyle(const size_t Index);

   /** Borra un estilo por el nombre */
   bool DeleteStyle(const std::string &StyleName);

   /** Clona la tabla */
   VectorStyleTable * Clone(const std::string &NewCategoryName = "") const;

protected:
   std::string categoryName_; /*! Nombre de la tabla */
   Vector::VectorType vType_; /*! Tipo de vector */
   size_t styleIndex_; /*! Indice al siguiente estilo default */
   std::vector<VectorStyle *> table_; /*! Tabla de estilos */
private:
};
} /*namespace suri*/

#endif /*VECTORSTYLETABLE_H_*/
