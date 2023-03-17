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

#include "ItemId.h"

// Includes standard
#include <string>
// Includes Suri

// Includes Wx

// Includes App

// Defines

namespace suri {
/**
 * Recibe por parametro el identificador unico
 * @param[in] Id valor del id
 */
ItemId::ItemId(std::string Id) :
      id_(Id) {
}

/** Ctor de copia */
ItemId::ItemId(const SuriObject &Id) : id_(Id.GetId()) {
}

/** Ctor de copia */
ItemId::ItemId(const ItemId &Id) {
   id_ = Id.id_;
}

/** Dtor */
ItemId::~ItemId() {
}

/**
 * Operador de copia
 * @param[in] Id instancia con datos a copiar
 */
ItemId& ItemId::operator=(const ItemId& Id) {
   id_ = Id.id_;
   return *this;
}

/**
 * Compara el id contra el del ItemId que recibe por parametro
 * @param[in] OtherItemId Identificador contra el que se compara el valor.
 * @return 0 si son iguales, <>0 si son diferentes
 */
int ItemId::Compare(const ItemId &OtherItemId) const {
   return id_.compare(OtherItemId.id_);
}

/**
 * Retorna id en formato string. Este metodo no se deberia usar fuera de wx.
 * @return std::string con id
 */
std::string ItemId::GetId() const {
   return id_;
}
}  // namespace
