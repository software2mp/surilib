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

// Includes suri
#include "AnotationPropertiesPart.h"
#include "suri/xmlnames.h"
#include "suri/Vector.h"
#include "suri/VectorEditor.h"
#include "suri/VectorStyle.h"
#include "suri/messages.h"
#include "resources.h"
#include "wxmacros.h"
#include "suri/AuxiliaryFunctions.h"
#include "VectorLayer.h"
#include "suri/VectorElement.h"
#include "CategorizedVectorRenderer.h"

// Includes de wx
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"
#include "wx/clrpicker.h"
#include "wx/spinctrl.h"

// Includes app

// Includes OGR
#include "ogrsf_frmts.h"

// Defines
#define EXPRESSION_TOKEN_INIT "{"
#define EXPRESSION_TOKEN_FINISH "}"

// Forwards
void InitAnotationPropertiesXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(AnotationPropertiesPartEvent, AnotationPropertiesPart)
      IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnCheckboxLabelEnableClicked, OnCheckboxLabelEnableClicked(event),
      wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
// ------------------------------------------------------------------------------

/**
 * Ctor. legacy.
 * @param[in]  pElement: elemento (Anotacion)
 */
AnotationPropertiesPart::AnotationPropertiesPart(VectorElement *pElement) :
      NEW_EVENT_OBJECT(AnotationPropertiesPartEvent),
      pElement_(pElement), pVectorLayer_(NULL),
      pVecStyle_(NULL), fieldList_(), selectedField_(-1),
      updatePreview_(false),
      hasFieldCombo_(false), isLabelChecked_(false) {
   windowTitle_ = _(caption_ANOTATION_PROPERTIES);
#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   textAnotation_ = wxT("");
#endif
   UpdateStyle();
}

/**
 * Ctor. default.
 * @param[in]  pVectorLayer capa vectorial (Anotacion)
 */
AnotationPropertiesPart::AnotationPropertiesPart(VectorLayer *pVectorLayer) :
      NEW_EVENT_OBJECT(AnotationPropertiesPartEvent),
      pElement_(NULL), pVectorLayer_(pVectorLayer),
      pVecStyle_(NULL), fieldList_(), selectedField_(-1),
      updatePreview_(false),
      hasFieldCombo_(false), isLabelChecked_(false) {
   windowTitle_ = _(caption_ANOTATION_PROPERTIES);
#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   textAnotation_ = wxT("");
#endif
   UpdateStyle();
}

// ------------------------------------------------------------------------------

/**
 * Ctor. legacy que Permite especificar si el panel de seleccion de campo sera visible o no
 * @param[in]  pElement: elemento (Anotacion)
 * @param[in]  ShowFieldSelector: booleano que determina si el part tendre el combo
 * de seleccion de campo oculta
 */
AnotationPropertiesPart::AnotationPropertiesPart(VectorElement *pElement,
                                                 bool ShowFieldSelector) :
      NEW_EVENT_OBJECT(
            AnotationPropertiesPartEvent),
      pElement_(pElement), pVectorLayer_(NULL),
      pVecStyle_(NULL), fieldList_(), selectedField_(-1),
      updatePreview_(false),
      hasFieldCombo_(false), isLabelChecked_(false) {
   windowTitle_ = _(caption_ANOTATION_PROPERTIES);
#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   textAnotation_ = wxT("");
#endif

   hasFieldCombo_ = ShowFieldSelector;
   UpdateStyle();
}


/**
 * Ctor. legacy que Permite especificar si el panel de seleccion de campo sera visible o no
 * @param[in]  pVectorLayer: elemento (Anotacion)
 * @param[in]  ShowFieldSelector: booleano que determina si el part tendre el combo
 * de seleccion de campo oculta
 */
AnotationPropertiesPart::AnotationPropertiesPart(VectorLayer *pVectorLayer,
                                                 bool ShowFieldSelector) :
      NEW_EVENT_OBJECT(AnotationPropertiesPartEvent),
      pElement_(NULL), pVectorLayer_(pVectorLayer), pVecStyle_(NULL),
      fieldList_(), selectedField_(-1), updatePreview_(false),
      hasFieldCombo_(false), isLabelChecked_(false) {
   windowTitle_ = _(caption_ANOTATION_PROPERTIES);
#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   textAnotation_ = wxT("");
#endif

   hasFieldCombo_ = ShowFieldSelector;
   UpdateStyle();
}
// ------------------------------------------------------------------------------

/**
 * Destructor.
 * Destruye el VectorStyle y el objeto de eventos.
 */
AnotationPropertiesPart::~AnotationPropertiesPart() {
   VectorStyle::Destroy(pVecStyle_);
   DELETE_EVENT_OBJECT;
}

// ------------------------------------------------------------------------------
/**
 * Carga la ventana diseniada. Si tuvo exito, asocia evento UIUpdate a un
 * wxStaticBitmap donde se muestra el preview de la Anotacion.
 * @return true si pudo crear la ventana.
 * @return false No puede cargar el panel de los recursos.
 * @return false No puede agregar el control de texto para modificar la anotacion
 */
