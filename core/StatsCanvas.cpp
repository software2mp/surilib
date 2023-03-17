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

#include "StatsCanvas.h"

// Includes estandar
#include <limits>

// Includes Suri
#include "suri/DataTypes.h"

/**
 * Tabla de dependencias para el calculo de variables estadisticas.
 * El minimo y el maximo se agregan al calculo de todas las estadisticas, no por
 * ser necesarios sino porque no agregan calculo excesivo.
 * El numero de muestras se calcula siempre.
 * Para el significado de los hexadecimales ver enum en Statistics::StatisticsFlag
 */
/** El minimo no tiene dependencias */
#define MINVALUE_DEPENDENCIES Statistics::MaxValue
/** El maximo no tiene dependencias */
#define MAXVALUE_DEPENDENCIES Statistics::MinValue
/** La media no tiene dependencias */
#define MEAN_DEPENDENCIES Statistics::MaxValue | Statistics::MinValue
/** La varianza requiere la media */
#define VARIANCE_DEPENDENCIES Statistics::MaxValue | Statistics::MinValue \
   | Statistics::Mean
/** La matriz de covarianza requiere la varianza y la media */
#define COVARIANCE_MATRIX_DEPENDENCIES Statistics::MaxValue | Statistics::MinValue \
   | Statistics::Mean | Statistics::Variance
/** La matriz de correlacion requiere la varianza y la matriz de covarianza */
#define CORRELATION_MATRIX_DEPENDENCIES Statistics::MaxValue | Statistics::MinValue \
   | Statistics::Mean | Statistics::Variance | Statistics::CovarianceMatrix

/** El histograma no tiene dependencias */
#define HISTOGRAM_DEPENDENCIES Statistics::MaxValue | Statistics::MinValue

