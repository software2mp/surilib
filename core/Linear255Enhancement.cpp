/*! \file Linear255Enhancement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandar
#include <utility>

// Includes Suri
#include "suri/AuxiliaryFunctions.h"
#include "Linear255Enhancement.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, Linear255Enhancement, 0);

Linear255Enhancement::Linear255Enhancement() {
}

Linear255Enhancement::~Linear255Enhancement() {
}

/** Crea una lut para la banda indicada con los parametros recibios */
LookUpTable Linear255Enhancement::CreateLut(const Statistics &Stats,
                                            const std::pair<double, double> &OutputRange,
                                            int BinCount, size_t BandNumber) const {
   LookUpTable lut;
   double slope = 0, intercept = 0, segment = 0, min = 0.0, max = 255.0;
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

/** Devuelve los parametros relevantes segun la funcion de realce */
void Linear255Enhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::MinValue | Statistics::MaxValue;
}

}
