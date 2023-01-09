/*! \file ViewportLinkManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <map>

// Includes Suri
#include "suri/ViewportLinkManager.h"
#include "suri/CoordinatesTransformInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

ViewportLinkManager::ViewportLinkManager(
      ViewportManagerInterface *pViewportManager,
      ViewcontextManagerInterface *pViewcontextManager) :
      pViewportManager_(pViewportManager), pViewcontextManager_(pViewcontextManager),
      isSychronizing_(false), pViewportLink_(NULL) {
}

ViewportLinkManager::~ViewportLinkManager() {
}

bool ViewportLinkManager::AddViewport(ViewportInterface *pViewport, bool Master) {
   bool retval =
         linkedViewports_.insert(std::make_pair(pViewport->GetId(), Master)).second;
   if (retval) {
      if (Master) {
         World *pvpworld = AS_VW(pViewport) ? AS_VW(pViewport)->GetWorld() : NULL;
         if (pvpworld) {
            WorldView<ViewportLinkManager> *pworldviewer = new WorldView<
                  ViewportLinkManager>(this, &ViewportLinkManager::OnMasterWorldChange,
                                       &ViewportLinkManager::OnMasterWorldDelete);
            viewportWorldViewers_.insert(
                  std::make_pair(pViewport->GetId(), pworldviewer));

            pvpworld->RegisterViewer(pworldviewer);
         } else {
            RemoveViewport(pViewport->GetId());
            retval = false;
         }
      }
   }
   return retval;
}

bool ViewportLinkManager::RemoveViewport(const SuriObject::UuidType &ViewportId) {
   std::map<SuriObject::UuidType, bool>::iterator found = linkedViewports_.find(
         ViewportId);
   if (found != linkedViewports_.end()) {
      ViewportInterface *pviewport = pViewportManager_->GetViewport(found->first);
      World *pvpworld = AS_VW(pviewport) ? AS_VW(pviewport)->GetWorld() : NULL;
      if (pvpworld) {
         std::map<SuriObject::UuidType, WorldView<ViewportLinkManager>*>::iterator it =
               viewportWorldViewers_.find(ViewportId);
         if (it != viewportWorldViewers_.end()) {
            pvpworld->UnregisterViewer(it->second);
            delete it->second;
            viewportWorldViewers_.erase(it);
         }
      }
      linkedViewports_.erase(found);
   } else {
      return false;
   }
   return true;
}

bool ViewportLinkManager::RemoveViewports() {
   while (!linkedViewports_.empty() && RemoveViewport(linkedViewports_.begin()->first)) {
   }
   return linkedViewports_.empty();
}

ViewportLinkInterface* ViewportLinkManager::SetLinkCommand(
      ViewportLinkInterface *pLink) {
   ViewportLinkInterface *poldlink = pViewportLink_;
   pViewportLink_ = pLink;
   return poldlink;
}

std::vector<SuriObject::UuidType> ViewportLinkManager::GetLinkedViewports() const {
   std::vector<SuriObject::UuidType> linkedvps;
   std::map<SuriObject::UuidType, bool>::const_iterator it = linkedViewports_.begin();
   while (it != linkedViewports_.end()) {
      linkedvps.push_back(it->first);
      ++it;
   }
   return linkedvps;
}

bool ViewportLinkManager::IsMasterViewport(
      const SuriObject::UuidType &ViewportId) const {
   std::map<SuriObject::UuidType, bool>::const_iterator found = linkedViewports_.find(
         ViewportId);
   if (found == linkedViewports_.end()) return false;
   return found->second;
}

void ViewportLinkManager::OnMasterWorldChange(Model *pWorld) {
   if (isSychronizing_) return;
   isSychronizing_ = true;
   World *pchangedworld = dynamic_cast<World*>(pWorld);
   if (pchangedworld) {
      std::vector<SuriObject::UuidType> likedvps;
      std::map<SuriObject::UuidType, bool>::iterator vpit = linkedViewports_.begin();
      while (vpit != linkedViewports_.end()) {
         SuriObject::UuidType vpid = vpit->first;
         ViewportInterface *pviewport = pViewportManager_->GetViewport(vpid);
         World *pvpworld = AS_VW(pviewport) ? AS_VW(pviewport)->GetWorld() : NULL;
         if (pvpworld != pchangedworld) {
            likedvps.push_back(vpid);
         } else {
            pViewportLink_->SetMasterViewport(vpid);
         }
         ++vpit;
      }
      pViewportLink_->SetLinkedViewports(likedvps);
      pViewportLink_->SetManagers(pViewportManager_, pViewcontextManager_);
      pViewportLink_->Execute();
      pViewportLink_->SetManagers(NULL, NULL);
   }
   isSychronizing_ = false;
}

void ViewportLinkManager::OnMasterWorldDelete(Model *pWorld) {
   // ??!?
}

}

