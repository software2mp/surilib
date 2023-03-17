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

#include <vector>

#include "SRStatisticsCanvas.h"
#include "suri/DataTypes.h"

namespace suri {
namespace render {

/**
 * Ctor.
 */
StatisticsCanvas::StatisticsCanvas(bool InterBandStatistics) :
      pStatistics_(NULL), interBandStatistics_(InterBandStatistics) {
}

/**
 * Dtor.
 */
StatisticsCanvas::~StatisticsCanvas() {

}

/**
 * Devuelve las estadisticas.
 */
SRD::StatisticsBase* StatisticsCanvas::GetStatistics() {
   return pStatistics_;
}

/**
 * Computa los parametros deseados.
 */
void StatisticsCanvas::Flush(const suri::Mask *pMask) {
   // Creo el objeto para guardar las estadisticas.
   if (pStatistics_ == NULL) {
      pStatistics_ = CreateStatisticsFromDataType(GetDataType(), GetBandCount());
      pStatistics_->SetNoDataValue(GetNoDataValue());
      pStatistics_->SetNoDataValueAvailable(IsNoDataValueAvailable());
      pStatistics_->SetAllBandsNdv(GetAllBandsNdv());
   }

   // Leo datos.
   std::vector<void*> indata;
   std::vector<int> bands;
   for (int ix = 0; ix < GetBandCount(); ++ix)
      bands.push_back(ix);
   GetInternalData(bands, indata);

   int width = 0, height = 0;
   GetSize(width, height);

   if (!interBandStatistics_) {
      // Proceso los datos
      for (int bandix = 0, bandlen = GetBandCount(); bandix < bandlen; ++bandix) {
         pStatistics_->Process(bandix, width * height, indata[bandix]);
      }
   } else {
      pStatistics_->Process(width * height, indata);
   }

}

/**
 * Crea el objeto para calculo de estadisticas a partir del tipo de dato del provisto.
 */
SRD::StatisticsBase* StatisticsCanvas::CreateStatisticsFromDataType(const std::string& DataType, int BandCount) {
   SRD::StatisticsBase* pret = NULL;
   if (DataType.compare(suri::DataInfo<unsigned char>::Name) == 0) {
      pret = new SRD::Statistics<unsigned char>(BandCount);
   } else if (DataType.compare(suri::DataInfo<unsigned short>::Name) == 0) {
      pret = new SRD::Statistics<unsigned short>(BandCount);
   } else if (DataType.compare(suri::DataInfo<short>::Name) == 0) {
      pret = new SRD::Statistics<short>(BandCount);
   } else if (DataType.compare(suri::DataInfo<unsigned int>::Name) == 0) {
      pret = new SRD::Statistics<unsigned int>(BandCount);
   } else if (DataType.compare(suri::DataInfo<int>::Name) == 0) {
      pret = new SRD::Statistics<int>(BandCount);
   } else if (DataType.compare(suri::DataInfo<float>::Name) == 0) {
      pret = new SRD::Statistics<float>(BandCount);
   } else if (DataType.compare(suri::DataInfo<double>::Name) == 0) {
      pret = new SRD::Statistics<double>(BandCount);
   }
   return pret;
}

}  // namespace render
}  // namespace suri
