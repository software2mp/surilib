/*! \file GcpDetectionSubprocess.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

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
