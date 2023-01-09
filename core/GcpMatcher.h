/*! \file GcpMatcher.h */
/**
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. ( http: //www.suremptec.com/)
 */

#ifndef GCPMATCHER_H
#define GCPMATCHER_H

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "suri/GcpList.h"
#include "ImageBuffer.h"
// Includes Wx
// Defines
// forwards

namespace suri {
   
class GroundControlPointExtended;
class Image;
struct GcpMatchResult;

class GcpMatcher {
public:
   /**
    * Constructor. Indica qué imagenes serán corregistradas
    */
   GcpMatcher(const Image* pBaseImage, const Image* pWarpImage,
              const std::vector<int>& BaseBands, const std::vector<int>& WarpBands);
   /**
    * Recorre la imagen en una ventana en torno a la coordenada pasada por 
    * parametro. Por cada punto en la ventana, calcula los coeficientes de
    * correlacion con otras subventanas ubicadas en la imagen warp. Finalmente,
    * devuelve el mejor par de coordenadas entre la imagen base y la warp
    * que encuentra.
    */
   GroundControlPointExtended* FindGcp(unsigned int candidateX,
                                       unsigned int candidateY);
   /** Ajusta la ventana de scan **/
   void SetScanWindowSize(unsigned int ScanWindowSize);
   void SetCorrelWindowSize(unsigned int CorrelWindowSize);
   void SetSearchWindowSize(unsigned int SearchWindowSize);
   /**
    * Ancho default de la ventana de barrido
    */
   static const unsigned int kScanWindowSize;
   /**
    * Ancho default de la ventana de correlacion
    */
   static const unsigned int kCorrelWindowSize;
   /**
    * Ancho default de la ventana de busqueda
    */
   static const unsigned int kSearchWindowSize;
private:
   /**
    * Arma y carga un buffer que rodea al punto (candidateX, candidateY)
    * de la imagen base.
    */
   void LoadBaseWindow(unsigned int candidateX,
                       unsigned int candidateY,
                       std::vector< float >& baseWindowBuffer);
   /**
    * Arma y carga un buffer que rodea al punto (candidateX, candidateY)
    * de la imagen warp.
    */
   void LoadWarpWindow(unsigned int candidateX,
                       unsigned int candidateY,
                       std::vector< float >& warpWindowBuffer);
   /**
    * Obtiene la correlacion
    */
   GcpMatchResult GetMatchCorrelation(const std::vector<float>& baseWindow,
                                      const std::vector<float>& warpSearchWindow) const;
   /**
    * 
    */
   GcpMatchResult FindMatch(unsigned int baseX,
                            unsigned int baseY,
                            const std::vector<float>& baseWindowBuffer,
                            const std::vector<float>& warpSearchWindow) const;
   /**
    * Obtiene una ventana a partir de un buffer de memoria
    * @param buffer fuente de datos
    * @param bufferWidth ancho del buffer fuente
    */
   std::vector<float> GetSubwindow(const std::vector<float>& buffer, unsigned int x,
                     unsigned int y, unsigned int bufferWidth) const;
   /**
    * Ancho de la ventana de barrido
    */
   unsigned int scanWindowSize_;
   /**
    * Ancho de la ventana de correlacion
    */
   unsigned int correlWindowSize_;
   /**
    * Ancho de la ventana de busqueda
    */
   unsigned int searchWindowSize_;
   /**
    * Ancho del buffer de las ventanas de la imagen base
    * correlWindowSize_ + scanWindowSize_ - 1
    */
   unsigned int baseWindowSize_;
   /**
    * Imagen base de la cual se leen los candidatos a GCP
    */
   ImageBuffer baseImage_;
   /**
    * Imagen warp en la cual se intenta matchear un GCP.
    */
   ImageBuffer warpImage_;
   /**
    * Banda base
    */
   std::vector<int> baseBands_;
   /**
    * Banda warp
    */
   std::vector<int> warpBands_;
   
};

struct GcpMatchResult{
   float correlation;
   int baseX, baseY, warpX, warpY;
};

} // namespace suri

#endif // GCPMATCHER_H
