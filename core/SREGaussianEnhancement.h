/*! \file SREGaussianEnhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SREGAUSSIANENHANCEMENT_H_
#define SREGAUSSIANENHANCEMENT_H_

#include <map>

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

class GaussianEnhancement : public Enhancement {
public:
   typedef std::pair<double, double> range;

   /**
    * Ctor.
    */
   GaussianEnhancement(int BandCount, int* pNumBins, double* pMin, double* pMax,
                       double** ppBAFP, double* pMean, double* pStdev);

   /**
    * Dtor.
    */
   virtual ~GaussianEnhancement();

   /**
    * Crea una lookup table a partir de un histograma de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de GaussianEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);

private:
   double* pMin_;
   double* pMax_;
   double* pMean_;
   double* pStdev_;
   double** ppBAFP_;  // Porcentaje de frecuencia acumulada por bin.
   std::map<range, int> gaussRangeTable_;

   /**
    * Metodo auxiliar que llena la tabla de rangos para la dist normal.
    */
   void LoadRangeTable();

   /**
    * Obtiene el valor de intensidad correspondiente al rango dado.
    */
   int GetValueFromMap(const std::pair<double, double>& Pair);
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif /* SREGAUSSIANENHANCEMENT_H_ */
