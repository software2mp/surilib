/*! \file RegistrabilityStrategy.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <algorithm>
// Includes Suri
#include "EntropyStrategy.h"
#include "SRDHistogram.h"
// Includes Wx
// Defines
// forwards

namespace suri {

const int EntropyByTiles::kTileSize = 100;
const double EntropyByTiles::kHistogramBins = 255;

/**
 * Constructor
 */
EntropyByTiles::EntropyByTiles(float* const pData, int sizeX, int sizeY) :
   pData_(pData), sizeX_(sizeX), sizeY_(sizeY){}

/**
 * Destructor
 */
EntropyStrategy::~EntropyStrategy() {}

std::vector<EntropyResult> EntropyByTiles::CalculateEntropy() const {
   std::vector<EntropyResult> resultVector;
   for (int iy = 0; iy < this->sizeY_; iy += kTileSize) {
      for (int ix = 0; ix < this->sizeX_; ix += kTileSize) {
         EntropyResult tileResult = GetEntropyForTile(ix, iy);
         resultVector.push_back(tileResult);
      }
   }
   return resultVector;
}

EntropyResult EntropyByTiles::GetEntropyForTile(int tileX, int tileY) const {
   int tileHeight = std::min<int>(this->sizeY_ - tileY, kTileSize);
   int tileWidth = std::min<int>(this->sizeX_ - tileX, kTileSize);
   int dataLength = tileWidth * tileHeight;
   std::vector<float>tileData(dataLength);
   int tileDataIndex = 0;
   for (int iy = 0; iy < tileHeight; ++iy){
      for (int ix = 0; ix < tileWidth; ++ix){
         int dataIndex = tileX + ix + (tileY + iy) * this->sizeX_;
         tileData [tileDataIndex] = this->pData_[dataIndex];
         ++tileDataIndex;
      }
   }
   int numBins = kHistogramBins;
   double minValue = 0;
   double maxValue = 0;
   raster::data::Histogram<float>::GetLimits(&tileData[0], dataLength, minValue, maxValue);
   raster::data::Histogram<float> hist(1, &numBins, &minValue, &maxValue);
   hist.CountPixels(0, dataLength, &tileData[0]);
   double* entropy = hist.GetEntropy();
   EntropyResult result(entropy[0], tileX, tileY,
                        tileX + tileWidth, tileY + tileHeight);
   delete[] entropy;
   return result;
}

} /** namespace suri */
