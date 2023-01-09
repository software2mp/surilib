/*! \file ManualEnhancementPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>
#include <set>
#include <vector>
#include <memory>

#include "suri/AuxiliaryFunctions.h"
#include "suri/ManualEnhancementPart.h"
#include "suri/RasterElement.h"
#include "RasterLayer.h"
#include "suri/StatisticsCalculator.h"
#include "suri/PlotterWidget.h"
#include "suri/wx2DPlotter.h"
#include "resources.h"
#include "SREEnhancementFactory.h"
#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"
#include "SREEnhancementUtils.h"
#include "suri/xmlnames.h"

#include "wx/choice.h"
#include "wx/textctrl.h"

namespace suri {

namespace ui {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ManualEnhancementEventHandler, ManualEnhancementPart)
   IMPLEMENT_EVENT_CALLBACK(OnMinValueChange, OnMinValueChange(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMaxValueChange, OnMaxValueChange(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResetButtonClicked, OnResetButtonClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
ManualEnhancementPart::ManualEnhancementPart(RasterLayer* pRaster) :
      HistogramVisualizationPart(pRaster),
      pEnhancementHandler_(new ManualEnhancementEventHandler(this)) {
   windowTitle_ = _("Realce Interactivo");
   useCombination_ = true;
}

ManualEnhancementPart::~ManualEnhancementPart() {
   delete pEnhancementHandler_;
}

/** Indica si el Part tiene cambios para salvar. */
bool ManualEnhancementPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool ManualEnhancementPart::CommitChanges() {
   HistogramVisualizationPart::CommitChanges();
   // actualiza los max y min del histograma en funcion de lo
   // editado por el usuario
   raster::data::HistogramBase* pcustomhistogram = NULL;
   suri::raster::data::StatisticsCalculator calculator(
         dynamic_cast<RasterElement*>(pRaster_->GetElement()));
   calculator.CalculateHistogram(pStats_, &pcustomhistogram, false, currentMins_,
                                 currentMaxs_);
   // Configuracion de realce
   double* pmin = new double[pcustomhistogram->GetBandCount()];
   double* pmax = new double[pcustomhistogram->GetBandCount()];
   double* pmean = new double[pcustomhistogram->GetBandCount()];
   double* pstdev = new double[pcustomhistogram->GetBandCount()];

   for (int ix = 0, lenix = pcustomhistogram->GetBandCount(); ix < lenix; ++ix) {
      pmin[ix] = pcustomhistogram->GetMin()[ix];
      pmax[ix] = pcustomhistogram->GetMax()[ix];
      pmean[ix] = pStats_->GetMean(ix);
      pstdev[ix] = pStats_->GetStdev(ix);
   }

   std::auto_ptr<suri::ParameterCollection> params(new suri::ParameterCollection());

   params.get()->AddValue<int>("BandCount", pcustomhistogram->GetBandCount());
   params.get()->AddValue<int*>("NumBins", pcustomhistogram->GetNumBins());
   params.get()->AddValue<double**>("BAFP",
                                    pHistogram_->GetBinAccumFrequencyPercentage());
   params.get()->AddValue<int*>("AccumFreq", pcustomhistogram->GetBandAccumFrequency());
   params.get()->AddValue<double*>("Min", pmin);
   params.get()->AddValue<double*>("Max", pmax);
   params.get()->AddValue<double*>("Mean", pmean);
   params.get()->AddValue<double*>("Stdev", pstdev);

   namespace SRE = suri::raster::enhancement;
   // Obtiene la lista de realcesi
   std::vector < std::string > enhancements =
         SRE::EnhancementFactory::GetInstance()->GetAvailableEnhancements();

   std::string lutname = enhancements[XRCCTRL(*pToolWindow_, "ID_ENHANCEMENT_CHOICE",
                                              wxChoice)->GetSelection()];

   SRE::Enhancement* pEnhancement = SRE::EnhancementFactory::GetInstance()->Create(
         lutname, *params.get());

   int** pLut = pEnhancement->CreateLUT(pcustomhistogram->GetBins());
   wxXmlNode* pnewlutnode = NULL;
   SRE::utils::LutToXml(pnewlutnode, true, pcustomhistogram->GetBandCount(), lutname,
                        pHistogram_->GetNumBins(), pcustomhistogram->GetMin(),
                        pcustomhistogram->GetMax(), pLut);

   Element* pelement = pRaster_->GetElement();
   wxXmlNode* prendernode = pelement->GetNode(RENDERIZATION_NODE);
   // reemplazo el nodo de lut para realce
   wxXmlNode* plastenhancement = pelement->GetNode(
   RENDERIZATION_NODE NODE_SEPARATION_TOKEN ENHANCEMENT_NODE);
   if (!pelement->ReplaceNode(prendernode, pnewlutnode, plastenhancement)) {
      // si no lo reemplaza entonces lo agrego
      wxXmlNode* pnode = prendernode->GetChildren();
      wxXmlNode* plastnode = NULL;
      while (pnode != NULL) {
         if (pnode->GetName().compare(CANVAS_CACHE_NODE) == 0) {
            plastnode = pnode;
            break;
         } else if (pnode->GetName().compare(BAND_COMBINATION_NODE) == 0) {
            plastnode = pnode;
         }
         pnode = pnode->GetNext();
      }
      prendernode->InsertChildAfter(pnewlutnode, plastnode);
   }

   delete[] pmin;
   delete[] pmax;
   delete[] pmean;
   delete[] pstdev;

   for (int ix = 0, lenix = pcustomhistogram->GetBandCount(); ix < lenix; ++ix) {
      delete[] pLut[ix];
      pLut[ix] = NULL;
   }

   delete[] pLut;
   pLut = NULL;

   pelement->SetChanged();
   pelement->SendViewerUpdate();
   maxs_ = currentMaxs_;
   mins_ = currentMins_;
   modified_ = false;
   return true;
}

