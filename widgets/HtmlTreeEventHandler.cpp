/** \file HtmlTreeEventHandler.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

