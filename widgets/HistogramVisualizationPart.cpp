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

#include <string>
#include <set>
#include <vector>
#include <memory>

#include "suri/AuxiliaryFunctions.h"
#include "suri/HistogramVisualizationPart.h"
#include "suri/RasterElement.h"
#include "RasterLayer.h"
#include "suri/StatisticsCalculator.h"
#include "suri/PlotterWidget.h"
#include "suri/wx2DPlotter.h"
#include "resources.h"
#include "suri/xmlnames.h"

#include "wx/choice.h"
#include "wx/textctrl.h"

// Defines
#define BAND_COMBINATION_CHOICE "Combinacion de bandas RVA"
#define SHOW_ALL_BANDS_CHOICE "Todas las bandas"

namespace suri {

namespace ui {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(HistogramVisualizationHandler, HistogramVisualizationPart)
   IMPLEMENT_EVENT_CALLBACK(OnRefreshClicked, OnRefreshClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
   IMPLEMENT_EVENT_CALLBACK(OnBandSelectionChange, OnBandSelectionChange(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
HistogramVisualizationPart::HistogramVisualizationPart(RasterLayer* pRaster) :
      pRaster_(pRaster), pStats_(NULL), pHistogram_(NULL),
      pEventHandler_(new HistogramVisualizationHandler(this)), pPlotRender_(NULL),
      pPlotter_(NULL), useCombination_(false) {
   windowTitle_ = _("Histograma");
}

HistogramVisualizationPart::~HistogramVisualizationPart() {
   delete pEventHandler_;
}

/** Indica si el Part tiene cambios para salvar. */
bool HistogramVisualizationPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool HistogramVisualizationPart::CommitChanges() {
   mins_ = currentMins_;
   maxs_ = currentMaxs_;
   return true;
}

/** Restaura los valores originales del Part. */
bool HistogramVisualizationPart::RollbackChanges() {
   currentMins_ = mins_;
   currentMaxs_ = maxs_;
   return true;
}

/** Inicializa el part */
void HistogramVisualizationPart::SetInitialValues() {
   //  
   RasterElement* pelement = dynamic_cast<RasterElement*>(pRaster_->GetElement());
   wxXmlNode *prnode = pelement->GetNode(RENDERIZATION_NODE);
   wxXmlNode *pcombinationnode = pelement->GetNode(BAND_COMBINATION_NODE, prnode);
   std::vector<int> bandcombination;
   if (pcombinationnode) {
      wxXmlNode *pbandsnode = pcombinationnode->GetChildren();
      if (pbandsnode) {
         std::string bands = pbandsnode->GetContent().c_str();
         std::vector < std::string > bandlist = tokenizer(bands, " ");
         std::vector<std::string>::iterator iter = bandlist.begin();
         for (; iter != bandlist.end(); iter++)
            bandcombination.push_back(atoi((*iter).c_str()));
      }
   }
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_BAND_SELECTION_CHOICE"),
                               wxChoice);
   pchoice->Clear();
   if (useCombination_) {
      std::vector<int>::iterator it = bandcombination.begin();
      for (; it != bandcombination.end(); ++it) {
         std::string bandname;
         pelement->GetBandName(bandname, *it);
         pchoice->Append(bandname);
      }
   } else {
      for (int i = 0; i < pelement->GetBandCount(); ++i) {
         std::string bandname;
         pelement->GetBandName(bandname, i);
         pchoice->Append(bandname);
      }
   }
   pchoice->SetSelection(0);
   modified_ = false;
}

/** Actualiza el estado de la parte */
void HistogramVisualizationPart::Update() {
}

/** Inicializa la ventana y sus componentes **/
bool HistogramVisualizationPart::CreateToolWindow() {
   if (pToolWindow_) return false;
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_MANUALENHANCEMENT_PANEL"));

   pToolWindow_->Connect(wxEVT_SIZE,
                         wxSizeEventHandler(HistogramVisualizationHandler::OnResize),
                         NULL, pEventHandler_);
   GetFrame()->SetMinSize(wxSize(800, 600));
   GET_CONTROL(*(this->GetWidget()->GetWindow()), "ID_REFRESH_HISTOGRAM_BTN",
         wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(HistogramVisualizationHandler::OnRefreshClicked), NULL,
         pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_BAND_SELECTION_CHOICE", wxChoice)->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(HistogramVisualizationHandler::OnBandSelectionChange),
         NULL, pEventHandler_);

   SetInitialValues();

   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_ENHANCEMENT_CONFIG_PANEL", wxPanel)->Hide();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_INVERT_MAX_MIN_CHECK", wxCheckBox)->Hide();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_MIN_TXT", wxTextCtrl)->Disable();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_MAX_TXT", wxTextCtrl)->Disable();
   return true;
}

/** Metodo que se llama cuando se presiona el boton de actualizar histograma **/
void HistogramVisualizationPart::OnRefreshClicked(wxCommandEvent& Event) {
   Refresh();
}

void HistogramVisualizationPart::OnBandSelectionChange(wxCommandEvent& Event) {
   Refresh();
}

/** metodo que fuerza el redibujado en pantalla **/
void HistogramVisualizationPart::Refresh() {
   // TODO: ojo que no es secuencial la combinacion de bandas 
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_BAND_SELECTION_CHOICE"),
                               wxChoice);
   int bandsel = pchoice->GetSelection();
   if (!useCombination_) {
      std::string bselname = pchoice->GetStringSelection().c_str();
      for (int i = 0; i < pRaster_->GetBandCount(); ++i) {
         std::string bandname;
         pRaster_->GetBandName(bandname, i);
         if (bandname.compare(bselname) == 0) {
            bandsel = i;
            break;
         }
      }
   }

