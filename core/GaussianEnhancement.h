/*! \file GaussianEnhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GAUSSIANENHANCEMENT_H_
#define GAUSSIANENHANCEMENT_H_

// Includes suri
#include "Enhancement.h"
#include "suri/LookUpTable.h"
#include "suri/StatisticsFunctions.h"

/** namespace suri */
namespace suri {
/** Clase que crea el realce (Lut) de tipo Gaussiano */
/**
 * Clase que crea una Lut que aplica un realce de tipo Gaussiano.
 * Devuelve cuales son los parametros estadisticos necesarios para este realce
 * (Media, Varianza, Min, Max). Aplicar la funcion que crea los valores para
 * el realce.
 */
class GaussianEnhancement : public Enhancement {
public:
   /** Ctor. */
   GaussianEnhancement();
   /** Dtor. */
   virtual ~GaussianEnhancement();
private:
   /** Crea una Lut a partir de los parametros recibidos */
   virtual LookUpTable CreateLut(const Statistics &Stats,
                                 const std::pair<double, double> &OutputRange,
                                 int BinCount,
                                 size_t BandNumber) const;
   /** Devuelve los parametros estadisticos relevantes para la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const;
};
}

#endif /* GAUSSIANENHANCEMENT_H_ */
