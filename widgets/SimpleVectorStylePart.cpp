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

// Includes Estandar
// Includes Suri
#include "SimpleVectorStylePart.h"
#include "suri/VectorStyle.h"
#include "VectorLayer.h"
#include "LibraryItemEditorNotificator.h"
#include "suri/PartContainerWidget.h"
#include "suri/VectorElement.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/clrpicker.h" // color picker
#include "wx/spinctrl.h" // textbox con up y down arrows


// Defines
#define LAYER_POLYGON_RED \
   "VECTORSTYLE[3,\"Poligono rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]]]"

#define PANEL_XRC "ID_SIMPLE_STYLE_PART"
#define LIB_XRC "ID_STYLE_LIBRARY_PANEL"
/** Macro que obtiene el estilo de un vector. */
/*
 * Obtiene el estilo de un vector a partir del elemento.
 * @param[out] pVecStyle estilo del vector.
 * \todo verificar nulidad de pElement_
 */
#define GET_VECTOR_STYLE(pVecStyle, pElement) do {                                       \
      suri::VectorStyle::Destroy(pVecStyle);                                     \
      VectorRenderer::Parameters parameters = suri::VectorRenderer::GetParameters \
                                                 (pElement->GetNode(wxT("") ) );   \
      pVecStyle = suri::VectorStyle::Create(parameters.layerstyle_.begin()->     \
                                            second); \
} while ( 0 )

// forwards

namespace suri {

namespace ui {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SimpleVectorStylePartEvent, SimpleVectorStylePart)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLibraryCheckClick, OnLibraryCheckClick(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor por defecto de la clase **/
SimpleVectorStylePart::SimpleVectorStylePart(
      const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
      VectorLayer *pLayer,
      const UniversalGraphicalComponentPartInterface::FeatureCombinationEnum& Feature) :
      LibraryItemEditorPart(LibraryId, pManager, PANEL_XRC, LIB_XRC, Feature),
      pEventHandler_(new SimpleVectorStylePartEvent(this)), pLayer_(pLayer),
      pVecStyle_(NULL), pWkt_(NULL) {
   windowTitle_ = _(caption_VECTOR_PROPERTIES);
}

SimpleVectorStylePart::SimpleVectorStylePart(std::string* pWkt,LibraryManager* pManager,const SuriObject::UuidType& LibraryId) :
      LibraryItemEditorPart(
            LibraryId, pManager, PANEL_XRC, LIB_XRC,
            UniversalGraphicalComponentPartInterface::EditionWithFavorites),
            pEventHandler_(new SimpleVectorStylePartEvent(this)),
            pLayer_(NULL),
      updatePreview_(true), pVecStyle_(NULL), libraryCheck_(false), pWkt_(pWkt) {
   windowTitle_ = _(caption_VECTOR_PROPERTIES);
}

SimpleVectorStylePart::~SimpleVectorStylePart() {
   DELETE_EVENT_OBJECT;
   suri::VectorStyle::Destroy(pVecStyle_);
}

void SimpleVectorStylePart::SetInitialValues() {
   if (!pVecStyle_) {
      return;
   }

   Element* pelement = pLayer_ ? pLayer_->GetElement() : NULL;

   wxXmlNode* pnode = pelement ? pelement->GetNode(
         RENDERIZATION_NODE NODE_SEPARATION_TOKEN LAYERS_NODE) : NULL;
   wxString val;
   if (pnode && pnode->GetPropVal(LIBRARY_STYLE_NODE_PROPERTY, &val)) {
      XRCCTRL(*(this->GetWidget()->GetWindow()),
            "ID_LIBRARY_CHECK", wxCheckBox)->SetValue(true);
      /**
       *  Tomo el item seleccionado del CGU y se lo paso la variable
       *  de estilo miembro para que cargue el estilo.
       */
      pNotificator_->NotifyItemSelectionFromEditor(val.c_str());
      const LibraryItem* pItem = pCgu_->GetSelectedItem();
      if (pItem) {
         const LibraryItemAttribute* attr = pItem->GetAttribute("ESTILO");
         std::string auxWkt = attr->GetValue();
         pVecStyle_ = suri::VectorStyle::Create(auxWkt);
         libraryCheck_ = true;
         pCgu_->Enable();
      }
   } else {
      XRCCTRL(*(this->GetWidget()->GetWindow()),
            "ID_LIBRARY_CHECK", wxCheckBox)->SetValue(false);
      if (pCgu_)
         pCgu_->Disable();
      if (!pManager_ || !pCgu_) {
         XRCCTRL(*(this->GetWidget()->GetWindow()),
                     "ID_LIBRARY_CHECK", wxCheckBox)->Enable(false);
      }
      libraryCheck_ = false;
   }

   VectorStyle::Pen *ppen = pVecStyle_->GetPen();
   if (!ppen) {
      GET_CONTROL(*pToolWindow_, "ID_CMB_LINE", wxComboBox)->Enable(false);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl)->Enable(false);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_LINE", wxColourPickerCtrl)->Enable(false);
   } else {
      GET_CONTROL(*pToolWindow_, "ID_CMB_LINE", wxComboBox)->SetSelection(ppen->id_);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl)->SetValue(ppen->width_);
      wxColour pencolour(ppen->color_.red_, ppen->color_.green_, ppen->color_.blue_,
                         ppen->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_LINE", wxColourPickerCtrl)->SetColour(
            pencolour);
   }
   VectorStyle::Brush *pbrush = pVecStyle_->GetBrush();
   if (!pbrush) {
      GET_CONTROL(*pToolWindow_, "ID_CMB_BRUSH", wxComboBox)->Enable(false);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_BRUSH", wxColourPickerCtrl)->Enable(false);
   } else {
      GET_CONTROL(*pToolWindow_, "ID_CMB_BRUSH", wxComboBox)->SetSelection(pbrush->id_);
      wxColour brushcolour(pbrush->color_.red_, pbrush->color_.green_,
                           pbrush->color_.blue_, pbrush->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_BRUSH", wxColourPickerCtrl)->SetColour(
            brushcolour);
   }
   VectorStyle::Symbol *psymbol = pVecStyle_->GetSymbol();
   if (!psymbol) {
      GET_CONTROL(*pToolWindow_, "ID_CMB_SYMBOL", wxComboBox)->Enable(false);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_SYMBOL", wxSpinCtrl)->Enable(false);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_SYMBOL", wxColourPickerCtrl)->Enable(false);
   } else {
      GET_CONTROL(*pToolWindow_, "ID_CMB_SYMBOL", wxComboBox)->SetSelection(
            psymbol->id_);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_SYMBOL", wxSpinCtrl)->SetValue(
            psymbol->size_);
      wxColour symbolcolour(psymbol->color_.red_, psymbol->color_.green_,
                            psymbol->color_.blue_, psymbol->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_SYMBOL", wxColourPickerCtrl)->SetColour(
            symbolcolour);
   }
   updatePreview_ = true;
}

