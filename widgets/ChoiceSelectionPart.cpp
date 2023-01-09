/*! \file ChoiceSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Inclues suri
#include "ChoiceSelectionPart.h"
#include "resources.h"
#include "RasterLayer.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/choice.h"
#include "wx/bmpbuttn.h"

/** Inicializa los recursos de la ventana */
void InitChoiceSelectionXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ChoiceSelectionPartEvent, ChoiceSelectionPart)IMPLEMENT_EVENT_CALLBACK(OnCheckboxChecked, OnCheck(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRefreshButton, Refresh(), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnListSelection, OnListSelection(), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 * @param[in] pElement elemento sobre el que se aplica el realce
 */
ChoiceSelectionPart::ChoiceSelectionPart(RasterElement * pElement) :
      Part(true, false), pElement_(pElement), pRasterLayer_(NULL),
      isActive_(true), selectedItem_(0),
      refresh_(false), NEW_EVENT_OBJECT(ChoiceSelectionPartEvent) {
   // Esto evita que el choice sea mas alto de lo que necesita
   wantedHeight_ = -1;
   wantedWidth_ = -1;
}

/** Ctor.
 * @param[in] pRasterLayer elemento sobre el que se aplica el realce
 */
ChoiceSelectionPart::ChoiceSelectionPart(RasterLayer * pRasterLayer) :
       Part(true, false), pElement_(NULL), pRasterLayer_(pRasterLayer),
       isActive_(true), selectedItem_(0),
            refresh_(false), NEW_EVENT_OBJECT(ChoiceSelectionPartEvent) {
   wantedHeight_ = -1;
   wantedWidth_ = -1;
}

/*
 * Destructor
 */
ChoiceSelectionPart::~ChoiceSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea la ventana de la herramienta.
 * @return true si pudo crear la ventana de la herramienta.
 * @return false si no pudo crear la ventana de la herramienta.
 */
bool ChoiceSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_CHOICE_SELECTION_PART"));
   Element* pelement = pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_;
   if (!pToolWindow_ || !pelement) {
      return false;
   }
   // Conecta el evento al boton refrescar
   GET_CONTROL(*pToolWindow_, wxT("ID_REFRESH_BUTTON"), wxBitmapButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                wxCommandEventHandler(ChoiceSelectionPartEvent::OnRefreshButton), NULL,
                pEventHandler_);
   // Conecta el evento a la lista
   GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_LIST"), wxChoice)
      ->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                wxCommandEventHandler(ChoiceSelectionPartEvent::OnListSelection), NULL,
                pEventHandler_);
   // Conecta el evento al checkbox
   GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_CHECKBOX"), wxCheckBox)
      ->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,
                wxCommandEventHandler(ChoiceSelectionPartEvent::OnCheckboxChecked),
                NULL, pEventHandler_);

   return true;
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void ChoiceSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   // GET_BITMAP_RESOURCE(icon_ELEMENT_CHOICE_SELECTION,ToolBitmap);
}

/**
 * Devuelve si la parte tuvo cambios.
 * @return modified_
 */
bool ChoiceSelectionPart::HasChanged() {
   return modified_;
}

/**
 *	Llama a ConfigureElement, si tiene exito setea modified_=false y llama a
 *	update del elemento.
 *	@return returnvalue true si se pudo configurar el elemento con los datos
 *	en el part.
 */
bool ChoiceSelectionPart::CommitChanges() {
   if (!ConfigureElement()) {
      SHOW_ERROR(errorMessage_);
      return false;
   }

   modified_ = false;
   refresh_ = false;
   Element* pelement = pRasterLayer_ ? pRasterLayer_->GetElement() : pElement_;
   pelement->SetChanged();
   pelement->SendViewerUpdate();
   ResetList();
   return true;
}

/**
 * Llama a ResetList, setea modified_=false.
 * @return informa si pudo deshacer los cambios.
 */
bool ChoiceSelectionPart::RollbackChanges() {
   bool returnvalue = ResetList();
   refresh_ = false;
   if (returnvalue) {
      modified_ = false;
   }
   return returnvalue;
}

/**
 * refresh_=false
 * Si ResetList(), setea modified_=false.
 */
