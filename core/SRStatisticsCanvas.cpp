/** \file SRStatisticsCanvas.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
