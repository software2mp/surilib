/*! \file GaussianEnhancement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "GaussianEnhancement.h"

// Includes suri
#include "suri/AuxiliaryFunctions.h"

// Defines parametros para la funcion de realce gaussiano, copiados de
// SuriPluma clase CEnhanceGaussian.h lineas 27 y 28
/** Parametro desvio de referencia */
#define DEVIATION_REFERENCE 64
/** Parametro media de referencia */
#define MEAN_REFERENCE 128

/** namespace suri */
namespace suri {
/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, GaussianEnhancement, 0);

/**
 * Constructor
 */
GaussianEnhancement::GaussianEnhancement() {
}

/**
 * Destructor
 */
GaussianEnhancement::~GaussianEnhancement() {
}

/**
 * Crea la lut que aplica el realce gaussiano. Verifica los parametros necesarios.
 *	@param[in] Stats estadisticas de la imagen
 *	@param[in] OutputRange maximo y minimo de los valores de salida
 *	@param[in] BinCount cantidad de intervalos
 *	@param[in] BandNumber numero de banda sobre la que se calcula la lut
 *	@return lut.
 */
LookUpTable GaussianEnhancement::CreateLut(const Statistics &Stats,
                                           const std::pair<double, double> &OutputRange,
                                           int BinCount,
                                           size_t BandNumber) const {
   // Verifica que la varianza sea positiva y mayor a cero
   if (Stats.variance_[BandNumber] <= 0) {
      return LookUpTable();
   }
   LookUpTable lut;

   double segment = 0, outputvalue = 0, min = 0, max = 0, deviation = 0, mean = 0;

   min = Stats.min_[BandNumber];
   max = Stats.max_[BandNumber];
   deviation = sqrt(Stats.variance_[BandNumber]);
   mean = Stats.mean_[BandNumber];
   // Calcula el tamanio de cada bin
   segment = (max - min) / (BinCount - 1);
   // Si los limites son iguales asigna cero a la cantiad de bins para agregar
   // una sola entrada en la tabla.
   if (max == min) {
      BinCount = 0;
   }
   // El for es hasta <= la cantidad de bins, porque es necesario agregar
   // bins+1 entradas
   for (int i = 0; i < BinCount; i++) {
      // Funcion copiada de SuriPluma, clase CEnhanceGaussian.h linea 43
      outputvalue = (DEVIATION_REFERENCE / deviation) * ((min + i * segment) - mean) +
      MEAN_REFERENCE;
      // verifica el rango de la salida
      outputvalue = std::max(outputvalue, OutputRange.first);
      outputvalue = std::min(outputvalue, OutputRange.second);
      lut.AddBin(min + i * segment, SURI_ROUND(int, outputvalue));
   }
   return lut;
}

/**
 * Devuelve los parametros relevantes (Maximo y minimo, media y varianza).
 * @param[out] Parameters representa maximo, minimo, media y varianza.
 */
void GaussianEnhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::Mean | Statistics::Variance | Statistics::MinValue
         | Statistics::MaxValue;
}
}
