/*! \file ViewportLinkImpl.cpp */
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

// Includes Suri
#include "suri/ViewportLinkImpl.h"
#include "suri/SuriObject.h"
#include "suri/CoordinatesTransformInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

ViewportLinkImpl::ViewportLinkImpl() :
      masterViewportId_(SuriObject::NullUuid), pViewportManager_(NULL),
      pViewcontextManager_(NULL) {
}

ViewportLinkImpl::~ViewportLinkImpl() {
}

void ViewportLinkImpl::SetMasterViewport(const SuriObject::UuidType &MasterViewport) {
   masterViewportId_ = MasterViewport;
}

void ViewportLinkImpl::SetLinkedViewports(
      const std::vector<SuriObject::UuidType> &LinkedViewports) {
   linkedViewportIds_ = LinkedViewports;
}

void ViewportLinkImpl::SetManagers(ViewportManagerInterface *pViewportManager,
                                   ViewcontextManagerInterface *pViewcontextManager) {
   pViewportManager_ = pViewportManager;
   pViewcontextManager_ = pViewcontextManager;
}

ViewportInterface *ViewportLinkImpl::GetMasterViewport() const {
   return GetViewportManager()->GetViewport(masterViewportId_);
}

std::vector<ViewportInterface*> ViewportLinkImpl::GetLinkedViewports() const {
   std::vector<ViewportInterface*> linkedvps;
   ViewportManagerInterface *pviewportmanger = GetViewportManager();
   for (std::vector<SuriObject::UuidType>::const_iterator it =
         linkedViewportIds_.begin(); it != linkedViewportIds_.end(); ++it) {
      linkedvps.push_back(pviewportmanger->GetViewport(*it));
   }
   return linkedvps;
}

ViewportManagerInterface* ViewportLinkImpl::GetViewportManager() const {
   return pViewportManager_;
}

ViewcontextManagerInterface* ViewportLinkImpl::GetViewcontextManager() const {
   return pViewcontextManager_;
}

} /** namespace suri */
