/*! \file Enhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENHANCEMENT_H_
#define ENHANCEMENT_H_

// Includes estandar

// Includes Suri

// Includes Wx
#include "suri/FactoryHelpers.h"
#include "suri/LutArray.h"
#include "suri/StatisticsFunctions.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase base para la implementacion de realces automaticos */
/**
 *  Clase de la que se debe heredar para implementar un realce automatico. Debe
 * generar un LutArray con las tablas a aplicar para dicho realce.
 * Utiliza las estadisticas de ser necesario segun la implementacion del realce.
 *
 *  Utiliza Factoria y metodo Create(Id).
 *
 * Metodo con el cual se consulta los parametros estadisticos que necesita
 * el tipo de realce.
 */
class Enhancement {
   /** Declara metodos privados y publicos de la clase Enhancement. */
HAS_FACTORY(Enhancement, std::string);
public:
   /** ctor */
   Enhancement();
   /** dtor */
   virtual ~Enhancement();
   /** Calcula el LutArray en funcion de las estadisticas, el rango y los bins */
   /**
    *
    * @param[out] Lut Tablas que implementan el realce dado, para las
    *    estadisticas.
    * @param[in] Stats Estadisticas que se utilizan para calcuar el realce.
    * @param[in] OutputRange Rango de salida de las Tablas
    * @param[in] BinCount Cantidad de intervalos para el rango
    * @return True si pudo calcular la tabla
    * @return False si falla al calcular la tabla
    */
   virtual bool ComputeLut(LutArray &Lut, const Statistics &Stats,
                           const std::pair<double, double> &OutputRange,
                           int BinCount) const;
   /** Calcula el LutArray para realces con rango 0-255 y 256 bins */
   bool ComputeLut(LutArray &Lut, const Statistics &Stats) const;
   /** Crea una lut para la banda indicada con los parametros recibios */
   virtual LookUpTable CreateLut(const Statistics &Stats,
                                 const std::pair<double, double> &OutputRange,
                                 int BinCount,
                                 size_t BandNumber) const=0;
   /** Devuelve los parametros relevantes segun la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const=0;
};
}

#endif /* ENHANCEMENT_H_ */
