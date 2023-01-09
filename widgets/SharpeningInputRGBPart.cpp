/*! \file SharpeningInputRGBPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <sstream>
#include <vector>
#include <limits>

#include "SharpeningInputRGBPart.h"

#include "MultiLayerBandSelectionPart.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Element.h"
#include "suri/Part.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"
#include "suri/Widget.h"

#include "resources.h"

#include "wx/bitmap.h"
#include "wx/checkbox.h"
#include "wx/dcclient.h"
#include "wx/icon.h"
#include "wx/imaglist.h"
#include "wx/listctrl.h"
#include "wx/textctrl.h"
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// Tamanio de las imagenes de las listas
#define LIST_IMAGE_SIZE 16

// Posicion de la columna en la lista
#define IMAGE_COLUMN 0

// Posicion de la columna en la lista
#define INPUT_BAND_NAME_COLUMN 1

// Ancho de la columna de imagen en pixeles
#define IMAGE_COLUMN_EXTRA_WIDTH 10

// Ancho de la columna con el nombre de la banda
#define BANDNAME_COLUMN_WIDTH 256

#define RGB_RED_POS     0
#define RGB_GREEN_POS   1
#define RGB_BLUE_POS    2

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(SharpeningInputRGBPartEvent, SharpeningInputRGBPart)
   IMPLEMENT_EVENT_CALLBACK(OnRedRadioClicked, OnRedRadioClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnGreenRadioClicked, OnGreenRadioClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnBlueRadioClicked, OnBlueRadioClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnEnableNDVCheckboxClicked, OnEnableNDVCheckboxClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
SharpeningInputRGBPart::SharpeningInputRGBPart(RasterElement* pElement, bool Enable,
                                               bool Modified) :
      Part(Enable, Modified), NEW_EVENT_OBJECT(SharpeningInputRGBPartEvent),
      pElement_(pElement) {
   windowTitle_ = _(caption_SHARPENING_INPUT_RGB);
   outputRGBBands_.resize(3);
   changed_ = false;
   rgbix_ = -1;
}

/**
 * Dtor.
 */
SharpeningInputRGBPart::~SharpeningInputRGBPart() {

}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool SharpeningInputRGBPart::HasChanged() {
   return changed_ || !HasValidData();
}

/**
 * Salva los cambios realizados en el Part.
 */
bool SharpeningInputRGBPart::CommitChanges() {
   pPal_->AddAttribute<std::vector<BandInfo> >(ProcessAdaptLayer::OutputRGBBands, outputRGBBands_);

   wxCheckBox* pcheck = XRCCTRL(*pToolWindow_,
                                wxT("ID_SHAPENING_INPUT_RGB_ENABLE_NDV_CHECKBOX"),
                                wxCheckBox);

   if (pcheck && pcheck->IsChecked()) {
      bool checked = true;
      pPal_->AddAttribute<bool>(ProcessAdaptLayer::OutputRGBNVDAvailable, checked);

      wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_SHAPENING_INPUT_RGB_NDV_TEXT"),
                                  wxTextCtrl);

      double val = StringToNumber<double>(ptext->GetValue().mb_str());
      pPal_->AddAttribute<double>(ProcessAdaptLayer::OutputRGBNVD, val);
   } else {
      bool checked = false;
      pPal_->AddAttribute<bool>(ProcessAdaptLayer::OutputRGBNVDAvailable, checked);
   }

   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool SharpeningInputRGBPart::RollbackChanges() {
   changed_ = false;
   return true;
}

/**
 * Inicializa el part.
 */
void SharpeningInputRGBPart::SetInitialValues() {
   InitializeRadioButtons();
}

/**
 * Devuelve si la parte tiene datos validos.
 */
bool SharpeningInputRGBPart::HasValidData() {
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHAPENING_INPUT_RGB_RED_TEXT"), wxTextCtrl);
   if (ptext->GetValue().Len() == 0) return false;

   ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                   wxT("ID_SHAPENING_INPUT_RGB_GREEN_TEXT"), wxTextCtrl);
   if (ptext->GetValue().Len() == 0) return false;

   ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                   wxT("ID_SHAPENING_INPUT_RGB_BLUE_TEXT"), wxTextCtrl);
   if (ptext->GetValue().Len() == 0) return false;

   return true;
}

