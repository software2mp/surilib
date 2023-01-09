/** \file SRELinearEnhancement.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRELINEARENHANCEMENT_H_
#define SRELINEARENHANCEMENT_H_

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase que implementa un realce lineal.
 */
class LinearEnhancement : public Enhancement {
public:
   /**
    * Ctor.
    */
   LinearEnhancement(int BandCount, int* pNumBins, double** ppBAFP = NULL);

   /**
    * Dtor.
    */
   virtual ~LinearEnhancement();

   /**
    * Crea una lookup table a partir de un histograma de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de EqualizationEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);

protected:
   double** ppBAFP_;  // Porcentaje de frecuencia acumulada por bin.
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SRELINEARENHANCEMENT_H_
