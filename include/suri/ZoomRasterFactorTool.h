/*! \file ZoomRasterFactorTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ZOOMRASTERFACTORTOOL_H_
#define ZOOMRASTERFACTORTOOL_H_

// Includes standard

// Includes Suri
#include "suri/WorldView.h"
#include "suri/ListView.h"
#include "suri/Tool.h"

// Includes Wx

// Includes App

// Defines

// Forwards
class wxFrame;

/** namespace suri */
namespace suri {
/** Clase que muestra el factor de zoom en el statusbar */
/**
 * Actualiza herramienta con relacion entre el tamanio de la
 * ventana y el del raster. La comparacion la hace en coordenadas
 * de pixel-linea
 */
class ZoomRasterFactorTool : public Tool {
   /** Ctor. de Copia. */
   ZoomRasterFactorTool(const ZoomRasterFactorTool &ZoomRasterFactorTool);

public:
   /** Ctor. default. */
   ZoomRasterFactorTool();
   /** Dtor. */
   virtual ~ZoomRasterFactorTool();
   /** Metodo virtual para indicar que la herramienta se ha des/activado */
   virtual bool DoSetActive(bool Active);
   /** Metodo virtual que se llama al ejecutar SetList */
   virtual bool DoSetList(LayerList *pList);
   /** Metodo virtual que se llama al ejecutar SetViwer. */
   virtual bool DoSetViewer(ViewerWidget *pViewer);
   /** Obtiene las dimensiones de la matriz raster y viewport */
   void ObtainMatrixSizes();
   /** Calcula el factor de zoom y actualiza el statusbar */
   void UpdateZoomRasterFactor();
   /** Actualiza el factor de zoom ante cambios en el mundo */
   void OnChange(Model*);
   /** Metodo que actualiza el factor de zoom */
   virtual void Update();
#ifdef __UNUSED_CODE__
   /** Calcula el maximo comun divisor entre dos numeros */
   int ObtainGreatestCommonDivisor(int, int);
#endif
private:
   WorldView<ZoomRasterFactorTool> *pWorldViewer_; /*! view del mundo (registra */
   /* los cambios) */
   ListView<ZoomRasterFactorTool> *pListViewer_; /*! view de la lista (registra */
   /* los cambios) */
   unsigned int viewportWidth_; /*! Ancho de viewport en P y L */
   unsigned int viewportHeight_; /*! Alto de viewport en P y L */
   float rasterWidth_; /*! Ancho de raster en P y L */
   float rasterHeight_; /*! Alto de raster en P y L */
};
}  // namespace suri

#endif /* ZOOMRASTERFACTORTOOL_H_ */