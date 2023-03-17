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

#ifndef GUITREEMODEL_H_
#define GUITREEMODEL_H_

// Includes standard
#include <set>
#include <list>

// Includes Suri
#include "suri/NodePath.h"
#include "suri/WorkGroupInterface.h"

namespace suri {

/**
 * Clase con modelo del arbol a mostrar, esto incluye la estructura y la
 * informacion visual. Permite obtener la raiz del arbol, que grupos estan
 * expandidos, que nodos estan inhabilitados y la seleccion.
 */
class GuiTreeModel {
public:
   /** Ctor */
   GuiTreeModel();
   /** Dtor */
   virtual ~GuiTreeModel();

   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual WorkGroupInterface* GetRootWorkGroup() = 0;
   /** Retorna iterador del arbol completo */
   virtual TreeIterator GetIterator() const = 0;
   /** Retorna iterador del arbol debajo de GroupPath */
   virtual TreeIterator GetIterator(const NodePath &GroupPath) const = 0;
   /** Retorna iterador a las hojas del arbol completo */
   virtual TreeIterator GetLeafIterator() const = 0;
   /** Retorna iterador a las hojas del arbol debajo de GroupPath */
   virtual TreeIterator GetLeafIterator(const NodePath &GroupPath) const = 0;
   /** Recorre el arbol buscando el nodo con Id indicado. */
   virtual NodePath SearchNode(const SuriObject::UuidType &Id) const = 0;
   /** Para un nodo, retorna el path relativo al grupo raiz. */
   virtual NodePath GetPathToRoot(TreeNodeInterface* pNode) const = 0;

   /** Mueve un nodo al final del arbol. */
   virtual NodePath MoveNodeToEnd(const NodePath &Node) = 0;
   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index) = 0;

   /** Permite modificar los nodos seleccionados */
   virtual void ChangeSelection(const NodePath &Path, bool Selected) = 0;
   /** Retorna el path de nodos seleccionados */
   virtual std::list<NodePath> GetSelectedNodes() = 0;
   /** Limpia la seleccion de nodos(items y grupos) */
   virtual void ResetSelection() = 0;

   /** Expande/contrae un grupo en interfaz grafica */
   virtual void ExpandGroup(const NodePath &Group, bool Expand);
   /** Indica si un grupo esta expandido */
   virtual bool IsExpanded(const NodePath &Group);
   /** Indica si un nodo es visible en interfaz */
   virtual bool IsVisible(const NodePath &Node);

   /** Habilita/Deshabilita un nodo */
   virtual void Enable(const NodePath &Node, bool Enabled);
   /** Indica si un nodo esta habilitado */
   virtual bool IsEnabled(const NodePath &Node);

protected:
   /** Elimina grupos desplegados que no esten en el modelo */
   void RefreshModel();
   /** Limpia los id invalidos del conjunto */
   void RemoveInvalidItems(std::set<SuriObject::UuidType> &WorkingSet);

   /** Listado con grupos expandidos */
   std::set<SuriObject::UuidType> expandedGroups_;
   /** Listado con grupos inhabilitados */
   std::set<SuriObject::UuidType> disabledGroups_;
};

} /* namespace suri */
#endif /* GUITREEMODEL_H_ */