/**
 * Actualiza el estado de la parte.
 */
void SharpeningInputRGBPart::Update() {
//   int rgbix = 0;
//   pPal_->GetAttribute<int>(ProcessAdaptLayer::InputRGBElement, rgbix);
//
//   if (rgbix_ != rgbix) {
//      InitializeElement();
//      InitializeTextBoxes();
//   }
}

/**
 * Crea la ventana de la parte
 * @return true : si pudo cargar la parte
 * @return false : si no pudo cargar la parte
 */
bool SharpeningInputRGBPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_SHAPENING_INPUT_RGB_PART"));

   wxPanel* pcttpanel = XRCCTRL(*pToolWindow_, wxT("ID_SHAPENING_INPUT_CONTAINER_PANEL"), wxPanel);

   // Agrego y configuro el part para seleccionar las bandas de origen.
   std::vector<Element*> elements;
   pPal_->GetAttribute<std::vector<Element*> >(ProcessAdaptLayer::InputElementsKeyAttr,
                                               elements);
   std::vector<RasterElement*> rasters = GetLowResRasterElements(elements);

   MultiLayerBandSelectionPart* ppart = new MultiLayerBandSelectionPart(rasters);

   ppart->CreatePartTool(pcttpanel);
   ppart->SetInputBandListener(this);

   if (!AddControl(ppart, pcttpanel, ControlArrangement(1, 0))) return false;

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         wxT("ID_SHAPENING_INPUT_RGB_ENABLE_NDV_CHECKBOX"), wxCheckBox)->Connect(
         wxEVT_COMMAND_CHECKBOX_CLICKED,
         wxCommandEventHandler(SharpeningInputRGBPartEvent::OnEnableNDVCheckboxClicked),
         NULL, pEventHandler_);
   return true;
}

/**
 * Retorna el icono de la herramienta.
 */
void SharpeningInputRGBPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int) const {
   GET_BITMAP_RESOURCE(icon_RASTER_RGB_SPECTRAL_SELECTION, ToolBitmap);
}

/**
 * Handler para el evento de click del boton de la componente roja.
 */
void SharpeningInputRGBPart::OnRedRadioClicked(wxCommandEvent &Event) {

}

/**
 * Handler para el evento de click del boton de la componente roja.
 */
void SharpeningInputRGBPart::OnGreenRadioClicked(wxCommandEvent &Event) {

}

/**
 * Handler para el evento de click del boton de la componente roja.
 */
void SharpeningInputRGBPart::OnBlueRadioClicked(wxCommandEvent &Event) {

}

/**
 * Handler para el evento de click del checkbox para poder ingresar el
 * valor digital no valido.
 */
void SharpeningInputRGBPart::OnEnableNDVCheckboxClicked(wxCommandEvent &Event) {
   wxCheckBox* pcheck = XRCCTRL(*pToolWindow_,
                                wxT("ID_SHAPENING_INPUT_RGB_ENABLE_NDV_CHECKBOX"),
                                wxCheckBox);

   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_SHAPENING_INPUT_RGB_NDV_TEXT"),
                               wxTextCtrl);

   if (pcheck && ptext) {
      if (pcheck->IsChecked())
         ptext->Enable();
      else
         ptext->Disable();
   }
}

/**
 * Metodo a implementar por la clase que recibe las notificaciones
 * para recibir la lista de bandas seleccionadas.
 */
void SharpeningInputRGBPart::OnBandSelected(std::vector<BandInfo> BandIxsP) {
   if (BandIxsP.empty())
      return;
   int radio = GetSelectedRadioButtonPos();
   outputRGBBands_[radio] = BandIxsP[0];

   std::string textname;
   std::string nextradio;

   switch (radio) {
      case kRedRadio:
         textname = "ID_SHAPENING_INPUT_RGB_RED_TEXT";
         nextradio = "ID_SHAPENING_INPUT_RGB_GREEN_RADIO";
         break;

      case kGreenRadio:
         textname = "ID_SHAPENING_INPUT_RGB_GREEN_TEXT";
         nextradio = "ID_SHAPENING_INPUT_RGB_BLUE_RADIO";
         break;

      case kBlueRadio:
         textname = "ID_SHAPENING_INPUT_RGB_BLUE_TEXT";
         nextradio = "ID_SHAPENING_INPUT_RGB_RED_RADIO";
         break;

      default: // no deberia pasar esto pero lo ponemos por buenas practicas.
         textname = "";
         nextradio = "";
         break;
   }

   if (!textname.empty() && !nextradio.empty()) {
      SelectRadioButtonByName(nextradio);

      wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                  wxT(textname.c_str()), wxTextCtrl);
      ptext->SetValue(_(BandIxsP[0].GetName().c_str()));

      changed_ = true;
   }
}