/** Restaura los valores originales del Part. */
bool ManualEnhancementPart::RollbackChanges() {
   /** minimos y maximos sin cambios **/
   HistogramVisualizationPart::RollbackChanges();
   SetInitialValues();
   return true;
}

/** Inicializa el part */
void ManualEnhancementPart::SetInitialValues() {

   HistogramVisualizationPart::SetInitialValues();

   namespace SRE = suri::raster::enhancement;
   XRCCTRL(*pToolWindow_, "ID_ENHANCEMENT_CHOICE", wxChoice)->Clear();
   // Obtiene la lista de realcesi
   std::vector < std::string > enhancements =
         SRE::EnhancementFactory::GetInstance()->GetAvailableEnhancements();

   // Carga los realces en la propiedad
   std::vector<std::string>::const_iterator it;
   for (it = enhancements.begin(); it != enhancements.end(); ++it)
      XRCCTRL(*pToolWindow_, "ID_ENHANCEMENT_CHOICE", wxChoice)->Append(
            _((*it).c_str()));

   XRCCTRL(*pToolWindow_, "ID_ENHANCEMENT_CHOICE", wxChoice)->SetSelection(0);
   modified_ = false;
}

/** Inicializa la ventana y sus componentes **/
bool ManualEnhancementPart::CreateToolWindow() {
   if (!HistogramVisualizationPart::CreateToolWindow()) return false;
   GET_CONTROL(*(this->GetWidget()->GetWindow()), "ID_RESET_BTN",
         wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(ManualEnhancementEventHandler::OnResetButtonClicked),
         NULL, pEnhancementHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_MIN_TXT", wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(ManualEnhancementEventHandler::OnMinValueChange), NULL,
         pEnhancementHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_MAX_TXT", wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(ManualEnhancementEventHandler::OnMaxValueChange), NULL,
         pEnhancementHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_ENHANCEMENT_CONFIG_PANEL", wxPanel)->Show();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_INVERT_MAX_MIN_CHECK", wxCheckBox)->Show();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_MIN_TXT", wxTextCtrl)->Enable();
   GET_CONTROL(*(this->GetWidget()->GetWindow()),"ID_MAX_TXT", wxTextCtrl)->Enable();
   return true;
}

/** Metodo que se llama cuando se cambia el minimo del histograma **/
void ManualEnhancementPart::OnMinValueChange(wxCommandEvent& Event) {
   int bandsel = GetSelectedBandIndex();
   double min = StringToNumber<double>(
         XRCCTRL(*pToolWindow_, wxT("ID_MIN_TXT"), wxTextCtrl)->GetValue().c_str());
   if (min < pHistogram_->GetMin()[bandsel]) {
      SHOW_ERROR("No es posible configurar un valor fuera del rango del histograma");
      XRCCTRL(*pToolWindow_, wxT("ID_MIN_TXT"), wxTextCtrl)->ChangeValue(
            NumberToString<double>(currentMins_[bandsel]));
      return;
   }
	currentMins_[bandsel] = min;
	pPlotRender_->SetMinValue(min);
	pPlotter_->GetWindow()->Refresh();
	pPlotter_->GetWindow()->Update();
	pToolWindow_->Layout();
	pToolWindow_->Refresh();
	pToolWindow_->Update();
	modified_ = true;
}

