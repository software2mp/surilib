/*! \file WorldWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WORLDWIDGET_H_
#define WORLDWIDGET_H_

// Include suri
#include "suri/Widget.h"
#include "suri/World.h"
#include "suri/WorldView.h"

// Includes wx
#include "wx/wx.h"

/** namespace suri */
namespace suri {
/** utilizado para tener disponible o no la herramienta que muestra la ventana dentro del mundo */
#ifdef __DEBUG__
#  define __ENABLE_WORLDWATCHER_TOOL__
#endif   // __DEBUG__
#ifdef __ENABLE_WORLDWATCHER_TOOL__

/** Declara clase WorldWidgetEvent */
DECLARE_EVENT_OBJECT(WorldWidgetEvent);

/** Widget que muestra la posicion de la ventana dentro del mundo */
/**
 * Muestra dos recuadros, uno representa al mundo(extent de la imagen)
 * y el otro a la ventana.
 * \todo Con muy poco trabajo se puede hacer una ventana "Panorama"
 */
class WorldWidget : public Widget {
   /** Ctor. de Copia. */
   WorldWidget(const WorldWidget &WorldWidget);

public:
   /** Declara friend la clase de evento WorldWidgetEvent */
   FRIEND_EVENT_OBJECT(WorldWidgetEvent);
   /** Ctor */
   WorldWidget(World *pWorld);
   /** Dtor */
   virtual ~WorldWidget();
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();

protected:
   /** evento que dibuja un rectangulo para el mundo y otro para la ventana */
   void OnPaint(wxPaintEvent &Event);
   /** Actualiza las coordenadas del centro del mundo */
   void WorldUpdate(Model *pWorld);
   /** se llama cuando se elimina el world */
   void WorldDelete(Model *pWorld);
private:
   EVENT_OBJECT_PROPERTY(WorldWidgetEvent); /*! Objeto para conectar eventos */
   WorldView<WorldWidget> *pWorldViewer_; /*! View del mundo */
   double offsetX_; /*! coord x del centro del mundo */
   double offsetY_; /*! coord y del centro del mundo */
   double scaleFactor_; /*! relacion de tamano entre viewer y mundo */
};

// -------------------------- CLASE PARA VER EL MUNDO --------------------------
#endif   // __ENABLE_WORLDWATCHER_TOOL__
}

#endif /*WORLDWIDGET_H_*/