   if (!pPlotter_) {
      RasterElement* pelement = dynamic_cast<RasterElement*>(pRaster_->GetElement());
      pPlotter_ = new HistogramPlotterWidget(this);
      raster::data::StatisticsCalculator statscalculator(pelement);
      if (pHistogram_ != NULL) {
         delete pHistogram_;
         delete pStats_;
      }
      statscalculator.CalculateStatistics(pStats_, pHistogram_, !useCombination_);
      pPlotRender_ = new wx::wxHistogramPlotter(pHistogram_);
      pPlotRender_->SetActiveBand(bandsel);
      pPlotter_->SetPlotter(pPlotRender_);
      AddControl(pPlotter_, "ID_HISTOGRAM_CANVAS_PANEL");
      for (int b = 0; b < pHistogram_->GetBandCount(); ++b) {
         mins_.push_back(pHistogram_->GetMin()[b]);
         maxs_.push_back(pHistogram_->GetMax()[b]);
      }
      currentMins_ = mins_;
      currentMaxs_ = maxs_;
   } else {
      /** cambio la banda seleccionada **/
      pPlotRender_->SetActiveBand(bandsel);
   }
   XRCCTRL(*pToolWindow_, wxT("ID_MIN_TXT"), wxTextCtrl)->ChangeValue(
         DoubleToString(currentMins_[bandsel]));
   XRCCTRL(*pToolWindow_, wxT("ID_MAX_TXT"), wxTextCtrl)->ChangeValue(
         DoubleToString(currentMaxs_[bandsel]));
   pPlotRender_->SetMaxValue((currentMaxs_[bandsel]));
   pPlotRender_->SetMinValue((currentMins_[bandsel]));
   pPlotter_->GetWindow()->Refresh();
   pPlotter_->GetWindow()->Update();
}
/** Retorna el icono de la herramienta */
void HistogramVisualizationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
                                               int Y) const {
   GET_BITMAP_RESOURCE(tool_HISTOGRAM_VIEWER_32, ToolBitmap);
}
/** Lanzado cuando cambia el tamanio de la ventana */
void HistogramVisualizationPart::OnResize(wxSizeEvent &Event) {
   if (pPlotter_) {
      Refresh();
      pPlotter_->GetWindow()->Refresh();
      pPlotter_->GetWindow()->Update();
      pToolWindow_->Layout();
      pToolWindow_->Refresh();
      pToolWindow_->Update();
   }
}
/** Metodo polimorfico que se llama cuando se cambiaron los limites del histograma sobre la banda activa**/
void HistogramVisualizationPart::HistogramMaxChange() {
}

void HistogramVisualizationPart::HistogramMinChange() {
}

void HistogramVisualizationPart::PointSelected(double X, double Y) {
   if (pPlotRender_) {
      std::string statustext = pPlotRender_->GetXLabel() + ":" + DoubleToString(X);
      statustext += ";" + pPlotRender_->GetYLabel() + ":" + DoubleToString(Y);
      XRCCTRL(*pToolWindow_, wxT("ID_HISTOGRAM_STATUSBAR"), wxStatusBar)->SetStatusText(
            statustext, 1);
   }
}
} /** namespace ui **/
} /** namespace suri **/
