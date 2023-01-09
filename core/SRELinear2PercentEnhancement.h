/** \file SRELinear2PercentEnhancement.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRELINEAR2PERCENTENHANCEMENT_H_
#define SRELINEAR2PERCENTENHANCEMENT_H_

#include <climits>

#include "SRELinearEnhancement.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase que implementa un realce lineal 2 porciento.
 */
class Linear2PercentEnhancement : public LinearEnhancement {
public:
   /**
    * Ctor.
    */
   Linear2PercentEnhancement(int BandCount, int* pNumBins, double** ppBAFP);

   /**
    * Dtor.
    */
   virtual ~Linear2PercentEnhancement();

   /**
    * Crea una lookup table a partir de un histograma de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de EqualizationEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SRELINEAR2PERCENTENHANCEMENT_H_
