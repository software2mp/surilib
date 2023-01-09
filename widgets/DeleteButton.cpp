/*! \file DeleteButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "DeleteButton.h"

// Includes standard

// Includes suri
#include "suri/messages.h"
#include "VectorElementEditor.h"
#include "resources.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(DeleteButtonEvent, DeleteButton)IMPLEMENT_EVENT_CALLBACK(OnHideWithNoPointsUiUpdate, OnHideWithNoPointsUiUpdate(
            event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Ctor.
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pVectorElementEditor elemento para la edicion
 */
DeleteButton::DeleteButton(wxWindow *pToolbar,
                           VectorElementEditor* pVectorElementEditor) :
      Button(pToolbar, wxT(button_GPS_DELETE), wxT(tooltip_DELETE)),
      NEW_EVENT_OBJECT(DeleteButtonEvent),
      pVectorElementEditor_(pVectorElementEditor) {
}

/**
 * Dtor.
 */
DeleteButton::~DeleteButton() {
}

/**
 * Elimina el ultimo punto ingresado en el VectorEditor.
 * @param[in] Event evento click sobre el boton
 */
void DeleteButton::OnButtonClicked(wxCommandEvent &Event) {
   pVectorElementEditor_->DeletePoint(-1);
}

/**
 * Metodo de creacion del boton-herramienta
 * @return true si pudo crear la herramienta, false en otro caso
 */
bool DeleteButton::CreateToolWindow() {
   bool ret = Button::CreateToolWindow();
   if (ret) {
      pToolWindow_->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler
      (DeleteButtonEvent::OnHideWithNoPointsUiUpdate),
                            NULL, pEventHandler_);
   }
   return ret;
}

/**
 * Esconde boton cuando no hay puntos en vector en edicion
 * @param[in] Event evento UiUpdate
 */
void DeleteButton::OnHideWithNoPointsUiUpdate(wxUpdateUIEvent &Event) {
   Event.Skip(false);
   if (pVectorElementEditor_->GetPointCount() > 0) {
      ((wxWindow*) Event.GetEventObject())->Enable();
   } else {
      ((wxWindow*) Event.GetEventObject())->Enable(false);
   }
}
}  // namespace suri
