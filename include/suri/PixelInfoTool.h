/*! \file PixelInfoTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PIXELINFOTOOL_H_
#define PIXELINFOTOOL_H_

// Includes standard

// Includes Suri
#include "suri/WorldView.h"
#include "suri/Tool.h"
#include "suri/ListView.h"
#include "RasterDnInfo.h"
#include "suri/ObserverInterface.h"

// Includes Wx

// Includes App

// Defines

// Forwards
class wxFrame;
class wxMouseEvent;

/** namcespace suri */
namespace suri {
// Forwards
class CoordinatesTransformation;
class PixelInfoToolEvent;
class MemoryCanvas;

/** Clase de informacion de coordenadas */
/**
 * Permite conocer las coordenadas de una posicion en una imagen a medida que el
 * mouse se desplaza sobre ella.
 * Las coordenadas son expuestas en el statusbar de la ventana principal de la
 * aplicacion.
 */
class PixelInfoTool : public Tool, public ObserverInterface {
   /** Ctor. de Copia. */
   PixelInfoTool(const PixelInfoTool &PixelInfoTool);

public:
   /** Ctor. default. */
   PixelInfoTool(const World *pWorldWindow);
   /** Dtor. */
   virtual ~PixelInfoTool();

   /** Utilizo ventana en modo debug para poder tener una interfaz de control */
#ifdef __DEBUG__
   /** Retorna la ventana asociada a la herramienta */
   /**
    * @return Puntero a la ventana asociada a la herramienta o NULL en caso de
    *       de no tener ventana asociada.
    */
   virtual wxWindow *GetToolWindow() const;
#endif
   /** Setea el estado de la herramienta */
   virtual bool DoSetActive(bool State);
   /** Asigna la lista de elementos asociada a la herramienta */
   virtual void SetList(LayerList *pList) {
   }
   /** Asigna el visualizador del contexto asociado a la herramienta */
   virtual void SetViewer(ViewerWidget *pViewer);
   /** Evento de entrada de mouse a la ventana */
   void OnMouseEnter();
   /** Evento de salida de mouse de la ventana */
   void OnMouseLeave();
   /** Evento de mouse para actualizar los datos */
   void OnMouse(wxMouseEvent &Event);
   /** Evento para actualizar los datos */
   void OnIdle();
   /** Metodo de actualizacion de la lista */
   void ListUpdate(Model *pModel);

   /** Metodo que desactiva la consulta de coordenadas **/
   void DisableCoordinatesInfo();
   /** Metodo que activa la consulta de coordenadas */
   void EnableCoordinatesInfo();

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification) {
      pNotification->ApplyNotification(this);
   }

private:
   /** Carga las coordenadas del cursor en el string */
   wxString GetCoordinatesString(const Coordinates &Position);
   /** Carga los datos bajo el cursor en el string */
   wxString GetDigitalNumberString();
#ifdef __SHOW_RGB_VALUE__
   /** Carga el valor RGB del pixel */
   wxString GetPixelRgbString();
#endif   // __SHOW_RGB_VALUE__
   /** Carga la posicion en coordenadas de imagen */
   wxString GetPixelPositionString();
   /** Actualiza la funcion de transformacion para coordenadas pixel **/
   void UpdatePixelTransformation();
   /** Metodo auxiliar para actualizar la transformacion para obtener las coordenadas**/
   void UpdateTransformation();
   wxFrame *pViewerFrame_; /*! Frame del viewer asociado */
   ViewerWidget *pViewer_; /*! Ventana del viewer asociado */
   PixelInfoToolEvent *pEventHandler_; /*! Manejador de eventos */
   int viewportX_; /*! Coordenadas de viewport donde esta el mouse x */
   int viewportY_; /*! Coordenadas de viewport donde esta el mouse y */
   const World *pWorld_; /*! Permite leer el mundo */
   CoordinatesTransformation *pCT_; /*! Transformacion entre coords de mundo y */
   /*! Transformacion para obtener el pl asociado a las coords de mundo*/
   CoordinatesTransformation *pPixelPosT_;
   /* coords lat/long */
   bool onIdleConnected_; /*! Lleva control si el evento onIdle esta conectado. */
   /* En linux no siempre atrapa OnEnter. */
   ListView<PixelInfoTool> pListViewer_; /*! View de la lista */
   MemoryCanvas *pRawRasterData_; /*! Canvas con la informacion cruda del raster */
   Element *pRasterElement_; /*! Elemento raster desplegado */
   /** Clase para obtener informacion de las bandas de la capa **/
   RasterDnInfo dnInfo_;
};
}  // namespace suri

#endif /*PIXELINFOTOOL_H_*/