bool AnotationPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, wxT("ID_PANEL_ANOTATION_PROPERTIES"));

   if (!pToolWindow_) {
      return false;
   }

   GET_CONTROL(*pToolWindow_, "ID_BMP_PREVIEW", wxStaticBitmap)
      ->Connect(wxEVT_UPDATE_UI,
                wxUpdateUIEventHandler(AnotationPropertiesPartEvent::OnUIUpdate), NULL,
                pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_CHECKBOX_LABEL_ENABLE", wxCheckBox)
      ->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(
            AnotationPropertiesPartEvent::OnCheckboxLabelEnableClicked),
                NULL, pEventHandler_);

#ifndef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   // Oculto el text y su sizer
   wxSizer *psizer =
         USE_CONTROL(*pToolWindow_, "ID_PANEL_PROP", wxPanel, GetSizer(), NULL);
   if (!psizer) {
      return false;
   }
   wxSizerItem *psizeritem = psizer->GetItem((size_t) 0);
   if (psizeritem && psizeritem->GetSizer()) {
      psizer->Hide(psizeritem->GetSizer(), true);
   }
   psizer->Layout();
#else
   // asocio la propiedad textAnotation_ con la text para edicion de la anotacion.
   GET_CONTROL(*pToolWindow_, "ID_TXT_ANOTATION", wxTextCtrl)->SetValidator(
         wxGenericValidator(&textAnotation_) );
#endif  // __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   GET_CONTROL(*pToolWindow_, wxT("ID_PANEL_FIELD_SELECTION"), wxPanel)->Enable(true);
   GET_CONTROL(*pToolWindow_, wxT("ID_COLOUR_BACKGROUND"), wxColourPickerCtrl)
      ->Enable(true);

   if (pVecStyle_) {
      bool haslabel =
            pVecStyle_->GetLabel() ? pVecStyle_->GetLabel()->id_
                                           != VectorStyle::Label::NoLabel :
                                     false;
      GET_CONTROL(*pToolWindow_, wxT("ID_CHECKBOX_LABEL_ENABLE"), wxCheckBox)
         ->SetValue(haslabel);
      int labelid =
            (!haslabel) ? VectorStyle::Label::Default : pVecStyle_->GetLabel()->id_;
      GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_FONT_TYPE"), wxChoice)
         ->SetSelection(labelid > 0? labelid -1 : 0);
   }

   SetInitialValues();

   if (this->hasFieldCombo_ && this->selectedField_ == -1) {
      // enabled_ = false;
   }

   return true;
}

// ------------------------------------------------------------------------------
/**
 * Inicializa los controles con los datos que tiene el Estilo del Vector, es
 * decir, carga en los controles los datos guardados en la clase Label del
 * VectorStyle. Si no existen tales datos, (Label==NULL) carga los controles con
 * datos default.
 */
