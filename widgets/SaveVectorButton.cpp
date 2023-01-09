/*! \file SaveVectorButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SaveVectorButton.h"

// Includes suri
#include "resources.h"
#include "suri/messages.h"
#include "VectorElementEditor.h"

// Includes App

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar Ventana donde se incorpora el boton
 * @param[in] pVectorElementEditor VectorEditor
 * @return instancia de la clase SaveVectorButton
 */
SaveVectorButton::SaveVectorButton(wxWindow *pToolbar, GeometryElementEditor* pGeometryElementEditor,
                                   VectorElementEditor* pVectorElementEditor) :
    Button(pToolbar, wxT(button_SAVE_VECTOR), wxT(tooltip_SAVE)), pGeoEditor_(pGeometryElementEditor),
    pVectorElementEditor_(pVectorElementEditor) {
}

/**
 * Destructor
 */
SaveVectorButton::~SaveVectorButton() {
}

/**
 * Salva el vector en edicion.
 * @param[in] Event evento.
 */
void SaveVectorButton::OnButtonClicked(wxCommandEvent &Event) {
   if (pGeoEditor_) {
      pGeoEditor_->SaveVector();
      return;
   }
   if (pVectorElementEditor_) {
      pVectorElementEditor_->SaveVector();
   }
}

/**
 * Verifica las condiciones de activacion del boton.
 * @return true si hay un vector abierto
 * @return false si no hay un vector abierto
 */
bool SaveVectorButton::IsEnabled() {
   if (pGeoEditor_) {
      pGeoEditor_ && pGeoEditor_->IsVectorOpen();
      return true;
   }
   return pVectorElementEditor_ && pVectorElementEditor_->IsVectorOpen();
}
}  // namespace suri