/** Metodo que se llama cuando se cambia el maximo del histograma **/
void ManualEnhancementPart::OnMaxValueChange(wxCommandEvent& Event) {	
   int bandsel = GetSelectedBandIndex();
   double max = StringToNumber<double>(
         XRCCTRL(*pToolWindow_, wxT("ID_MAX_TXT"), wxTextCtrl)->GetValue().c_str());
   if (max > pHistogram_->GetMax()[bandsel]) {
      SHOW_ERROR("No es posible configurar un valor fuera del rango del histograma");
      XRCCTRL(*pToolWindow_, wxT("ID_MAX_TXT"), wxTextCtrl)->ChangeValue(
            NumberToString<double>(currentMaxs_[bandsel]));
      return;
   }
   currentMaxs_[bandsel] = max;
	pPlotRender_->SetMaxValue(max);		
	pPlotter_->GetWindow()->Refresh();
	pPlotter_->GetWindow()->Update();
	pToolWindow_->Layout();
	pToolWindow_->Refresh();
	pToolWindow_->Update();
	modified_ = true;
}

/** Metodo polimorfico que se llama cuando se cambiaron los limites del histograma sobre la banda activa**/
void ManualEnhancementPart::HistogramMaxChange() {
	int bandsel = GetSelectedBandIndex();
   XRCCTRL(*pToolWindow_, wxT("ID_MAX_TXT"), wxTextCtrl)->ChangeValue(
         NumberToString<double>(pPlotRender_->GetMax()));
   currentMaxs_[bandsel] = pPlotRender_->GetMax();
	pPlotter_->GetWindow()->Refresh();
	pPlotter_->GetWindow()->Update();
	pToolWindow_->Layout();
	pToolWindow_->Refresh();
	pToolWindow_->Update();
	modified_ = true;
}

void ManualEnhancementPart::HistogramMinChange() {
	int bandsel = GetSelectedBandIndex();
   XRCCTRL(*pToolWindow_, wxT("ID_MIN_TXT"), wxTextCtrl)->ChangeValue(
         NumberToString<double>(pPlotRender_->GetMin()));
   currentMins_[bandsel] = pPlotRender_->GetMin();
	pPlotter_->GetWindow()->Refresh();
	pPlotter_->GetWindow()->Update();
	pToolWindow_->Layout();
	pToolWindow_->Refresh();
	pToolWindow_->Update();
	modified_ = true;
}
/** Metodo que se llama cuando se presiona el boton para reiniciar los cambios del histograma **/
void ManualEnhancementPart::OnResetButtonClicked(wxCommandEvent& Event) {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_BAND_SELECTION_CHOICE"),
                                    wxChoice);
   int bandsel = pchoice->GetSelection();
   SetInitialValues();
   currentMins_ = mins_;
   currentMaxs_ = maxs_;
   pchoice->SetSelection(bandsel);
   XRCCTRL(*pToolWindow_, wxT("ID_MIN_TXT"), wxTextCtrl)->ChangeValue(
         DoubleToString(mins_[bandsel]));
   XRCCTRL(*pToolWindow_, wxT("ID_MAX_TXT"), wxTextCtrl)->ChangeValue(
         DoubleToString(maxs_[bandsel]));
   Refresh();
}

/** Retorna el icono de la herramienta */
void ManualEnhancementPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_MANUAL_ENHANCEMENT_32, ToolBitmap);
}

/** metodo auxiliar que obtiene el indice de la banda seleccionada en el choice **/
int ManualEnhancementPart::GetSelectedBandIndex() {
   wxChoice* pchoice = XRCCTRL(*pToolWindow_, wxT("ID_BAND_SELECTION_CHOICE"),
                               wxChoice);
   std::string bselname = pchoice->GetStringSelection().c_str();
   int bandsel = pchoice->GetSelection();
   for (int i = 0; i < pRaster_->GetBandCount(); ++i) {
      std::string bandname;
      pRaster_->GetBandName(bandname, i);
      if (bandname.compare(bselname) == 0) {
         bandsel = i;
         break;
      }
   }
   return bandsel;
}
} /** namespace ui **/
} /** namespace suri **/
