/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
