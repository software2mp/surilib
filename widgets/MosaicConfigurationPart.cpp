/*! \file ImageFormatSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MosaicConfigurationPart.h"

// Includes standard
// Includes Suri
#include "suri/Part.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"

// Includes wx
#include "wx/listctrl.h"
#include "wx/spinctrl.h"
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"

// Includes App
#include "suri/messages.h"
#include "resources.h"

// Defines


/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(MosaicConfigurationPartEvent, MosaicConfigurationPart)
   IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUpButton, OnUpButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDownButton, OnDownButton(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnElementActivate, OnElementActivate(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 */
MosaicConfigurationPart::MosaicConfigurationPart(std::vector<Element*> Selected) :
      Part(false, false), NEW_EVENT_OBJECT(MosaicConfigurationPartEvent) {
   windowTitle_ = _(caption_MOSAIC_CONFIGURATION);
   selected_ = Selected;
   pixelDistance_ = 0;
   noDataColour_ = 0;
}
/**
 * Destructor
 */
MosaicConfigurationPart::~MosaicConfigurationPart() {
   DELETE_EVENT_OBJECT;
}


/** Devuelve el valor de difuminado establecido en el part **/
int MosaicConfigurationPart::GetPixelDistance() {
   wxSpinCtrl* pSpin = XRCCTRL(*pToolWindow_, wxT("ID_SPINCTRL1"),
                               wxSpinCtrl);
   if (!pSpin) return -1;
   return  pSpin->GetValue();
}


/** Devuelve el valor de difuminado establecido en el part **/
unsigned char MosaicConfigurationPart::GetNoDataColour() {
   wxSpinCtrl* pSpin = XRCCTRL(*pToolWindow_, wxT("ID_SPINNODATA"),
                               wxSpinCtrl);
   if (!pSpin) return 0;
   return  static_cast<unsigned char>(pSpin->GetValue());
}

/**
 * Crea la ventana de la parte
 * @return false : si no pudo agregar el elemento XRC
 * @return true : si crea la ventana
 */
bool MosaicConfigurationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_MOSAIC_CONFIGURATION_PART"));
   if (!pToolWindow_) {
      return false;
   }
   imagelist_ = XRCCTRL(*pToolWindow_, wxT("ID_LISTCTRL"), wxListCtrl);
   GET_CONTROL(*pToolWindow_, wxT("ID_UP_BUTTON"), wxBitmapButton)
          ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                 wxCommandEventHandler(MosaicConfigurationPartEvent::OnUpButton), NULL,
                 pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_UP_BUTTON"), wxBitmapButton)
          ->SetToolTip(_(tooltip_UP_BUTTON));
   GET_CONTROL(*pToolWindow_, wxT("ID_DOWN_BUTTON"), wxBitmapButton)
             ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                    wxCommandEventHandler(MosaicConfigurationPartEvent::OnDownButton), NULL,
                    pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_DOWN_BUTTON"), wxBitmapButton)
             ->SetToolTip(_(tooltip_DOWN_BUTTON));
   imagelist_->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                       wxCommandEventHandler(MosaicConfigurationPartEvent::OnElementActivate), NULL,
                       pEventHandler_);
   return true;
}


/**
 * Devuelve el estado de la parte
 * @return true cuando la parte esta habilitada
 * @return false cuado la parte esta desabilitada
 */
bool MosaicConfigurationPart::IsEnabled() {
   return enabled_;
}

/**
 * Informa si la parte tiene cambios
 * @return true tiene cambios
 * @return false no tiene cambios
 */
bool MosaicConfigurationPart::HasChanged() {
   if ( (pixelDistance_ != GetPixelDistance()) || (noDataColour_ != GetNoDataColour())) {
      modified_ = true;
      noDataColour_ = GetNoDataColour();
      pixelDistance_ = GetPixelDistance();

   }
   return modified_;
}

void MosaicConfigurationPart::OnElementActivate(wxCommandEvent &Event) {
   int imageindex = 0;
   imageindex = imagelist_->GetNextItem (-1, wxLIST_NEXT_ALL,
                                                wxLIST_STATE_SELECTED);
   wxListItem item;
   item.SetId(imageindex);
   item.SetColumn(0);
   item.SetMask (wxLIST_MASK_TEXT);
   imagelist_->GetItem(item);
   if (item.GetText() == "1") item.SetText("0");
   else item.SetText("1");
   imagelist_->SetItem(imageindex, 0, item.GetText());
   item.SetId(imageindex);
   item.SetColumn(1);
   item.SetMask (wxLIST_MASK_TEXT);
   imagelist_->GetItem(item);
   ActiveElement(std::string(item.GetText()));
   modified_ = true;
}

void MosaicConfigurationPart::SetElementsOrder() {
   if (pPal_) {
         pPal_->AddAttribute<int>(ProcessAdaptLayer::MosaicPixelDistanceAttr, pixelDistance_);
         pPal_->AddAttribute<unsigned char>(ProcessAdaptLayer::MosaicNoDataColourAttr,
                                            noDataColour_);
         std::vector<Element*>::const_iterator it = selected_.begin();
         std::vector<Element*> aux;
         for (; it != selected_.end(); ++it){
            if (activeElements_[(*it)->GetName()]) aux.push_back(*it);
         }
         pPal_->AddAttribute< std::vector<Element*> > (ProcessAdaptLayer::MosaicRasterVectorAttr,
                                                       aux);
      }
}


