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
#include "WarpParametersPart.h"
#include "suri/TextFileGcpLoader.h"
#include "suri/TransformationFactory.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/filepicker.h"
#include "wx/pickerbase.h"

// Includes App

// Defines
#define caption_WARP_PARAMETERS "Modelo"
#define ID_FILECTRL "ID_FILECTRL"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SelectionEvent, WarpParametersPart)
IMPLEMENT_EVENT_CALLBACK(OnSelectionChanged, OnSelectionChanged(event),wxCommandEvent)
IMPLEMENT_EVENT_CALLBACK(OnFilePickerChanged, OnFilePickerChanged(event),wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

const std::string WarpParametersPart::ExactAlgorithm = "Exacta";
const std::string WarpParametersPart::PolynomialAlgorithm = "Polinomica";
const std::string WarpParametersPart::TriangulationAlgorithm = "Triangulacion";
const std::string WarpParametersPart::OrderLabel = "Orden";

/** Ctor */
WarpParametersPart::WarpParametersPart(const std::string& FileName, int Flags) :
      PartCollection(wxT("ID_WARP_PARAMETERS_PANEL"), _(caption_WARP_PARAMETERS)),
      flags_(Flags), NEW_EVENT_OBJECT(SelectionEvent),
      fileName_(FileName), parseResult_(false) {
}

/** Dtor */
WarpParametersPart::~WarpParametersPart() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool WarpParametersPart::HasChanged() {
   wxSpinCtrl* pspin = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_POLYNOM_ORDER_SELECTOR_SPINCTRL"),
                               wxSpinCtrl);
   wxFilePickerCtrl* pfilepicker =
         XRCCTRL(*(this->GetWidget()->GetWindow()),
                 wxT(ID_FILECTRL), wxFilePickerCtrl);
   if (pfilepicker) {
      std::string filenamevalue =
            pfilepicker->HasTextCtrl() ? pfilepicker->GetTextCtrl()->GetValue().c_str() : fileName_;
      if (lastFileName_.compare(fileName_) != 0) {
         return true;
      }
   }
   bool exact = flags_ & Exact;
   return modified_ || (!exact && fileName_.empty()) || pspin->GetValue() != lastOrderSelected_;
}

   /** Salva los cambios realizados en el Part. */
bool WarpParametersPart::CommitChanges() {
   wxFilePickerCtrl* pfilepicker = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT(ID_FILECTRL), wxFilePickerCtrl);
   if (pfilepicker)
      fileName_ = pfilepicker->HasTextCtrl() ?
                                 pfilepicker->GetTextCtrl()->GetValue().c_str() :
                                 pfilepicker->GetPath().c_str();
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                  wxT("ID_ALGORITHM_SELECTOR_CHOICE"), wxChoice);
   if (!pchoice)
      return false;

   choicePositionSelected_ = pchoice->GetCurrentSelection();
   algorithmSelected_ =static_cast<char*>(pchoice->GetClientData(choicePositionSelected_));
   wxSpinCtrl* pspin = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_POLYNOM_ORDER_SELECTOR_SPINCTRL"), wxSpinCtrl);
   orderSelected_ = pspin->GetValue();
   modified_ = false;
   lastFileName_ = fileName_;
   lastChoicePositionSelected_ = choicePositionSelected_;
   lastAlgorithmSelected_ = algorithmSelected_;
   lastOrderSelected_ = orderSelected_;
   return true;
}

/** Restaura los valores originales del Part. */
bool WarpParametersPart::RollbackChanges() {

   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_ALGORITHM_SELECTOR_CHOICE"), wxChoice);
   wxSpinCtrl* pspin = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_POLYNOM_ORDER_SELECTOR_SPINCTRL"),
                               wxSpinCtrl);
   if (pchoice && pspin) {
      choicePositionSelected_ = lastChoicePositionSelected_;
      pchoice->Select(choicePositionSelected_);

      orderSelected_ = lastOrderSelected_;
      pspin->SetValue(orderSelected_);
   }

   wxFilePickerCtrl* pfilepicker = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           wxT(ID_FILECTRL), wxFilePickerCtrl);
   if (pfilepicker) {
      pfilepicker->SetPath(lastFileName_.c_str());
   }
   fileName_ = lastFileName_;
   modified_ = false;
   return true;
}

/** Inicializa el part */
void WarpParametersPart::SetInitialValues() {
   wxPanel* ppanel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                                 wxT("ID_LOAD_GCP_FILE"),
                                                 wxPanel);
   if(flags_ & GcpFile)
      ppanel->Show();
   else
      ppanel->Hide();
}

/** Actualiza el estado de la parte */
void WarpParametersPart::Update() {
}

