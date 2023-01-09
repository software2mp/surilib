/*! \file Enhancement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandar
// Includes Suri
#include "suri/AbstractFactory.h"
// Includes Wx

// Includes App

// Defines

// Migrar a xmlnames.h
#define ENHANCEMENT_TYPE "Realce"

#include "Enhancement.h"

/** namespace suri */
namespace suri {
/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION_COMMON(Enhancement, 0)

/** ctor */
Enhancement::Enhancement() {
}

/** dtor */
Enhancement::~Enhancement() {
}

/**
 *	Obtiene las estadisticas relevantes y verifica la consistencia.
 *	Recorre la dimension de los vectores estadisticos y para cada valor, que
 *	representa una banda de entrada, obtiene el valor maximo y el valor minimo y
 *	genera una lut.
 *	@param[out] Lut array donde se agregan las Lut generadas
 *	@param[in]  Stats estadisticas
 *	@param[in] OutputRange rango para los valores de salida de la lut
 *	@param[in] BinCount cantidad de bins para dividir el rango de valores
 *	@return true si las dimensiones estadisticas de los parametros son consistentes
 *	@return false si las dimensiones son inconsistentes.
 */
bool Enhancement::ComputeLut(LutArray &Lut, const Statistics &Stats,
                             const std::pair<double, double> &OutputRange,
                             int BinCount) const {
   // Verifica la consistencia de los parametros relevantes
   Statistics::StatisticsFlag parameters = Statistics::All;
   int dimensions = 0;
   GetValidParameters(parameters);
   // si necesita algun tipo de estadistica calcula las dimensiones
   if (parameters != Statistics::None) {
      dimensions = Stats.GetDimension(parameters);
      // si hay inconsistencia de datos, sale
      if (dimensions == -1 || BinCount < 1 || OutputRange.first > OutputRange.second) {
         return false;
      }
   }
   // Configura los parametros de la lut
   Lut.SetName(GetClassId());

   // Creo una Lut por cada banda
   for (size_t i = 0; i < static_cast<unsigned int>(dimensions); i++) {
      LookUpTable lut = CreateLut(Stats, OutputRange, BinCount, i);
      Lut.AddLookUpTable(lut, i);
   }

   return true;
}

/**
 * Calcula el LutArray para realces con rango 0-255 y 256 bins
 *	@param[out] Lut array donde se agregan las Lut generadas
 *	@param[in]  Stats estadisticas
 *	@return true si pudo crear las luts, false en otro caso
 */
bool Enhancement::ComputeLut(LutArray &Lut, const Statistics &Stats) const {
   return ComputeLut(Lut, Stats, std::make_pair(0, 255), 256);
}
}