/**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
 *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
 *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
 *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
 *   @return vector con atributos
 *   @return NULL en caso de que la informnacion del part no sea valida
 *   **/
std::vector<LibraryItemAttribute*>* SimpleVectorStylePart::CreateAttributesFromFields() {
   if (!libraryCheck_) {
      return NULL;
   }
   std::vector<LibraryItemAttribute*>* pattributes = new std::vector<
         LibraryItemAttribute*>;
   if (pVecStyle_ == NULL)
      return NULL;

   // Reflejamos los cambios en el pVecStyle_.
   SetPen();
   SetBrush();
   SetSymbol();

   // Actualizo el WKT en el archivo de texto plano.
   suri::LibraryItemAttribute* libattr = new suri::LibraryItemAttribute(
         "ESTILO", pVecStyle_->GetWkt());

   pattributes->push_back(libattr);
   return pattributes;
}
/**
 *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
 *  Este metodo es el que realmente tienen que implementar las clases hijas para
 * lograr la especializacion a la hora de iniciar el part (metodo que se llama
 * en CreateToolWindow, a modo de Template Method).
 **/
bool SimpleVectorStylePart::ConfigureFeatures() {
   if (pEventHandler_)
      GET_CONTROL(*pToolWindow_, "ID_BMP_PREVIEW", wxStaticBitmap)->Connect(
               wxEVT_UPDATE_UI,
               wxUpdateUIEventHandler(
                     SimpleVectorStylePartEvent::OnUIUpdate), NULL,
               pEventHandler_);
   GET_CONTROL(*(this->GetWidget()->GetWindow()),
         "ID_LIBRARY_CHECK", wxCheckBox)->Connect(
         wxEVT_COMMAND_CHECKBOX_CLICKED,
         wxCommandEventHandler(SimpleVectorStylePartEvent::OnLibraryCheckClick),
         NULL, pEventHandler_);
   LoadControls();
   if (!libraryCheck_ && pCgu_)
      pCgu_->Disable();
   else if (pCgu_ && libraryCheck_)
      pCgu_->Enable();
   return true;
}

