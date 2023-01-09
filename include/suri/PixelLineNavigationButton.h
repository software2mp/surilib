/*! \file PixelLineNavigationButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef PIXELLINENAVIGATIONBUTTON_H_
#define PIXELLINENAVIGATIONBUTTON_H_

// includes suri
#include "suri/ToggleButton.h"
#include "suri/ListView.h"

/** namespace suri */
namespace suri {
// forwards
class ViewerWidget;
class GoButton;
class RasterElement;
class PixelLineInputWidget;

/** Activa NavigationWidget para recorrer el mundo y muestra cruz si corresponde */
/**
 * Agrega GoButton a NavigationWidget(Activandose si no estaba visible).
 * Escucha por el evento Paint y dibuja la cruz si las coordenadas Navigator
 * estan en la ventana.
 */
class PixelLineNavigationButton : public ToggleButton {
public:
   /** ctor */
   PixelLineNavigationButton(wxWindow *pToolbar, ViewerWidget* pViewer);
   /** dtor */
   virtual ~PixelLineNavigationButton();
   /** Setea el estado del boton. Ingresa/Saca boton de NavigationWidget. */
   virtual bool DoSetValue(bool State);
   /** Evita que cambie el cursor al navegar */
   void DoSetCursor(bool State) {
   }
protected:
   /** Actualiza el elemento raster activo en PixelLineInputWidget */
   RasterElement* GetRasterElement();
   /** Metodo de actualizacion de la lista */
   void ListUpdate(Model *pModel);
private:
   ViewerWidget* pViewer_; /*! Se usa para obtener el navegador y para */
   /* mostrar cruz. */
   GoButton* pGoButton_; /*! Boton que se agrega al NavigationWidget para ir */
   /* al destino deseado */
   bool drawCross_; /*! Determina si se tiene que dibujar la cruz. */
   suri::PixelLineInputWidget* pInputWidget_; /*! Guarda inputwidget que se */
   /* cargo en navigationWidget */
   ListView<PixelLineNavigationButton> *pListViewer_; /*! View de la lista */
   RasterElement* pRasterElement_; /*! Elemento raster activo de la lista */
};
}

#endif /* PIXELLINENAVIGATIONBUTTON_H_ */
