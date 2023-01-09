/*! \file NavigationButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef NAVIGATIONBUTTON_H_
#define NAVIGATIONBUTTON_H_

#include "suri/ToggleButton.h"

/** namespace suri */
namespace suri {

class GoButton;
class CrossHairPaintEvent;
class ViewerWidget;

/** Activa NavigationWidget para recorrer el mundo y muestra cruz si corresponde */
/**
 * Agrega GoButton a NavigationWidget(Activandose si no estaba visible).
 * Escucha por el evento Paint y dibuja la cruz si las coordenadas Navigator
 * estan en la ventana.
 */
class NavigationButton : public ToggleButton {
public:
   /** ctor */
   NavigationButton(wxWindow *pToolbar, ViewerWidget* pViewer);

   /** dtor */
   virtual ~NavigationButton();

   /** Setea el estado del boton. Ingresa/Saca boton de NavigationWidget. */
   virtual bool DoSetValue(bool State);

   /** Evita que cambie el cursor al navegar */
   void DoSetCursor(bool State) { }

private:
   ViewerWidget* pViewer_;  // Se usa para obtener el navegador y para mostrar cruz
   GoButton* pGoButton_;  // Boton que se agrega al NavigationWidget para ir al destino deseado
   bool drawCross_;  // Determina si se tiene que dibujar la cruz
   CrossHairPaintEvent* pCrossHair_;
};
}

#endif /*NAVIGATIONBUTTON_H_*/
