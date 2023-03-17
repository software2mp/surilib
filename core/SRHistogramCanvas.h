/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
