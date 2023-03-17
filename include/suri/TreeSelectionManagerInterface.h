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

#ifndef TREESELECTIONMANAGERINTERFACE_H_
#define TREESELECTIONMANAGERINTERFACE_H_

// Includes standard
#include <list>
// Includes suri
#include "NodePath.h"

namespace suri {

/**
 * Administra la seleccion de capas y grupos
 */
class TreeSelectionManagerInterface {
public:
   /** Dtor */
   virtual ~TreeSelectionManagerInterface() {}

   /**
    * Limpia la seleccion de grupos e items
    * @param[in] ViewcontextId Identificador del viewcontext
    */
   virtual void ResetSelection() = 0;

   /**
    * Permite modificar los nodos seleccionados
    * @param[in] Path NodePath del nodo
    * @param[in] Selected flag que indica si el nodo debe estar seleccionado
    * o no.
    */
   virtual void ChangeSelection(const NodePath &Path, bool Selected) = 0;

   /**
    * Retorna el path de nodos seleccionados
    * @return listado con el path de los nodos seleccionados
    */
   virtual std::list<NodePath> GetSelectedNodes() = 0;
};
} /* namespace suri */

#endif /* TREESELECTIONMANAGERINTERFACE_H_ */
