/** \file SRDStatistics.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandard
#include <limits>
#include <cmath>
#include <cstddef>
#include <vector>

// Includes Suri
#include "NoDataValue.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"

#include "Eigen/Eigenvalues"

#ifndef SRDSTATISTICS_H_
#define SRDSTATISTICS_H_

#define EIG_NONE           0
#define EIG_CORRELATION    1
#define EIG_COVARIANCE     2

// Esto es un arreglo para alineacion de punteros en GCC para Windows.
// En versiones gcc (GCC) 4.4.0 no deberia hacer falta.
#ifdef __WINDOWS__
#define EIGEN_SAFE __attribute__((force_align_arg_pointer))
#else
#define EIGEN_SAFE
#endif  // __WINDOWS__

namespace suri {
namespace raster {
namespace data {

/**
 * Clase base para estadisticas que ofrece la funcionalidad
 * base no especializada.
 */
class StatisticsBase : public suri::NoDataValue {
public:
   /**
    * Dtor.
    */
   virtual ~StatisticsBase() {
      delete[] pMin_;
      pMin_ = NULL;

      delete[] pMax_;
      pMax_ = NULL;

      delete[] pMean_;
      pMean_ = NULL;

      delete[] pAccumVariance_;
      pAccumVariance_ = NULL;

      delete[] pPointCount_;
      pPointCount_ = NULL;
   }

   /**
    * Procesa el bloque de datos dado.
    */
   virtual void Process(int Band, int DataLength, const void* pData) = 0;

   /**
    * Proceso un bloque de datos para cada banda.
    */
   virtual void Process(int DataLength, const std::vector<void*>& Data) = 0;

   /**
    * Obtiene el valor minimo encontrado hasta el momento
    * de la ultima ejecucion del metodo Process.
    */
   double GetMin(int Band) { return pMin_[Band]; }

   /**
    * Obtiene el valor maximo encontrado hasta el momento
    * de la ultima ejecucion del metodo Process.
    */
   double GetMax(int Band) { return pMax_[Band]; }

   /**
    * Obtiene el valor promedio encontrado hasta el momento
    * de la ultima ejecucion del metodo Process.
    */
   double GetMean(int Band) { return pMean_[Band]; }

   /**
    * Obtiene la desviacion estandar encontrada hasta el momento
    * a partir de la varianza calculada de la ultima
    * ejecucion del metodo Process.
    */
   double GetStdev(int Band) { return sqrt(pAccumVariance_[Band] / (pPointCount_[Band] - 1)); }

   /**
    * Devuelve el valor de varianza para la banda solicitada.
    */
   double GetVariance(int Band) { return pAccumVariance_[Band]/ (pPointCount_[Band] - 1); }

   /**
    * Devuelve la matriz de correlacion.
    */
   std::vector<std::vector<double> > GetCorrelationMatrix() {
      // Se utilizan los datos de la matriz de covarianza
      // por lo tanto se calcula.
      return CalculateCorrelationMatrix(GetCovarianceMatrix());
   }

   /**
    * Devuelve la matriz de covarianza.
    */
   std::vector<std::vector<double> > GetCovarianceMatrix() {
      return CalculateCovarianceMatrix();
   }

   /**
    * Devuelve la cantidad de bandas.
    */
   int GetBandCount() { return bandCount_; }

   /**
    * Obtiene el tamano del tipo de dato.
    */
   int GetTypeSize() { return typeSize_; }

   /**
    * Obtiene el nombre del tipo de dato.
    */
   std::string GetDataName() { return dataName_; }

   /**
    * Devuelve la cantidad de puntos procesados segun la banda solicitada.
    */
   long GetPointCount(int Band) { return pPointCount_[Band]; }

   /**
    * Obtiene un vector de autovalores segun la matriz especificada.
    */
   EIGEN_SAFE std::vector<double> GetEigenValues(int Mtx) {
      // Tipo de matriz a utilizar.
      std::vector<std::vector<double> > selmtx =
            Mtx == EIG_CORRELATION ? GetCorrelationMatrix() : GetCovarianceMatrix();

      // Adapto al formato de la clase Matrix.
      int selmtxsize = selmtx.size();

      Eigen::MatrixXd comtx(selmtxsize, selmtxsize);
      for (int ix = 0; ix < selmtxsize; ++ix)
         for (int jx = 0; jx < selmtxsize; ++jx)
            comtx(ix, jx) = selmtx[ix][jx];

      Eigen::EigenSolver<Eigen::MatrixXd> solver(comtx);
      Eigen::EigenSolver<Eigen::MatrixXd>::EigenvalueType eigenvals =
            solver.eigenvalues();

      std::vector<double> veigenvals;

      for (int rx = 0; rx < selmtxsize; ++rx)
         veigenvals.push_back(eigenvals(rx).real());

      return veigenvals;
   }

