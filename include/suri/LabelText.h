/*! \file LabelText.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LABELTEXT_H_
#define LABELTEXT_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "Widget.h"

// Includes Wx
// Defines
// forwards
class wxStaticText;

namespace suri {

/*!
 * Clase que representa un control grafico que contiene
 * un texto estatico
 */
class LabelText : public suri::Widget {
   /** Constructor copia **/
   LabelText(const LabelText&);

public:
   /** Constructor**/
   LabelText(wxWindow *pViewerToolbar, const std::string& Text);
   /** Destructor **/
   virtual ~LabelText();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Obtiene el texto del control grafico. */
   std::string GetValue() const;
   /** Indica el texto a mostrar por el control grafico. */
   void SetValue(std::string& Value);
   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled() const;

private:
   std::string text_;
   bool enabled_;
   wxStaticText* pTextCtrl_;
};

} /** namespace suri */

#endif /* LABELTEXT_H_ */
