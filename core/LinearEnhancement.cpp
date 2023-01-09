/*! \file LinearEnhancement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Inclues standar
#include <algorithm>

// Includes suri
#include "LinearEnhancement.h"
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {

/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, LinearEnhancement, 0);

/**
 * Constructor
 */
LinearEnhancement::LinearEnhancement() {
}

/**
 * Destructor
 */
LinearEnhancement::~LinearEnhancement() {
}

/**
 * Crea una Lut utilizando para transformar los valores de entrada una funcion
 * lineal.
 * @param[in] Stats estadisticas de la banda
 * @param[in] OutputRange rango para los valores de salida
 * @param[in] BinCount numero de bins (segementos) entre los que se divide el
 *    rango de valores
 * @param[in] BandNumber numero de banda de la imagen
 * @return lut tabla de busqueda correspondiente al realce, o una lut vacia si
 *    fallo alguno de los parametros.
 */
LookUpTable LinearEnhancement::CreateLut(const Statistics &Stats,
                                         const std::pair<double, double> &OutputRange,
                                         int BinCount, size_t BandNumber) const {
   LookUpTable lut;
   double slope = 0, intercept = 0, segment = 0, min = 0, max = 0;
   GetLimits(Stats, BandNumber, min, max);
   // Obtiene la pendiente y la ordenada de la recta (funcion lineal de realce)
   CalculateFunctionParameters(min, max, OutputRange, slope, intercept);
   // Calcula el tamanio de cada bin
   segment = (max - min) / (BinCount - 1);
   // Si los limites son iguales asigna cero a la cantiad de bins para agregar
   // una sola entrada en la tabla.
   if (max == min) {
      BinCount = 0;
   }
   // El for es hasta <= la cantidad de bins, porque es necesario agregar
   // bins+1 entradas
   for (int i = 0; i < BinCount; i++)
      lut.AddBin(min + i * segment, SURI_ROUND(int, (min+i*segment)*slope+intercept));
   return lut;
}

/**
 * Devuelve el parametro relevante para la funcion de realce, en este caso
 * el maximo y el minimo.
 * @param[out] Parameters minimo y maximo
 */
void LinearEnhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::MinValue | Statistics::MaxValue;
}

/**
 * Obtiene el maximo y el minimo valor de la banda.
 * @param[in] Stats estadisticas de las bandas
 * @param[in] BandNumber numero de banda de la cual se quieren el max y min
 * @param[out] Min minimo valor de la banda
 * @param[out] Max maximo valor de la banda
 */
void LinearEnhancement::GetLimits(const Statistics &Stats, size_t BandNumber,
                                  double &Min, double &Max) const {
   if (Stats.min_.size() < BandNumber || Stats.max_.size() < BandNumber) {
      return;
   }
   Min = Stats.min_[BandNumber];
   Max = Stats.max_[BandNumber];
}

/**
 * Obtiene la pendiente y la ordenada al origen de la recta.
 * @param[in] Min minimo valor
 * @param[in] Max maximo valor
 * @param[in] OutputRange rango de valores de salida
 * @param[out] Slope pendiente de la recta
 * @param[out] Intercept ordenada al origen de la recta
 */
void LinearEnhancement::CalculateFunctionParameters(
      double Min, double Max, const std::pair<double, double> &OutputRange,
      double &Slope,
      double &Intercept) const {
   if (Max != Min) {
      Slope = (OutputRange.second - OutputRange.first) / (Max - Min);
      Intercept = (OutputRange.first * Max - OutputRange.second * Min) / (Max - Min);
   }
   // Si el maximo y el minimo son iguales devuelve el valor del Min si esta dentro
   // del rango de salida, o el limite del rango que supero (superior o inferior)
   else {
      Slope = 0;
      Intercept =
            (Min < OutputRange.first) ? OutputRange.first :
                                        std::min(Min, OutputRange.second);
   }
}

}
