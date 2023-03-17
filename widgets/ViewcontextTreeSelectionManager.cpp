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

// Includes Standard
#include <list>
#include <vector>
// Includes suri
#include "suri/ViewcontextTreeSelectionManager.h"
#include "suri/GroupManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"

namespace suri {

/** Ctor */
ViewcontextTreeSelectionManager::ViewcontextTreeSelectionManager(
                        GroupManagerInterface* pGroupManager,
                        ViewcontextManagerInterface* pViewcontextManager) {
   pGroupManager_ = pGroupManager;
   pViewcontextManager_ = pViewcontextManager;
}

/** Dtor */
ViewcontextTreeSelectionManager::~ViewcontextTreeSelectionManager() {
}


/** Limpia la seleccion de items */
void ViewcontextTreeSelectionManager::ResetSelection() {
   std::vector<SuriObject::UuidType> ids = pViewcontextManager_->GetViewcontextIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   for (; it != ids.end(); ++it)
      pViewcontextManager_->GetViewcontext(*it)->ResetSelection();

   selectedGroupsList_.clear();
}

/**
 * Permite modificar los nodos seleccionados
 * @param[in] Path NodePath del nodo
 * @param[in] Selected flag que indica si el nodo debe estar seleccionado
 * o no.
 */
void ViewcontextTreeSelectionManager::ChangeSelection(const NodePath &Path, bool Selected) {
   if (!Path.AllowsChildren()) {
      SuriObject::UuidType datasourceid = Path.GetLastPathNode()->GetContent();

      std::vector<SuriObject::UuidType> ids = pViewcontextManager_->GetViewcontextIds();
      std::vector<SuriObject::UuidType>::iterator it = ids.begin();
      for (; it != ids.end(); ++it) {
         ViewcontextInterface * pviewcontext = pViewcontextManager_->GetViewcontext(*it);
         LayerInterface* player = pviewcontext->GetAssociatedLayer(datasourceid);
         if (player)
            pviewcontext->SelectLayer(player->GetId(), Selected);
      }
   } else {
      SelectGroup(Path.GetLastPathNode()->GetId(), Selected);
   }
}

/**
 * Retorna el path de nodos seleccionados
 * @return listado con el path de los nodos seleccionados
 */
std::list<NodePath> ViewcontextTreeSelectionManager::GetSelectedNodes() {
   std::list<NodePath> selectednodes;
   // Como los items en el contexto 3D y 2D pueden ser diferentes la seleccion del
   // viewcontext actual puede ser diferente a la de los otros viewcontexts.
   // Se da prioridad al viewcontext actual porque es el que ve el usuario.
   ViewcontextInterface* pviewcontext = pViewcontextManager_->GetSelectedViewcontext();
   if (!pviewcontext) {
      REPORT_DEBUG("D: No hay Viewcontext seleccionado");
      return selectednodes;
   }

   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();

   // Agrego al listado las capas seleccionadas
   std::list<SuriObject::UuidType> selectedlayers = pviewcontext->GetSelectedLayers();
   std::list<SuriObject::UuidType>::const_iterator it = selectedlayers.begin();
   for (; it != selectedlayers.end(); ++it) {
      DatasourceInterface* pselecteddatasource = pviewcontext->GetAssociatedDatasource(*it);
      if (pselecteddatasource) {
         NodePath path = pworkgroup->SearchContent(pselecteddatasource->GetId());
         if (path.IsValid())
            selectednodes.push_back(path);
      }
   }

   // Agrego al listado los grupos seleccionados
   UpdateSelectedGroupsWithModel();
   std::list<SuriObject::UuidType>::iterator itgrps = selectedGroupsList_.begin();
   for (; itgrps != selectedGroupsList_.end(); ++itgrps) {
      NodePath path = pworkgroup->SearchNode(*itgrps);
      if (path.IsValid())
         selectednodes.push_back(path);
   }

   delete pworkgroup;
   return selectednodes;
}

/** 
 * Cambia estado de seleccion de un grupo 
 * @param[in] GroupId Id del grupo
 * @param[in] State flag que indica si el nodo debe estar seleccionado 
 */
void ViewcontextTreeSelectionManager::SelectGroup(
                           const SuriObject::UuidType &GroupId, bool State) {
   GroupUuidList::iterator it = selectedGroupsList_.begin();
   for (; it != selectedGroupsList_.end() && *it != GroupId; ++it);

   if (State && it == selectedGroupsList_.end())
      selectedGroupsList_.push_back(GroupId);
   if (!State && it != selectedGroupsList_.end())
      selectedGroupsList_.erase(it);
}

/** Busca los grupos seleccionados en modelo y elimina los que no encuentra */
void ViewcontextTreeSelectionManager::UpdateSelectedGroupsWithModel() {
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   GroupUuidList::iterator it = selectedGroupsList_.begin();
   while (it != selectedGroupsList_.end()) {
      if (!pworkgroup->SearchNode(*it).IsValid())
         selectedGroupsList_.erase(it++);
      else
         ++it;
   }
   delete pworkgroup;
}


} /* namespace suri */
