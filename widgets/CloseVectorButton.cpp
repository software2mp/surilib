/*! \file CloseVectorButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "CloseVectorButton.h"

// Includes suri
#include "suri/messages.h"
#include "VectorElementEditor.h"
#include "resources.h"

// Includes App

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pVectorElementEditor elemento para la edicion
 */
CloseVectorButton::CloseVectorButton(wxWindow *pToolbar, GeometryElementEditor* pGeometryElementEditor,
                                     VectorElementEditor* pVectorElementEditor) :
      Button(pToolbar, wxT(button_CLOSE_VECTOR), wxT(tooltip_DISCARD)), pGeometryElementEditor_(pGeometryElementEditor),
      pVectorElementEditor_(pVectorElementEditor) {
}
/**
 * Destructor
 */
CloseVectorButton::~CloseVectorButton() {
}

/**
 * Cierra el vector en edicion.
 * @param[in] Event evento click sobre el boton
 */
void CloseVectorButton::OnButtonClicked(wxCommandEvent &Event) {
   if (pGeometryElementEditor_ )
      pGeometryElementEditor_->CloseVector();
   else if (pVectorElementEditor_)
      pVectorElementEditor_->CloseVector();

   return;
}

/**
 * Verifica las condiciones de activacion del boton.
 * @return true si hay un vector abierto
 */
bool CloseVectorButton::IsEnabled() {

   if (pGeometryElementEditor_)
      return pGeometryElementEditor_->IsEditing();
   else
   {
      /** Si la insancia no es Geometry, por descarte es pVectorElementEditor_ */
      if (pVectorElementEditor_)
         return pVectorElementEditor_->IsVectorOpen();
      else
         return false;
   }
}

}  // namespace suri

