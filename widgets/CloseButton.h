/*! \file CloseButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef CLOSEBUTTON_H_
#define CLOSEBUTTON_H_

// Includes standard
// Includes Suri
#include "suri/Button.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
// Forwards
class VectorElementEditor;

/** Boton que cierra el feature abierto en el vector que se esta editando */
/**
 * Boton que permite durante la edicion de vectores cerrar el feature abierto.
 * Este boton se agrega a las herramientas de edicion de que no responden a los
 * eventos de mouse (GPS).
 */
class CloseButton : public Button {
public:
   /** ctor */
   CloseButton(wxWindow *pToolbar, VectorElementEditor* pVectorElementEditor);
   /** dtor */
   virtual ~CloseButton();
protected:
   /** Cierra el feature abierto en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
private:
   VectorElementEditor* pVectorElementEditor_; /*! Elemento para la edicion */
};
}  // namespace suri

#endif /*CLOSEBUTTON_H_*/
