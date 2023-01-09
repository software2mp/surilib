/** \file IndependentTreeSelectionManager.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "IndependentTreeSelectionManager.h"

namespace suri {

IndependentTreeSelectionManager::IndependentTreeSelectionManager(
                                 GroupManagerInterface* pGroupManager) {
   pGroupManager_ = pGroupManager;
}

IndependentTreeSelectionManager::~IndependentTreeSelectionManager() {
}

/** Limpia la seleccion de grupos e items */
void IndependentTreeSelectionManager::ResetSelection() {
   selectedNodes_.clear();
}

/** Permite modificar los nodos seleccionados */
void IndependentTreeSelectionManager::ChangeSelection(const NodePath &Path,
                                                               bool Selected) {
   SuriObject::UuidType nodeid = Path.GetLastPathNode()->GetId();
   if (Selected)
      selectedNodes_.insert(nodeid);
   else
      selectedNodes_.erase(nodeid);
}

/** Retorna el path de nodos seleccionados */
std::list<NodePath> IndependentTreeSelectionManager::GetSelectedNodes() {
   std::list<NodePath> selectednodepaths;
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   NodeUuidSet::iterator it = selectedNodes_.begin();
   while (it != selectedNodes_.end()) {
      NodePath path = pworkgroup->SearchNode(*it);
      if (path.IsValid()) {
         selectednodepaths.push_back(path);
         ++it;
      } else {
         selectedNodes_.erase(it++);
      }
   }
   delete pworkgroup;
   return selectednodepaths;
}

} /* namespace suri */
