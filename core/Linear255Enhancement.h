/*! \file Linear255Enhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef LINEAR255ENHANCEMENT_H_
#define LINEAR255ENHANCEMENT_H_

// Includes estandar

// Includes Suri
#include "LinearEnhancement.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/**
 * todo: documentar
 */
class Linear255Enhancement : public LinearEnhancement {
public:
   /** ctor */
   Linear255Enhancement();
   /** dtor */
   virtual ~Linear255Enhancement();
   /** Crea una lut para la banda indicada con los parametros recibios */
   virtual LookUpTable CreateLut(const Statistics &Stats,
                                 const std::pair<double, double> &OutputRange,
                                 int BinCount,
                                 size_t BandNumber) const;
   /** Devuelve los parametros relevantes segun la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const;
};
}

#endif /* LINEAR255ENHANCEMENT_H_ */
