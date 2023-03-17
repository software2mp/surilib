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

// Includes standard
#include <list>
// Includes Suri
#include "TreeNodeAuxiliaryFunctions.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/**
 * Para un nodo, busca su posicion en el grupo que lo contiene
 * @param[in] pParent TreeNodeInterfacedonde donde quiero buscar.
 * @param[in] pChild TreeNodeInterfacedonde del que me interesa la
 * posicion
 * @return Posicion de pChild en pParent.
 * @return -1 si no lo encuentra
 */
int GetIndexOfChild(TreeNodeInterface* pGroupNode, TreeNodeInterface* pChild) {
   int index = -1;
   int childcount = pGroupNode->GetSize();
   for (int i = 0; i < childcount && index < 0; i++)
      if (pGroupNode->GetChildAt(i) == pChild)
         index = i;
   return index;
}

}  /* namespace suri */



