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