/** Carga los controles(combobox) */
void SimpleVectorStylePart::LoadControls() {
   if (!pVecStyle_) {

      suri::VectorStyle *pvecstyle = NULL;
      Element* pelement = pLayer_ ? pLayer_->GetElement() : NULL;
      // si tiene estilo simple entonces configuro el part con el estilo
      if (pelement && pelement->GetNode(
      RENDERIZATION_NODE NODE_SEPARATION_TOKEN LAYERS_NODE)) {
         GET_VECTOR_STYLE(pvecstyle, pelement);
         if (pvecstyle) {
            pVecStyle_ = pvecstyle;
         }
      }
      else {
         if (currentWkt_.empty())
            pVecStyle_ = suri::VectorStyle::Create(LAYER_POLYGON_RED);
         else
            pVecStyle_ = suri::VectorStyle::Create(currentWkt_);
      }
   }

   VectorStyle::Pen *ppen = pVecStyle_->GetPen();
   if (ppen) {
      // Carga los controles con todos los tipos encontrados.
      for (size_t i = 0; i < VectorStyle::Pen::Count; i++)
         GET_CONTROL(*pToolWindow_, "ID_CMB_LINE", wxComboBox)->Append(
                  _(ppen->GetTypeAsString(i).c_str()));
   }
   // Carga los controles con todos los tipos encontrados.
   VectorStyle::Brush *pbrush = pVecStyle_->GetBrush();
   if (pbrush) {
      for (size_t i = 0; i < VectorStyle::Brush::Count; i++)
         GET_CONTROL(*pToolWindow_, "ID_CMB_BRUSH", wxComboBox)->Append(
                  _(pbrush->GetTypeAsString(i).c_str()));
   }
   // Carga los controles con todos los tipos encontrados.
   VectorStyle::Symbol *psymbol = pVecStyle_->GetSymbol();
   if (psymbol) {
      for (size_t i = 0; i < VectorStyle::Symbol::Count; i++)
         GET_CONTROL(*pToolWindow_, "ID_CMB_SYMBOL", wxComboBox)->Append(
                  _(psymbol->GetTypeAsString(i).c_str()));
   }
   UpdateStyleControls();
}

/**
 * Metodo auxiliar privado que es necesario implementar en las clases hijas.
 * Este metodo es el encargado de actualizar realmente la informacion en el part
 * Es llamado en el metodo SetActiveItem a modo de Template Method
 **/
void SimpleVectorStylePart::UpdateFields(const LibraryItem* pItem, bool Modifiable) {
   if (pItem) {
      const LibraryItemAttribute* pattr = pItem->GetAttribute("ESTILO");
      std::string auxWkt;
      if (pattr)
         auxWkt = pattr->GetValue();
      suri::VectorStyle *suriVecStyle = NULL;
      suriVecStyle = suri::VectorStyle::Create(auxWkt);

      if (!suriVecStyle)
         return;

      VectorStyle::Symbol *psymbol = suriVecStyle->GetSymbol();
      VectorStyle::Brush *pbrush = suriVecStyle->GetBrush();
      VectorStyle::Pen *ppen = suriVecStyle->GetPen();

      // Symbol
      GET_CONTROL(*pToolWindow_, "ID_CMB_SYMBOL", wxComboBox)->SetSelection(
               psymbol->id_);
      wxColour symbolcolour(psymbol->color_.red_, psymbol->color_.green_,
            psymbol->color_.blue_, psymbol->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_SYMBOL", wxColourPickerCtrl)->SetColour(
               symbolcolour);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl)->SetValue(
            psymbol->size_);


      // Brush
      GET_CONTROL(*pToolWindow_, "ID_CMB_BRUSH", wxComboBox)->SetSelection(
               pbrush->id_);
      wxColour brushcolour(pbrush->color_.red_, pbrush->color_.green_,
            pbrush->color_.blue_, pbrush->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_BRUSH", wxColourPickerCtrl)->SetColour(
               brushcolour);

      // Brush
      GET_CONTROL(*pToolWindow_, "ID_CMB_LINE", wxComboBox)->SetSelection(
               ppen->id_);
      wxColour pencolour(ppen->color_.red_, ppen->color_.green_,
            ppen->color_.blue_, ppen->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_LINE", wxColourPickerCtrl)->SetColour(
               pencolour);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl)->SetValue(
            ppen->width_);

      updatePreview_ = true;
   }
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el commit involucra objetos adicionales a los items de biblioteca
 * del editor (ej. reproyeccion, en donde se necesita actualizar los params
 * de reproyeccion ademas de leer el item de biblioteca)
 * @return true en caso de realizar correctamente el commit "interno"
 * @return false en caso de que ocurra algun error durante el commit
 */
