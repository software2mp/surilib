/*! \file SharpeningInputElementsPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