void AnotationPropertiesPart::SetInitialValues() {
   LoadControls();
   UpdateStyle();
	this->Enable();
   if (!pVecStyle_) {
		vstyles_.clear();
      vector::render::CategorizedVectorRenderer::CategorizedParameters params = 
					vector::render::CategorizedVectorRenderer::GetParameters(pVectorLayer_->GetElement()->GetNode(""));
		size_t activelayer = dynamic_cast<VectorElement*>(pVectorLayer_->GetElement())->GetActiveLayer();                                                  
      std::vector< std::pair<std::string, std::string> >::iterator stylesit = params.categorizedlayerstyle_[activelayer].begin();             
		for (; stylesit != params.categorizedlayerstyle_[activelayer].end(); ++stylesit) {
			vstyles_.push_back(stylesit->second);
		}
		pVecStyle_ = VectorStyle::Create(vstyles_[0]);
   }

   if (!hasFieldCombo_) {
      // Verifico integridad de datos
      // TODO(Sebastian - TCK #839): En este codigo es cuando se fuerzan los valores para arreglar
      // inconsistencias. Es correcto al inicio, pero es indeseado en las actualizaciones.
      if (pVecStyle_->GetLabel()->id_ == 0) {
         pVecStyle_->GetLabel()->id_ = VectorStyle::Label::Default;
      }
      pVecStyle_->GetSymbol()->id_ = VectorStyle::Symbol::NoSymbol;
      // Borra el checkbox y el panel que contiene al combo de seleccion de campo
      Hide(wxT("ID_CHECKBOX_LABEL_ENABLE"));
   } else {
      GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_FIELD"), wxChoice)
         ->SetSelection(selectedField_);
   }

   VectorStyle::Label *plabel = pVecStyle_->GetLabel();
   if (!plabel) {
      GET_CONTROL(*pToolWindow_, "ID_CHECKBOX_LABEL_ENABLE", wxCheckBox)
         ->SetValue(false);
      isLabelChecked_ = false;
      GET_CONTROL(*pToolWindow_, "ID_CHOICE_FONT_TYPE", wxChoice)
         ->SetSelection(VectorStyle::Label::Default);
      GET_CONTROL(*pToolWindow_, "ID_SPINCTRL_FONT_SIZE", wxSpinCtrl)->SetValue(1);
      GET_CONTROL(*pToolWindow_, "ID_CHK_BOLD", wxCheckBox)->SetValue(false);
      GET_CONTROL(*pToolWindow_, "ID_CHK_ITALIC", wxCheckBox)->SetValue(false);
      GET_CONTROL(*pToolWindow_, "ID_CHK_UNDERLINE", wxCheckBox)->SetValue(false);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_FONT", wxColourPickerCtrl)
         ->SetColour(wxColour(0, 0, 0, 0));
      GET_CONTROL(*pToolWindow_, "ID_CHK_BACKGROUND", wxCheckBox)->SetValue(false);
      GET_CONTROL(*pToolWindow_, "ID_COLOUR_BACKGROUND", wxColourPickerCtrl)
         ->SetColour(wxColour(255, 255, 255, 0));
#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
      GET_CONTROL(*pToolWindow_, "ID_TXT_ANOTATION", wxTextCtrl)->
      SetValue(wxT("") );
#endif
      return;
   }

   // Seteo tipo de fuente
   if (plabel->id_ != 0) {
      GET_CONTROL(*pToolWindow_, "ID_CHECKBOX_LABEL_ENABLE", wxCheckBox)
         ->SetValue(true);
      isLabelChecked_ = true;
      GET_CONTROL(*pToolWindow_, "ID_CHOICE_FONT_TYPE", wxChoice)
         ->SetSelection((static_cast<int> (plabel->id_)) - 1);
      GET_CONTROL(*pToolWindow_, "ID_PANEL_LABEL_CONFIGURATION_PARENT", wxPanel)
         ->Enable(true);
      GET_CONTROL(*pToolWindow_, "ID_PANEL_FIELD_SELECTION", wxPanel)->Enable(true);
   } else {
      GET_CONTROL(*pToolWindow_, "ID_CHECKBOX_LABEL_ENABLE", wxCheckBox)
         ->SetValue(false);
      GET_CONTROL(*pToolWindow_, "ID_PANEL_LABEL_CONFIGURATION_PARENT", wxPanel)
         ->Enable(false);
      GET_CONTROL(*pToolWindow_, "ID_PANEL_FIELD_SELECTION", wxPanel)->Enable(false);
   }

   // Seteo el size de la fuente
   GET_CONTROL(*pToolWindow_, "ID_SPINCTRL_FONT_SIZE", wxSpinCtrl)
      ->SetValue(plabel->size_);

   // Seteo efectos de la fuente
   GET_CONTROL(*pToolWindow_, "ID_CHK_BOLD", wxCheckBox)->SetValue(plabel->bold_);
   GET_CONTROL(*pToolWindow_, "ID_CHK_ITALIC", wxCheckBox)->SetValue(plabel->italic_);
   GET_CONTROL(*pToolWindow_, "ID_CHK_UNDERLINE", wxCheckBox)
      ->SetValue(plabel->underlined_);

   // Seteo el color de fuente y el color de fondo de la fuente
   wxColour fontcolour(plabel->color_.red_, plabel->color_.green_, plabel->color_.blue_,
                       plabel->color_.alpha_);
   GET_CONTROL(*pToolWindow_, "ID_COLOUR_FONT", wxColourPickerCtrl)
      ->SetColour(fontcolour);
   wxColour bkgcolour(plabel->backColor_.red_, plabel->backColor_.green_,
                      plabel->backColor_.blue_, plabel->backColor_.alpha_);
   GET_CONTROL(*pToolWindow_, "ID_COLOUR_BACKGROUND", wxColourPickerCtrl)
      ->SetColour(bkgcolour);
   bool opaque = (plabel->backColor_.alpha_ != 0);
   GET_CONTROL(*pToolWindow_, "ID_CHK_BACKGROUND", wxCheckBox)->SetValue(opaque);

   // Seteo la orientacion (estrategia de posicionamiento)
   GET_CONTROL(*pToolWindow_, "ID_CHOICE_ORIENTATION", wxChoice)
      ->SetSelection(plabel->anchor_);

   // Seteo el campo
   GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_FIELD"), wxChoice)
      ->SetSelection(selectedField_);

#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   Element* pelement = pVectorLayer_ ? pVectorLayer_->GetElement() : pElement_;
   GET_CONTROL(*pToolWindow_, "ID_TXT_ANOTATION", wxTextCtrl)->
   SetValue(pelement->GetAnotationFieldValue() );
#endif

   updatePreview_ = true;
}

// ------------------------------------------------------------------------------
/**
 * Verifica si los datos que tenia guardados son distintos de los que hay
 * cargados en los controles.
 * @return true Los datos cambiaron.
 * @return false Los datos no cambiaron.
 */
