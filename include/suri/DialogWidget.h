/*! \file DialogWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DIALOGWIDGET_H_
#define DIALOGWIDGET_H_

// Include suri
#include "Widget.h"

// Forwards
class wxDialog;

/** namespace suri */
namespace suri {
/** Widget que crea un dialogo en lugar de un frame */
/**
 *  Esta clase permite crear un wxDialog en lugar de un wxFrame para insertar
 * el panel de la clase derivada. De esta manera y exponiendo un metodo extra
 * se puede tener clases derivadas de Widget que sean modales.
 * \note: da la funcionalidad de Widget pero sobreescribe el metodo
 *        CreateContainingWindow para que se genere dialogo en vez de frame
 */
class DialogWidget : public Widget {
   /** Ctor. de Copia. */
   DialogWidget(const DialogWidget &DialogWidget);

public:
   /** ctor */
   DialogWidget(wxWindow *pParent = NULL);
   /** dtor */
   virtual ~DialogWidget();

   /** Crea la herramienta */
   virtual bool CreateTool(wxTopLevelWindow *pParentFrame = NULL, bool Show = true);
   /** Retorna el frame dentro del cual se encuentra la ventana */
   virtual wxTopLevelWindow *GetFrame() const;
   /** Muestra el dialogo en forma modal */
   int ShowModal(bool Show = true);
   /** Termina el dialogo modal */
   void EndModal(int ReturnCode);
protected:
   /** Crea la ventana que contiene a la ventana de la herramienta */
   virtual wxTopLevelWindow* CreateContainingWindow(wxTopLevelWindow *pParentFrame);
private:
   wxDialog *pParentDialog_; /*! Puntero al parent como dialogo */
};
}  // namespace name
#endif /*DIALOGWIDGET_H_*/
