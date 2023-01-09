/** \file SRHistogramCanvas.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRHISTOGRAMCANVAS_H_
#define SRHISTOGRAMCANVAS_H_

#include "SRDStatistics.h"
#include "MemoryCanvas.h"

namespace suri {

namespace raster {
namespace data {
// forwards
class HistogramBase;
}
}

namespace render {

namespace SRD = suri::raster::data;

/**
 * Clase que representa un canvas para la generacion de un histograma
 * de intensidad.
 */
class HistogramCanvas : public suri::MemoryCanvas {
public:
   static const int kIntensityBins = 256;

   /**
    * Ctor.
    */
   explicit HistogramCanvas(int Bins = kIntensityBins);

   /**
    * Dtor.
    */
   virtual ~HistogramCanvas();

   /**
    * Establece las estadisticas.
    */
   void SetStatistics(SRD::StatisticsBase* pStatistics);

   /**
    * Obtiene el histograma.
    */
   SRD::HistogramBase* GetHistogram();

   /**
    * Computa los parametros deseados.
    */
   virtual void Flush(const suri::Mask *pMask);
   /**
    * Crea el objeto para calculo del histograma a partir del tipo de dato del provisto.
    */
   static SRD::HistogramBase* CreateHistogramFromDataType(const std::string& DataType,
                                                          int BandCount, int* pBins,
                                                          double* pMins, double* pMaxs);

   /**
    * Devuelve el valor minimo definido de manera personalizada.
    */
   double GetCustomMin(int Band = 0) const;

   /**
    * Establece el valor minimo de forma personalizada.
    */
   void SetCustomMin(double Min);
   /**
    * Establece el valor minimo de forma personalizada.
    */
   void SetCustomMin(double Min, int Band);

   /**
    * Devuelve el valor minimo definido de manera personalizada.
    */
   double GetCustomMax(int Band = 0) const;

   /**
    * Establece el valor minimo de forma personalizada.
    */
   void SetCustomMax(double Max);
   /**
    * Establece el valor minimo de forma personalizada.
    */
   void SetCustomMax(double Max, int Band);

   /**
    * Indica si tiene minimos y maximos que no vienen de la estadistica basica.
    */
   bool HasCustomMinMax() const;

private:
   SRD::StatisticsBase* pStatistics_;
   SRD::HistogramBase* pHistogram_;
   int bins_;
   std::vector<double> customMin_;
   std::vector<double> customMax_;
   bool customMinMaxSet_;
};

}  // namespace render
}  // namespace suri

#endif  // SRHISTOGRAMCANVAS_H_
