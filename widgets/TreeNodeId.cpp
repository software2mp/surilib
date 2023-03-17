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

#include "TreeNodeId.h"

// Includes standard

// Includes Suri

// Includes Wx

// Includes App

// Defines
#define LEVELSEPARATOR '/'

namespace suri {
/** Ctor */
TreeNodeId::TreeNodeId(const ItemId &Id) :
      ItemId(Id) {
}

/** Ctor */
TreeNodeId::TreeNodeId(std::string Id) :
      ItemId(Id) {
}

/** Dtor */
TreeNodeId::~TreeNodeId() {
}

/**
 * Compara dos ids usando la jerarquia del arbol.
 * @param[in] ItemId Identificador contra el que se compara el valor.
 * @return >0 si el nodo que representa OtherItemId esta
 * debajo de la jerarquia del nodo que representa esta clase.
 * @return 0 si son iguales.
 * @return <0 los nodos no estan relacionados.
 */
int TreeNodeId::Compare(const ItemId &OtherItemId) const {
   if (id_.compare(INVALIDTREEID.id_) == 0) {
      return -1;
   }

   // Si son iguales retorna 0
   if (id_.compare(OtherItemId.GetId()) == 0) {
      return 0;
   }

   // Analiza si id_ esta contenido en OtherItemId.id_. En caso afirmativo
   // retorna la longitud de id_
   if (id_.size() + 1 < OtherItemId.GetId().size()) {
      std::string substring = OtherItemId.GetId().substr(0, id_.size());
      if (id_.compare(substring) == 0 &&
      OtherItemId.GetId()[id_.size()]==LEVELSEPARATOR) {return id_.size();
   }
}

      // Si id no esta contenido en OtherItemId.id_ retorna -1
   return -1;
}

/**
 * Se usa para generar ids de subnodos. Id debe ser string unico
 * que no se repita en otro nodo debajo del mismo padre.
 * @param[in] Id identificador unico dentro de un nodo
 */
void TreeNodeId::AppendStringToId(std::string SubnodeId) {
   id_ = id_ + LEVELSEPARATOR + SubnodeId;
}
}

