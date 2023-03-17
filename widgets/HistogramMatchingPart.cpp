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
#include <sstream>

// Includes Suri
#include "suri/Part.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/RasterElement.h"
#include "HistogramMatchingPart.h"

// Includes wx
#include "wx/listctrl.h"
#include "wx/spinctrl.h"
#include "wx/imaglist.h"
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"

// Includes App
#include "suri/messages.h"
#include "resources.h"


/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(HistogramMatchingPartEvent, HistogramMatchingPart)
   IMPLEMENT_EVENT_CALLBACK(OnUpButton, OnUpButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDownButton, OnDownButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDeleteBandButton, OnDeleteBandButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnAddBandButton, OnAddBandButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChoiceSelected, OnChoiceSelected(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnInputSelection, OnInputSelection(event), wxListEvent)
   IMPLEMENT_EVENT_CALLBACK(OnOutputSelection, OnOutputSelection(event), wxListEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 */
HistogramMatchingPart::HistogramMatchingPart(std::vector<Element*> Selected) :
      Part(false, false), NEW_EVENT_OBJECT(HistogramMatchingPartEvent) {
   windowTitle_ = _(caption_HISTOGRAM_MATCHING);
   selected_ = Selected;
   changed_ = false;
}
/**
 * Destructor
 */
HistogramMatchingPart::~HistogramMatchingPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea la ventana de la parte
 * @return false : si no pudo agregar el elemento XRC
 * @return true : si crea la ventana
 */
bool HistogramMatchingPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_HISTOGRAM_MATCHING_PART"));
   if (!pToolWindow_) {
      return false;
   }
   pChoice_ = XRCCTRL(*pToolWindow_, wxT("ID_CHOICE"), wxChoice);
   pChoice_->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(HistogramMatchingPartEvent::OnChoiceSelected), NULL,
         pEventHandler_);
   pInputBands_ = XRCCTRL(*pToolWindow_, wxT("ID_INPUT_BAND_LIST"), wxListCtrl);
   pOutputBands_ = XRCCTRL(*pToolWindow_, wxT("ID_OUTPUT_BAND_LIST"), wxListCtrl);
   GET_CONTROL(*pToolWindow_, wxT("ID_UP_BUTTON"), wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(HistogramMatchingPartEvent::OnUpButton), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_DOWN_BUTTON"), wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(HistogramMatchingPartEvent::OnDownButton), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_ADD_BUTTON"), wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(HistogramMatchingPartEvent::OnAddBandButton), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_REMOVE_BUTTON"), wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(HistogramMatchingPartEvent::OnDeleteBandButton), NULL,
         pEventHandler_);

   // Conecta a la lista de salida el evento doble click sobre un elemento
   pInputBands_->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                         wxListEventHandler(HistogramMatchingPartEvent::OnInputSelection), NULL,
                         pEventHandler_);

   pOutputBands_->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                          wxListEventHandler(HistogramMatchingPartEvent::OnOutputSelection), NULL,
                          pEventHandler_);

   lastSelection_ = -1;
   return true;
}

/** Maneja el evento de doble click sobre un elemento de la lista de bandas de entrada */
void HistogramMatchingPart::OnInputSelection(wxCommandEvent &Event) {
   OnAddBandButton(Event);
}

/** Maneja el evento de doble click sobre un elemento de la lista de bandas de salida */
void HistogramMatchingPart::OnOutputSelection(wxCommandEvent &Event) {
   OnDeleteBandButton(Event);
}

/**
 * Devuelve el estado de la parte
 * @return true cuando la parte esta habilitada
 * @return false cuado la parte esta desabilitada
 */
bool HistogramMatchingPart::IsEnabled() {
   return enabled_;
}

/**
 * Informa si la parte tiene cambios
 * @return true tiene cambios
 * @return false no tiene cambios
 */
bool HistogramMatchingPart::HasChanged() {
   if (pChoice_->GetSelection() != -1){
      changed_ = true;
      return changed_;
   }
   return changed_;
}


std::vector<std::pair<wxString, wxString> > HistogramMatchingPart::GetBands() {
   std::vector < std::pair<wxString, wxString> > bands;
   for (int row = 0; row < pOutputBands_->GetItemCount(); row++) {
      wxString bandName = GetItemSelection(pOutputBands_, 2, row);
      wxString bandNumber = GetItemSelection(pOutputBands_, 1, row);
      bands.push_back(std::pair<wxString, wxString>(bandNumber, bandName));
   }
   return bands;
}

/**
 * Maneja el evento de click sobre el boton que modifica el ordenamiento de las bandas
 * @param[in] Event evento de click sobre el boton
 */