/** Oculta browser de gcps si askGcpFile_=false. */
bool WarpParametersPart::ConfigureWidget() {
   PartCollection::ConfigureWidget();
   wxFilePickerCtrl* pfilepicker = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           ID_FILECTRL, wxFilePickerCtrl);
   pfilepicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED,
                        wxCommandEventHandler(SelectionEvent::OnFilePickerChanged), NULL,
                        pEventHandler_);

   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_ALGORITHM_SELECTOR_CHOICE"), wxChoice);

   wxSpinCtrl* pspin = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_POLYNOM_ORDER_SELECTOR_SPINCTRL"), wxSpinCtrl);

   if (!pchoice)
      return false;
   pchoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                    wxCommandEventHandler(SelectionEvent::OnSelectionChanged), NULL,
                    pEventHandler_);

   pspin->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,
                    wxCommandEventHandler(SelectionEvent::OnSelectionChanged), NULL,
                    pEventHandler_);

   if (flags_ & Exact) {
      pchoice->SetClientData(
            pchoice->Append(ExactAlgorithm.c_str()),
            const_cast<char*>(suri::TransformationFactory::kExact.c_str()));
   }
   else {
      pspin->Enable(true);
   }

   if (flags_ & Polynomial) {
   pchoice->SetClientData(
         pchoice->Append(PolynomialAlgorithm.c_str()),
         const_cast<char*>(suri::TransformationFactory::kPolynomial.c_str()));
   }
   pchoice->Select(0);

   algorithmSelected_ = lastAlgorithmSelected_ = static_cast<char*>(pchoice->GetClientData(0));
   choicePositionSelected_ = lastChoicePositionSelected_ = 0;
   if (!(flags_ & GcpFile) || algorithmSelected_ == suri::TransformationFactory::kExact.c_str()) {
      wxPanel* ppanel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                wxT("ID_LOAD_GCP_FILE"), wxPanel);
      ppanel->Hide();
   }
   orderSelected_ = lastOrderSelected_ = pspin->GetValue();
   if (!fileName_.empty()) {
      wxFilePickerCtrl* pfilepicket = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                              ID_FILECTRL, wxFilePickerCtrl);
      pfilepicket->SetPath(fileName_.c_str());
   }
   lastFileName_ = fileName_;
   modified_ = false;
   return true;
}

/** Devuelve un string con el algoritmo seleccionado **/
std::string WarpParametersPart::GetAlgorithmSelected() const {
   return algorithmSelected_;
}
/**
 * devuelve el orden seleccionado desde el Part por el usuario
 * @return -1 en caso de que sea un orden invalido (ej. triangulacion)
 * @return orden del polinomio deseado
 */
int WarpParametersPart::GetPolynomialOrderSelected() const {
   return orderSelected_;
}

/** Evento para el check de valid data **/
void WarpParametersPart::OnSelectionChanged(wxCommandEvent &Event) {

   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_ALGORITHM_SELECTOR_CHOICE"), wxChoice);
   wxSpinCtrl* pspin = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_POLYNOM_ORDER_SELECTOR_SPINCTRL"), wxSpinCtrl);

   if (pchoice && pspin) {
      lastChoicePositionSelected_ = choicePositionSelected_;
      choicePositionSelected_ = pchoice->GetCurrentSelection();
      lastAlgorithmSelected_ = algorithmSelected_;
      algorithmSelected_ = static_cast<char*>(pchoice->GetClientData(choicePositionSelected_));

      bool isexact = (algorithmSelected_.compare(suri::TransformationFactory::kPolynomial) == 0);

      pspin->Enable(isexact);
      lastOrderSelected_ = orderSelected_;
      orderSelected_ = pspin->GetValue();
      modified_ = true;
   }

}

   /** Metodo que devuelve el filename*/
std::string WarpParametersPart::GetFileName() const {
   return fileName_;
}

bool WarpParametersPart::HasValidData() {
   if ((flags_ & GcpFile) && (fileName_.empty() || !this->parseResult_.isSuccess())) {
      return false;
   }
   return true;
}

void WarpParametersPart::OnFilePickerChanged(wxCommandEvent &Event) {
   //  lastFileName_ = fileName_;
   wxFilePickerCtrl* pfilepicker = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                           ID_FILECTRL, wxFilePickerCtrl);
   fileName_ = pfilepicker->HasTextCtrl() ?
         pfilepicker->GetTextCtrl()->GetValue().c_str() : pfilepicker->GetPath().c_str();
   //Chequeo que sea un archivo de GCPs  válidos
   this->parseResult_ = LoadGCPList();
   if(!this->parseResult_.isSuccess()) {
	   DisplayError();
   }
}

/** Metodo auxiliar que dado un archivo crea una lista de puntos de control */
ParserResult WarpParametersPart::LoadGCPList() {
   TextFileGcpLoader pgcpLoader;
   return pgcpLoader.Load(this->fileName_, this->gcpList_);
}

GcpList& WarpParametersPart::GetGcpList() {
	return this->gcpList_;
}

void WarpParametersPart::DisplayError() {
	SHOW_ERROR(message_GCP_PARSE_FAILED, this->parseResult_.getLineNumber(),
			this->parseResult_.getContent().c_str(), this->parseResult_.getDescription().c_str());
}
} /** namespace suri **/
