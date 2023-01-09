/** \file SREEnhancement.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SREENHANCEMENT_H_
#define SREENHANCEMENT_H_

#include <cstddef>

#include "wx/xml/xml.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase base para todos los tipos de realces (mejoras)
 */
class Enhancement {
public:
   /**
    * Ctor.
    */
   Enhancement(int BandCount, int* pNumBins) : bandCount_(BandCount), pNumBins_(pNumBins) { }

   /**
    * Dtor.
    */
   virtual ~Enhancement() { }

   /**
    * Crea una lookup table a partir de dos histogramas de intensidad.
    * (Los dos histogramas por el momento son solo utilizados por el realce
    * por combiancion de histogramas)
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL) = 0;

protected:
   int bandCount_;  // Cantidad de bandas.
   int* pNumBins_;  // Numero de bins por banda.
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // SREENHANCEMENT_H_
