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

#ifndef SURIOBJECTWORKGROUP_H_
#define SURIOBJECTWORKGROUP_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/MutableWorkGroupInterface.h"
#include "suri/ViewcontextInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

class TreeNodeInterface;
class SuriObjectManagerInterface;

class DefaultWorkGroup : public suri::MutableWorkGroupInterface {
public:
   /** Ctor */
   explicit DefaultWorkGroup(TreeNodeInterface* pRoot);
   /** Dtor */
   virtual ~DefaultWorkGroup();

   /** Retorna iterador del arbol */
   virtual TreeIterator GetIterator(NodeFilterInterface* pFilter = NULL) const;
   /** Retorna iterador del arbol */
   virtual TreeIterator GetIterator(const NodePath &GroupPath,
                                    NodeFilterInterface* pFilter = NULL) const;

   /** Recorre el grupo, subgrupos,... buscando el nodo con contenido igual a 'Id' */
   virtual NodePath SearchContent(const SuriObject::UuidType &Id) const;

   /** Recorre el arbol buscando el nodo con Id indicado. */
   virtual NodePath SearchNode(const SuriObject::UuidType &Id) const;

   /** Configura un NodePath que corresponde al string que recibe. */
   virtual NodePath StringToPath(const std::string &PathString) const;

   /**  Retorna string a partir del cual se puede reconstruir el NodePath. */
   virtual std::string PathToString(const NodePath &Path) const;

   /** Retorna el path al grupo raiz */
   virtual NodePath GetRootPath() const;

   /** Retorna el tamanio del grupo raiz */
   virtual int GetRootSize(ViewcontextInterface::ViewcontextType type = (ViewcontextInterface::ViewcontextType)0) const;

   /** Retorna el grupo raiz asoicado a instancia de WorkGroupInterface */
   virtual TreeNodeInterface* GetRoot();

   /** Para un nodo, retorna el path relativo al grupo raiz. */
   virtual NodePath GetPathToRoot(TreeNodeInterface* pNode) const;

   /** Agrega un grupo dentro de otro. */
   virtual NodePath InsertGroup(const std::string &GroupName,
                              const NodePath &ParentGroup, int Index);
   /** Agrega un grupo dentro de otro. */
   NodePath InsertContextGroup(
   		const SuriObject::UuidType& ViewcontextId, const std::string &GroupName,
   		const NodePath &ParentGroup, int Index, ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);

   /** Agrega item con id recibido dentro de un grupo. */
   virtual NodePath InsertObject(const SuriObject::UuidType &Object,
                              const NodePath &ParentGroup, int Index,
                              ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);

   /** Valida si se puede mover un nodo dentro de un grupo en el arbol */
   virtual bool ValidateMoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Elimina un nodo del arbol */
   virtual bool Remove(const NodePath &Node);

   /** Valida path contra la raiz */
   bool ValidatePath(const NodePath &Node);

private:
   /** Nodo raiz con el que trabaja SuriObjectWorkGroup */
   TreeNodeInterface* pRoot_;
};

} /* namespace suri */
#endif /* SURIOBJECTWORKGROUP_H_ */
