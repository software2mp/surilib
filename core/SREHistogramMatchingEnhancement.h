/*! \file SREHistogramMatchingEnhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LIB_SURI_CORE_SREHISTOGRAMMATCHINGENHANCEMENT_H_
#define LIB_SURI_CORE_SREHISTOGRAMMATCHINGENHANCEMENT_H_

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase que implementa un realce por combinacion de histogramas.
 */
class HistogramMatchingEnhancement : public Enhancement {
public:
   /**
    * Ctor.
    */
   HistogramMatchingEnhancement(int BandCount, int* pNumBins);

   /**
    * Dtor.
    */
   virtual ~HistogramMatchingEnhancement();

   /**
    * Crea una lookup table a partir de dos histogramas de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de EqualizationEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);

private:
   /**
    * Calcula la funcion de distribucion acumulada para el histograma dado.
    */
   double** CalculateCdf(int** ppHist);
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // LIB_SURI_CORE_SREHISTOGRAMMATCHINGENHANCEMENT_H_
