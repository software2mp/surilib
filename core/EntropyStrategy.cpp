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