void ChoiceSelectionPart::SetInitialValues() {
   refresh_ = false;
   if (ResetList()) {
      modified_ = false;
   }
}

/**
 * Llama a OnUpdate al comienzo del metodo.
 * Setea modified_=refresh. Si no coinciden el estado de activacion del choice
 * en la herramienta y en el elemento(isActive_) setea modified_ a true.
 * Si el choice esta activo y la opcion seleccionada no coincide con la
 * del elemento(selectedItem_) setea modified_ a true.
 */
void ChoiceSelectionPart::Update() {
   OnUpdate();
   modified_ = refresh_;
   if (isActive_ != IsChoiceActive()
         || (IsChoiceActive() && GetSelectedItem() != selectedItem_)) {
      modified_ = true;
   }
}

/**
 * Des/habilita la ventana de la herramienta
 * @param[in] State nuevo estado de activacion del choice
 */
void ChoiceSelectionPart::SetEnabled(bool State) {
   if (State == false) {
      ActivateChoice(false);
   }
   Part::SetEnabled(State);
   pToolWindow_->Enable(State);
}

/**
 * Retorna la posicion del item seleccionado en el choice
 * @return indice en choice del elemento seleccionado
 */
int ChoiceSelectionPart::GetSelectedItem() {
   return USE_CONTROL(*pToolWindow_, wxT("ID_CHOICE_LIST"),
         wxChoice, GetSelection(), wxNOT_FOUND);
}

/**
 * Actualiza el contenido del choice con los datos en choiceContent_ y
 * selecciona el item correspondiente(selectedItem_).
 */
void ChoiceSelectionPart::ConfigureChoiceList() {
   // limipio el contenido del choice
   GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_LIST"), wxChoice)->Clear();

   // Actualiza el contenido del choice con los datos en choiceContent_
   int count = choiceContent_.size();
   for (int index = 0; index < count; index++) {
      USE_CONTROL(*pToolWindow_, wxT("ID_CHOICE_LIST"), wxChoice,
                  Append(choiceContent_[index]), wxNOT_FOUND);
   }

   // Selecciona el item correspondiente(selectedItem_)
   GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_LIST"), wxChoice)->Select(selectedItem_);
}

/**
 * Modifica estado de activacion del checkbox, llama a ActivateControls,
 * setea refresh_=false y actualiza la pantalla.
 * @param[in] State Nuevo estado del choice
 */
void ChoiceSelectionPart::ActivateChoice(bool State) {
   GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_CHECKBOX"), wxCheckBox)->SetValue(State);
   ActivateControls(State);
   refresh_ = false;
   Update();
}

/**
 * Des/habilita la lista de realces y el boton refrescar(si esta desplegado)
 * @param[in] State nuevo estado de los controles
 */
void ChoiceSelectionPart::ActivateControls(bool State) {
   // Cambia el estado de habilitacion de la lista
   GET_CONTROL(*pToolWindow_, wxT("ID_CHOICE_LIST"), wxChoice)->Enable(State);
   // Cambia el estado de habilitacion del boton
   if (USE_CONTROL(*pToolWindow_, wxT("ID_REFRESH_BUTTON"), wxBitmapButton, IsShown(), false)) {
      GET_CONTROL(*pToolWindow_, wxT("ID_REFRESH_BUTTON"), wxBitmapButton)
         ->Enable(State);
   }
}

/**
 * Informa si el checkbox dentro del part esta activo.
 * @return valor del wxCheckbox
 */
bool ChoiceSelectionPart::IsChoiceActive() {
   return USE_CONTROL(*pToolWindow_, wxT("ID_CHOICE_CHECKBOX"), wxCheckBox, IsChecked(), false);
}

/**
 * Modifica el estado de refresh_ a true y llama a update
 */
void ChoiceSelectionPart::Refresh() {
   refresh_ = true;
   Update();
}

/**
 * Modifica el estado de activacion de los controles del part
 * @param[in] Event Evento check
 */
void ChoiceSelectionPart::OnCheck(wxCommandEvent &Event) {
   ActivateChoice(Event.IsChecked());
}

/**
 * Al modificar el item seleccionado se setea el flag refresh_ a falso.
 * Luego llama a update
 */
void ChoiceSelectionPart::OnListSelection() {
   refresh_ = false;
   Update();
}
}  // namespace suri