/**
 * Si los datos de los controles wx son validos los guarda en las variables.
 * Si el archivo existe confirma sobreescritura.
 * Si no son validos muestra el mensaje de error que devuelve HasValidData.
 */
bool MosaicConfigurationPart::CommitChanges() {
   if (!HasValidData()) {
      REPORT_ERROR(errorMessage_);
      return false;
   }
   SetElementsOrder();
   return true;
}

void MosaicConfigurationPart::ActiveElement(const wxString& Name){
   if (activeElements_[Name]) activeElements_[Name] = false;
   else activeElements_[Name] = true;
}

void MosaicConfigurationPart::OnUpButton(wxCommandEvent &Event) {
   // Tiene que haber UN SOLO item seleccionado, sino sale.
   if (imagelist_->GetSelectedItemCount() != 1) return;
   // Obtengo el item seleccionado

   int imageindex = 0;
   imageindex = imagelist_->GetNextItem (imageindex, wxLIST_NEXT_ALL,
                                               wxLIST_STATE_SELECTED);
   // Si es la primera
   if (imageindex == -1) return ;
   wxListItem item;
   item.SetId(imageindex);
   imagelist_->GetItem(item);
   item.SetId(imageindex - 1);
   item.SetState(wxLIST_STATE_SELECTED);
   imagelist_->InsertItem(item);
   for( int i = 0; i < imagelist_->GetColumnCount(); i++ ) {
      wxListItem auxitem;
      auxitem.SetId(imageindex + 1);
      auxitem.SetColumn(i);
      auxitem.SetMask (wxLIST_MASK_TEXT);
      imagelist_->GetItem (auxitem);
      imagelist_->SetItem( imageindex - 1, i, auxitem.GetText());
   }
   imagelist_->DeleteItem(imageindex + 1);
   std::vector<Element*>::iterator it = selected_.begin();
   int x = 0;
   while (x != imageindex - 1) {
      x++;
      it = ++it;
   }
   Element* auxElement = *(++it);
   selected_.erase(it);
   selected_.insert(--it, auxElement);
   modified_ = true;
   return ;
}


void MosaicConfigurationPart::OnDownButton(wxCommandEvent &Event) {
   // Tiene que haber UN SOLO item seleccionado, sino sale.
   if (imagelist_->GetSelectedItemCount() != 1) return;
   int imageindex = 0;
   imageindex = imagelist_->GetNextItem (-1, wxLIST_NEXT_ALL,
                                                 wxLIST_STATE_SELECTED);
   if (imageindex == imagelist_->GetItemCount () - 1) return ;
   std::vector<Element*>::iterator it = selected_.begin();
   int x = 0;
   while (x != imageindex) {
         x++;
         it = ++it;
   }
   Element* auxElement = *it;
   selected_.erase(it);
   selected_.insert(++it, auxElement);
   imagelist_->ClearAll();
   std::vector<Element*>::const_iterator it2 = selected_.begin();
   int rowIndex = 0;
   imagelist_->InsertColumn(0, wxT("Validez"));
   imagelist_->InsertColumn(1, wxT("Imagen"));
   imagelist_->SetColumnWidth(0,50);
   imagelist_->SetColumnWidth(1,500);

   for (; it2 != selected_.end(); ++it2) {
      imagelist_->InsertItem(rowIndex, _(""));
      if (activeElements_[(*it2)->GetName()]){
         imagelist_->SetItem(rowIndex, 0, "1");
      } else {
         imagelist_->SetItem(rowIndex, 0, "0");
      }
      imagelist_->SetItem(rowIndex, 1, (*it2)->GetName());
      rowIndex++;
   }
   modified_ = true;
   return ;
}

bool MosaicConfigurationPart::RollbackChanges() {
   imagelist_->InsertColumn(0, wxT("Validez"));
   imagelist_->InsertColumn(1, wxT("Imagen"));
   imagelist_->SetColumnWidth(0,50);
   imagelist_->SetColumnWidth(1,500);
   std::vector<Element*>::const_iterator it = selected_.begin();
   int rowIndex = 0;
   for (; it != selected_.end(); ++it){
      imagelist_->InsertItem(rowIndex, _(""));
      imagelist_->SetItem(rowIndex, 0, "1");
      imagelist_->SetItem(rowIndex, 1, (*it)->GetName());
      activeElements_[(*it)->GetName()] = true;
      rowIndex++;
   }
   SetElementsOrder();
   return true;
}



void MosaicConfigurationPart::SetInitialValues() {
   RollbackChanges();
}




/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void MosaicConfigurationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_IMAGE_FORMAT_SELECTION, ToolBitmap);
}

/**
 * Compara los valores de los controles wx con los valores de las propiedades,
 * y modifica el valor de la propiedad modified_
 * @param[in] Event evento UIUpdate
 */
void MosaicConfigurationPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = false;
   Event.Skip();
}


bool MosaicConfigurationPart::HasValidData() {
   bool result = true;
   return result;
}

}