   /**
    * Obtiene un vector de autovectores segun la matriz especificada.
    */
   EIGEN_SAFE std::vector<std::vector<double> > GetEigenVectors(int Mtx) {
      // Tipo de matriz a utilizar.
      std::vector<std::vector<double> > selmtx =
                  Mtx == EIG_CORRELATION ? GetCorrelationMatrix() : GetCovarianceMatrix();

      // Adapto al formato de la clase Matrix.
      int selmtxsize = selmtx.size();

      Eigen::MatrixXd comtx(selmtxsize, selmtxsize);
      for (int ix = 0; ix < selmtxsize; ++ix)
         for (int jx = 0; jx < selmtxsize; ++jx)
            comtx(ix, jx) = selmtx[ix][jx];

      Eigen::EigenSolver<Eigen::MatrixXd> solver(comtx);
      Eigen::EigenSolver<Eigen::MatrixXd>::EigenvectorsType eigenvecs =
            solver.eigenvectors();

      std::vector<std::vector<double> > veigenvecs;

      for (int rx = 0; rx < selmtxsize; ++rx) {
         std::vector<double> veigenvec;
         for (int sx = 0; sx < selmtxsize; ++sx)
            veigenvec.push_back(eigenvecs(sx, rx).real());
         veigenvecs.push_back(veigenvec);
      }

      return veigenvecs;
   }

   /**
    * Establece en base a que matriz se van a calcular los autovalores
    * y los autovectores.
    */
   void SetEig(int Eig) { eig_ = Eig; }

   /**
    * Devuelve en base a que matriz se van a calcular los autovalores
    * y los autovectores.
    */
   int GetEig() { return eig_; }


   /**
    * Obtiene el acumulador para calculo de covarianza.
    */
   std::vector<std::vector<double> > GetAccum4Covar() {
      return ppAccum4Covar_;
   }

   /**
    * Establece el acumulador para calculo de covarianza.
    */
   void SetAccum4Covar(const std::vector<std::vector<double> >& Accum4Covar) {
      ppAccum4Covar_ = Accum4Covar;
   }

   int bandCount_;  // Cantidad de bandas.
   double* pMin_;  // Valor minimo.
   double* pMax_;  // Valor maximo.
   double* pMean_;  // Valor promedio.
   double* pAccumVariance_;  // Varianza acumulada.
   long* pPointCount_;  // Cantidad de puntos procesados.
   int typeSize_;  // tamano del tipo de dato.
   std::string dataName_;

   std::vector<std::vector<double> > ppAccum4Covar_;

   int eig_;

protected:
   /**
    * Calcula la matriz de covarianza.
    */
   virtual std::vector<std::vector<double> > CalculateCovarianceMatrix() = 0;

   /**
    * Metodo auxiliar para el calculo de la matriz de la correlacion.
    */
   virtual std::vector<std::vector<double> > CalculateCorrelationMatrix(
         const std::vector<std::vector<double> >& CovarMatrix) = 0;
};

/**
 * Clase especializada para estadisticas. Complementa con funcionalidad
 * especializada.
 */
template<typename T>
class Statistics : public StatisticsBase {
public:
   /**
    * Ctor.
    */
   Statistics(int BandCount);

   /**
    * Procesa el bloque de datos dado.
    */
   virtual void Process(int Band, int DataLength, const void* pData);

   /**
    * Proceso un bloque de datos para cada banda.
    */
   virtual void Process(int DataLength, const std::vector<void*>& Data);

protected:
   /**
    * Calcula la matriz de covarianza.
    */
   virtual std::vector<std::vector<double> > CalculateCovarianceMatrix();

   /**
    * Metodo auxiliar para el calculo de la matriz de la correlacion.
    */
   virtual std::vector<std::vector<double> > CalculateCorrelationMatrix(
         const std::vector<std::vector<double> >& CovarMatrix);
};

/**
 * Ctor.
 */
template<typename T>
Statistics<T>::Statistics(int BandCount)  {
   bandCount_ = BandCount;
   pMin_ = new double[BandCount];
   pMax_ = new double[BandCount];
   pMean_ = new double[BandCount];
   pAccumVariance_ = new double[BandCount];
   pPointCount_ = new long[BandCount];
   typeSize_ = sizeof(T);
   dataName_ = DataInfo<T>::Name;
   ppAccum4Covar_ = std::vector<std::vector<double> >(
         bandCount_, std::vector<double>(bandCount_, 0.0));
   for (int bandix = 0; bandix < BandCount; ++bandix) {
      pMin_[bandix] = std::numeric_limits<double>::max();
      pMax_[bandix] = -std::numeric_limits<double>::max();
      pMean_[bandix] = 0.0;
      pAccumVariance_[bandix] = 0.0;
      pPointCount_[bandix] = 0;
   }

   eig_ = EIG_NONE;
}

