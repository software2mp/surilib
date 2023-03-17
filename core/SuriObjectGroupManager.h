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

#ifndef SURIOBJECTGROUPMANAGER_H_
#define SURIOBJECTGROUPMANAGER_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/GroupManagerInterface.h"
#include "suri/MutableWorkGroupInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

// fordward
class DefaultTreeNode;
class SuriObjectManagerInterface;

class SuriObjectGroupManager : public GroupManagerInterface {
public:
   /** Ctor */
   explicit SuriObjectGroupManager(SuriObjectManagerInterface* pObjectManager);
   /** Dtor */
   virtual ~SuriObjectGroupManager();

   /** Elimina el contenido del arbol. Solo queda el nodo raiz. */
   virtual void Clear();
   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual WorkGroupInterface* GetRootWorkGroup();
   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual MutableWorkGroupInterface* GetRootMutableWorkGroup();
   /** Agrega un grupo dentro de otro. */
   virtual NodePath InsertGroup(const std::string &GroupName,
                                    const NodePath &ParentGroup, int Index);
   /** Agrega un grupo dentro de otro. */
   virtual NodePath InsertContextGroup(const SuriObject::UuidType& ViewcontextId,
		   	   	   	   	   	   	   	   const std::string &GroupName,
		   	   	   	   	   	   	   	   const NodePath &ParentGroup, int Index, ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);
   /** Elimina un grupo del arbol */
   virtual bool RemoveGroup(const NodePath &Node);
   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

#ifdef __DEBUG__
   /** Muestra por log el estado del arbol y la lista */
   void LogState(const std::string &Header);
#endif

private:
   /** Mueve un elemento en el manager asociado */
   bool MoveItemContent(TreeNodeInterface* pItemNode, SuriObject::UuidType DestinationId);

   /** Valida que el orden de los items en arbol sea igual al del manager */
   bool ValidateOrder();
   /** Permite recorrer los nodos con contenido secuencialmente. */
   bool GetNextContentNode(TreeNodeInterface* pGroupNode,
                                    TreeNodeInterface* pCurrentContentNode,
                                    TreeNodeInterface* &pNextContentNode);
   /** Retorna el ultimo nodo con contenido dentro del grupo. */
   TreeNodeInterface* GetLastContentNode(TreeNodeInterface* pGroupNode);

   /** Nodo raiz del arbol */
   DefaultTreeNode* pRoot_;
   /** Manager asociado al arbol */
   SuriObjectManagerInterface* pObjectManager_;
   /** Id de observer en pObjectManager_ */
   int observerId_;
   /** Flag que se usa para ignorar las notificaciones. */
   bool ignoreNotification_;
};

} /* namespace suri */

#endif /* SURIOBJECTGROUPMANAGER_H_ */
