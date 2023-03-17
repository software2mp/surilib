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

#ifndef GCP_DETECTION_SUBPROCESS_H_
#define GCP_DETECTION_SUBPROCESS_H_

// Includes Estandar
#include <vector>
// Includes Suri
#include "RasterProcess.h"
#include "Subset.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class Mask;
class Canvas;
class MemoryCanvas;
class FileCanvas;
class DataViewManager;
class Element;
class RasterElement;

/**
 * Construye una mascara a partir del calculo de entropia
 * luego calcula el modulo wavelet del raster, lo enmascara,
 * y le aplica un umbral para resaltar los puntos mas significativos para
 * la deteccion de puntos GCP
 */
class GcpDetectionSubprocess {
public:
   /** Ctor. */
   GcpDetectionSubprocess(Element* element, const std::vector<int>& bands);
   /**
    * Destructor
    */
   virtual ~GcpDetectionSubprocess();
   /** Corre el proceso y devuelve el raster con los Gcps candidatos */
   RasterElement* RunProcess();
   
private:
   /**
    * Prefijo para los archivos temporales
    */
   std::string tempFilePath_;
    /**
    * Raster element de la imagen de entrada
    */
   RasterElement* pInputElement_;
   /**
    * Canvas con la mascara para candidatos a GCP de la imagen (temporal)
    */
   FileCanvas* pEntropyCanvas_;
   FileCanvas* pEntropyCanvas2_;
   FileCanvas* pEntropyCastCanvas_;
   /**
    * Canvas con todos los candidatos a GCP de la imagen (temporal)
    */
   FileCanvas* pGcpCanvas_;
   FileCanvas* pGcpCanvas2_;
   /**
    * Canvas con los candidatos a GCP de la imagen filtrados por entropia.
    */
   FileCanvas* pFilteredGcpCanvas_;
   /**
    * Devuelve un canvas para persistir los puntos candidatos
    */
   FileCanvas* CreateFileCanvas(RasterElement* element,
                            const std::string& filepath) const;
   /**
    * Ejecuta un pipeline de renderizacion
    */
   void RenderElement(RasterElement* element, Canvas* pCanvas, Element* pMask = 0);
   /**
    * Formato de salida de los canvas temporales
    */
   static const std::string kOutputFormat;
   /**
    * Bandas de la imagen a procesar.
    */
   std::vector<int> bands_;
};

} /** namespace suri */

#endif /* GCPDETECTIONSUBPROCESS_H_ */
