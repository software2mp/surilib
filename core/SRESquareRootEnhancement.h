/*! \file SRESquareRootEnhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRESQUAREROOTENHANCEMENT_H_
#define SRESQUAREROOTENHANCEMENT_H_

#include "SRELinearEnhancement.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase que implementa un realce de tipo raiz cuadrada.
 */
class SquareRootEnhancement : public LinearEnhancement {
public:
   /**
    * Ctor.
    */
   SquareRootEnhancement(int BandCount, int* pNumBins);

   /**
    * Dtor.
    */
   virtual ~SquareRootEnhancement();

   /**
    * Crea una lookup table a partir de un histograma de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de SquareRootEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);
};

} /** namespace enhancement */
} /** namespace raster */
} /** namespace suri */

#endif /* SRESQUAREROOTENHANCEMENT_H_ */