/**
 * Procesa el bloque de datos dado.
 */
template<typename T>
void Statistics<T>::Process(int Band, int DataLength, const void* pData) {
   const T* pdata = static_cast<const T*>(pData);

   for (int ix = 0; ix < DataLength; ++ix) {
      if (pdata[ix] != INFINITY && pdata[ix] != -INFINITY && !std::isnan(pdata[ix])
            && !((IsNoDataValueAvailable() && AreEqual(pdata[ix], GetNoDataValue()))
                  || (IsNdvAvailableForBand(Band) && AreEqual(pdata[ix], GetBandNdv(Band))))) {
         pPointCount_[Band] += 1;

         if (pdata[ix] < pMin_[Band])
            pMin_[Band] = pdata[ix];

         if (pdata[ix] > pMax_[Band])
            pMax_[Band] = pdata[ix];

         double prevmean = pMean_[Band];

         pMean_[Band] += (pdata[ix] - pMean_[Band]) / pPointCount_[Band];

         pAccumVariance_[Band] += (pdata[ix] - pMean_[Band]) * (pdata[ix] - prevmean);
      }
   }
}

/**
 * Proceso un bloque de datos para cada banda.
 */
template<typename T>
void Statistics<T>::Process(int DataLength, const std::vector<void*>& Data) {
   for (int ix = 0; ix < DataLength; ++ix) {
      bool masked = true;
      for (size_t b = 0; masked && b < Data.size(); ++b) {
         T* pdata = static_cast<T*>(Data[b]);
         if (pdata[ix] != INFINITY && pdata[ix] != -INFINITY && !std::isnan(pdata[ix])
               && !(IsNoDataValueAvailable() && AreEqual(pdata[ix], GetNoDataValue()))) {
            masked = false;
         }
      }

      // Si el pixel esta enmascarado lo salteo.
      if (masked)
         continue;

      // Calculo las estadisticas para todas las bandas.
      for (size_t Band = 0; Band < Data.size(); Band++) {
         T* pdata = static_cast<T*>(Data[Band]);
         pPointCount_[Band] += 1;

         if (pdata[ix] < pMin_[Band])
            pMin_[Band] = pdata[ix];

         if (pdata[ix] > pMax_[Band])
            pMax_[Band] = pdata[ix];

         double prevmean = pMean_[Band];

         pMean_[Band] += (pdata[ix] - pMean_[Band]) / pPointCount_[Band];

         pAccumVariance_[Band] += (pdata[ix] - pMean_[Band]) * (pdata[ix] - prevmean);

         for (int accix = 0, lenaccix = Data.size(); accix < lenaccix; ++accix) {
            T* paccdata = static_cast<T*>(Data[accix]);
            ppAccum4Covar_[Band][accix] += pdata[ix] * paccdata[ix];
         }
      }
   }
}

/**
 * Calcula la matriz de covarianza.
 */
template<typename T>
std::vector<std::vector<double> > Statistics<T>::CalculateCovarianceMatrix() {
   std::vector<std::vector<double> > retmtx = std::vector<std::vector<double> >(
         GetBandCount(), std::vector<double>(GetBandCount(), 0.0));

   // Deberia ser la misma cantidad para todas las bandas.
   int pcount = GetPointCount(0);

   // Calculo los valores de la matriz.
   for (int rowix = 0, lenrowix = GetBandCount(); rowix < lenrowix; ++rowix) {
      for (int colix = 0, lencolix = GetBandCount(); colix < lencolix; ++colix) {
         retmtx[rowix][colix] = (ppAccum4Covar_[rowix][colix]
               - (pcount * GetMean(rowix) * GetMean(colix)))
               / static_cast<double>(pcount);
      }
   }

   return retmtx;
}

/**
 * Metodo auxiliar para el calculo de la matriz de la correlacion.
 */
template<typename T>
std::vector<std::vector<double> > Statistics<T>::CalculateCorrelationMatrix(
      const std::vector<std::vector<double> >& CovarMatrix) {

   std::vector<std::vector<double> > retmtx = std::vector<std::vector<double> >(
         GetBandCount(), std::vector<double>(GetBandCount(), 0.0));

   // Calculo los valores de la matriz.
   for (int rowix = 0, lenrowix = GetBandCount(); rowix < lenrowix; ++rowix) {
      for (int colix = 0, lencolix = GetBandCount(); colix < lencolix; ++colix) {
         retmtx[rowix][colix] = CovarMatrix[rowix][colix]
               / std::sqrt(CovarMatrix[rowix][rowix] * CovarMatrix[colix][colix]);
      }
   }

   return retmtx;
}

}  // namespace data
}  // namespace raster
}  // namespace suri

#endif  // SRDSTATISTICS_H_
