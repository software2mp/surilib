/*! \file MarkButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MARKBUTTON_H_
#define MARKBUTTON_H_

// Includes standard
// Includes Suri
#include "suri/Button.h"
// Includes Wx
// Includes App

// Defines

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;
class VectorElementEditor;

/** Agrega puntos a vector */
/**
 * Obtiene puntos del NavegationWidget y si son validos agrega el punto al
 * Vector en edicion.
 */
class MarkButton : public Button {
public:
   /** ctor */
   MarkButton(wxWindow *pToolbar, ViewerWidget *pViewer_,
              VectorElementEditor* pVectorElementEditor);
   /** dtor */
   virtual ~MarkButton();
protected:
   /** Obtiene coordenadas del NavigationWidget y se las da al Navegador */
   virtual void OnButtonClicked(wxCommandEvent &Event);
private:
   ViewerWidget* pViewer_; /*! View de donde se obtiene el NavigationWidget */
   VectorElementEditor* pVectorElementEditor_; /*! VectorEditor que se utilza */
   /* para agregar puntos a los */
   /* vectores */
};
}  // namespace suri

#endif /*MARKBUTTON_H_*/
