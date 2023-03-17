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
