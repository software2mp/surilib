/*! \file GcpMatcher.cpp */
/**
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. ( http: //www.suremptec.com/)
 */

// Includes Estandard
#include <limits>
#include <vector>
// Includes Suri
#include "GcpMatcher.h"
#include "suri/Image.h"
#include "suri/DataTypes.h"
#include "SRDStatistics.h"

namespace suri{
   
/**
 * Ancho default de la ventana de barrido
 */
const unsigned int GcpMatcher::kScanWindowSize = 5;
/**
 * Ancho default de la ventana de correlacion
 */
const unsigned int GcpMatcher::kCorrelWindowSize = 11;
/**
 * Ancho default de la ventana de busqueda
 */
const unsigned int GcpMatcher::kSearchWindowSize = 21;

/**
 * Función para interpretar los buffers leidos a float
 */
typedef void (*BufferCastFunctionType)(void* pSrc, float* pDest, unsigned int buffSize);
   
template<class T>
void bufferCast(void* pSrc, float* pDest, unsigned int buffSize){
   for (unsigned int i = 0; i < buffSize; ++i){
      pDest[i] = static_cast<T*>(pSrc)[i];
   }
}

INITIALIZE_DATATYPE_MAP(BufferCastFunctionType, bufferCast);

/**
 * Constructor
 */
GcpMatcher::GcpMatcher(const Image* pBaseImage, const Image* pWarpImage,
              const std::vector<int>& BaseBands, const std::vector<int>& WarpBands) :
    baseImage_(pBaseImage, BaseBands), warpImage_(pWarpImage, WarpBands){
   this->correlWindowSize_ = GcpMatcher::kCorrelWindowSize;
   this->scanWindowSize_ = GcpMatcher::kScanWindowSize;
   this->searchWindowSize_ = GcpMatcher::kSearchWindowSize;
   this->baseWindowSize_ = this->correlWindowSize_ + this->scanWindowSize_ - 1;
   this->baseBands_.push_back(0);
}

/**
 * Recorre la imagen en una ventana en torno a la coordenada pasada por 
 * parametro. Por cada punto en la ventana, calcula los coeficientes de
 * correlación con otras subventanas ubicadas en la imagen warp. Finalmente,
 * devuelve el mejor par de coordenadas entre la imagen base y la warp
 * que encuentra.
 */
GroundControlPointExtended* GcpMatcher::FindGcp(unsigned int candidateX,
                                                unsigned int candidateY) {
   std::vector<float> baseWindowBuffer, warpWindowBuffer;
   LoadBaseWindow(candidateX, candidateY, baseWindowBuffer);
   LoadWarpWindow(candidateX, candidateY, warpWindowBuffer);
   GcpMatchResult gcp = FindMatch(candidateX, candidateY, baseWindowBuffer, warpWindowBuffer);
   return new GroundControlPointExtended(gcp.baseX, gcp.baseY, 0, gcp.warpX, gcp.warpY, 0);
}

void GcpMatcher::LoadBaseWindow (unsigned int candidateX,
                                 unsigned int candidateY,
                                 std::vector< float >& baseWindowBuffer) {
   baseWindowBuffer.resize(baseWindowSize_*baseWindowSize_);
   std::vector<unsigned char> imgBuff(baseWindowBuffer.size() * this->baseImage_.GetDataSize());
   unsigned int ulx = candidateX - baseWindowSize_ / 2;
   unsigned int uly = candidateY - baseWindowSize_ / 2;
   this->baseImage_.Read(&imgBuff.front(), ulx, uly,
                           ulx + baseWindowSize_, uly + baseWindowSize_);
   std::string imgDataType(this->baseImage_.GetDataType());
   bufferCastTypeMap[imgDataType](&imgBuff[0], &baseWindowBuffer.front(), baseWindowBuffer.size());
}

void GcpMatcher::LoadWarpWindow (unsigned int candidateX,
                                 unsigned int candidateY,
                                 std::vector<float>& warpWindowBuffer) {
   warpWindowBuffer.resize(this->searchWindowSize_*this->searchWindowSize_);
   std::vector<unsigned char> imgBuff(warpWindowBuffer.size() * this->warpImage_.GetDataSize());
   unsigned int ulx = candidateX - this->searchWindowSize_ / 2;
   unsigned int uly = candidateY - this->searchWindowSize_ / 2;
   this->warpImage_.Read(&imgBuff.front(), ulx, uly,
                           ulx + this->searchWindowSize_, uly + this->searchWindowSize_);
   std::string imgDataType(this->warpImage_.GetDataType());
   bufferCastTypeMap[imgDataType](&imgBuff[0], &warpWindowBuffer.front(), warpWindowBuffer.size());
}

GcpMatchResult GcpMatcher::FindMatch(unsigned int baseX,
                                     unsigned int baseY,
                                     const std::vector<float>& baseWindowBuffer,
                                     const std::vector<float>& warpSearchWindow) const {
   std::vector<float> baseWindow;
   GcpMatchResult bestResult;
   bestResult.correlation = 0;
   int baseAdjust = this->baseWindowSize_ / 2;
   int warpAdjust = this->searchWindowSize_/ 2;
   for(unsigned int y = 0; y < this->scanWindowSize_; ++y){
      for(unsigned int x = 0; x < this->scanWindowSize_; ++x){
         baseWindow = GetSubwindow(baseWindowBuffer, x, y, this->baseWindowSize_);
         GcpMatchResult curResult = GetMatchCorrelation(baseWindow, warpSearchWindow);
         if (curResult.correlation > bestResult.correlation){
            bestResult = curResult;
            bestResult.baseX = baseX + x - baseAdjust;
            bestResult.baseY = baseY + y - baseAdjust;
            bestResult.warpX += baseX - warpAdjust;
            bestResult.warpY += baseY - warpAdjust;
         }
      }
   }
   return bestResult;
}

GcpMatchResult GcpMatcher::GetMatchCorrelation(const std::vector<float>& baseWindow,
                                               const std::vector<float>& warpSearchWindow) const {
   unsigned int warpSearchWidth = this->searchWindowSize_ - this->correlWindowSize_;
   unsigned int windowSize = this->correlWindowSize_ * this->correlWindowSize_;
   GcpMatchResult result;
   result.correlation = - std::numeric_limits<float>::max();
   for(unsigned int y = 0; y < warpSearchWidth; ++y){
      for(unsigned int x = 0; x < warpSearchWidth; ++x){
         std::vector<float> warpWindow = GetSubwindow(warpSearchWindow, x, y, this->searchWindowSize_);
         std::vector<void*> windowList;
         windowList.push_back(const_cast<float*>(&baseWindow.front()));
         windowList.push_back(const_cast<float*>(&warpWindow.front()));
         raster::data::Statistics<float> stats(2);
         stats.Process(windowSize, windowList);
         std::vector<std::vector<double> > correlationMatrix = stats.GetCorrelationMatrix();
         // La matriz es simetrica
         double correlation = correlationMatrix[0][1];
         if (SURI_ISNAN(correlation)){
            correlation = 1;
         }
         if (correlation > result.correlation){
            result.correlation = correlation;
            result.warpX = x;
            result.warpY = y;
         }
         if (correlation == 1){
            return result;
         }
      }
   }
   return result;
}

std::vector<float> GcpMatcher::GetSubwindow(const std::vector<float>& buffer,
   unsigned int x, unsigned int y, unsigned int bufferWidth ) const {
   unsigned int windowSize = this->correlWindowSize_ * this->correlWindowSize_;
   std::vector<float> subWindow(windowSize);
   unsigned int subWindowIdx = 0;
   for (unsigned int iy = y; iy < y + this->correlWindowSize_; ++iy){
      for (unsigned int ix = x; ix < x + this->correlWindowSize_; ++ix){
         unsigned int bufPos = iy * bufferWidth + ix;
         subWindow[subWindowIdx] = buffer[bufPos];
         ++subWindowIdx;
      }
   }
   return subWindow;
}


}