void HistogramMatchingPart::OnDownButton(wxCommandEvent &Event) {
   int imageindex = -1;
   int itemcount = pOutputBands_->GetItemCount();
   std::vector < std::pair<wxString, wxString> > bands = GetBands();
   imageindex = pOutputBands_->GetNextItem (-1, wxLIST_NEXT_ALL,
                                                            wxLIST_STATE_SELECTED);

   if ( (imageindex == itemcount - 1) || (imageindex == -1) ) return ;
   pOutputBands_->DeleteAllItems();
   for (int row = 0; row < imageindex; row++) {
      AddBand(bands[row].first, bands[row].second);
   }
   AddBand(bands[imageindex + 1].first, bands[imageindex +1].second);
   AddBand(bands[imageindex].first, bands[imageindex].second);
   for (int row = imageindex + 2; row < itemcount; row++) {
      AddBand(bands[row].first, bands[row].second);
   }
}

/**
 * Maneja el evento de click sobre el boton que modifica el ordenamiento de las bandas
 * @param[in] Event evento de click sobre el boton
 */
void HistogramMatchingPart::OnUpButton(wxCommandEvent &Event) {
   int imageindex = -1;
   int itemcount = pOutputBands_->GetItemCount();
   std::vector < std::pair<wxString, wxString> > bands = GetBands();
   imageindex = pOutputBands_->GetNextItem (-1, wxLIST_NEXT_ALL,
                                                            wxLIST_STATE_SELECTED);
   if ( (imageindex == 0) || (imageindex == -1)) return;
   pOutputBands_->DeleteAllItems();
   for (int row = 0; row < imageindex - 1; row++) {
      AddBand(bands[row].first, bands[row].second);
   }
   AddBand(bands[imageindex].first, bands[imageindex].second);
   AddBand(bands[imageindex - 1].first, bands[imageindex - 1].second);
   for (int row = imageindex + 1; row < itemcount; row++) {
      AddBand(bands[row].first, bands[row].second);
   }
}

/** Agrega la banda a la lista pOutputBands_, si ya se encuentra no hace nada */
void HistogramMatchingPart::AddBand(const wxString& BandNumber, const wxString& BandName) {
   int newItemPosition = pOutputBands_->GetItemCount();
   for (int row = 0; row < pOutputBands_->GetItemCount(); row++) {
      wxString bandName = GetItemSelection(pOutputBands_, 2, row);
      if (bandName == BandName) return;
   }
   pOutputBands_->InsertItem(newItemPosition , _(""));
   pOutputBands_->SetItem(newItemPosition, 1, BandNumber);
   pOutputBands_->SetItem(newItemPosition, 2, BandName);
}

/** Elimina una banda de la lista de bandas de salida */
void HistogramMatchingPart::DeleteBand(const wxString& BandName) {
   for (int row = 0; row < pOutputBands_->GetItemCount(); row++) {
      wxString bandName = GetItemSelection(pOutputBands_, 2, row);
      if (bandName == BandName) {
         pOutputBands_->DeleteItem(row);
      }
   }
}

/**
 * Maneja el evento de click sobre el boton que modifica las bandas de salida
 * @param[in] Event evento de click sobre un boton
 */
void HistogramMatchingPart::OnDeleteBandButton(wxCommandEvent &Event){
   wxString bandName = GetItemSelection(pOutputBands_, 2);
   DeleteBand(bandName);
}

/**
 * Maneja el evento de click sobre el boton que modifica las bandas de entrada
 * @param[in] Event evento de click sobre un boton
 */
void HistogramMatchingPart::OnAddBandButton(wxCommandEvent &Event){
   wxString bandNumber = GetItemSelection(pInputBands_, 1);
   wxString bandName = GetItemSelection(pInputBands_, 2);
   AddBand(bandNumber, bandName);
}

/** refresca la lista de bandas disponibles **/
void HistogramMatchingPart::OnChoiceSelected(wxCommandEvent &Event) {
   int selected = pChoice_->GetSelection();
   if (lastSelection_ == selected)
      return;
   lastSelection_ = selected;
   pInputBands_->DeleteAllItems();
   pOutputBands_->DeleteAllItems();
   RasterElement* pRasterElement = dynamic_cast<RasterElement*>(selected_[selected]);
   int rowIndex = 0;
   std::stringstream ss;
   for (int bands = 0; bands < pRasterElement->GetBandCount(); bands++) {
      ss.str("");
      std::string bandName = "";
      pRasterElement->GetBandName(bandName, bands);
      pInputBands_->InsertItem(rowIndex, _(""));
      ss << rowIndex + 1;
      pInputBands_->SetItem(rowIndex, 1, ss.str());
      pInputBands_->SetItem(rowIndex, 2, bandName);
      rowIndex++;
   }
   pInputBands_->SetColumnWidth(1, wxLIST_AUTOSIZE);
}

/**
 * Si los datos de los controles wx son validos los guarda en las variables.
 * Si el archivo existe confirma sobreescritura.
 * Si no son validos muestra el mensaje de error que devuelve HasValidData.
 */
