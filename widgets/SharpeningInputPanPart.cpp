/*! \file SharpeningInputPanPart.cpp */
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

#include "SharpeningInputPanPart.h"

#include "SingleLayerBandSelectionPart.h"
#include "suri/Element.h"
#include "suri/Part.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"
#include "suri/Widget.h"

#include "resources.h"

#include "wx/bitmap.h"
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

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(SharpeningInputPanPartEvent, SharpeningInputPanPart)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 */
SharpeningInputPanPart::SharpeningInputPanPart(RasterElement* pElement, bool Enable,
                                               bool Modified) :
      Part(Enable,
           Modified),
      NEW_EVENT_OBJECT(SharpeningInputPanPartEvent), pElement_(pElement) {
   windowTitle_ = _(caption_SHARPENING_INPUT_PAN);
   changed_ = false;
   panix_ = -1;
}

SharpeningInputPanPart::~SharpeningInputPanPart() {
   // TODO Auto-generated destructor stub
}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool SharpeningInputPanPart::HasChanged() {
   return changed_ || !HasValidData();
}

/**
 * Salva los cambios realizados en el Part.
 */
bool SharpeningInputPanPart::CommitChanges() {
   pPal_->AddAttribute<BandInfo>(ProcessAdaptLayer::OutputHighResBand, band_);
   return true;
}

/**
 * Restaura los valores originales del Part.
 */
bool SharpeningInputPanPart::RollbackChanges() {
   changed_ = false;
   return true;
}

/**
 * Inicializa el part.
 */
void SharpeningInputPanPart::SetInitialValues() {
}

/**
 * Devuelve si la parte tiene datos validos.
 */
bool SharpeningInputPanPart::HasValidData() {
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHARPENING_INPUT_PAN_HIGH_RES_TEXT"), wxTextCtrl);
   if (ptext->GetValue().Len() == 0) return false;

   return true;
}

/**
 * Actualiza el estado de la parte.
 */
void SharpeningInputPanPart::Update() {
//   int panix = 0;
//   pPal_->GetAttribute<int>(ProcessAdaptLayer::InputRGBElement, panix);
//
//   if (panix_ != panix) {
//      InitializeElement();
//      InitializeList();
//      InitializeTextBox();
//   }
}

/**
 * Crea la ventana de la parte
 * @return true : si pudo cargar la parte
 * @return false : si no pudo cargar la parte
 */
bool SharpeningInputPanPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_SHARPENING_INPUT_PAN_PART"));

   wxPanel* pcttpanel = XRCCTRL(*pToolWindow_, wxT("ID_SHAPENING_INPUT_CONTAINER_PAN_PANEL"), wxPanel);

   // Agrego y configuro el part para seleccionar las bandas de origen.
   std::vector<Element*> elements;
   pPal_->GetAttribute<std::vector<Element*> >(ProcessAdaptLayer::InputElementsKeyAttr,
                                               elements);
   RasterElement* praster = GetHiResRasterElement(elements);

   SingleLayerBandSelectionPart* ppart = new SingleLayerBandSelectionPart(praster);

   ppart->CreatePartTool(pcttpanel);
   ppart->SetInputBandListener(this);

   if (!AddControl(ppart, pcttpanel, ControlArrangement(1, 0))) return false;

   return true;
}

/**
 * Retorna el icono de la herramienta.
 */
void SharpeningInputPanPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int) const {
   GET_BITMAP_RESOURCE(icon_RASTER_PANCROMATIC_BAND_SELECTION, ToolBitmap);
}

/**
 * Metodo a implementar por la clase que recibe las notificaciones
 * para recibir la lista de bandas seleccionadas.
 */
void SharpeningInputPanPart::OnBandSelected(std::vector<BandInfo> BandIxsP) {
   if (BandIxsP.empty())
      return;
   this->band_ = BandIxsP[0];
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHARPENING_INPUT_PAN_HIGH_RES_TEXT"),
                               wxTextCtrl);
   ptext->SetValue(_(this->band_.GetName().c_str()));
   changed_ = true;
}

/**
 * Inicializa el textbox.
 */
void SharpeningInputPanPart::InitializeTextBox() {
   wxTextCtrl* ptext = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_SHARPENING_INPUT_PAN_HIGH_RES_TEXT"), wxTextCtrl);
   ptext->SetValue("");
}

/**
 * Devuelve el elemento de alta resolucion.
 */
RasterElement* SharpeningInputPanPart::GetHiResRasterElement(
      const std::vector<Element*>& Elements) {

   RasterElement* pretval = NULL;
   int hiresix = -1;
   double hiresval = std::numeric_limits<double>::max();
   for (int ix = 0, lenix = Elements.size(); ix < lenix; ++ix) {
      RasterElement* paux = dynamic_cast<RasterElement*>(Elements[ix]);
      if (paux) {
         if (hiresval > paux->GetPixelRes()) {
            hiresval = paux->GetPixelRes();
            hiresix = ix;
         }
      }
   }

   if (hiresix > -1) {
      pretval = dynamic_cast<RasterElement*>(Elements[hiresix]);
   }

   return pretval;
}

} /** namespace suri */
