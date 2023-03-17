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

#ifndef LAYERTREEMODEL_H_
#define LAYERTREEMODEL_H_

// Includes standard
#include <list>

// Includes suri
#include "suri/GuiTreeModel.h"
#include "suri/TreeSelectionManagerInterface.h"
#include "suri/GroupManagerInterface.h"

namespace suri {

/**
 * Adapta GuiTreeModel para que trabaje con capas y grupos de proyecto.
 */
class LayerTreeModel : public GuiTreeModel {
public:
   /** Ctor */
   LayerTreeModel(GroupManagerInterface* pGroupManager,
                        TreeSelectionManagerInterface* pTreeSelectionManager);
   /** Dtor */
   virtual ~LayerTreeModel();

   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual WorkGroupInterface* GetRootWorkGroup();
   /** Retorna iterador del arbol completo */
   virtual TreeIterator GetIterator() const;
   /** Retorna iterador del arbol debajo de GroupPath */
   virtual TreeIterator GetIterator(const NodePath &GroupPath) const;
   /** Retorna iterador a las hojas del arbol completo */
   virtual TreeIterator GetLeafIterator() const;
   /** Retorna iterador a las hojas del arbol debajo de GroupPath */
   virtual TreeIterator GetLeafIterator(const NodePath &GroupPath) const;
   /** Recorre el arbol buscando el nodo con Id indicado. */
   virtual NodePath SearchNode(const SuriObject::UuidType &Id) const;
   /** Para un nodo, retorna el path relativo al grupo raiz. */
   virtual NodePath GetPathToRoot(TreeNodeInterface* pNode) const;

   /** Mueve un nodo al final del arbol. */
   virtual NodePath MoveNodeToEnd(const NodePath &Node);
   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Permite modificar los nodos seleccionados */
   virtual void ChangeSelection(const NodePath &Path, bool Selected);
   /** Retorna el path de nodos seleccionados */
   virtual std::list<NodePath> GetSelectedNodes();
   /** Limpia la seleccion de nodos(items y grupos) */
   virtual void ResetSelection();

   /** Configura un filtro para las capas del arbol */
   void SetFilter(NodeFilterInterface* pFilter);

protected:
   /** Manager de grupos. */
   GroupManagerInterface* pGroupManager_;
   /** Interfaz que permite interactuar con items */
   TreeSelectionManagerInterface* pTreeSelectionManager_;

   /** Filtro que se le debe aplicar al arbol */
   NodeFilterInterface* pFilter_;
};

} /* namespace suri */
#endif /* LAYERTREEMODEL_H_ */