bool AnotationPropertiesPart::HasChanged() {
   updatePreview_ = false;
   Element* pelement = pVectorLayer_ ? pVectorLayer_->GetElement() : pElement_;
   VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pelement);

   if (!pvectorelement) {
      return updatePreview_;
   }
   /** TODO(Gabriel - #5427): Se debe actualizar para las anotaciones sobre estilos
    * clasificados */
   // Genero un VectorStyle con los parametros anteriores a las modificaciones.
   VectorStyle *pvecstyle = NULL;
   std::string style = pvectorelement->GetStyle(pvectorelement->GetActiveLayer());

   pvecstyle = VectorStyle::Create(style);
   if (!pvecstyle) {
       vector::render::CategorizedVectorRenderer::CategorizedParameters params = 
					vector::render::CategorizedVectorRenderer::GetParameters(pVectorLayer_->GetElement()->GetNode(""));
		size_t activelayer = dynamic_cast<VectorElement*>(pVectorLayer_->GetElement())->GetActiveLayer();                                                  
      if (params.categorizedlayerstyle_[activelayer].empty())
			return updatePreview_;
		std::vector< std::pair<std::string, std::string> >::iterator stylesit = params.categorizedlayerstyle_[activelayer].begin();             
		pvecstyle = VectorStyle::Create(stylesit->second);
   }

   VectorStyle::Label *plabel = pvecstyle ? pvecstyle->GetLabel() : NULL;
   if (!plabel) {
      VectorStyle::Destroy(pvecstyle);
      return updatePreview_;
   }

   // Obtengo los valores de los controles
   int selectedfont, sizefont, anchor, selectedfield;
   bool bold, italic, underline, validate, opaque, labelchecked, backgroundchecked;
   wxColour colourfont, colourbkg;
   GetValuesFromControl(selectedfont, sizefont, bold, italic, underline, colourfont,
                            opaque, colourbkg, validate, labelchecked, selectedfield,
                            backgroundchecked, anchor);
   if (!validate) {
      VectorStyle::Destroy(pvecstyle);
      return updatePreview_;
   }

   wxColour colorfontusr(plabel->color_.red_, plabel->color_.green_,
                         plabel->color_.blue_, plabel->color_.alpha_);

   wxColour colorbkgusr(plabel->backColor_.red_, plabel->backColor_.green_,
                        plabel->backColor_.blue_, plabel->backColor_.alpha_);

#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   /* Hago esta verificacion ya que este metodo se llama dde el UIUpdate del
    * PartContainerWidget, y con este codigo se accede a la base de datos del vector.
    */
   if ( USE_CONTROL(*pToolWindow_, "ID_TXT_ANOTATION", wxTextCtrl, IsModified(), false)) {
      wxString savedvalue = pvectorelement->GetAnotationFieldValue();
#ifdef __UNUSED_CODE__
      // No funciona (asi como esta) el metodo TransferDataFromWindow()
      pToolWindow_->TransferDataFromWindow();
#endif
      textAnotation_ = USE_CONTROL(*pToolWindow_, "ID_TXT_ANOTATION", wxTextCtrl,
            GetValue(), wxT("") );
      /* \TODO(03/04/2009 - Alejandro): esta comparacion deberia hacerse en
       * el if siguiente que es donde se decide si hubo o no cambios.*/
      if (savedvalue.Cmp(textAnotation_) != 0) {
         updatePreview_ = true;
      }
   }
#endif

   bool opaqueaux = (plabel->backColor_.alpha_ != 0);

   if (labelchecked && ((selectedfont != (plabel->id_ - 1) && plabel->id_ > 0)|| sizefont != plabel->size_
         || bold != plabel->bold_ || italic != plabel->italic_
         || underline != plabel->underlined_ || colourfont != colorfontusr
         || opaque != opaqueaux || (opaque ? colourbkg != colorbkgusr : false)
         || plabel->anchor_ != anchor
         || (labelchecked ? (plabel->id_ == 0) : (plabel->id_ > 0))
         || selectedField_ != selectedfield)) {
      updatePreview_ = true;
   }

   VectorStyle::Destroy(pvecstyle);
   return updatePreview_ || (labelchecked != isLabelChecked_);
}

// ------------------------------------------------------------------------------
/**
 * Salva los cambios realizados en el Part.
 * \post Setea modified_ == false
 * @return true en caso de exito.
 * @return false C.O.C.
 */