/** namespace suri */
namespace suri {
/** Template covmatrix */
/**
 * Calcula estadisticas
 * @param[in] pSrc imagen. Es un puntero que apunta a punteros a bandas de la imagen.
 * @param[in] Size tamano de la banda.
 * @param[in] Bands cantidad de bandas.
 * @param[out] Counts cantidad de valores que se tienen en cuenta para calcular las estadisticas.
 * @param[in] pMeans vector con medias de cada banda
 * @param[out] pMeans vector con medias actualizada de cada banda
 * @param[in] pVariances varianza de cada banda
 * @param[out] pVariances varianza de cada banda actualizada
 * @param[in] pCovarianceMatrix matriz de covarianza
 * @param[out] pCovarianceMatrix matriz de covarianza actualizada
 * @param[in] pMins minimos de cada banda
 * @param[out] pMins minimos de cada banda actualizado
 * @param[in] pMaxs maximos de cada banda
 * @param[out] pMaxs maximos de cada banda actualizado
 * @param[in] pHistograms histograma
 * @param[in] pHistograms histograma actualizado
 * \todo Falta utilizar la mascara, toma 0 (cero) como valor no valido.
 * \todo revisar comentarios.
 */
template<typename T>
void statisticparameters(void** pSrc, size_t Size, size_t Bands, size_t &Counts,
                         double NoDataValue,
                         double *pMeans, double *pVariances, double **pCovarianceMatrix,
                         double **pCorrelationMatrix, double *pMins, double *pMaxs,
                         Histogram *pHistograms) {
   // reinterpret es peligroso, pero static no anda con **
   T** psrc = reinterpret_cast<T**>(pSrc);
   // acumuladores para los distintos momentos
   size_t accum0 = 0;   // 0 = cuentas
   std::vector<double> accum1(Bands, 0.0);  // 1 = media
   std::vector<double> accum2(Bands, 0.0);  // 2 = varianza
   std::vector<std::vector<double> > accum2crossed(Bands,
                                                   std::vector<double>(Bands, 0.0));  // 2 cruzado covariaza
   for (size_t i = 0; i < Size; i++) {
      bool masked = false;
      // salgo si algun dato es nulo (enmascarado - guinyo guinyo)
      for (size_t b = 0; b < Bands; b++)
         if (psrc[b][i] == 0 || SURI_ISNAN(psrc[b][i]) || !IsFinite(psrc[b][i])) {
            masked = true;
         }
      if (!masked) {
         // Acumulacion de los momentos
         // acumulo cuentas
         ++accum0;
         for (size_t b = 0; b < Bands; b++) {
            // debo acumular orden 1 para cualquier caso
            if (pMeans || pVariances || pCovarianceMatrix) {
               accum1[b] += psrc[b][i];
            }
            // debo acumular orden 2 para varianza y matriz de covarianza
            if (pVariances || pCovarianceMatrix) {
               accum2[b] += psrc[b][i] * psrc[b][i];
            }
            // calculo por encima de la diagonal
            if (pCovarianceMatrix) {
               for (size_t j = b + 1; j < Bands; j++)
                  accum2crossed[b][j] += psrc[b][i] * psrc[j][i];
            }
            // acumula el minimo
            if (pMins) {
               if (psrc[b][i] < pMins[b]) {
                  pMins[b] = psrc[b][i];
               }
            }
            // acumula el minimo
            if (pMaxs) {
               if (psrc[b][i] > pMaxs[b]) {
                  pMaxs[b] = psrc[b][i];
               }
            }
            // Acumula el histograma
            if (pHistograms) {
               pHistograms[b].IncreaseValue(psrc[b][i]);
            }
         }
      }
   }
   size_t newcount = Counts + accum0;
   // calculo los resultados en forma incremental
   if (pMeans && accum0 != 0) {
      for (size_t b = 0; b < Bands; b++) {
         double newmean = (pMeans[b] * Counts + accum1[b]) / newcount;
         // debo acumular orden 2 para varianza y matriz de covarianza
         if (pVariances) {
            // es la media en k-1 del parametro al cuadrado
            double meanofsquared = 0.0;
            // multiplico por el factor para traerlo del estimador N-1
            if (Counts > 0) {
               meanofsquared = pVariances[b] * (Counts - 1) / Counts
                     + (pMeans[b] * pMeans[b]);
            }
            // es la media en k del parametro al cuadrado
            meanofsquared = (meanofsquared * Counts + accum2[b]) / newcount;
            // es la varianza
            pVariances[b] = meanofsquared - newmean * newmean;
            if (newcount > 1) {
               // Multiplico para convertirlo en el estimador N-1
               pVariances[b] = pVariances[b] * newcount / (newcount - 1);
            } else {
               pVariances[b] = 0.0;
            }
         }

         // calculo por encima de la diagonal
         if (pCovarianceMatrix && pVariances) {
            for (size_t j = 0; j < Bands; j++) {
               // calculo solo por debajo de la diagonal
               if (j > b) {
                  // es la media en k-1 del producto de los parametros
                  double meanofproduct = 0.0;
                  if (Counts > 0) {
                     meanofproduct = pCovarianceMatrix[b][j] * (Counts - 1) / Counts
                           + (pMeans[b] * pMeans[j]);
                  }
                  meanofproduct = (meanofproduct * Counts + accum2crossed[b][j])
                        / newcount;
                  // es la media en k-1 del segundo parametro
                  double newmeanj = (pMeans[j] * Counts + accum1[j]) / newcount;
                  pCovarianceMatrix[b][j] = meanofproduct - newmean * newmeanj;
                  if (newcount > 1) {
                     pCovarianceMatrix[b][j] = pCovarianceMatrix[b][j] * newcount
                           / (newcount - 1);
                  } else {
                     pCovarianceMatrix[b][j] = 0.0;
                  }
               }
               // la diagonal tiene la varianza
               if (j == b) {
                  pCovarianceMatrix[b][b] = pVariances[b];
               }
               // es simetrica
               if (j < b) {
                  pCovarianceMatrix[b][j] = pCovarianceMatrix[j][b];
               }
            }
         }
         // medias
         pMeans[b] = newmean;
      }

      // Matriz de Correlacion
      for (size_t b = 0; b < Bands; b++) {
         if (pCorrelationMatrix && pCovarianceMatrix && pVariances) {
            for (size_t j = 0; j < Bands; j++) {
               if (j > b) {
                  if (pVariances[b] * pVariances[j] == 0)
                     // Medida temporal; Encontrar una mejor solucion a esto.
                     pCorrelationMatrix[b][j] = 1337;
                  else
                     pCorrelationMatrix[b][j] = pCovarianceMatrix[b][j]
                           / (sqrt(pVariances[b] * pVariances[j]));
               }
               if (j == b) pCorrelationMatrix[b][j] = 1;
               if (j < b) pCorrelationMatrix[b][j] = pCorrelationMatrix[j][b];
            }
         }
      }
   }
   // acumulo cuentas
   Counts = newcount;
}

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(
      void(*)(void**, size_t, size_t, size_t&, double , double*, double*, double**, double**, double*, double*, Histogram*),
      statisticparameters);

/**
 * Constructor, inicializa la propiedad con las variables requeridas en All.
 * Si el cliente que solicita las estadisticas no aclara cuales son las que
 * necesita, se calculan todas.
 * @return instancia de la clase StatsCanvas
 */
StatsCanvas::StatsCanvas() :
      requiredStats_(Statistics::All), reset_(true) {
}

/** Destructor. */
StatsCanvas::~StatsCanvas() {
}

/**
 * Canvas vectorial
 * Retorna NULL ya que para el calculo de estadisticas no quiero tener vectores
 * dibujados en el canvas.
 * @return NULL para evitar vectores en las estadisticas.
 */
wxDC *StatsCanvas::GetDC() {
   return NULL;
}

/** Computa los parametros deseados */
/**
 *  Calcula los parametros estadisticos deseados
 */
void StatsCanvas::Flush(const Mask *pMask) {
   // Si la dimension de alguna de las estadisticas requeridas no coincide con la
   // cantidad de bandas, reseteo las variables
#ifndef __UNUSED_CODE__
   if (reset_) {
      if (((requiredStats_ & Statistics::Mean)
            && static_cast<int>(statistics_.mean_.size()) != GetBandCount())
            || ((requiredStats_ & Statistics::Variance)
                  && static_cast<int>(statistics_.variance_.size()) != GetBandCount())
            || ((requiredStats_ & Statistics::CovarianceMatrix)
                  && static_cast<int>(statistics_.covarianceMatrix_.size())
                        != GetBandCount())
            || ((requiredStats_ & Statistics::Histogram)
                  && static_cast<int>(statistics_.histogram_.size()) != GetBandCount())
            || ((requiredStats_ & Statistics::MinValue)
                  && static_cast<int>(statistics_.min_.size()) != GetBandCount())
            || ((requiredStats_ & Statistics::MaxValue)
                  && static_cast<int>(statistics_.max_.size()) != GetBandCount())) {
#else
         // Alternativamente, obtengo la dimension de las estadisticas requeridas
         // y comparo contra la cantidad de bandas.
         if ( statistics_.GetDimension(requeredStats_) != GetBandCount() ) {
#endif   // __UNUSED_CODE__
         ResetStats();
      }
   }

   std::vector<void*> data;
   std::vector<int> idx;
   for (int i = 0; i < GetBandCount(); i++)
      idx.push_back(i);
   GetInternalData(idx, data);
   int x, y;
   GetSize(x, y);
   // reservo espacio para los punteros a los datos
   void **pdata = reinterpret_cast<void**>(new unsigned char*[GetBandCount()]);
   // apunto a los datos
   for (int b = 0; b < GetBandCount(); b++)
      pdata[b] = data[b];

   double *pmeans = NULL;
   double *pvariances = NULL;
   double **pcovariancematrix = NULL;
   double **pcorrelationmatrix = NULL;
   double *pmins = NULL;
   double *pmaxs = NULL;
   Histogram *phistograms = NULL;

   // reservo espacio para los parametros requeridos
   if (!statistics_.mean_.empty()) {
      pmeans = new double[GetBandCount()];
   }
   if (!statistics_.variance_.empty()) {
      pvariances = new double[GetBandCount()];
   }
   if (!statistics_.covarianceMatrix_.empty()) {
      pcovariancematrix = new double*[GetBandCount()];
   }
   if (!statistics_.correlationMatrix_.empty()) {
      pcorrelationmatrix = new double*[GetBandCount()];
   }
   if (!statistics_.min_.empty()) {
      pmins = new double[GetBandCount()];
   }
   if (!statistics_.max_.empty()) {
      pmaxs = new double[GetBandCount()];
   }
   if (!reset_ && statistics_.histogram_.empty()) {
      phistograms = new Histogram[GetBandCount()];
   }
   if (!statistics_.histogram_.empty()) {
      phistograms = new Histogram[GetBandCount()];
   }
   // copio los datos
   for (int b = 0; b < GetBandCount(); b++) {
      if (!statistics_.mean_.empty()) {
         pmeans[b] = statistics_.mean_[b];
      }
      if (!statistics_.variance_.empty()) {
         pvariances[b] = statistics_.variance_[b];
      }
      if (!statistics_.covarianceMatrix_.empty()) {
         pcovariancematrix[b] = new double[GetBandCount()];
         for (int j = 0; j < GetBandCount(); j++)
            pcovariancematrix[b][j] = statistics_.covarianceMatrix_[b][j];
      }
      if (!statistics_.correlationMatrix_.empty()) {
         pcorrelationmatrix[b] = new double[GetBandCount()];
         for (int j = 0; j < GetBandCount(); j++)
            pcorrelationmatrix[b][j] = statistics_.correlationMatrix_[b][j];
      }
      if (!statistics_.min_.empty()) {
         pmins[b] = statistics_.min_[b];
      }
      if (!statistics_.max_.empty()) {
         pmaxs[b] = statistics_.max_[b];
      }
      if (!reset_ && statistics_.histogram_.empty()) {
         statistics_.histogram_.resize(GetBandCount(), Histogram());
      }
      if (!statistics_.histogram_.empty()) {
         phistograms[b] = statistics_.histogram_[b];
         phistograms[b].SetParameters(pmins[b], pmaxs[b], 256);
      }
   }
   // calculo los parametros en forma incremental
   statisticparametersTypeMap[GetDataType()](pdata, x * y, GetBandCount(),
                                             statistics_.count_, GetNoDataValue(), pmeans, pvariances,
                                             pcovariancematrix, pcorrelationmatrix,
                                             pmins, pmaxs, phistograms);
   // reasigno los datos calculados
   if (!statistics_.mean_.empty()) {
      statistics_.mean_ = std::vector<double>(pmeans, pmeans + GetBandCount());
   }
   if (!statistics_.variance_.empty()) {
      statistics_.variance_ = std::vector<double>(pvariances,
                                                  pvariances + GetBandCount());
   }
   if (!statistics_.covarianceMatrix_.empty()) {
      statistics_.covarianceMatrix_ = std::vector<std::vector<double> >(GetBandCount());
   }
   if (!statistics_.correlationMatrix_.empty()) {
      statistics_.correlationMatrix_ = std::vector<std::vector<double> >(
            GetBandCount());
   }
   if (!statistics_.min_.empty()) {
      statistics_.min_ = std::vector<double>(pmins, pmins + GetBandCount());
   }
   if (!statistics_.max_.empty()) {
      statistics_.max_ = std::vector<double>(pmaxs, pmaxs + GetBandCount());
   }
   if (!statistics_.histogram_.empty()) {
      statistics_.histogram_ = std::vector<Histogram>(phistograms,
                                                      phistograms + GetBandCount());
   }

   // libero memoria
   if (!statistics_.covarianceMatrix_.empty()) {
      for (int b = 0; b < GetBandCount(); b++) {
         statistics_.covarianceMatrix_[b] = std::vector<double>(
               pcovariancematrix[b], pcovariancematrix[b] + GetBandCount());
         delete[] pcovariancematrix[b];
      }
   }
   if (!statistics_.correlationMatrix_.empty()) {
      for (int b = 0; b < GetBandCount(); b++) {
         statistics_.correlationMatrix_[b] = std::vector<double>(
               pcorrelationmatrix[b], pcorrelationmatrix[b] + GetBandCount());
         delete[] pcorrelationmatrix[b];
      }
   }

   delete[] pcovariancematrix;
   delete[] pcorrelationmatrix;
   delete[] pvariances;
   delete[] pmeans;
   delete[] pmins;
   delete[] pmaxs;
   delete[] phistograms;
   delete[] reinterpret_cast<unsigned char**>(pdata);
}

// --------------------------- ESTADISTICAS ---------------------------

/* Devuelve la propiedad statistics_
 * @return devuelve las estadisticas.
 */
Statistics StatsCanvas::GetStatistics() {
   return statistics_;
}

#ifdef __UNUSED_CODE__

/** Todos estos metodos son reemplazados por el metodo GetStatistics */
/** Retorna la cuenta de muestras */
size_t StatsCanvas::GetCount() const
{
   return count_;
}

/** Retorna los valores de las medias computadas para cada banda */
std::vector<double> StatsCanvas::GetMean() const
{
   return mean_;
}

/** Retorna los valores de las varianzas computadas para cada banda */
std::vector<double> StatsCanvas::GetVariance() const
{
   return variance_;
}

/** Retorna los valores de la matriz de covarianza */
std::vector<std::vector<double> > StatsCanvas::GetCovarianceMatrix() const
{
   return covarianceMatrix_;
}

/** Retorna los valores minimos para cada banda */
std::vector<double> StatsCanvas::GetMin() const
{
   return min_;
}

/** Retorna los valores maximos para cada banda */
std::vector<double> StatsCanvas::GetMax() const
{
   return max_;
}

#endif

/*
 * Vuelve a estado inicial los parametros estadisticos.
 * Asigna la dimension de la variable (== a la cantidad de bandas de la imagen),
 * solo para aquellas variables que van a ser calculadas.
 */
void StatsCanvas::ResetStats() {
   statistics_.count_ = 0;

   statistics_.mean_.clear();
   if (requiredStats_ & Statistics::Mean) {
      statistics_.mean_.resize(GetBandCount(), 0.0);
   }

   statistics_.variance_.clear();
   if (requiredStats_ & Statistics::Variance) {
      statistics_.variance_.resize(GetBandCount(), 0.0);
   }

   statistics_.covarianceMatrix_.clear();
   if (requiredStats_ & Statistics::CovarianceMatrix) {
      statistics_.covarianceMatrix_.resize(GetBandCount(),
                                           std::vector<double>(GetBandCount(), 0.0));
   }

   statistics_.correlationMatrix_.clear();
   if (requiredStats_ & Statistics::CorrelationMatrix) {
      statistics_.correlationMatrix_.resize(GetBandCount(),
                                            std::vector<double>(GetBandCount(), 0.0));
   }

   statistics_.min_.clear();
   if (requiredStats_ & Statistics::MinValue) {
      statistics_.min_.resize(GetBandCount(), std::numeric_limits<double>::max());
   }

   statistics_.max_.clear();
   if (requiredStats_ & Statistics::MaxValue) {
      statistics_.max_.resize(GetBandCount(), -std::numeric_limits<double>::max());
   }

   statistics_.histogram_.clear();
   // Para visualizaciones esta bien, luego, debe modificarse para que Histogram
   // tome en forma dinamica el maximo y minimo
   if (requiredStats_ & Statistics::Histogram) {
      statistics_.histogram_.resize(GetBandCount(), Histogram());
   }
}

/**
 * Recibe que estadisticas se requieren, y verifica que se cumplan las dependencias
 * de las variables solicitadas.
 * @param[in] RequiredStats que variables estadisticas se requieren.
 */
void StatsCanvas::SetRequiredStats(Statistics::StatisticsFlag RequiredStats) {
   requiredStats_ = Statistics::None;
   if (RequiredStats & Statistics::MinValue) {
      requiredStats_ = requiredStats_ | RequiredStats | MINVALUE_DEPENDENCIES;
   }

   if (RequiredStats & Statistics::MaxValue) {
      requiredStats_ = requiredStats_ | RequiredStats | MAXVALUE_DEPENDENCIES;
   }

   if (RequiredStats & Statistics::Mean) {
      requiredStats_ = requiredStats_ | RequiredStats | MEAN_DEPENDENCIES;
   }

   if (RequiredStats & Statistics::Variance) {
      requiredStats_ = requiredStats_ | RequiredStats | VARIANCE_DEPENDENCIES;
   }

   if (RequiredStats & Statistics::CovarianceMatrix) {
      requiredStats_ = requiredStats_ | RequiredStats | COVARIANCE_MATRIX_DEPENDENCIES;
   }

   if (RequiredStats & Statistics::CorrelationMatrix) {
      requiredStats_ = requiredStats_ | RequiredStats | CORRELATION_MATRIX_DEPENDENCIES;
   }

   if (RequiredStats & Statistics::Histogram) {
      requiredStats_ = requiredStats_ | RequiredStats | HISTOGRAM_DEPENDENCIES;
   }
   // Resetea las estadisticas, para que solo tengan dimension mayor a cero las
   // estadisticas requeridas
   if (reset_)
      ResetStats();
}

/** Establece si debe resetearse el estado interno de las estadisticas o no */
void StatsCanvas::SetReset(bool Reset) {
   reset_ = Reset;
}

}
