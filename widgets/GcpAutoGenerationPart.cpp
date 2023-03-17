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

#include "GcpAutoGenerationPart.h"
// Includes Estandar
#include <vector>
// Includes Suri
#include "suri/DatasourceInterface.h"
// Includes Wx
#include "wx/filepicker.h"
#include "wx/slider.h"
#include "wx/spinctrl.h"
// Defines
// forwards

namespace suri {

// Forwards
class DatasourceManagerInterface;

const char* GcpAutoGenerationPart::kBaseBandCtrlName = "ID_BASE_BAND_SPINCTRL";
const char* GcpAutoGenerationPart::kWarpBandCtrlName = "ID_WARP_BAND_SPINCTRL";
const char* GcpAutoGenerationPart::kGcpFilePickerCtrlName = "ID_GCP_FILECTRL";
const char* GcpAutoGenerationPart::kAgressionSpinCtrlName = "ID_AGGRESSION_SPINCTRL";

START_IMPLEMENT_EVENT_OBJECT(GcpAutoGenerationPartEvent, GcpAutoGenerationPart)
   IMPLEMENT_EVENT_CALLBACK(OnBaseBandValueChanged, OnBaseBandValueChanged(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnWarpBandValueChanged, OnWarpBandValueChanged(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnGcpFileChanged, OnGcpFileChanged(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnAggressionLevelChanged, OnAggressionLevelChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
GcpAutoGenerationPart::GcpAutoGenerationPart(bool Enable, bool Modified) :
      Viewer2DSelectorPart(Enable, Modified),
      selectedBaseBand_(1), selectedWarpBand_(1), 
      aggressionLevel_(0), NEW_EVENT_OBJECT(GcpAutoGenerationPartEvent) {}

GcpAutoGenerationPart::~GcpAutoGenerationPart() {
   DELETE_EVENT_OBJECT;
}


/** Crea la ventana de la parte */
void GcpAutoGenerationPart::Initialize() {
   Viewer2DSelectorPart::Initialize();
   /**
    * Obtengo el sizer de esta manera indirecta debido a un bug
    * en wxSmith
    */
   wxPanel* pPanel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                 wxT("ID_VIEWER_2D_SELECTOR_PANEL"), wxPanel);
   wxSizer* pSizer = pPanel->GetSizer();
   // Agrego las configuraciones extras para el algoritmo de reconocimiento de GCPs
   wxPanel* pExtPanel = wxXmlResource::Get()->LoadPanel(pPanel,
                                    wxT("ID_COREGISTER_GCP_GENERATION_PANEL"));
   pSizer->Add(pExtPanel, wxSizerFlags().Expand());
   ConnectEvents();
   int maxBase = this->viewerBands_.at(baseId_);
   int maxWarp = this->viewerBands_.at(warpId_);
   SetBaseMaxBand(maxBase);
   SetWarpMaxBand(maxWarp);
}

void GcpAutoGenerationPart::ConnectEvents() {
   wxSpinCtrl *pBaseBand = GetSpinCtrl(kBaseBandCtrlName);
   wxFilePickerCtrl *pGcpFilePicker = GetFilePicker();
   wxSlider *pAggressionSlider = GetSlider();
   pBaseBand->Connect(
         wxEVT_COMMAND_SPINCTRL_UPDATED,
         wxCommandEventHandler(GcpAutoGenerationPartEvent::OnBaseBandValueChanged),
         NULL, pEventHandler_);
   pGcpFilePicker->Connect(
         wxEVT_COMMAND_FILEPICKER_CHANGED,
         wxCommandEventHandler(GcpAutoGenerationPartEvent::OnGcpFileChanged),
         NULL, pEventHandler_);
   pAggressionSlider->Connect(
         wxEVT_COMMAND_SLIDER_UPDATED,
         wxCommandEventHandler(GcpAutoGenerationPartEvent::OnAggressionLevelChanged),
         NULL, pEventHandler_);
}

bool GcpAutoGenerationPart::CommitChanges() {
   return HasValidData();
}

bool GcpAutoGenerationPart::HasValidData() {
   bool valid = Viewer2DSelectorPart::HasValidData() && !this->gcpFilePath_.empty();
   return valid;
}

void GcpAutoGenerationPart::OnChBaseSelectionChanged(wxCommandEvent& Event) {
    Viewer2DSelectorPart::OnChBaseSelectionChanged(Event);
    int maxBand = this->viewerBands_.at(baseId_);
    SetBaseMaxBand(maxBand);
}

void GcpAutoGenerationPart::OnChWarpSelectionChanged(wxCommandEvent& Event) {
    Viewer2DSelectorPart::OnChWarpSelectionChanged(Event);
    int maxBand = this->viewerBands_.at(warpId_);
    SetWarpMaxBand(maxBand);
}

/** Retorna el numero de banda de la imagen base a usar para corregistrar */
void GcpAutoGenerationPart::OnBaseBandValueChanged(wxCommandEvent &Event) {
   this->selectedBaseBand_ = GetSpinCtrl(kBaseBandCtrlName)->GetValue();
}

/** Retorna el numero de banda de la imagen base a usar para corregistrar */
void GcpAutoGenerationPart::OnWarpBandValueChanged(wxCommandEvent &Event) {
   this->selectedWarpBand_ = GetSpinCtrl(kWarpBandCtrlName)->GetValue();
}

void GcpAutoGenerationPart::OnGcpFileChanged(wxCommandEvent& Event ){
   this->gcpFilePath_ = GetFilePicker()->GetPath();
}

void GcpAutoGenerationPart::OnAggressionLevelChanged(wxCommandEvent& Event ){
   this->aggressionLevel_ = GetSlider()->GetValue();
}

GcpAutoGenerationProcess::Parameters GcpAutoGenerationPart::GetParameters() const {
   GcpAutoGenerationProcess::Parameters params;
   params.baseBands_.push_back(this->selectedBaseBand_ - 1);
   params.warpBands_.push_back(this->selectedWarpBand_ - 1);
   params.seedGcpsPath_  = this->gcpFilePath_;
   params.agressionLevel_  = this->aggressionLevel_;
   return params;
}

void GcpAutoGenerationPart::SetViewerBands(const std::map<SuriObject::UuidType, int>& ViewerBands) {
   this->viewerBands_ = ViewerBands;
}

/** Indica el valor maximo que puede tomar el spinner de banda base **/
void GcpAutoGenerationPart::SetBaseMaxBand(int maxBand) {
   GetSpinCtrl(kBaseBandCtrlName)->SetRange(1, maxBand);
}
/** Indica el valor maximo que puede tomar el spinner de banda base **/
void GcpAutoGenerationPart::SetWarpMaxBand(int maxBand) {
   GetSpinCtrl(kWarpBandCtrlName)->SetRange(1, maxBand);
}

/** Retorna el spin control correspondiente al nombre **/
wxSpinCtrl* GcpAutoGenerationPart::GetSpinCtrl(const char* ControlName) {
   wxSpinCtrl *pSpinCtrl = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT(ControlName), wxSpinCtrl);
   return pSpinCtrl;
}

/** Retorna el filePicker correspondiente al archivo de gcps **/
wxFilePickerCtrl* GcpAutoGenerationPart::GetFilePicker() {
   wxFilePickerCtrl *pFilePicker = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT(kGcpFilePickerCtrlName), wxFilePickerCtrl);
   return pFilePicker;
}

wxSlider* GcpAutoGenerationPart::GetSlider() {
   wxSlider *pSlider = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                   wxT(kAgressionSpinCtrlName), wxSlider);
   return pSlider;
}


} /** namespace suri */
