/*! \file GoButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef GOBUTTON_H_
#define GOBUTTON_H_

#include "suri/Button.h"



namespace suri {

class ViewerWidget;
class ViewportManagerInterface;
class ViewcontextManagerInterface;
class Viewer2D;
class RasterElement;
class Navigator;

/**
 * Obtiene coordenadas del NavigationWidget y se las da al Navegador.
 *
 * Boton que obtiene las coordenadas del GoButton y dibuja una cruz en el viewer.
 * Recibe un CallBack cuando el mundo es modificado y redibuja la cruz en la
 * posicion que corresponde.
 */
class GoButton : public Button {
public:
   /** ctor */
   GoButton(wxWindow *pToolbar, ViewerWidget* pViewer, Element* pElement = NULL,
            ViewportManagerInterface* pViewportManager = NULL,
            ViewcontextManagerInterface* pViewcontextManager = NULL,
            Navigator* pNavigator = NULL);

   /** dtor */
   virtual ~GoButton();

protected:
   /** Obtiene coordenadas del NavigationWidget y se las da al Navegador */
   virtual void OnButtonClicked(wxCommandEvent &Event);

   /** Metodo de actualizacion del mundo */
   /**
    *  Metodo Update que se ejecuta cuando pWorldWindow es modificado. Es el
    * callback que se registra en pWorldViewer_.
    */
   void DoOnPaint(wxPaintEvent &Event);

   /** Dibuja cruz invirtiendo colores */
   void DrawCrossWithInvertedColor(const Coordinates &Point1);

   /** Evita que cambie el cursor al navegar */
   void DoSetCursor(bool State) {
   }
private:
   /** Retorna navegador */
   Navigator* GetNavigator();
   ViewerWidget* pViewer_; /*! Se usa para obtener el navegador y para mostrar */
   /* cruz. */
   bool buttonClick_; /*! Recuerda el primer click para no mostrar la cruz */
   /* hasta que esto ocurra */
   int startX_; /*! Guarda la posicion horizontal de scroll inicial */
   int startY_; /*! Guarda la posicion vertical de scroll inicial */
   Subset lastWindow_; /*! Guarda la ventana inicial */
   Element* pElement_; /*! Elemento activo **/
   ViewportManagerInterface* pViewportManager_;
   ViewcontextManagerInterface* pViewcontextManager_;
   Navigator *pNavigator_; /*! permite recorrer el mundo usando puntos */

   /**
    * Envia eventos de simulacion de un click con el mouse.
    */
   void SendMouseClick(const Coordinates& LatLong, bool Transform = true);
   ViewerWidget* GetActiveViewer();
#ifdef __UNUSED_CODE__
   /**
    * Devuelve el visualizador que debe ser utilizado.
    * \deprecated: Utilizar GetActiveViewer. La herramienta de Navegacion se extiende a V3D.
    */
   Viewer2D* GetViewer2D();
#endif

   /**
    * Devuelve el elemento que debe ser utilizado.
    */
   RasterElement* GetElement();
};
}

#endif /*GOBUTTON_H_*/