bool HistogramMatchingPart::CommitChanges() {
   if (!HasValidData()) {
      REPORT_ERROR(errorMessage_);
      return false;
   }
   if (pPal_) {
      std::vector<int> outputBands;
      int pos = pChoice_->GetSelection();
      std::vector<Element*> input;
      input.push_back(selected_[0]);
      input.push_back(selected_[pos + 1]);
      pPal_->AddAttribute< std::vector<Element*> >
               (ProcessAdaptLayer::HistogramMatchingInputAttr, input);
      std::vector<int> outputbands = GetOutputBands();
      pPal_->AddAttribute< std::vector<int> >(
            ProcessAdaptLayer::HistogramMatchingOutputBandsAttr, outputbands);
   }
   return true;
}

std::vector<int> HistogramMatchingPart::GetOutputBands() {
   std::vector<int> outputbands;
   //std::stringstream ss;
   std::vector<std::pair<wxString, wxString> > out = GetBands();
   std::vector<std::pair<wxString, wxString> >::const_iterator it = out.begin();
   for (; it != out.end(); ++it) {
      std::stringstream ss(std::string((*it).first));
      int bandnumber = -1;
      ss >> bandnumber ;
      // resto uno para que coincida con el valor del banda del xml
      bandnumber--;
      outputbands.push_back(bandnumber);
   }
   return outputbands;
}

wxString HistogramMatchingPart::GetItemSelection(wxListCtrl* pList, int Column, int Row) const{
   int imageindex = Row;
   if (imageindex == -1){
      imageindex = pList->GetNextItem (-1, wxLIST_NEXT_ALL,
                                                         wxLIST_STATE_SELECTED);
   }
   if ( (Column > pList->GetColumnCount()) || (imageindex == -1) ) return  "";
   wxListItem item;
   item.SetId(imageindex);
   item.SetColumn(Column);
   item.SetMask(wxLIST_MASK_TEXT);
   pList->GetItem(item);
   return item.GetText();
}

/** Restaura los valores originales del Part. */
bool HistogramMatchingPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void HistogramMatchingPart::SetInitialValues() {
   if (selected_.size() == 0)
      return;
   modified_ = false;
   wxBitmap imageband;
   GET_BITMAP_RESOURCE(wxT(icon_IMAGE_BAND), imageband);
     // Creo la lista de imagenes para la lista de entrada
   wxImageList *pimageinlist = new wxImageList(16, 16);
   pimageinlist->Add(imageband);
   wxImageList *pimageinlist2 = new wxImageList(16, 16);
   pimageinlist2->Add(imageband);
   std::vector<Element*>::const_iterator it = selected_.begin();
   ++it;
   for (; it!= selected_.end(); ++it) {
      pChoice_->Append((*it)->GetName());
   }
   pInputBands_->AssignImageList(pimageinlist, wxIMAGE_LIST_SMALL);
   pOutputBands_->AssignImageList(pimageinlist2, wxIMAGE_LIST_SMALL);

   InitializeList(pInputBands_);
   InitializeList(pOutputBands_);

   pChoice_->SetSelection(0);
   int selected = pChoice_->GetSelection();
   if (lastSelection_ == selected)
      return;
   lastSelection_ = selected;
   pInputBands_->DeleteAllItems();
   pOutputBands_->DeleteAllItems();
   RasterElement* pRasterElement = dynamic_cast<RasterElement*>(selected_[selected]);
   int rowIndex = 0;
   std::stringstream ss;
   for (int bands = 0; bands < pRasterElement->GetBandCount(); bands++) {
      ss.str("");
      std::string bandName = "";
      pRasterElement->GetBandName(bandName, bands);
      pInputBands_->InsertItem(rowIndex, _(""));
      ss << rowIndex + 1;
      pInputBands_->SetItem(rowIndex, 1, ss.str());
      pInputBands_->SetItem(rowIndex, 2, bandName);
      rowIndex++;
   }
   pInputBands_->SetColumnWidth(1, wxLIST_AUTOSIZE);
}

/**
 * Inicializa la lista pasada por parametro
 * @param[in] pList puntero a la lista
 */
void HistogramMatchingPart::InitializeList(wxListCtrl* pList) {
   pList->InsertColumn(0, _(""));
   pList->InsertColumn(1, wxT("#"));
   pList->InsertColumn(2, wxT("Nombre"));
   pList->SetColumnWidth(0,16);
   pList->SetColumnWidth(1,20);
   pList->SetColumnWidth(2,150);
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void HistogramMatchingPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_SPECTRAL_SELECTION, ToolBitmap);
}

/** Devuelve si la parte tiene datos validos */
bool HistogramMatchingPart::HasValidData() {
   return true;
}

}
