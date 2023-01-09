/*! \file EqualizationEnhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef EQUALIZATIONENHANCEMENT_H_
#define EQUALIZATIONENHANCEMENT_H_

// Includes suri
#include "Enhancement.h"

/** namespace suri */
namespace suri {
/** Clase para generar un realce (Lut) de Ecualizacion de Histograma */
/**
 * Clase que crea una Lut que aplica un realce de tipo Ecualizacion de Histograma.
 * Devuelve cuales son los parametros estadisticos necesarios para este realce
 * (Min, Max e Histograma). Calcula los parametros necesarios para aplicar la
 * funcion que crea los valores para el realce.
 */
class EqualizationEnhancement : public Enhancement {
public:
   /** Constructor */
   EqualizationEnhancement();
   /** Dtor. */
   virtual ~EqualizationEnhancement();
private:
   /** Crea una Lut a partir de los parametros recibidos */
   virtual LookUpTable CreateLut(const Statistics &Stats,
                                 const std::pair<double, double> &OutputRange,
                                 int BinCount,
                                 size_t BandNumber) const;
   /** Devuelve los parametros estadisticos relevantes para la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const;
   /** Obtiene el histograma normalizado acumulado */
   void CalculateFunctionParameters(
         const Histogram &Histogram,
         std::map<double, double> &NormalizedHistogram) const;
};
}

#endif /* EQUALIZATIONENHANCEMENT_H_ */
