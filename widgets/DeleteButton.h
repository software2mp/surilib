/*! \file DeleteButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DELETEBUTTON_H_
#define DELETEBUTTON_H_

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
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(DeleteButtonEvent);

/** Boton que elimina el ultimo punto ingresado en el vector */
/**
 * Boton para las herramientas de edicion de vectores que no responden a eventos
 * de mouse (GPS). Permite eliminar el ultimo punto de un feature abierto del
 * vector en edicion. El boton se esconde si no hay puntos para eliminar.
 */
class DeleteButton : public suri::Button {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(DeleteButtonEvent);
   /** ctor */
   DeleteButton(wxWindow *pToolbar, VectorElementEditor* pVectorElementEditor);
   /** dtor */
   virtual ~DeleteButton();
protected:
   /** Elimina el ultimo punto ingresado en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
// Metodo de creacion del boton-herramienta
   virtual bool CreateToolWindow();
   /** Esconde boton cuando no hay puntos en vector en edicion */
   virtual void OnHideWithNoPointsUiUpdate(wxUpdateUIEvent &Event);
private:
   EVENT_OBJECT_PROPERTY(DeleteButtonEvent); /*! Objeto para conectar eventos */
   VectorElementEditor* pVectorElementEditor_; /*! VectorElementEditor que se */
   /* utilza para eliminar el punto */
};
}  // namespace suri

#endif /*DELETEBUTTON_H_*/
