/*! \file Choice.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Choice.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LabelText.h"

// Includes Wx
#include "wx/choice.h"

// Defines
// forwards

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ChoiceEventHandler, Choice)
      IMPLEMENT_EVENT_CALLBACK(OnSelectionChanged, OnSelectionChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


/**
 * Constructor
 * @param pViewerToolbar referencia a la ventana que contendra el choice
 * @param Tooltip tooltip a configurar
 * @param Flag flag para la configuracion de eventos
 */
Choice::Choice(wxWindow *pViewerToolbar, const std::string& Tooltip, int Flag) :
      Widget(pViewerToolbar), NEW_EVENT_OBJECT(ChoiceEventHandler), enabled_(true) {
   windowTitle_ = _(Tooltip.c_str());
}

/** Destructor **/
Choice::~Choice() {
   DELETE_EVENT_OBJECT;
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool Choice::CreateToolWindow() {
   wxString choices[] = {""};
   pChoice_ = new wxChoice(pParentWindow_, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                           0, choices);
   pToolWindow_ = pChoice_;
   pChoice_->SetToolTip(windowTitle_);
   pChoice_->Enable(IsEnabled());
   pChoice_->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                         wxCommandEventHandler(ChoiceEventHandler::OnSelectionChanged),
                         NULL, pEventHandler_);
   OnCreateToolWindow();
   return false;
}

/** Metodo que se llama cuando se ejecuta CreateToolWindow **/
void Choice::OnCreateToolWindow() {
}

/** Obtiene la seleccion activa en el control grafico. */
std::string Choice::GetSelection() const {
   return pChoice_->GetStringSelection().c_str();
}

/** Indica la seleccion activa en el control grafico. */
void Choice::SetSelection(const std::string& Selection) {
   int pos = pChoice_->FindString(Selection.c_str());
   if (pos != wxNOT_FOUND)
      pChoice_->Select(pos);
}

/** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
void Choice::OnSelectionChanged(wxCommandEvent &Event) {
}

/** Setea la propiedad enabled_ */
void Choice::SetEnabled(bool Enabled) {
   pChoice_->Enable(Enabled);
   enabled_ = Enabled;
}

/** Verifica si el boton se puede o no habilitar */
bool Choice::IsEnabled() const {
   return enabled_;
}

/** Agrega una opcion al choice **/
void Choice::AddChoiceOption(const std::string& Option) {
   pChoice_->Append(Option.c_str());
}

/** Metodo que indica si posee un label asociado **/
bool Choice::HasLabel() const {
   return !label_.empty();
}

/** Metodo que obtiene el label **/
LabelText* Choice::GetLabel() const {
   return new LabelText(pParentWindow_, label_);
}
} /** namespace suri */