bool SimpleVectorStylePart::OnCommitChanges() {
   SetPen();
   SetBrush();
   SetSymbol();

   if (pWkt_) {
      *pWkt_ = pVecStyle_->GetWkt();
   }
   Element* pelement = pLayer_ ? pLayer_->GetElement() : NULL;
   /* Obtengo el nuevo wkt y se lo seteo a los parametros para generar el nuevo
    * nodo de renderizacion */
   if (pelement) {
      // Tomo el item del CGU
      const LibraryItem* pitem = libraryCheck_ ? pCgu_->GetSelectedItem() : NULL;
      // configuracion de estilo simple
      VectorRenderer::Parameters parameters = suri::VectorRenderer::GetParameters(
            pelement->GetNode(wxT("")));
      parameters.layerstyle_[0] = pVecStyle_->GetWkt();
      wxXmlNode* pstylerendernode = VectorRenderer::GetXmlNode(parameters);
      wxXmlNode* pnode = pstylerendernode->GetChildren();
      while (pnode && pnode->GetName().compare(LAYERS_NODE)) {
         pnode = pnode->GetNext();
      }
      if (pitem && pnode) {
         pnode->AddProperty(LIBRARY_STYLE_NODE_PROPERTY,
                            pitem->GetPrincipalAttribute()->GetValue().c_str());
      } else if (pnode) {
         pnode->DeleteProperty(LIBRARY_STYLE_NODE_PROPERTY);
      }
      VectorRenderer::SetXmlNode(pelement, pstylerendernode);
      VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pelement);
      pvectorelement->SetStyle(pVecStyle_->GetWkt(), pvectorelement->GetActiveLayer());
      pelement->SetChanged();
      pelement->SendViewerUpdate();
   }

   modified_ = false;
   updatePreview_ = false;
   return true;
}

/**
 * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
 * clase base. Este comportamiento adicional es necesario cuando
 * el rollback involucra objetos adicionales a los items de biblioteca
 * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
 * los mismos)
 * @return true en caso de realizar correctamente el rollback "interno"
 * @return false en caso de que ocurra algun error durante el rollback
 */
bool SimpleVectorStylePart::OnRollback() {
   SetInitialValues();
   return true;
}

/** Evento UIUpdate */
/**
 * Si el flag updatePreview_ esta activo actualiza los valores del elemento y setea
 * updatePreview_ a falso
 * @param[in] Event evento de actualizacion de interfaz de usuario
 */
void SimpleVectorStylePart::OnUIUpdate(wxUpdateUIEvent &Event) {
   if (!HasChanged() || !updatePreview_ || !pVecStyle_) {
      return;
   }

   SetPen();
   SetBrush();
   SetSymbol();
   wxBitmap bmp;
   int width, height;
   GET_CONTROL(*pToolWindow_, "ID_BMP_PREVIEW", wxStaticBitmap)->GetClientSize(
            &width, &height);

   VectorRenderer::PreviewStyle(pVecStyle_, bmp, width, height);
   GET_CONTROL(*pToolWindow_, "ID_BMP_PREVIEW", wxStaticBitmap)->SetBitmap(
            bmp);
   updatePreview_ = false;
   Event.Skip();
}