bool AnotationPropertiesPart::CommitChanges() {
   // Obtengo el nuevo wkt y lo seteo a los parametros para generar
   // el nuevo nodo de renderizacion
   int selectedfont, sizefont, anchor, selectedfield;
   bool bold, italic, underline, validate, opaque, labelchecked, backgroundchecked;
   wxColour colourfont, colourbkg;
   GetValuesFromControl(selectedfont, sizefont, bold, italic, underline, colourfont,
                            opaque, colourbkg, validate, labelchecked, selectedfield,
                            backgroundchecked, anchor);
   Element* pelement = pVectorLayer_ ? pVectorLayer_->GetElement() : pElement_;
   VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pelement);
   //VectorStyle::Destroy(pVecStyle_);
   VectorStyle* pvecstyle = VectorStyle::Create(pvectorelement->GetStyle(pvectorelement->GetActiveLayer()));
	// caso estilo simple
	if (pvecstyle) {
   	if (!labelchecked) {
	      pvecstyle->GetLabel()->id_ = VectorStyle::Label::NoLabel;
	   } else {
	      SetLabel(pvecstyle);
	      SetAnchor(pvecstyle);
	      SetPlacement(pvecstyle);
	      SetExpresion(pvecstyle);
	      SetAngle(pvecstyle);
   	   SetDx(pvecstyle);
	      SetDy(pvecstyle);
	   }
	   pvectorelement->SetStyle(pvecstyle->GetWkt(), 0);
	} else {
		// caso estilo clasificado
	   vector::render::CategorizedVectorRenderer::CategorizedParameters params = 
					vector::render::CategorizedVectorRenderer::GetParameters(pVectorLayer_->GetElement()->GetNode(""));
		size_t activelayer = dynamic_cast<VectorElement*>(pVectorLayer_->GetElement())->GetActiveLayer();                                                  
      std::vector< std::pair<std::string, std::string> >::iterator stylesit = params.categorizedlayerstyle_[activelayer].begin();             
		for (; stylesit != params.categorizedlayerstyle_[activelayer].end(); ++stylesit) {
			VectorStyle* pvs = VectorStyle::Create(stylesit->second);
	   	if (!labelchecked) {
	      	pvs->GetLabel()->id_ = VectorStyle::Label::NoLabel;
	   	} else {
		      SetLabel(pvs);
		      SetAnchor(pvs);
		      SetPlacement(pvs);
		      SetExpresion(pvs);
		      SetAngle(pvs);
	   	   SetDx(pvs);
		      SetDy(pvs);
		   }
			stylesit->second = pvs->GetWkt();
			VectorStyle::Destroy(pvs);
		}
		wxXmlNode* prender = vector::render::CategorizedVectorRenderer::GetXmlNode(params);
		// actualizo el nodo de renderizacion
		pvectorelement->AddNode(pVectorLayer_->GetElement()->GetNode(""), prender, true);
	}
   // Actualizo el campo de las etiquetas
   if (this->hasFieldCombo_) {
      this->selectedField_ = USE_CONTROL(*pToolWindow_, "ID_CHOICE_FIELD", wxChoice,
            GetCurrentSelection(), -1);
   }
#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   if ( !SetAnotationText() ) {
      REPORT_DEBUG("D:Error al intentar guardar texto de la anotacion.");
   }
#endif
   isLabelChecked_ = labelchecked;
   pelement->SetChanged();
   pelement->SendViewerUpdate();
   modified_ = false;

   return true;
}

// ------------------------------------------------------------------------------
/**
 * Restaura los valores originales del Part.
 * @return true.
 */
bool AnotationPropertiesPart::RollbackChanges() {
   SetInitialValues();
   return true;
}

// ------------------------------------------------------------------------------
/**
 * Este metodo se ejecuta cuando sucede un evento de UIUpdate, el cual se ejecuta
 * cuando la cola de eventos de usuario (que maneja wx) esta vacia.
 * Verifica flag de actualizacion y si corresponde actualiza el preview de la
 * Anotacion. Actualiza la clase Label del Estilo del Vector con los datos que
 * se encuentran en los controles.
 * @param[in]  Event: evento.
 * \post updatePreview_ toma valor false
 */
void AnotationPropertiesPart::OnUIUpdate(wxUpdateUIEvent &Event) {
   if (!updatePreview_) {
      return;
   }

   SetLabel(pVecStyle_);
   SetAnchor(pVecStyle_);
   SetPlacement(pVecStyle_);
   SetExpresion(pVecStyle_);
   SetAngle(pVecStyle_);
   SetDx(pVecStyle_);
   SetDy(pVecStyle_);

   wxBitmap bmp;
   int width, height;
   GET_CONTROL(*pToolWindow_, "ID_BMP_PREVIEW", wxStaticBitmap)
      ->GetClientSize(&width, &height);

   /* Genero un VectorElement para obtener el preview del elemento;
    * Sino se hace esto, habria que guardar el elemento (CommitChanges)
    * para poder obtener el preview actualizado.
    */
   Element* pelement = pVectorLayer_ ? pVectorLayer_->GetElement() : pElement_;
   if (!pelement) {
      return;
   }

   VectorElement *pnewvectorelement = MemoryVectorElement::Create("/preview", "", 1,
                                                                  Vector::Point);
   Element *pvectortruch = pnewvectorelement;
   VectorStyle *ppreviewstyle = VectorStyle::Create(Vector::Point, "Label Preview");

   pVecStyle_->CopyLabel(ppreviewstyle);
   pVecStyle_->CopyPen(ppreviewstyle);
   pVecStyle_->CopyBrush(ppreviewstyle);
   pVecStyle_->CopySymbol(ppreviewstyle);

   // Seteo el estilo copiado para hacer el preview
   pnewvectorelement->SetStyle(ppreviewstyle->GetWkt(), 0);
   pnewvectorelement->GetPreview(bmp, width, height);

   Element::Destroy(pvectortruch);
   VectorStyle::Destroy(ppreviewstyle);

   GET_CONTROL(*pToolWindow_, "ID_BMP_PREVIEW", wxStaticBitmap)->SetBitmap(bmp);

   updatePreview_ = false;
   Event.Skip();
}

