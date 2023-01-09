/** \file ViewcontextTreeSelectionManager.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