/** Actualiza el VectorStyle a partir del elemento(pElement). */
void SimpleVectorStylePart::Refresh() {
   Element* pelement = pLayer_ ? pLayer_->GetElement() : NULL;
   if (!pelement || !pVecStyle_) {
      return;
   }
   GET_VECTOR_STYLE(pVecStyle_, pelement);
}

/** Realiza los cambios en el vector */
/**
 * Pide el Pen a el pVecStyle_, si existe lo actualiza
 * @return true en caso de poder modificar la propiedad Pen del estilo del Vector
 * @return false en caso de no poder modificar la propiedad Pen del estilo del Vector
 */
bool SimpleVectorStylePart::SetPen() {
   if (!pVecStyle_) {
      return false;
   }

   VectorStyle::Pen *ppen = pVecStyle_->GetPen();
   if (!ppen) {
      return true;
   }

   int id, size;
   wxColour color;

   // seteo valores de Pen
   id = USE_CONTROL(*pToolWindow_, "ID_CMB_LINE",
         wxComboBox, GetSelection(), wxNOT_FOUND);
   size = USE_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl, GetValue(), 0);
   color = USE_CONTROL(*pToolWindow_, "ID_COLOUR_LINE", wxColourPickerCtrl,
         GetColour(), wxNullColour);

   if (id == wxNOT_FOUND || size <= 0 || color == wxNullColour) {
      return false;
   }

   ppen->id_ = (VectorStyle::Pen::PenType) (id);
   ppen->width_ = size;
   ppen->color_.red_ = color.Red();
   ppen->color_.green_ = color.Green();
   ppen->color_.blue_ = color.Blue();
   ppen->color_.alpha_ = color.Alpha();

   // TODO(Gabriel - TCK #4782): Fix rapido para poder configurar estilo de color "negro"
   if (color.Red() == 0 && color.Green() == 0 && color.Blue() == 0) {
      ppen->color_.red_ = 1;
      ppen->color_.green_ = 1;
      ppen->color_.blue_ = 1;
   }
   return true;
}

// ------------------------------------------------------------------------------
/** Realiza los cambios en la superficie */
/**
 * Pide el Brush a el pVecStyle_, si existe lo actualiza
 * @return true en caso de poder modificar la propiedad Brush del estilo del Vector
 * @return false en caso de no poder modificar la propiedad Brush del estilo del Vector
 */
bool SimpleVectorStylePart::SetBrush() {
   if (!pVecStyle_) {
      return false;
   }

   VectorStyle::Brush *pbrush = pVecStyle_->GetBrush();
   if (!pbrush) {
      return true;
   }
   int id;
   wxColour color;

   // seteo valores de Brush
   id = USE_CONTROL(*pToolWindow_, "ID_CMB_BRUSH", wxComboBox, GetSelection(),
         wxNOT_FOUND);
   color = USE_CONTROL(*pToolWindow_, "ID_COLOUR_BRUSH", wxColourPickerCtrl,
         GetColour(), wxNullColour);
   if (id == wxNOT_FOUND || color == wxNullColour) {
      return false;
   }

   pbrush->id_ = (VectorStyle::Brush::BrushType) (id);
   pbrush->color_.red_ = color.Red();
   pbrush->color_.green_ = color.Green();
   pbrush->color_.blue_ = color.Blue();
   pbrush->color_.alpha_ = color.Alpha();
   // TODO(Gabriel - TCK #4782): Fix rapido para poder configurar estilo de color "negro"
   if (color.Red() == 0 && color.Green() == 0 && color.Blue() == 0) {
      pbrush->color_.red_ = 1;
      pbrush->color_.green_ = 1;
      pbrush->color_.blue_ = 1;
   }
   return true;
}

// ------------------------------------------------------------------------------
/** Realiza los cambios en el punto */
/**
 * Pide el Symbol a el pVecStyle_, si existe lo actualiza
 * @return true en caso de poder modificar la propiedad Symbol del estilo del Vector
 * @return false en caso de no poder modificar la propiedad Brush del estilo del Vector
 */