// ------------------------------------------------------------------------------
/**
 * Este metodo se ejecuta cuando se hace click sobre el checkbox "Etiqueta Visible"
 * Si esta seleccionado, las opciones son visible. En caso contrario, no lo son.
 * @param[in]  Event: evento.
 * \post Si esta seleccionado, las opciones son visible. En caso contrario, no lo son.
 */
void AnotationPropertiesPart::OnCheckboxLabelEnableClicked(wxCommandEvent &Event) {
   bool islabelchecked = USE_CONTROL(*pToolWindow_, "ID_CHECKBOX_LABEL_ENABLE",
         wxCheckBox, IsChecked(), false);

   GET_CONTROL(*pToolWindow_, "ID_PANEL_LABEL_CONFIGURATION_PARENT", wxPanel)
      ->Enable(islabelchecked);

   GET_CONTROL(*pToolWindow_, "ID_PANEL_FIELD_SELECTION", wxPanel)->Enable(islabelchecked);
}

/**
 * Actualiza el VectorStyle a partir del elemento (pElement).
 */
void AnotationPropertiesPart::UpdateStyle() {
   if (pVecStyle_) {
      VectorStyle::Destroy(pVecStyle_);
   }
   Element* pelement = pVectorLayer_ ? pVectorLayer_->GetElement() : pElement_;
   VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pelement);
   pVecStyle_ = VectorStyle::Create(pvectorelement->GetStyle(pvectorelement->GetActiveLayer()));
   if (!pVecStyle_) {
      return;
   }

   // Si tengo una anotacion (no etiqueta), apago el simbolo
   if (!hasFieldCombo_) {
      VectorStyle::Symbol *psymbol = pVecStyle_->GetSymbol();
      if (psymbol) {
         psymbol->id_ = VectorStyle::Symbol::NoSymbol;
         psymbol->size_ = 1;
      }
   }
}

/**
 * Obtiene el icono para la ventana de propiedades especificas de la anotacion.
 * @param[in]  X: sin utilizar
 * @param[in]  Y: sin utilizar
 * @param[out] ToolBitmap: bitmap con el icono de la ventana.
 */
void AnotationPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_ANOTATION_PROPERTIES, ToolBitmap);
}

/**
 * Carga los controles con los valores que pueden ser seleccionados por el usuario.
 * Estos valores estan definidos en la clase Label del VectorStyle.
 */
void AnotationPropertiesPart::LoadControls() {
   if (!pVecStyle_) {
      return;
   }

   VectorStyle::Label *plabel = pVecStyle_->GetLabel();
   if (!plabel) {
      return;
   }

   // Empieza desde 1 para no agregar al control el valor "Sin Etiqueta"
   for (size_t i = 1; i < VectorStyle::Label::CountLabel; i++) {
      GET_CONTROL(*pToolWindow_, "ID_CHOICE_FONT_TYPE", wxChoice)
         ->Append(plabel->GetTypeAsString(i));
   }
   std::string fieldselect =
         trim(trim(plabel->expression_, EXPRESSION_TOKEN_INIT),
              EXPRESSION_TOKEN_FINISH);
   Element* pelement = pVectorLayer_ ? pVectorLayer_->GetElement() : pElement_;
   VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pelement);

   bool success = false;
   if (hasFieldCombo_) {
      GET_CONTROL(*pToolWindow_, "ID_CHOICE_FIELD", wxChoice)->Clear();
      std::string url;
      url = WS2S((pvectorelement->GetUrl()));
      VectorEditor editor;
      bool successVector = editor.OpenVector(url);
      if (successVector) {
         bool successLayer = editor.OpenLayer(pvectorelement->GetActiveLayer());
         if (successLayer) {
            int fieldcount = editor.GetFieldCount();
            for (int i = 0; i < fieldcount; i++) {
               std::string fieldname = editor.GetFieldName(i);
               this->fieldList_.push_back(fieldname);
               int index =
                     USE_CONTROL(*pToolWindow_, "ID_CHOICE_FIELD", wxChoice,
                                 Append(fieldname.c_str()), wxNOT_FOUND);
               if (index != wxNOT_FOUND &&
                     fieldselect.compare(fieldname) == 0)
                  selectedField_ = index;
            }
            if (fieldcount > 0) {
               this->selectedField_ = selectedField_ >= 0? selectedField_ : 0;
               GET_CONTROL(*pToolWindow_, "ID_CHOICE_FIELD", wxChoice)
                  ->SetSelection(this->selectedField_);
               success = true;
            }
         }
      }
   }
   if (!success) {
      this->selectedField_ = -1;
   }

#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
   // obtengo el texto de la anotacion.
   textAnotation_ = pElement_->GetAnotationFieldValue();
   pToolWindow_->TransferDataToWindow();
#endif
}

