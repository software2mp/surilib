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

#include <vector>

#include "SharpeningInputElementsPart.h"
#include "suri/Element.h"
#include "suri/Part.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"
#include "suri/Widget.h"

#include "wx/choice.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(SharpeningInputElementsPartEvent, SharpeningInputElementsPart)
   IMPLEMENT_EVENT_CALLBACK(OnRGBChoiceSelected, OnRGBChoiceSelected(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnPanChoiceSelected, OnPanChoiceSelected(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
SharpeningInputElementsPart::SharpeningInputElementsPart(bool Enable, bool Modified) :
      Part(wxT("ID_SHARPENING_ELEMENTS_PART"), _(caption_SHARPENING_INPUT_ELEMENTS),
           Enable, Modified),
      NEW_EVENT_OBJECT(SharpeningInputElementsPartEvent), changed_(false) {

}

/**
 * Dtor.
 */
SharpeningInputElementsPart::~SharpeningInputElementsPart() {

}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool SharpeningInputElementsPart::HasChanged() {
   return changed_ || !HasValidData();
}

/**
 * Salva los cambios realizados en el Part.
 */
bool SharpeningInputElementsPart::CommitChanges() {
//   pPal_->AddAttribute<int>(ProcessAdaptLayer::InputRGBElement, rgbix_);
//   pPal_->AddAttribute<int>(ProcessAdaptLayer::InputPanElement, panix_);
   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool SharpeningInputElementsPart::RollbackChanges() {
   changed_ = false;
   return true;
}

/**
 * Inicializa el part.
 */
void SharpeningInputElementsPart::SetInitialValues() {
   InitializeChoices();
   InitializeChoicesContent();
}

/**
 * Devuelve si la parte tiene datos validos.
 */
bool SharpeningInputElementsPart::HasValidData() {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHARPENING_ELEMENTS_RGB_CHOICE"), wxChoice);

   int rgbix = *(static_cast<int*>(pchoice->GetClientData(pchoice->GetSelection())));

   std::vector<Element*> inputelements;
   pPal_->GetAttribute<std::vector<Element*> >(ProcessAdaptLayer::InputElementsKeyAttr,
                                               inputelements);

   RasterElement* praster = dynamic_cast<RasterElement*>(inputelements[rgbix]);
   if (praster && praster->GetBandCount() >= 3) {
      return true;
   }

   return false;
}

/**
 * Actualiza el estado de la parte.
 */
void SharpeningInputElementsPart::Update() {

}

/**
 * Inicializa los combos.
 */
void SharpeningInputElementsPart::InitializeChoices() {
   wxChoice* prgb = XRCCTRL(*(this->GetWidget()->GetWindow()),
                            wxT("ID_SHARPENING_ELEMENTS_RGB_CHOICE"), wxChoice);
   wxChoice* ppan = XRCCTRL(*(this->GetWidget()->GetWindow()),
                            wxT("ID_SHARPENING_ELEMENTS_PAN_CHOICE"), wxChoice);

   prgb->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(SharpeningInputElementsPartEvent::OnRGBChoiceSelected),
         NULL, pEventHandler_);

   ppan->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(SharpeningInputElementsPartEvent::OnPanChoiceSelected),
         NULL, pEventHandler_);
}

/**
 * Carga los combos.
 */
void SharpeningInputElementsPart::InitializeChoicesContent() {
   wxChoice* prgb = XRCCTRL(*(this->GetWidget()->GetWindow()),
                            wxT("ID_SHARPENING_ELEMENTS_RGB_CHOICE"), wxChoice);
   wxChoice* ppan = XRCCTRL(*(this->GetWidget()->GetWindow()),
                            wxT("ID_SHARPENING_ELEMENTS_PAN_CHOICE"), wxChoice);

   std::vector<Element*> inputelements;
   pPal_->GetAttribute<std::vector<Element*> >(ProcessAdaptLayer::InputElementsKeyAttr,
                                               inputelements);

   for (int ix = 0, lenix = inputelements.size(); ix < lenix; ++ix) {
      int lastindexrgb = prgb->Append(inputelements[ix]->GetName());
      int lastindexpan = ppan->Append(inputelements[ix]->GetName());
      int* pix = new int; *pix = ix;
      prgb->SetClientData(lastindexrgb, pix);
      ppan->SetClientData(lastindexpan, pix);
   }

   prgb->Select(0);
   ppan->Select(1);

   rgbix_ = *(static_cast<int*>(prgb->GetClientData(prgb->GetSelection())));
   panix_ = *(static_cast<int*>(ppan->GetClientData(prgb->GetSelection())));
}

/**
 * Handler para el evento de seleccion de item del choice
 * de seleccion del elemento para composicion RGB.
 */
void SharpeningInputElementsPart::OnRGBChoiceSelected(wxCommandEvent &Event) {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHARPENING_ELEMENTS_RGB_CHOICE"), wxChoice);

   rgbix_ = *(static_cast<int*>(pchoice->GetClientData(pchoice->GetSelection())));

   changed_ = true;
}

/**
 * Handler para el evento de seleccion de item del choice
 * de seleccion del elemento pancromatico.
 */
void SharpeningInputElementsPart::OnPanChoiceSelected(wxCommandEvent &Event) {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHARPENING_ELEMENTS_PAN_CHOICE"), wxChoice);

   panix_ = *(static_cast<int*>(pchoice->GetClientData(pchoice->GetSelection())));

   changed_ = true;
}

} /** namespace suri */
