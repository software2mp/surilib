/*! \file Linear2PercentEnhancement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "Linear2PercentEnhancement.h"

/** Porcentaje de valores a cada lado de la distribucion que son excluidos para el calculo del */
// realce
#define EXCLUDED_VALUES_PERCENT 2

/** namespace suri */
namespace suri {
/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, Linear2PercentEnhancement, 0);

/** Constructor */
Linear2PercentEnhancement::Linear2PercentEnhancement() {
}

/** Destructor */
Linear2PercentEnhancement::~Linear2PercentEnhancement() {
}

/**
 *	Devuelve los parametros estadisticos relevantes para la funcion de realce
 *	@param[out] Parameters histograma, el maximo y el minimo, necesarios para
 *	el calculo de la lut.
 */
void Linear2PercentEnhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::Histogram;
}

/**
 * Obtiene el minimo y el maximo relevantes de una banda para la funcion de
 * realce, valores que excluyen el 2% de los valores a cada extremo del histograma.
 * @param[in] Stats estadisticas
 * @param[in] BandNumber numero de banda
 * @param[out] Min valor minimo, que deja fuera el 2% de los valores.
 * @param[out] Max valor maximo, que deja fuera el 2% de los valores.
 */
void Linear2PercentEnhancement::GetLimits(const Statistics &Stats, size_t BandNumber,
                                          double &Min, double &Max) const {
   Min = GetMinNPercent(Stats.histogram_[BandNumber], EXCLUDED_VALUES_PERCENT);
   Max = GetMaxNPercent(Stats.histogram_[BandNumber], EXCLUDED_VALUES_PERCENT);
}
}