/**
 * Actualiza el Label del VectorStyle, setea el tipo y el tamanio de la letra.
 * @return true en caso de que la actualizacion tenga exito.
 * @return false C.O.C.
 */
bool AnotationPropertiesPart::SetLabel(VectorStyle* pVectorStyle) {
   VectorStyle::Label *plabel = NULL;
   if (!GetLabel(plabel, pVectorStyle))
      return false;

   // Obtengo los valores de los controles
   int selectedfont, sizefont, anchor, selectedfield;
   bool bold, italic, underline, validate, opaque, labelchecked, backgroundchecked;
   wxColour colourfont, colourbkg;
   GetValuesFromControl(selectedfont, sizefont, bold, italic, underline, colourfont,
                            opaque, colourbkg, validate, labelchecked, selectedfield,
                            backgroundchecked, anchor);

   if (!validate) {
      return false;
   }

   // Seteo los valores
   if (!labelchecked) {
      plabel->id_ = VectorStyle::Label::NoLabel;
      return true;
   } else {
      plabel->id_ = (VectorStyle::Label::LabelType) (selectedfont + 1);
      GET_CONTROL(*pToolWindow_, "ID_CHOICE_FONT_TYPE", wxChoice)
         ->SetSelection(selectedfont);
   }
   plabel->size_ = sizefont;
   plabel->bold_ = bold;
   plabel->italic_ = italic;
   plabel->underlined_ = underline;
   plabel->color_.red_ = colourfont.Red();
   plabel->color_.green_ = colourfont.Green();
   plabel->color_.blue_ = colourfont.Blue();
   plabel->color_.alpha_ = colourfont.Alpha();
   plabel->backColor_.red_ = colourbkg.Red();
   plabel->backColor_.green_ = colourbkg.Green();
   plabel->backColor_.blue_ = colourbkg.Blue();
   // TODO(Gabriel - TCK #4782): Fix rapido para poder configurar estilo de color "negro"
   if (colourfont.Red() == 0 && colourfont.Green() == 0 && colourfont.Blue() == 0) {
      plabel->color_.red_ = 1;
      plabel->color_.green_ = 1;
      plabel->color_.blue_ = 1;
   }
   if (colourbkg.Red() == 0 && colourbkg.Green() == 0 && colourbkg.Blue() == 0) {
      plabel->backColor_.red_ = 1;
      plabel->backColor_.green_ = 1;
      plabel->backColor_.blue_ = 1;
   }
   plabel->backColor_.alpha_ = opaque ? 255 : 0;
   plabel->anchor_ = (VectorStyle::Label::AnchorType) (anchor);

   return true;
}

/**
 * Actualiza el Label del VectorStyle, setea el punto de anclaje.
 * @return true en caso de exito. false C.O.C.
 */
bool AnotationPropertiesPart::SetAnchor(VectorStyle* pVectorStyle) {
   VectorStyle::Label *plabel = NULL;
   if (!GetLabel(plabel, pVectorStyle))
      return false;

   // \TODO: Se asume que el mapeo de los tipos de posicion son los mismos
   // que los que se levantan en la interfaz grafica.
   // Seria bueno que el mapeo sea explicito en el codigo.
   int anchor = USE_CONTROL(*pToolWindow_, "ID_CHOICE_ORIENTATION",
         wxChoice, GetCurrentSelection(), -1);

   plabel->anchor_ = (VectorStyle::Label::AnchorType) anchor;

   return true;
}

/**
 * Actualiza el Label del VectorStyle, setea el punto donde debe establecerse
 * la anotacion.
 * @return true.
 */
bool AnotationPropertiesPart::SetPlacement(VectorStyle* pVectorStyle) {
   // \TODO: No esta implementado (Revisar si es necesario)
   return true;
}

/**
 * Actualiza el Label del VectorStyle, setea la expresion.
 * @return true.
 * \TODO: Utilizar un define para el formato de la expresion
 */
bool AnotationPropertiesPart::SetExpresion(VectorStyle* pVectorStyle) {
   VectorStyle::Label *plabel = NULL;
   if (!GetLabel(plabel, pVectorStyle))
      return false;
   plabel->expression_ = wxT("{");
   if (!hasFieldCombo_) {
      plabel->expression_ += wxT(SUR_ANOTATION_FIELD);
   } else {
      int selection = USE_CONTROL(*pToolWindow_, "ID_CHOICE_FIELD",
            wxChoice, GetCurrentSelection(), -1);
      if (selection != -1) {
         plabel->expression_ += this->fieldList_.at(selection);
      }
   }
   plabel->expression_ += wxT("}");
   return true;
}

/**
 * Actualiza el Label del VectorStyle, setea el angulo de inclinacion de la
 * anotacion.
 * @return true.
 */
bool AnotationPropertiesPart::SetAngle(VectorStyle* pVectorStyle) {
   VectorStyle::Label *plabel = NULL;
   if (!GetLabel(plabel, pVectorStyle))
      return false;

   plabel->angle_ = 0;
   return true;
}

/**
 * Actualiza el Label del VectorStyle, setea el desplazamiento en X de la
 * anotacion.
 * @return true.
 */
