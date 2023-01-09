/*! \file SaveVectorButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SAVEVECTORBUTTON_H_
#define SAVEVECTORBUTTON_H_

// Includes Suri
#include "suri/Button.h"
#include "GeometryElementEditor.h"

/** namespace suri */
namespace suri {
// Forwards
class VectorElementEditor;

/** Button que salva el vector en edicion. */
/**
 * Clase que crea un boton para salvar el vector en edicion para las
 * herramientas de edicion de vectores.
 */
class SaveVectorButton : public Button {
public:
   /** Ctor. */
   SaveVectorButton(wxWindow *pToolbar, GeometryElementEditor* pGeometryElementEditor,
                    VectorElementEditor* pVectorElementEditor = NULL);
   /** Dtor. */
   virtual ~SaveVectorButton();
   /** Verifica el estado de activacion del boton */
   virtual bool IsEnabled();
   /** Asigna el vector en edicion */
   void SetVectorElementEditor(VectorElementEditor* pVectorElementEditor) {
      pVectorElementEditor_ = pVectorElementEditor;
   }
   void SetGeometryElementEditor(GeometryElementEditor* pGeoEditor) {
      pGeoEditor_ = pGeoEditor;
   }
protected:
   /** Elimina el ultimo punto ingresado en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
private:
   GeometryElementEditor* pGeoEditor_;
   VectorElementEditor* pVectorElementEditor_; /*! VectorEditor */
};
}  // namespace suri

#endif /* SAVEVECTORBUTTON_H_ */
