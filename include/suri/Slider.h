/*! \file Slider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SLIDER_H_
#define SLIDER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "Widget.h"

// Includes Wx
#include "wx/slider.h"

// Defines
// forwards

namespace suri {
// forwards
DECLARE_EVENT_OBJECT(SliderEventHandler);
class LabelText;

/*!
 * Clase que representa un control grafico de tipo slider.
 * Permite obtener e indicar el valor actual en el control grafico.
 */
class Slider : public suri::Widget {
   /** ctor copia **/
   Slider(const Slider&);
public:
   /** Constructor **/
   Slider(wxWindow *pViewerToolbar, const std::string& Tooltip, int Flag);
   /** Destructor **/
   virtual ~Slider();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Obtiene la seleccion activa en el control grafico. */
   int GetValue() const;
   /** Indica la seleccion activa en el control grafico. */
   void SetValue(int Value);
   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled() const;
   /** Metodo que indica si posee un label asociado **/
   bool HasLabel() const;
   /** Metodo que obtiene el label **/
   LabelText* GetLabel() const;

protected:
   /** Metodo que se llama cuando se ejecuta CreateToolWindow **/
   virtual void OnCreateToolWindow();
   /** Metodo que se llama cuando cambia el valor actual en el control grafico.  */
   virtual void OnValueChanged(wxScrollEvent &Event);
   wxSlider* pSlider_;
   bool enabled_;
   EVENT_OBJECT_PROPERTY(SliderEventHandler);
   FRIEND_EVENT_OBJECT(SliderEventHandler);
   /** label asociado al control grafico **/
   std::string label_;
};

} /** namespace suri */

#endif /* SLIDER_H_ */
