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

#include "SRHistogramCanvas.h"
#include "suri/DataTypes.h"
#include "SRDHistogram.h"

namespace suri {
namespace render {

/**
 * Ctor.
 */
HistogramCanvas::HistogramCanvas(int Bins) :
      pHistogram_(NULL), bins_(Bins), customMinMaxSet_(false) {

}

/**
 * Dtor.
 */
HistogramCanvas::~HistogramCanvas() {

}

/**
 * Devuelve las estadisticas.
 */
void HistogramCanvas::SetStatistics(SRD::StatisticsBase* pStatistics) {
   pStatistics_ = pStatistics;
}

/**
 * Obtiene el histograma.
 */
SRD::HistogramBase* HistogramCanvas::GetHistogram() {
   return pHistogram_;
}

/**
 * Computa los parametros deseados.
 */
void HistogramCanvas::Flush(const suri::Mask *pMask) {
   // Creo el objeto para guardar las estadisticas (mover a donde corresponde)
   if (pHistogram_ == NULL) {
      int* pBins = new int[GetBandCount()];
      double* pMins = new double[GetBandCount()];
      double* pMaxs = new double[GetBandCount()];

      for (int ix = 0, lenix = GetBandCount(); ix < lenix; ++ix) {
         pBins[ix] = bins_;

         if (HasCustomMinMax()) {
            pMins[ix] = customMin_[ix];
            pMaxs[ix] = customMax_[ix];
         } else {
            pMins[ix] = pStatistics_->GetMin(ix);
            pMaxs[ix] = pStatistics_->GetMax(ix);
         }
      }
      pHistogram_ = CreateHistogramFromDataType(GetDataType(), GetBandCount(), pBins,
                                                pMins, pMaxs);

      pHistogram_->SetNoDataValue(GetNoDataValue());
      pHistogram_->SetNoDataValueAvailable(IsNoDataValueAvailable());
      pHistogram_->SetAllBandsNdv(GetAllBandsNdv());
   }

   // Leo datos.
   std::vector<void*> indata;
   std::vector<int> bands;
   for (int ix = 0; ix < GetBandCount(); ++ix)
      bands.push_back(ix);
   GetInternalData(bands, indata);

   int width = 0, height = 0;
   GetSize(width, height);

   // Proceso los datos
   for (int bandix = 0, bandlen = GetBandCount(); bandix < bandlen; ++bandix) {
      pHistogram_->CountPixels(bandix, width * height, indata[bandix]);
   }
}

/**
 * Crea el objeto para calculo del histograma a partir del tipo de dato del provisto.
 */
SRD::HistogramBase* HistogramCanvas::CreateHistogramFromDataType(
      const std::string& DataType, int BandCount, int* pBins, double* pMins,
      double* pMaxs) {
   SRD::HistogramBase* pret = NULL;
   if (DataType.compare(suri::DataInfo<unsigned char>::Name) == 0) {
      pret = new SRD::Histogram<unsigned char>(BandCount, pBins, pMins, pMaxs);
   } else if (DataType.compare(suri::DataInfo<unsigned short>::Name) == 0) {
      pret = new SRD::Histogram<unsigned short>(BandCount, pBins, pMins, pMaxs);
   } else if (DataType.compare(suri::DataInfo<short>::Name) == 0) {
      pret = new SRD::Histogram<short>(BandCount, pBins, pMins, pMaxs);
   } else if (DataType.compare(suri::DataInfo<unsigned int>::Name) == 0) {
      pret = new SRD::Histogram<unsigned int>(BandCount, pBins, pMins, pMaxs);
   } else if (DataType.compare(suri::DataInfo<int>::Name) == 0) {
      pret = new SRD::Histogram<int>(BandCount, pBins, pMins, pMaxs);
   } else if (DataType.compare(suri::DataInfo<float>::Name) == 0) {
      pret = new SRD::Histogram<float>(BandCount, pBins, pMins, pMaxs);
   } else if (DataType.compare(suri::DataInfo<double>::Name) == 0) {
      pret = new SRD::Histogram<double>(BandCount, pBins, pMins, pMaxs);
   }
   return pret;
}

/**
 * Devuelve el valor minimo definido de manera personalizada.
 */
double  HistogramCanvas::GetCustomMin(int Band) const {
   if (static_cast<size_t>(Band) > customMin_.size() - 1)
      return std::numeric_limits<double>::quiet_NaN();
   return customMin_[Band];
}

/**
 * Establece el valor minimo de forma personalizada.
 */
void  HistogramCanvas::SetCustomMin(double Min) {
   customMinMaxSet_ = true;
   int bandcount =
         pStatistics_ && GetBandCount() == 0 ? pStatistics_->GetBandCount() :
                                               GetBandCount();
   customMin_ = std::vector<double>(bandcount, Min);
}

/**
 * Devuelve el valor minimo definido de manera personalizada.
 */
double  HistogramCanvas::GetCustomMax(int Band) const {
   if (static_cast<size_t>(Band) > customMax_.size() - 1)
      return std::numeric_limits<double>::quiet_NaN();
   return customMax_[Band];
}

/**
 * Establece el valor minimo de forma personalizada.
 */
void  HistogramCanvas::SetCustomMax(double Max) {
   customMinMaxSet_ = true;
   int bandcount =
         pStatistics_ && GetBandCount() == 0 ? pStatistics_->GetBandCount() :
                                               GetBandCount();
   customMax_ = std::vector<double>(bandcount, Max);
}

/**
 * Establece el valor minimo de forma personalizada.
 */
void HistogramCanvas::SetCustomMax(double Max, int Band) {
   customMinMaxSet_ = true;
   if (customMax_.empty()) {
      int bandcount =
            pStatistics_ && GetBandCount() == 0 ? pStatistics_->GetBandCount() :
                                                  GetBandCount();
      customMax_ = std::vector<double>(bandcount);
   }
   if (static_cast<size_t>(Band) < customMax_.size()) customMax_[Band] = Max;
}

/**
 * Establece el valor minimo de forma personalizada.
 */
void HistogramCanvas::SetCustomMin(double Min, int Band) {
   customMinMaxSet_ = true;
   if (customMin_.empty()) {
      int bandcount =
            pStatistics_ && GetBandCount() == 0 ? pStatistics_->GetBandCount() :
                                                  GetBandCount();
      customMin_ = std::vector<double>(bandcount);
   }
   if (static_cast<size_t>(Band)) customMin_[Band] = Min;
}

/**
 * Indica si tiene minimos y maximos que no vienen de la estadistica basica.
 */
bool  HistogramCanvas::HasCustomMinMax() const {
   return customMinMaxSet_;
}

}  // namespace render
}  // namespace suri