bool SimpleVectorStylePart::SetSymbol() {
   if (!pVecStyle_) {
      return false;
   }

   VectorStyle::Symbol *psymbol = pVecStyle_->GetSymbol();
   if (!psymbol) {
      return true;
   }

   int id, size;
   wxColour color;

   // seteo valores de Symbol
   id = USE_CONTROL(*pToolWindow_, "ID_CMB_SYMBOL", wxComboBox, GetSelection(),
         wxNOT_FOUND);
   size =
         USE_CONTROL(*pToolWindow_, "ID_SPIN_SYMBOL", wxSpinCtrl, GetValue(), 0);
   color = USE_CONTROL(*pToolWindow_, "ID_COLOUR_SYMBOL", wxColourPickerCtrl,
         GetColour(), wxNullColour);
   if (id == wxNOT_FOUND || size <= 0 || color == wxNullColour) {
      return false;
   }

   psymbol->id_ = (VectorStyle::Symbol::SymbolType) (id);
   psymbol->size_ = size;
   psymbol->color_.red_ = color.Red();
   psymbol->color_.green_ = color.Green();
   psymbol->color_.blue_ = color.Blue();
   psymbol->color_.alpha_ = color.Alpha();
   return true;
}
/** Metodo que actualiza el estilo que se encuentra configurado */
void SimpleVectorStylePart::UpdateStyleControls() {
   VectorStyle::Symbol *psymbol = pVecStyle_->GetSymbol();

   // Symbol
   if (psymbol) {
      GET_CONTROL(*pToolWindow_, "ID_CMB_SYMBOL", wxComboBox)->SetSelection(
            psymbol->id_);
      wxColour symbolcolour(psymbol->color_.red_, psymbol->color_.green_,
                            psymbol->color_.blue_, psymbol->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_SYMBOL", wxColourPickerCtrl)->SetColour(
            symbolcolour);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl)->SetValue(psymbol->size_);
   }
   // Brush
   VectorStyle::Brush *pbrush = pVecStyle_->GetBrush();
   if (pbrush) {
      GET_CONTROL(*pToolWindow_, "ID_CMB_BRUSH", wxComboBox)->SetSelection(pbrush->id_);
      wxColour brushcolour(pbrush->color_.red_, pbrush->color_.green_,
                           pbrush->color_.blue_, pbrush->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_BRUSH", wxColourPickerCtrl)->SetColour(
            brushcolour);
   }
   // Pen
   VectorStyle::Pen *ppen = pVecStyle_->GetPen();
   if (ppen) {
      GET_CONTROL(*pToolWindow_, "ID_CMB_LINE", wxComboBox)->SetSelection(ppen->id_);
      wxColour pencolour(ppen->color_.red_, ppen->color_.green_, ppen->color_.blue_,
                         ppen->color_.alpha_);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_LINE", wxColourPickerCtrl)->SetColour(
            pencolour);
      GET_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl)->SetValue(ppen->width_);
   }
}

/** Metodo que se llama cuando se clickea sobre el check para utilizar la biblioteca **/
void SimpleVectorStylePart::OnLibraryCheckClick(wxCommandEvent& Event) {
   bool value = XRCCTRL(*(this->GetWidget()->GetWindow()),
            "ID_LIBRARY_CHECK", wxCheckBox)->GetValue();
   if (value) {
      pCgu_->Enable();
      const LibraryItem* pitem = pCgu_->GetSelectedItem();
      const LibraryItemAttribute* attr = pitem ? pitem->GetAttribute("ESTILO") : NULL;
      if (attr) {
         std::string auxWkt = attr->GetValue();
         pVecStyle_ = suri::VectorStyle::Create(auxWkt);
         UpdateStyleControls();
      }

   } else {
      pCgu_->Disable();
   }
   libraryCheck_ = value;
}

// ------------------------------------------------------------------------------
/** Indica si el Part tiene cambios para salvar. */
/**
 * Para cada tipo de propiedad verifica si cambiaron sus valores
 * @return true si se modifican las propiedades del Elemento.
 * @return false si no se modifican las propiedades del Elemento.
 */
