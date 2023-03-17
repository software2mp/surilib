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
#include <string>

// Includes Suri
#include "wxmacros.h"
#include "logmacros.h"
#include "SpatialReference.h"
#include "GdalSrsItemOrigin.h"
#include "Viewer2DProperties.h"
#include "UniversalGraphicalComponentPart.h"
#include "UniversalGraphicalComponentPartInterface.h"
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "suri/DynamicViewerWidget.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"

// Defines
#define SRS_LIBRARY_CODE "srs"
#define DEFAULT_SR "EPSG:4326"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(Viewer2DPropertiesEvent, Viewer2DProperties)
   IMPLEMENT_EVENT_CALLBACK(OnViewerModeChange, OnViewerModeChange(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param pViewer
 */
Viewer2DProperties::Viewer2DProperties(ViewerWidget* pViewer, LibraryManager* pManager) :
   PartCollection(wxT("ID_VIEWER2D_PROPERTIES_PANEL"), _(caption_VIEWPORT_PROPERTIES)),
     NEW_EVENT_OBJECT(Viewer2DPropertiesEvent) {
   pViewer_ = pViewer;
   pManager_ = pManager;
   Library* plibrary = pManager->GetLibraryByCode(SRS_LIBRARY_CODE,
                                                 LibraryManager::AccessKey(true));
   pSrsSelectionPart_ = new UniversalGraphicalComponentPart(
        plibrary->GetId(), pManager, NULL,
        UniversalGraphicalComponentPartInterface::ExtendedNoDropDownWithFullFledged,
        true);
   AddPart(pSrsSelectionPart_, wxT("ID_SR_LIBRARY_PANEL"));
   DynamicViewerWidget* p2d = dynamic_cast<DynamicViewerWidget*>(pViewer_);
   mode_ = p2d->GetVisualizationMode();
   windowTitle_ = _(caption_VIEWPORT_PROPERTIES);
}

/**
 * Destructor
 */
Viewer2DProperties::~Viewer2DProperties() {
   DELETE_EVENT_OBJECT;
}

/**
 * Configura Widget
 * @return
 */
bool Viewer2DProperties::ConfigureWidget() {
   PartCollection::ConfigureWidget();
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   GET_CONTROL(*pwidgetwindow, wxT("ID_VIEWER_NAME_LABEL"), wxStaticText)
      ->SetLabel(pViewer_->GetWindowTitle());
   GET_CONTROL(*pwidgetwindow, wxT("ID_VIEWER_MODE_CHOICE"), wxChoice)->Connect(
            wxEVT_COMMAND_CHOICE_SELECTED,
            wxCommandEventHandler(Viewer2DPropertiesEvent::OnViewerModeChange), NULL,
            pEventHandler_);

   DynamicViewerWidget* p2d = dynamic_cast<DynamicViewerWidget*>(pViewer_);
   if (p2d) {
      //std::string srwkt = p2d->GetWorld()->GetSpatialReference().c_str();
      int mode = p2d->GetVisualizationMode();
      switch (mode) {
         case DynamicViewerWidget::Raster:
            ChangeToRasterMode();
            break;
         case DynamicViewerWidget::Gis:
            ChangeToGisMode();
            break;
      }
   }

   return true;
}

/**
 * Metodo auxiliar para cambiar a modo raster
 */
void Viewer2DProperties::ChangeToRasterMode() {
   int mode = 1;
   pSrsSelectionPart_->UpdateFeaturesLayout(
         UniversalGraphicalComponentPartInterface::ExtendedNoDropDownWithFullFledged);

   std::string principal = "";
   std::string srwkt = pViewer_->GetWorld()->GetSpatialReference().c_str();
   if (!srwkt.empty()) {
      principal = SpatialReference::GetAuthorityId(pViewer_->GetWorld()->GetSpatialReference());
   }
   pSrsSelectionPart_->SelectItemByPrincipal(principal);

   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   GET_CONTROL(*pwidgetwindow, wxT("ID_VIEWER_MODE_CHOICE"), wxChoice)->SetSelection(mode);
   pSelectedItem_ = (principal.empty()) ? NULL : pSrsSelectionPart_->GetSelectedItem();
}

/**
 * Metodo auxiliar para cambiar a modo gis
 */
void Viewer2DProperties::ChangeToGisMode() {
   int mode = 0;
   pSrsSelectionPart_->UpdateFeaturesLayout(
         UniversalGraphicalComponentPartInterface::ExtendedRoWithFFAndNoLabel);

   std::string srwkt = pViewer_->GetWorld()->GetSpatialReference().c_str();
   std::string principal;
   if (SpatialReference::IsProjected(srwkt)) {
      principal = SpatialReference::GetProjCoordinateSystemAuthorityData(srwkt);
   } else if (SpatialReference::IsGeographic(srwkt)) {
      principal = SpatialReference::GetGeoCoordinateSystemAuthorityData(srwkt);
   } else {
      principal = Configuration::GetParameter("lib_spatial_reference_default", DEFAULT_SR);
   }
   pSrsSelectionPart_->SelectItemByPrincipal(principal);
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   GET_CONTROL(*pwidgetwindow, wxT("ID_VIEWER_MODE_CHOICE"), wxChoice)->SetSelection(mode);
   pSelectedItem_ = pSrsSelectionPart_->GetSelectedItem();
}

/**
 * Indica si el Part tiene cambios para salvar
 * @return
 */
bool Viewer2DProperties::HasChanged() {
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   int mode = (XRCCTRL(*(pwidgetwindow), wxT("ID_VIEWER_MODE_CHOICE"), wxChoice))->
         GetCurrentSelection();
   const LibraryItem* pSelectedItem = pSrsSelectionPart_->GetSelectedItem();
   bool modechanged = mode != mode_;
   bool selectionchanged = false;
   if (pSelectedItem_ != NULL && mode == DynamicViewerWidget::Gis)
      selectionchanged = !pSelectedItem->Equals(*pSelectedItem_);
   bool partchanged = PartCollection::HasChanged();
   return modechanged || selectionchanged || partchanged;
}

/**
 * Salva los cambios realizados en el Part
 * @return
 */
bool Viewer2DProperties::CommitChanges() {
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   int mode = (XRCCTRL(*(pwidgetwindow), wxT("ID_VIEWER_MODE_CHOICE"), wxChoice))->
         GetCurrentSelection();
   DynamicViewerWidget* p2d = dynamic_cast<DynamicViewerWidget*>(pViewer_);
   if (p2d) {
      p2d->DeactivateViewerElements();
      switch (mode) {
         case DynamicViewerWidget::Gis: {
            p2d->ChangeVisualizationMode(DynamicViewerWidget::Gis);
            const LibraryItem* pSelectedItem = pSrsSelectionPart_->GetSelectedItem();
            if (pSelectedItem) {
               const LibraryItemAttribute* pattr = pSelectedItem->GetAttribute(
                     GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME);
               if (pattr != NULL) {
                  std::string srwkt = pattr->GetValue();
                  p2d->GetWorld()->SetSpatialReference(srwkt);
                  Configuration::SetParameterEx("GISMode_World_Wkt", srwkt);
               }
            }
            break;
         }
         case DynamicViewerWidget::Raster:
            if (pViewer_->GetList()->GetActiveCount() > 0) {
               SHOW_ERROR(message_GIS_ERROR);
               return false;
            } else {
               Configuration::RemoveParameterEx("GISMode_World_Wkt");
               p2d->ChangeVisualizationMode(DynamicViewerWidget::Raster);
            }
            break;
      }
   }
   return PartCollection::CommitChanges();
}

/**
 * Restaura los valores originales del Part
 * @return
 */
bool Viewer2DProperties::RollbackChanges() {
   return PartCollection::RollbackChanges();
}

/**
 * Evento que se ejecuta al seleccionar un item en el choice cambiando el tooltip asociado
 * @param Event
 */
void Viewer2DProperties::OnViewerModeChange(wxCommandEvent &Event) {
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   int mode = (XRCCTRL(*(pwidgetwindow), wxT("ID_VIEWER_MODE_CHOICE"), wxChoice))->
         GetCurrentSelection();
   switch (mode) {
      case DynamicViewerWidget::Gis:
         ChangeToGisMode();
         break;
      case DynamicViewerWidget::Raster:
         ChangeToRasterMode();
         break;
   }
}
} /* namespace suri */
