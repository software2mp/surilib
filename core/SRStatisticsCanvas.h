/** \file SRStatisticsCanvas.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRSTATISTICSCANVAS_H_
#define SRSTATISTICSCANVAS_H_

#include "MemoryCanvas.h"
#include "SRDStatistics.h"

namespace suri {
namespace render {

namespace SRD = suri::raster::data;

/**
 * Clase que representa un canvas para la generacion de estadisticas.
 */
class StatisticsCanvas : public suri::MemoryCanvas {
public:
   /**
    * Ctor.
    */
   StatisticsCanvas(bool InterBandStatistics = false);

   /**
    * Dtor.
    */
   virtual ~StatisticsCanvas();

   /**
    * Devuelve las estadisticas.
    */
   SRD::StatisticsBase* GetStatistics();

   /**
    * Computa los parametros deseados.
    */
   virtual void Flush(const suri::Mask *pMask);

   /**
    * Crea el objeto para calculo de estadisticas a partir del tipo de dato del provisto.
    */
   static SRD::StatisticsBase* CreateStatisticsFromDataType(const std::string& DataType, int BandCount);

private:
   SRD::StatisticsBase* pStatistics_;
   bool interBandStatistics_;
};

}  // namespace render
}  // namespace suri

#endif  // SRSTATISTICSCANVAS_H_
