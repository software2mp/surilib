/*! \file CloseVectorButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLOSEVECTORBUTTON_H_
#define CLOSEVECTORBUTTON_H_

// Includes Suri
#include "suri/Button.h"
#include "GeometryElementEditor.h"
// Forwards

/** namespace suri */
namespace suri {
// Forwards
class VectorElementEditor;

/** Boton que permite descartar el vector en edicion. */
/**
 * Boton que sirve para descartar un vector en edicion, en las herramientas de
 * edicion de vectores. Se activa solo cuando hay un vector en edicion.
 */
class CloseVectorButton : public Button {
public:
   /** Ctor. */
   CloseVectorButton(wxWindow *pToolbar, GeometryElementEditor* pGeometryElementEditor,
                     VectorElementEditor* pVectorElementEditor = NULL);
   /** Ctor - sobrecargado para soportar la nueva geomentria. GeometryElementEditor */

   /** Dtor. */
   virtual ~CloseVectorButton();
   /** Verifica el estado de activacion del boton */
   virtual bool IsEnabled();
   /** Permite asignar el VectorElementEditor sobre el que se esta editando */
   void SetVectorElementEditor(VectorElementEditor* pVectorElementEditor) {
      pVectorElementEditor_ = pVectorElementEditor;
   }
   void SetGeometryElementEditor(GeometryElementEditor* pGeometryElementEditor) {
      pGeometryElementEditor_ = pGeometryElementEditor;
   }

protected:
   /** Elimina el ultimo punto ingresado en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
private:
   GeometryElementEditor* pGeometryElementEditor_;
   VectorElementEditor* pVectorElementEditor_; /*! Vector en edicion */

};
}  // namespace suri

#endif /* CLOSEVECTORBUTTON_H_ */
