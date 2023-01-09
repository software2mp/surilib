/*! \file StatisticsCalculator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef STATISTICSCALCULATOR_H_
#define STATISTICSCALCULATOR_H_

// Includes Estandar
// Includes Suri
#include "SRDStatistics.h"
#include "SRDHistogram.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class RasterElement;
class LayerList;
class World;

namespace raster {

namespace data {
/**
 * Clase utilitaria para el calculo de estadisticas sobre una capa raster
 *
 */
class StatisticsCalculator {
   /** constructor copia **/
   StatisticsCalculator(const StatisticsCalculator&);

public:
   /**
    * Ctor.
    */
   explicit StatisticsCalculator(RasterElement* pRaster);

   /**
    * Dtor.
    */
   ~StatisticsCalculator();

   /**
    * Realiza el calculo de estadisticas sobre el raster configurado.
    */
   bool CalculateStatistics(suri::raster::data::StatisticsBase* &pStatistics,
                            bool ComputeAllBands = false, bool InterBandStats = false,
                            bool UseViewer = true);


   /*
    * Realiza el calculo de estadisticas e histograma sobre el raster configurado.
    */
   bool CalculateStatistics(suri::raster::data::StatisticsBase* &pStatistics,
                            suri::raster::data::HistogramBase* &pHistogram,
                            bool ComputeAllBands = false, bool InterBandStats = false,
                            const std::vector<double>& Min = std::vector<double>(),
                            const std::vector<double>& Max = std::vector<double>());
   /** Realiza el calculo de histograma en funcion de las estadisticas que se pasan por parametro **/
   bool CalculateHistogram(suri::raster::data::StatisticsBase* pStatistics,
                           suri::raster::data::HistogramBase** pHistogram,
                           bool ComputeAllBands,
                           const std::vector<double>& Min = std::vector<double>(),
                           const std::vector<double>& Max = std::vector<double>());

private:
   /**
    * Metodo auxiliar que realiza el calculo de estadistica e histograma.
    */
   bool DoCalculateStatistics(suri::raster::data::StatisticsBase** pStatistics,
                              suri::raster::data::HistogramBase** pHistogram = NULL,
                              bool CalculateHistogram = false, bool ComputeAllBands =
                                    false,
                              bool InterBandStats = false,
                              const std::vector<double>& Min = std::vector<double>(),
                              const std::vector<double>& Max = std::vector<double>());

   /**
    * Configura el mundo para el renderizador. Si hay un raster activo asigna al
    * window y al extent del mundo recibido el subset interseccion entre el extent
    * y el window del muno activo. Si no hay raster activo asigna al mundo recibido
    * el extent del elemento.
    * @param[in] pWorld mundo que se quiere configurar
    */
   void ConfigureWorld(World *pWorld) ;

   /**
    * Agrega un elemento a la lista.
    * @param[in] pList Lista con el elemento a renderizar
    */
   void ConfigureList(LayerList *pList, bool ComputeAllBands);

   /**
    * Crea un raster a partir del elemento raster sobre el que se aplica la herramienta,
    * y le agrega la combinacion de bandas del elemento original.
    * @return pelement Elemento raster creado a partir del original
    * \attention el elemento devuelto y su eliminacion son responsabilidad del
    * solicitante.
    */
   RasterElement * CreateElement(bool ComputeAllBands = false);

   /**
    * Devuelve el valor de dato invalido si esta definido.
    */
   void RetrieveNoDataValue(bool& HasNoDataValue, double& NoDataValue);

   /** raster sobre el cual se calculan las estadisticas **/
   RasterElement* pElement_;

   bool useViewer_;
};

} /** namespace raster **/
} /** namespace data **/
} /** namespace suri */

#endif /* STATISTICSCALCULATOR_H_ */
