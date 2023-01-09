/** \file SRELinear255Enhancement.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRELINEAR255ENHANCEMENT_H_
#define SRELINEAR255ENHANCEMENT_H_

#include "SRELinearEnhancement.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase que implementa un realce lineal 0-255.
 */
class Linear255Enhancement : public LinearEnhancement {
public:
   /**
    * Ctor.
    */
   Linear255Enhancement(int BandCount, int* pNumBins);

   /**
    * Dtor.
    */
   virtual ~Linear255Enhancement();

   /**
    * Crea una instancia de Linear255Enhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SRELINEAR255ENHANCEMENT_H_
