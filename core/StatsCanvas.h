/*! \file StatsCanvas.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef STATSCANVAS_H_
#define STATSCANVAS_H_

/**
 *
 */

// Includes estandar
// Includes Suri
#include "suri/XmlFunctions.h"
#include "suri/StatisticsFunctions.h"
// Includes Wx

// Includes App

// Defines

#include "MemoryCanvas.h"

/** namespace suri */
namespace suri {
/** En flush utiliza los datos en imagen para calcular las estadisticas. */
/**
 * Primero se utiliza canvas para renderizar, luego al hacer flush se
 * utilizan los datos para calcular las estadisticas.
 * Por razones de performance se puede elegir las estadisticas que se calculan
 * utilizando SetRequiredStats.
 */
class StatsCanvas : public MemoryCanvas {
public:
   /** ctor */
   StatsCanvas();
   /** dtor */
   virtual ~StatsCanvas();

// ----------------------------- VECTORIAL -----------------------------
   /** Canvas vectorial */
   virtual wxDC *GetDC();
   /** Computa los parametros deseados */
   virtual void Flush(const Mask *pMask);
// --------------------------- ESTADISTICAS ---------------------------
   /** Devuelve las estadisticas. */
   Statistics GetStatistics();
#ifdef __UNUSED_CODE__
   /** Todos estos metodos se reemplazan por el metodo GetStatistics, que */
   /** que devuelve la variable que contiene a todos los valores estadisticos */
   /** Retorna la cuenta de muestras */
   size_t GetCount() const;
   /** Retorna los valores de las medias computadas para cada banda */
   std::vector<double> GetMean() const;
   /** Retorna los valores de las varianzas computadas para cada banda */
   std::vector<double> GetVariance() const;
   /** Retorna los valores de la matriz de covarianza */
   std::vector<std::vector<double> > GetCovarianceMatrix() const;
   /** Retorna los valores minimos para cada banda */
   std::vector<double> GetMin() const;
   /** Retorna los valores maximos para cada banda */
   std::vector<double> GetMax() const;
#endif
   /** Vuelve a estado inicial los parametros estadisticos */
   void ResetStats();
   /** Establece que estadisticas van a ser computadas */
   void SetRequiredStats(Statistics::StatisticsFlag RequiredStats);

   /** Establece si debe resetearse el estado interno de las estadisticas o no */
   void SetReset(bool Reset);

private:
   Statistics statistics_;  // Estadisticas
   Statistics::StatisticsFlag requiredStats_;  // Indica que estdadisticas van a ser calculadas
   bool reset_;  // Indica si debe resetearse el estado interno de las estadisticas o no
};
}

#endif /* STATSCANVAS_H_ */
