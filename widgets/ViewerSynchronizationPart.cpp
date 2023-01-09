/*! \file ViewerSynchronizationPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>

// Includes Suri
#include "suri/DataTypes.h"
#include "suri/ViewportLinkImpl.h"
#include "GeographicLink.h"
#include "RasterLink.h"
#include "ViewerSynchronizationPart.h"

// Includes Wx

// Defines

namespace suri {

const char *pViewportCheckIds[] = { "ID_VIEWER1_CHECK", "ID_VIEWER2_CHECK",
                                    "ID_VIEWER3_CHECK", "ID_VIEWER4_CHECK" };

/** Ctor */
ViewerSynchronizationPart::ViewerSynchronizationPart(
      ViewportManagerInterface *pViewportManager,
      ViewcontextManagerInterface *pViewcontextManager,
      ViewportLinkManager *pViewportLink, bool Enable, bool Modified) :
      Part(wxT("ID_SYNCHRONIZE_VIEWERS_PANEL"), _(caption_VIEWER_SYNCHRONIZATION),
           Enable, Modified),
      pViewportManager_(pViewportManager), pViewcontextManager_(pViewcontextManager),
      pViewportLink_(pViewportLink) {
   windowTitle_ = _(caption_VIEWER_SYNCHRONIZATION);
}

/** Dtor */
ViewerSynchronizationPart::~ViewerSynchronizationPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool ViewerSynchronizationPart::HasChanged() {
   bool retval = false;
   int viewers = 0;
   /** Gabriel TCK #2667: es necesario verificar si no esta
    *  ningun visualizador chequeado. */
   for (size_t i = 0; i < ARRAY_SIZE(pViewportCheckIds); ++i) {
      bool checked = USE_CONTROL(*GetWidget()->GetWindow(),
                  pViewportCheckIds[i], wxCheckBox, IsChecked(), false);
      retval |= checked;
      viewers = checked? viewers + 1 : viewers;
   }
   return retval | (viewers == 0);
}

/** Salva los cambios realizados en el Part. */
bool ViewerSynchronizationPart::CommitChanges() {
   if (!HasValidData())
      return false;
   if (!pViewportLink_)
      return false;
   else
      pViewportLink_->RemoveViewports();
   bool linkcenter = USE_CONTROL(*GetWidget()->GetWindow(),
         wxT("ID_ONLY_CENTER_CHECK"), wxCheckBox, IsChecked(), true);
   switch(USE_CONTROL(*GetWidget()->GetWindow(), wxT("ID_SYNCHRONIZATION_TYPE_CHOICE"),
         wxChoice, GetCurrentSelection(), 0)) {
      case 0:
      delete pViewportLink_->SetLinkCommand(new GeographicLink(linkcenter));
      break;
      default:
      case 1:
      delete pViewportLink_->SetLinkCommand(new RasterLink(linkcenter));
      break;
   }
   std::vector<SuriObject::UuidType> ids = pViewportManager_->GetViewportIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();

   size_t idx = 0;
   while (it != ids.end() && idx < ARRAY_SIZE(pViewportCheckIds)) {
      bool ischecked = USE_CONTROL(*GetWidget()->GetWindow(),
            pViewportCheckIds[idx], wxCheckBox, IsChecked(), false);
      if (ischecked) {
         pViewportLink_->AddViewport(pViewportManager_->GetViewport(*it));
      }
      ++idx;
      ++it;
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool ViewerSynchronizationPart::RollbackChanges() {
   SetInitialValues();
   return true;
}

/** Inicializa el part */
void ViewerSynchronizationPart::SetInitialValues() {
   if (!pViewportLink_)
      return;

   std::vector<SuriObject::UuidType> ids = pViewportManager_->GetViewportIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   size_t idx = 0;
   while (it != ids.end() && idx < ARRAY_SIZE(pViewportCheckIds)) {
      bool islinked = false;
      std::vector<SuriObject::UuidType> linkedviewportids =
            pViewportLink_->GetLinkedViewports();
      std::vector<SuriObject::UuidType>::iterator linkvpit = linkedviewportids.begin();
      while (linkvpit != linkedviewportids.end()) {
         if (*linkvpit == *it) islinked = true;
         ++linkvpit;
      }

      GET_CONTROL(*GetWidget()->GetWindow(), pViewportCheckIds[idx],
            wxCheckBox)->SetValue(islinked);
      ViewportLinkInterface *plink = pViewportLink_->SetLinkCommand(NULL);
      if (plink)
      GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_ONLY_CENTER_CHECK"),
            wxCheckBox)->SetValue(plink->IsCentered());
      int linktypeidx = 1;
      if (dynamic_cast<GeographicLink*>(plink)) linktypeidx = 0;
      pViewportLink_->SetLinkCommand(plink);
      GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_SYNCHRONIZATION_TYPE_CHOICE"),
            wxChoice)->SetSelection(linktypeidx);

      ++idx;
      ++it;
   }
}

/** Actualiza el estado de la parte */
void ViewerSynchronizationPart::Update() {
}

/** Modifica el nombre del viewer. */
bool ViewerSynchronizationPart::ConfigureWidget() {
   std::vector<SuriObject::UuidType> ids = pViewportManager_->GetViewportIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   size_t idx = 0;
   size_t activecount = 0;
   while (it != ids.end() && idx < ARRAY_SIZE(pViewportCheckIds)) {
      bool activate = false;
      if (pViewportManager_->IsViewportActive(*it)) {
         ++activecount;
         activate = true;
      }
      GetWidget()->Enable(pViewportCheckIds[idx], activate);
      ++idx;
      ++it;
   }
   return true;
}

/** Devuelve si la parte tiene datos validos */
bool ViewerSynchronizationPart::HasValidData() {
   /** Si hay un solo viewer activo, no tiene sentido sincronizar. **/
   int viewers = 0;
   for (size_t i = 0; i < ARRAY_SIZE(pViewportCheckIds); ++i) {
      bool checked = USE_CONTROL(*GetWidget()->GetWindow(),
            pViewportCheckIds[i], wxCheckBox, IsChecked(), false);
      viewers = checked ? viewers + 1 : viewers;
   }
   return viewers != 1;
}

}  // end namespace suri

