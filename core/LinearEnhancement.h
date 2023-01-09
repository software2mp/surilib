/*! \file LinearEnhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LINEARENHANCEMENT_H_
#define LINEARENHANCEMENT_H_

// Include suri
#include "Enhancement.h"

/** namespace suri */
namespace suri {
/** Clase que crea un realce (Lut) de tipo lineal */
/**
 * Clase que crea Luts a partir de parametros estadisticos utilizando una funcion
 * de realce lineal. Esta funcion es una recta que pasa por los puntos formados
 * por (minimo valor de la banda, minimo valor del rango de salida) y (maximo
 * valor de la banda, maximo valor del rango de salida).
 */
class LinearEnhancement : public Enhancement {
public:
   /** Ctor. */
   LinearEnhancement();
   /** Dtor. */
   virtual ~LinearEnhancement();
   /** Crea una Lut a partir de los parametros recibidos */
   virtual LookUpTable CreateLut(const Statistics &Stats,
                                 const std::pair<double, double> &OutputRange,
                                 int BinCount,
                                 size_t BandNumber) const;
   /** Devuelve los parametros estadisticos relevantes para la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const;
protected:
   /** Obtiene el minimo y el maximo relevantes de una banda para la funcion de realce */
   virtual void GetLimits(const Statistics &Stats, size_t BandNumber, double &Min,
                          double &Max) const;
   /** Calcula la pendiente y la ordenada al origen de la recta */
   void CalculateFunctionParameters(double Min, double Max,
                                    const std::pair<double, double> &OutputRange,
                                    double &Slope,
                                    double &Intercept) const;
};
}

#endif /* LINEARENHANCEMENT_H_ */