bool SimpleVectorStylePart::HasChanged() {
   int id, size;
   wxColour color;
   updatePreview_ = false;

   // Genero un VectorStyle con los parametros anteriores a las modificaciones.
   suri::VectorStyle *pvecstyle = NULL;
   Element* pelement = pLayer_ ? pLayer_->GetElement() : NULL;
   wxXmlNode* psimplestylenode = pelement ? pelement->GetNode(
               RENDERIZATION_NODE NODE_SEPARATION_TOKEN LAYERS_NODE) : NULL;
   if (pelement && psimplestylenode)
      GET_VECTOR_STYLE(pvecstyle, pelement);
   else if (!psimplestylenode && pWkt_)
      pvecstyle = VectorStyle::Create(*pWkt_);
   /**
    *  Si no puede armar estilo a partir de la configuracion inicial
    *  entonces se fuerza el cambio
    */
   if (!pvecstyle) {
      return true;
   }

   VectorStyle::Pen *ppen = pvecstyle->GetPen();
   if (ppen) {
      id =
            USE_CONTROL(*pToolWindow_, "ID_CMB_LINE", wxComboBox, GetSelection(),
                  wxNOT_FOUND);
      size =
            USE_CONTROL(*pToolWindow_, "ID_SPIN_LINE", wxSpinCtrl, GetValue(), 0);
      color = USE_CONTROL(*pToolWindow_, "ID_COLOUR_LINE", wxColourPickerCtrl,
            GetColour(), wxNullColour);

      wxColour colorpen(ppen->color_.red_, ppen->color_.green_,
            ppen->color_.blue_, ppen->color_.alpha_);
      if (ppen->id_ != id || ppen->width_ != size || colorpen != color) {
         VectorStyle::Destroy(pvecstyle);
         updatePreview_ = true;
         return updatePreview_;
      }
   }

   VectorStyle::Brush *pbrush = pvecstyle->GetBrush();
   if (pbrush) {
      id =
            USE_CONTROL(*pToolWindow_, "ID_CMB_BRUSH", wxComboBox, GetSelection(),
                  wxNOT_FOUND);
      color =
            USE_CONTROL(*pToolWindow_, "ID_COLOUR_BRUSH", wxColourPickerCtrl,
                  GetColour(), wxNullColour);
      wxColour colorbrush(pbrush->color_.red_, pbrush->color_.green_,
            pbrush->color_.blue_, pbrush->color_.alpha_);
      if (pbrush->id_ != id || colorbrush != color) {
         VectorStyle::Destroy(pvecstyle);
         updatePreview_ = true;
         return updatePreview_;
      }
   }

   VectorStyle::Symbol *psymbol = pvecstyle->GetSymbol();
   if (psymbol) {
      id =
            USE_CONTROL(*pToolWindow_, "ID_CMB_SYMBOL", wxComboBox, GetSelection(),
                  wxNOT_FOUND);
      size =
            USE_CONTROL(*pToolWindow_, "ID_SPIN_SYMBOL", wxSpinCtrl, GetValue(), 0);
      color =
            USE_CONTROL(*pToolWindow_, "ID_COLOUR_SYMBOL", wxColourPickerCtrl,
                  GetColour(), wxNullColour);

      wxColour colorsymbol(psymbol->color_.red_, psymbol->color_.green_,
            psymbol->color_.blue_, psymbol->color_.alpha_);
      if (psymbol->id_ != id || psymbol->size_ != size
            || colorsymbol != color) {
         VectorStyle::Destroy(pvecstyle);
         updatePreview_ = true;
         return updatePreview_;
      }
   }

   VectorStyle::Destroy(pvecstyle);
   return updatePreview_;
}

/** Configura el estilo vectorial que mostrara el editor desde codigo cliente **/
void SimpleVectorStylePart::SetVectorStyleWkt(const std::string& Wkt) {
   std::string wkt = Wkt;
   if (Wkt.empty()) {
      wkt = LAYER_POLYGON_RED;
   }
   VectorStyle::Destroy(pVecStyle_);
   pVecStyle_ = VectorStyle::Create(wkt);
   currentWkt_ = wkt;
   if (pWkt_)
      *pWkt_ = currentWkt_;
}
/** Retorna el puntero al estilo del vector */
suri::VectorStyle* SimpleVectorStylePart::GetVectorStyle() {
   return pVecStyle_;
}


} /** namespace ui **/
} /** namespace suri */