/**
 * Inicializa los botones.
 */
void SharpeningInputRGBPart::InitializeRadioButtons() {
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
               wxT("ID_SHAPENING_INPUT_RGB_RED_RADIO"), wxRadioButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SharpeningInputRGBPartEvent::OnRedRadioClicked), NULL,
         pEventHandler_);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
               wxT("ID_SHAPENING_INPUT_RGB_GREEN_RADIO"), wxRadioButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SharpeningInputRGBPartEvent::OnGreenRadioClicked), NULL,
         pEventHandler_);

   GET_CONTROL(*(this->GetWidget()->GetWindow()),
               wxT("ID_SHAPENING_INPUT_RGB_BLUE_RADIO"), wxRadioButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(SharpeningInputRGBPartEvent::OnBlueRadioClicked), NULL,
         pEventHandler_);

   SelectRadioButtonByName("ID_SHAPENING_INPUT_RGB_RED_RADIO");
}

/**
 * Inicializa los textboxes.
 */
void SharpeningInputRGBPart::InitializeTextBoxes() {
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHAPENING_INPUT_RGB_RED_TEXT"), wxTextCtrl);
   ptext->SetValue("");

   ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                   wxT("ID_SHAPENING_INPUT_RGB_GREEN_TEXT"), wxTextCtrl);
   ptext->SetValue("");

   ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                   wxT("ID_SHAPENING_INPUT_RGB_BLUE_TEXT"), wxTextCtrl);
   ptext->SetValue("");
}

/**
 * Obtiene el radio button seleccionado.
 */
int SharpeningInputRGBPart::GetSelectedRadioButtonPos() {
   // verifico el rojo
   wxRadioButton* pradio = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                   wxT("ID_SHAPENING_INPUT_RGB_RED_RADIO"),
                                   wxRadioButton);
   if (pradio->GetValue()) return kRedRadio;

   // verifico el verde
   pradio = XRCCTRL(*(this->GetWidget()->GetWindow()),
                    wxT("ID_SHAPENING_INPUT_RGB_GREEN_RADIO"), wxRadioButton);

   if (pradio->GetValue()) return kGreenRadio;

   // verifico el azul
   pradio = XRCCTRL(*(this->GetWidget()->GetWindow()),
                    wxT("ID_SHAPENING_INPUT_RGB_BLUE_RADIO"), wxRadioButton);

   if (pradio->GetValue()) return kBlueRadio;

   return -1; // no deberia pasar
}

/**
 * Seleccion el radio button especificado.
 */
void SharpeningInputRGBPart::SelectRadioButtonByName(const std::string& Name) {
   wxRadioButton* pradio = XRCCTRL(*(this->GetWidget()->GetWindow()), wxT(Name.c_str()),
                                   wxRadioButton);
   if (pradio)
      pradio->SetValue(true);
}

/**
 * Obtiene todos los elementos raster de baja resolucion.
 */
std::vector<RasterElement*> SharpeningInputRGBPart::GetLowResRasterElements(
      const std::vector<Element*>& Elements) {

   std::vector<RasterElement*> rasters;
   for (int ix = 0, lenix = Elements.size(); ix < lenix; ++ix) {
      rasters.push_back(dynamic_cast<RasterElement*>(Elements[ix]));
   }

   int hiresix = -1;
   double hiresval = std::numeric_limits<double>::max();
   for (int ix = 0, lenix = rasters.size(); ix < lenix; ++ix) {
      if (hiresval > rasters[ix]->GetPixelRes()) {
         hiresval = rasters[ix]->GetPixelRes();
         hiresix = ix;
      }
   }

   if (hiresix > -1)
      rasters.erase(rasters.begin() + hiresix);

   return rasters;
}

}  // namespace suri
