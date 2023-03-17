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

// Includes suri
#include "HtmlTreeEventHandler.h"
#include "TreeNodeAuxiliaryFunctions.h"
#include "suri/GuiTreeModel.h"

namespace suri {

HtmlTreeEventHandler::HtmlTreeEventHandler(GuiTreeModel* pGuiTreeModel) {
   pGuiTreeModel_ = pGuiTreeModel;
}

HtmlTreeEventHandler::~HtmlTreeEventHandler() {
}

/** 
 * Se ejecuta cuando se mueve un item despues de otro 
 * @param[in] Id Identificador del objeto a mover.
 * @param[in] DestinationId Identificador del objeto previo a donde
 * quiero mover el objeto.
 */
bool HtmlTreeEventHandler::DoOnMoveAfter(const ItemId &Id,
                                             const ItemId &DestinationId) {
   NodePath node = pGuiTreeModel_->SearchNode(Id.GetId());
   NodePath destination = pGuiTreeModel_->SearchNode(DestinationId.GetId());

   int index = 0;
   if (!destination.AllowsChildren()) {
      TreeNodeInterface* pdestparent = destination.GetLastPathNode()->GetParent();
      index = GetIndexOfChild(pdestparent, destination.GetLastPathNode()) + 1;
      destination = pGuiTreeModel_->GetPathToRoot(pdestparent);
   }

   return (pGuiTreeModel_->MoveNode(node, destination, index)).IsValid();
}

/** 
 * Se ejecuta cuando se mueve un item antes que otro
 * @param[in] Id Identificador del objeto a mover.
 * @param[in] DestinationId Identificador del objeto posterior a donde
 * quiero mover el objeto.
 */
bool HtmlTreeEventHandler::DoOnMoveBefore(const ItemId &Id,
                                                const ItemId &DestinationId) {
   NodePath node = pGuiTreeModel_->SearchNode(Id.GetId());
   NodePath destination = pGuiTreeModel_->SearchNode(DestinationId.GetId());

   TreeNodeInterface* pdestparent = destination.GetLastPathNode()->GetParent();
   int index = GetIndexOfChild(pdestparent, destination.GetLastPathNode());
   return (pGuiTreeModel_->MoveNode(node, pGuiTreeModel_->GetPathToRoot(
                                                pdestparent), index)).IsValid();
}

/** 
 * Se ejecuta cuando se mueve un item despues del ultimo elemento
 * @param[in] Id Identificador del objeto a mover.
 */
bool HtmlTreeEventHandler::DoOnMoveEnd(const ItemId &Id) {
   NodePath nodepath = pGuiTreeModel_->SearchNode(Id.GetId());
   return (pGuiTreeModel_->MoveNodeToEnd(nodepath)).IsValid();
}

/** Se ejecuta cuando se modifica la seleccion */
void HtmlTreeEventHandler::DoOnSelectionChanged(
                                    std::list<ItemId> &SelectedItems) {
   pGuiTreeModel_->ResetSelection();
   std::list<ItemId>::iterator it = SelectedItems.begin();
   while (it != SelectedItems.end()) {
      NodePath path = pGuiTreeModel_->SearchNode(it->GetId());
      pGuiTreeModel_->ChangeSelection(path, true);
      it++;
   }
}

} /* namespace suri */

