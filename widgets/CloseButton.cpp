/*! \file CloseButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "CloseButton.h"

// Includes standard

// Includes suri
#include "suri/messages.h"
#include "resources.h"
#include "VectorElementEditor.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar : ventana donde se inserta el boton.
 * @param[in] pVectorElementEditor : elemento para la edicion del vector.
 */
CloseButton::CloseButton(wxWindow *pToolbar, VectorElementEditor* pVectorElementEditor) :
      Button(pToolbar, wxT(button_GPS_CLOSE), wxT(tooltip_CLOSE_GEOMETRY)),
      pVectorElementEditor_(pVectorElementEditor) {
}

/** dtor */
CloseButton::~CloseButton() {
}

/**
 * Cierra el feature abierto.
 * @param[in] Event evento click sobre el boton
 */
void CloseButton::OnButtonClicked(wxCommandEvent &Event) {
   pVectorElementEditor_->CloseFeature();
}
}  // namespace suri
