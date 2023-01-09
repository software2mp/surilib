/** \file Viewer2DSelectorPart.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <map>
#include <vector>

#include "Viewer2DSelectorPart.h"

#include <wx/sizer.h>

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(Viewer2DSelectorPartEvent, Viewer2DSelectorPart)
      IMPLEMENT_EVENT_CALLBACK(OnChBaseSelectionChanged, OnChBaseSelectionChanged(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnChWarpSelectionChanged, OnChWarpSelectionChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

const std::string Viewer2DSelectorPart::kViewer = "Visualizador";

/**
 * Ctor
 */
Viewer2DSelectorPart::Viewer2DSelectorPart(bool Enable, bool Modified) :
      Part(wxT("ID_VIEWER_2D_SELECTOR_PART"), _(caption_VIEWER_2D_SELECTOR), Enable, Modified),
      NEW_EVENT_OBJECT(Viewer2DSelectorPartEvent) {

}

/**
 * Dtor
 */
Viewer2DSelectorPart::~Viewer2DSelectorPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Indica si el Part tiene cambios para salvar
 */
bool Viewer2DSelectorPart::HasChanged() {
   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_BASE"), wxChoice);

   wxChoice* pchwarp = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_WARP"), wxChoice);

   if (!AreEqualChoices()
         && (pchbase->GetSelection() != kInitialBasePos
               || pchwarp->GetSelection() != kInitialWarpPos))
      return true;

   return false;
}

/**
 * Salva los cambios realizados en el Part.
 * (No aplica para este part)
 */
bool Viewer2DSelectorPart::CommitChanges() {
   return !AreEqualChoices();
}

/**
 * Restaura los valor's originales del Part.
 * (No aplica para este part)
 */
bool Viewer2DSelectorPart::RollbackChanges() {
   return true;
}

/**
 * Inicializa el part
 */
void Viewer2DSelectorPart::SetInitialValues() {

}

/**
 * Actualiza el estado de la parte
 */
void Viewer2DSelectorPart::Update() {

}

/**
 * Modifica el nombre del viewer.
 */
bool Viewer2DSelectorPart::ConfigureWidget() {
   InitializePart();
   return true;
}

/**
 * Devuelve si la parte tiene datos validos
 */
bool Viewer2DSelectorPart::HasValidData() {
   return !AreEqualChoices();
}

/**
 * Configura los visualizadores disponibles.
 *
 * @param[in] CurrentViewers Vector con todos los visualizadores actualmente siendo utilizados.
 */
void Viewer2DSelectorPart::SetCurrentViewers(
      const std::map<SuriObject::UuidType, std::string>& CurrentViewers) {
   currentViewers_ = CurrentViewers;
}

/**
 * Obtiene los visualizadores seleccionados.
 *
 * @return Devuelve los dos visualizadores seleccionados.
 */
std::vector<SuriObject::UuidType> Viewer2DSelectorPart::GetSelectedViewers() const {
   std::vector<SuriObject::UuidType> retviewers;
   retviewers.push_back(baseId_);
   retviewers.push_back(warpId_);
   return retviewers;
}

/**
 * Inicializa el part realizando todas las configuraciones necesarias.
 */
void Viewer2DSelectorPart::InitializePart() {
   InitializeChoices();
   Initialize();
}

/**
 * Inicializa los controles choice para la seleccion de visualizadores.
 */
void Viewer2DSelectorPart::InitializeChoices() {
   InitializeChoiceBase();
   InitializeChoiceWarp();
}

/**
 * Inicializa el control choice para visualizador base.
 */
void Viewer2DSelectorPart::InitializeChoiceBase() {
   baseSelection_ = kInitialBasePos;

   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_BASE"), wxChoice);

   int lastindex = -1;
   std::map<SuriObject::UuidType, std::string>::iterator it;
   for (it = currentViewers_.begin(); it != currentViewers_.end(); ++it) {
      lastindex = pchbase->Append(it->second);
      pchbase->SetClientData(lastindex,
                             const_cast<char*>(it->first.c_str()));
   }

   pchbase->Select(baseSelection_);
   baseId_ = static_cast<char*>(pchbase->GetClientData(baseSelection_));

   pchbase->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(Viewer2DSelectorPartEvent::OnChBaseSelectionChanged),
         NULL, pEventHandler_);
}

/**
 * Inicializa el control choice para visualizador warp.
 */
void Viewer2DSelectorPart::InitializeChoiceWarp() {
   warpSelection_ = kInitialWarpPos;

   wxChoice* pchwarp = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_WARP"), wxChoice);

   int lastindex = -1;
   std::map<SuriObject::UuidType, std::string>::iterator it;
   for (it = currentViewers_.begin(); it != currentViewers_.end(); ++it) {
      lastindex = pchwarp->Append(it->second);
      pchwarp->SetClientData(lastindex,
                             const_cast<char*>(it->first.c_str()));
   }
   pchwarp->Select(warpSelection_);
   warpId_ = static_cast<char*>(pchwarp->GetClientData(warpSelection_));

   pchwarp->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(Viewer2DSelectorPartEvent::OnChWarpSelectionChanged),
         NULL, pEventHandler_);
}

/**
 * Inicializacion general.
 */
void Viewer2DSelectorPart::Initialize() {}

/**
 * Handler para el evento de cambio de seleccion de choice base.
 */
void Viewer2DSelectorPart::OnChBaseSelectionChanged(wxCommandEvent &Event) {
   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_BASE"), wxChoice);

   baseSelection_ = pchbase->GetSelection();
   baseId_ = static_cast<char*>(pchbase->GetClientData(baseSelection_));

   wxPanel* ppnlerr = XRCCTRL(*(this->GetWidget()->GetWindow()),
                              wxT("ID_VIEWER_2D_PANEL_ERR_MSG"), wxPanel);

   if (AreEqualChoices()) {
      ppnlerr->Show();
   } else {
      ppnlerr->Hide();
   }
   RefreshContainer();
}

/**
 * Handler para el evento de cambio de seleccion de choice warp.
 */
void Viewer2DSelectorPart::OnChWarpSelectionChanged(wxCommandEvent &Event) {
   wxChoice* pchwarp = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_WARP"), wxChoice);

   warpSelection_ = pchwarp->GetSelection();
   warpId_ = static_cast<char*>(pchwarp->GetClientData(warpSelection_));

   wxPanel* ppnlerr = XRCCTRL(*(this->GetWidget()->GetWindow()),
                              wxT("ID_VIEWER_2D_PANEL_ERR_MSG"), wxPanel);

   if (AreEqualChoices()) {
      ppnlerr->Show();
   } else {
      ppnlerr->Hide();
   }
   RefreshContainer();
}

/**
 * Verifica si los visualizadores seleccionados son distintos.
 *
 * @return true si esta seleccionado el mismo visualizador.
 */
bool Viewer2DSelectorPart::AreEqualChoices() const {
   if (baseId_.compare(warpId_) == 0) {
      return true;
   }
   return false;
}

/**
 * Redibujo panel padre (Fix luego de esconder o mostrar un elemento)
 */
void Viewer2DSelectorPart::RefreshContainer() {
   wxWindow* pframe = dynamic_cast<wxWindow*>(this->GetWidget()->GetFrame());
   wxPanel* pPanel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                 wxT("ID_VIEWER_2D_SELECTOR_PANEL"), wxPanel);
   pPanel->Layout();
   pframe->Fit();
}

}  // namespace suri
