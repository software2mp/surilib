/** \file SREEqualizationEnhancement.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SREEQUALIZATIONENHANCEMENT_H_
#define SREEQUALIZATIONENHANCEMENT_H_

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase que implementa un realce de ecualizacion.
 */
class EqualizationEnhancement : public Enhancement {
public:
   /**
    * Ctor.
    */
   EqualizationEnhancement(int BandCount, int* pNumBins, int* pAccumFreq);

   /**
    * Dtor.
    */
   virtual ~EqualizationEnhancement();

   /**
    * Crea una lookup table a partir de un histograma de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de EqualizationEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);


private:
   int* pAccumFreq_;  // Frecuencia acumulada.
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SREEQUALIZATIONENHANCEMENT_H_
