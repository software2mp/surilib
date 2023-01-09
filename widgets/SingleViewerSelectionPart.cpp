/*! \file SingleViewerSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <sstream>
#include <string>
#include <map>

#include "SingleViewerSelectionPart.h"

#include "wx/colour.h"
#include "wx/gdicmn.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(SingleViewerSelectionPartEvent, SingleViewerSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnChViewerChanged, OnChViewerChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

const std::string SingleViewerSelectionPart::SINGLE_VIEWER_SELECTION_XRC_NAME =
      wxT("ID_SINGLE_VIEWER_SELECTION_PANEL"); /*! Id del panel */
const std::string SingleViewerSelectionPart::kViewer = "Visualizador";

/** Ctor */
SingleViewerSelectionPart::SingleViewerSelectionPart() :
      Part(SINGLE_VIEWER_SELECTION_XRC_NAME, wxT(caption_SINGLE_VIEWER_PART)),
      NEW_EVENT_OBJECT(SingleViewerSelectionPartEvent) {
}

/** Dtor */
SingleViewerSelectionPart::~SingleViewerSelectionPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool SingleViewerSelectionPart::HasChanged() {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                  wxT("ID_VIEWER_SELECTION_CHOICE"), wxChoice);

   if (pchoice->GetSelection() != kInitialPos)
      return true;

   return false;
}

/** Salva los cambios realizados en el Part. */
bool SingleViewerSelectionPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool SingleViewerSelectionPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void SingleViewerSelectionPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void SingleViewerSelectionPart::Update() {
}

/**
 * Modifica el nombre del viewer.
 */
bool SingleViewerSelectionPart::ConfigureWidget() {
   InitializePart();
   return true;
}

/**
 * Configura los visualizadores disponibles.
 *
 * @param[in] CurrentViewers Vector con todos los visualizadores actualmente siendo utilizados.
 */
void SingleViewerSelectionPart::SetCurrentViewers(
      const std::map<suri::SuriObject::UuidType, std::string>& CurrentViewers) {
   currentViewers_ = CurrentViewers;
}

/**
 * Handler para el evento de cambio de seleccion de choice.
 */
void SingleViewerSelectionPart::OnChViewerChanged(wxCommandEvent &Event) {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_SELECTION_CHOICE"), wxChoice);

   selectedViewerId_ = static_cast<char*>(pchoice->GetClientData(pchoice->GetSelection()));
}

/**
 * Devuelve el visualizador seleccionado.
 *
 * @return Devuelve el id del visualizador seleccionado.
 */
SuriObject::UuidType SingleViewerSelectionPart::GetSelectedViewer() {
  return selectedViewerId_;
}

/**
 * Inicializa el part realizando todas las configuraciones necesarias.
 */
void SingleViewerSelectionPart::InitializePart() {
   InitializeChoice();
}

/**
 * Inicializa el combo de seleccion de visualizadores.
 */
void SingleViewerSelectionPart::InitializeChoice() {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_SELECTION_CHOICE"), wxChoice);

   int lastindex = -1;
   std::map<SuriObject::UuidType, std::string>::iterator it;
   for (it = currentViewers_.begin(); it != currentViewers_.end(); ++it) {
      lastindex = pchoice->Append(it->second);
      pchoice->SetClientData(lastindex, const_cast<char*>(it->first.c_str()));
   }

   pchoice->Select(kInitialPos);

   pchoice->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(SingleViewerSelectionPartEvent::OnChViewerChanged),
         NULL, pEventHandler_);

   selectedViewerId_ = static_cast<char*>(pchoice->GetClientData(pchoice->GetSelection()));
}

}  // namespace suri