bool AnotationPropertiesPart::SetDx(VectorStyle* pVectorStyle) {
   VectorStyle::Label *plabel = NULL;
   if (!GetLabel(plabel, pVectorStyle))
      return false;

   plabel->dx_ = 0;
   return true;
}

/**
 * Actualiza el Label del VectorStyle, setea el desplazamiento en Y de la
 * anotacion.
 * @return true.
 */
bool AnotationPropertiesPart::SetDy(VectorStyle* pVectorStyle) {
   VectorStyle::Label *plabel = NULL;
   if (!GetLabel(plabel, pVectorStyle))
      return false;

   plabel->dy_ = 0;
   return true;
}

#ifdef __SHOW_TEXTCTRL_FOR_EDIT_ANOTATION__
/**
 * Actualiza el Label del VectorStyle, setea el texto de la anotacion.
 * @return true en caso de exito
 * @return false C.O.C.
 */
bool AnotationPropertiesPart::SetAnotationText() {
   pToolWindow_->TransferDataFromWindow();
   pElement_->SetAnotationName(textAnotation_);
   GET_CONTROL(*pToolWindow_, "ID_TXT_ANOTATION", wxTextCtrl)->DiscardEdits();
   return true;
}
#endif

/** Devuelve si la parte tiene datos validos */
bool AnotationPropertiesPart::HasValidData() {
   int selectedfont, sizefont, anchor, selectedfield;
   bool bold, italic, underline, validate, opaque, labelchecked, backgroundchecked;
   wxColour colourfont, colourbkg;
   if (!IsEnabled())
      return true;

   GetValuesFromControl(selectedfont, sizefont, bold, italic, underline, colourfont,
                        opaque, colourbkg, validate, labelchecked, selectedfield,
                        backgroundchecked, anchor);
   return validate;
}


/** Obtiene los valores configurados por el usuario **/
void AnotationPropertiesPart::GetValuesFromControl(int &SelectedFont, int &SizeFont,
                                                   bool &Bold, bool &Italic,
                                                   bool &Underline, wxColour& ColorFont,
                                                   bool &Opaque, wxColour& ColorBkg,
                                                   bool &Validate, bool &LabelChecked,
                                                   int &SelectedField,
                                                   bool &BackgroundChecked,
                                                   int &Anchor) {
      /* Obtengo los valores de los controles */
      LabelChecked = USE_CONTROL(*pToolWindow_, "ID_CHECKBOX_LABEL_ENABLE",
                                 wxCheckBox, GetValue(), 0);
   if (!LabelChecked) {
         Validate = true;
         return;
      }

      SelectedField = USE_CONTROL(*pToolWindow_, "ID_CHOICE_FIELD",
                                  wxChoice, GetCurrentSelection(), -1);
      BackgroundChecked = USE_CONTROL(*pToolWindow_, "ID_CHK_BACKGROUND",
                                      wxCheckBox, GetValue(), 0);
      Anchor = USE_CONTROL(*pToolWindow_, "ID_CHOICE_ORIENTATION",
                           wxChoice, GetCurrentSelection(), -1);
      SelectedFont = USE_CONTROL(*pToolWindow_, "ID_CHOICE_FONT_TYPE",
                                 wxChoice, GetCurrentSelection(), -1);
      SizeFont = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_FONT_SIZE",
                             wxSpinCtrl, GetValue(), 0);
      Bold = USE_CONTROL(*pToolWindow_, "ID_CHK_BOLD", wxCheckBox,
                         GetValue(), 0);
      Italic = USE_CONTROL(*pToolWindow_, "ID_CHK_ITALIC", wxCheckBox,
                           GetValue(), 0);
      Underline = USE_CONTROL(*pToolWindow_, "ID_CHK_UNDERLINE", wxCheckBox,
                              GetValue(), 0);
      ColorFont = USE_CONTROL(*pToolWindow_, "ID_COLOUR_FONT",
                              wxColourPickerCtrl, GetColour(), wxNullColour);
      Opaque = USE_CONTROL(*pToolWindow_, "ID_CHK_BACKGROUND", wxCheckBox,
                           GetValue(), false);
      ColorBkg = USE_CONTROL(*pToolWindow_, "ID_COLOUR_BACKGROUND",
                             wxColourPickerCtrl, GetColour(), wxNullColour);
      /* Verifico que no esten fuera de rango */
      Validate = true;
      if (SelectedFont < 0 || SelectedFont >= VectorStyle::Label::CountLabel
           || Anchor < 0 || Anchor >= 9
           || SizeFont <= 0 || SizeFont > 72
           || ColorFont == wxNullColour
           || ColorBkg == wxNullColour ) {
         Validate = false; }
}

/** obtiene el label asociado al estilo **/
bool AnotationPropertiesPart::GetLabel(VectorStyle::Label* &pLabel, VectorStyle* pVectorStyle) {
   if (!pVecStyle_) {
      return false;
   }
   pLabel = pVectorStyle->GetLabel();
   if (!pLabel) {
      return false;
   }
   return true;
}
}  // namespace suri
