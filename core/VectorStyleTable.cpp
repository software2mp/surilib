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

#include "suri/VectorStyleTable.h"

// Includes suri
#include "suri/AuxiliaryFunctions.h"
#include "logmacros.h"

/** namespace suri */
namespace suri {
// --------------------------- Clase VectorStyleTable -----------------------------------------

/**
 * Ctor
 * @param[in]	VType tipo de vector.
 * @param[in]	CategoryName categoria de vector (layer, draw, meassure, grid)
 * @return instancia de la clase VectorStyleTable
 */
VectorStyleTable::VectorStyleTable(const Vector::VectorType &VType,
                                   const std::string &CategoryName) :
      categoryName_(CategoryName), vType_(VType), styleIndex_(0) {
}

/**
 * Crea una tabla de estilos a partir de los estilos default que recibe por
 * parametro.
 * @param[in]	VType tipo de vector.
 * @param[in]	CategoryName categoria de vector (layer, draw, meassure, grid)
 * @param[in]	Styles estilos default a configurar en la tabla.
 * @return instancia de la clase VectorStyleTable
 */
VectorStyleTable::VectorStyleTable(const Vector::VectorType &VType,
                                   const std::string &CategoryName,
                                   const std::vector<std::string> &Styles) :
      categoryName_(CategoryName), vType_(VType), styleIndex_(0) {
   for (size_t i = 0; i < Styles.size(); i++)
      AddStyle(VectorStyle::Create(Styles.at(i)));
}

/** Destructor */
VectorStyleTable::~VectorStyleTable() {
   while (GetStyleCount() > 0)
      DeleteStyle(0);
}

/**
 *  Devuelve el nombre de la tabla de estilos
 *  @return string con el nombre de la tabla.
 */
std::string VectorStyleTable::GetCategoryName() const {
   return categoryName_;
}

/**
 * Devuelve el tipo de vector de la tabla de estilos
 * @return Vector::VectorType con el tipo de vector
 */
Vector::VectorType VectorStyleTable::GetVectorType() const {
   return vType_;
}

/**
 * Cambia el nombre de la tabla
 * @param[in] NewCategoryName nuevo nombre de la tabla
 */
void VectorStyleTable::ChangeCategoryName(const std::string &NewCategoryName) {
   categoryName_ = NewCategoryName;
}

/**
 * Devuelve la cantidad de estilos
 * @return size_t con la cantidad de estilos
 */
size_t VectorStyleTable::GetStyleCount() const {
   return table_.size();
}

/**
 * Devuelve los nombres de los estilos
 * @return vector de strings con los nombres de los estilos
 */
std::vector<std::string> VectorStyleTable::GetStyleNames() const {
   std::vector<std::string> stylenames;
   for (size_t i = 0; i < table_.size(); i++)
      stylenames.push_back(table_[i]->GetName());
   return stylenames;
}

/**
 * Devuelve un estilo
 * @param[in] Index indice del estilo deseado
 * \attention el VectorStyle* retornado es responsabilidad de esta clase(no eliminar externamente)
 * @return devuelve un puntero a VectorStyle
 * @return NULL si el parametro recibido es mayor o igual que la cantidad de
 * estilos en la tabla de estilos.
 */
VectorStyle * VectorStyleTable::GetStyle(const size_t Index) const {
   if (Index >= table_.size()) {
      return NULL;
   }
   return table_[Index];
}

/**
 * Devuelve un estilo por el nombre
 * @param[in] StyleName nombre del estilo deseado
 * \attention el VectorStyle* retornado es responsabilidad de esta clase(no eliminar externamente)
 * @return devuelve un puntero a VectorStyle
 * @return NULL si el parametro recibido no se encuentra en la tabla de estilos.
 */
VectorStyle * VectorStyleTable::GetStyle(const std::string &StyleName) const {
   size_t i = 0;
   while (i < table_.size() && table_[i]->GetName() != StyleName)
      i++;
   return GetStyle(i);
}

/**
 * Devuelve el proximo estilo de vector a utilizar.
 * \pre styleIndex debe tener un valor entre 0 y cant. de estilos - 1
 * \post styleIndex con el indice del proximo estilo a asignar.
 * @return devuelve el proximo estilo de vector a utilizar.
 * \attention el VectorStyle* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
VectorStyle * VectorStyleTable::GetDefaultStyle() {
   // con esto logro que styleIndex_ cicle entre [0, n)
   styleIndex_ = (styleIndex_ + GetStyleCount()) % GetStyleCount();
   return GetStyle(styleIndex_++);
}

/**
 * Agrega un estilo
 * @param[in] pVStyle nuevo estilo
 * @return informa si pudo agregar el estilo
 */
bool VectorStyleTable::AddStyle(VectorStyle * pVStyle) {
   if (pVStyle == NULL || pVStyle->GetVectorType() != GetVectorType()) {
      REPORT_ERROR(
            "D:Error en Tabla de Estilos de vector %s: Se intento agregar un estilo invalido.", GetCategoryName().c_str());
      return false;
   }

   VectorStyle * pvectorstyle = pVStyle;

   // Evita repeticion de nombres
   std::string newname = pvectorstyle->GetName();
   size_t namecount = 0;
   while (GetStyle(newname) != NULL) {
      namecount++;
      newname = pvectorstyle->GetName() + " (" + LongToString(namecount) + ")";
   }

   pvectorstyle->ChangeName(newname);
   table_.push_back(pvectorstyle);
   return true;
}

/**
 * Borra un estilo
 * @param[in] Index indice del estilo a eliminar
 * @return informa si el estilo fue eliminado
 */
bool VectorStyleTable::DeleteStyle(const size_t Index) {
   VectorStyle * pvectorstyle = GetStyle(Index);
   if (pvectorstyle == NULL) {
      return false;
   }

   VectorStyle::Destroy(pvectorstyle);
   std::vector<VectorStyle *>::iterator it = table_.begin() + Index;
   table_.erase(it);
   return true;
}

/**
 * Borra un estilo por el nombre
 * @param[in] StyleName nombre del estilo a eliminar
 * @return informa si el estilo fue eliminado
 */
bool VectorStyleTable::DeleteStyle(const std::string &StyleName) {
   size_t i = 0;
   while (i < table_.size() && table_[i]->GetName() != StyleName)
      i++;
   return DeleteStyle(i);
}

/** Clona la tabla */
/**
 * @param[in] NewCategoryName nombre de la nueva tabla
 * @return una tabla con el nombre indicado y los mismos estilso que esta
 * \attention el VectorStyleTable retornado(y su eliminacion) es responsabilidad del invocante
 */
VectorStyleTable * VectorStyleTable::Clone(const std::string &NewCategoryName) const {
   std::string newcatname = NewCategoryName;
   if (newcatname.empty()) {
      newcatname = categoryName_;
   }
   VectorStyleTable * pvstyletable = new VectorStyleTable(vType_, NewCategoryName);
   for (size_t i = 0; i < GetStyleCount(); i++)
      pvstyletable->AddStyle(GetStyle(i)->Clone());
   return pvstyletable;
}
} /*namespace suri*/
