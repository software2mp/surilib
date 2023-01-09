/*! \file MaxLikelihoodAlgorithm.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cmath>
#include <limits>
#include <string>
#include <vector>

#include "MaxLikelihoodAlgorithm.h"
#include "MathFunctions.h"
#include "suri/DataTypes.h"
#include "suri/StatisticsFunctions.h"
#include "suri/xmlnames.h"

#define NOCLASS 0
#define PI 3.14159265

/** namespace suri */
namespace suri {
/** Macro para registrar el tipo de algoritmo de clasificacion de forma automatica */
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, MaxLikelihoodAlgorithm, 0)

/** Funcion clasificacion */
typedef void (*ClassMappingFunctionType)(int*, std::vector<void*>&, size_t,
                                         int NoClassPixelValue, int NDVPixelValue,
                                         double Ndv, bool NdvAvailable,
                                         double Threshold, Clusters* pClusters);

/** Template MaxLikelihood */
/**
 * Clasifica las bandas de entrada usando metodo de maxima verosimilitud
 * @param[out] pDest banda donde se guardan las clases asignadas(ints)
 * @param[in] pSource bandas de entrada que se usan para clasificar
 * @param[in] Size cantidad de valores (pixeles) de la banda
 * @param[in] NoClassPixelValue valor asignado a los pixels no clasificados
 * @param[in] Threshold umbral a partir del cual un pixel no se asigna a una
 * clase. Valores entre 0 y 1.
 * @param[in] pClusters Estadisticas de las clases que se usan para clasificar
 * los pixels.
 */
template<typename T>
void maxlikelihood(int* pDest, std::vector<void*> &pSource, size_t Size,
                   int NoClassPixelValue, int NDVPixelValue, double Ndv,
                   bool NdvAvailable, double Threshold, Clusters* pClusters) {
   int sourcesize = pSource.size();
   std::vector<T*> psrc;
   for (int i = 0; i < sourcesize; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   const std::vector<Clusters::ClusterData>& clusters = pClusters->GetClusterVector();
   int classcount = clusters.size();

   double threshold = log(Threshold / 100);

   // Calculo matrix LU usando las estadisticas.
   std::vector < std::vector<std::vector<double> > > lumatrices(classcount);
   for (int i = 0; i < classcount; i++) {
      CalculateLu(clusters[i].pStatistics_->GetCovarianceMatrix(),
                  lumatrices[i]);
   }
   // Genero vectores que se usan en algoritmo
   int matrixsize = clusters[0].pStatistics_->GetBandCount();
   std::vector<double> pixelvalues(matrixsize);
   std::vector<double> partialinverse(matrixsize);
   std::vector<double> inverse(matrixsize);

   // Calculo las constantes de las clases
   std::vector<double> constants(classcount);
   for (int classpos = 0; classpos < classcount; classpos++) {
      // Calculo el determinante de la matriz lu
      double determinant = lumatrices[classpos][0][0];
      for (int j = 1; j < matrixsize; j++)
         determinant *= lumatrices[classpos][j][j];

      // Calculo la constante de la clase
      constants[classpos] = -log(classcount) - log(determinant) / 2 -
                                          classcount / 2 * log(2 * PI);
   }

   // Para cada pixel en la imagen
   for (int pixelposition = 0; pixelposition < (int) Size; pixelposition++) {
      // La clase default es sin clase
      int pixelclass = NoClassPixelValue;

      bool invalidpixel = true;

      // Salida para pixel no valido.
      // (valor no valido en todas las bandas involucradas)
      // Ver TCK #7325
      if (NdvAvailable) {
         for (int i = 0; i < matrixsize && invalidpixel; ++i) {
            if (static_cast<double>(psrc[i][pixelposition]) != CLASSIFICATION_NDV)
               invalidpixel = false;
         }
      }  else {
         invalidpixel = false;
      }

      // Si el pixel es valido hago el proc normal.
      if (invalidpixel == false) {
         double pixelclassprobability = -std::numeric_limits<double>::max();

         // Para cada clase
         for (int classpos = 0; classpos < classcount; classpos++) {
            // Le resto a cada pixel la media
            for (int i = 0; i < matrixsize; i++) {
               pixelvalues[i] = static_cast<double>(psrc[i][pixelposition])
                                      - clusters[classpos].pStatistics_->pMean_[i];
            }
            // Calculo v en y=Lv
            for (int i = 0; i < matrixsize; i++) {
               partialinverse[i] = pixelvalues[i];
               for (int j = 0; j < i; j++)
                  partialinverse[i] -= partialinverse[j]
                                      * lumatrices[classpos][i][j];
            }
            // Calculo x en v=Ux
            for (int i = matrixsize - 1; i >= 0; i--) {
               inverse[i] = partialinverse[i];
               for (int j = matrixsize - 1; j > i; j--)
                  inverse[i] -= inverse[j] * lumatrices[classpos][i][j];
               inverse[i] = inverse[i] / lumatrices[classpos][i][i];
            }

            // Calculo la probabilidad que el pixel sea de la clase classpos
            double tempclassprobability = 0;
            for (int i = 0; i < matrixsize; i++)
               tempclassprobability += inverse[i] * pixelvalues[i];
            tempclassprobability = -tempclassprobability / 2;
            double finalclassprobability = constants[classpos]
                                           + tempclassprobability;

            // Comparo la ultima clase asignada y el threashold
            if (tempclassprobability > threshold
                  && finalclassprobability > pixelclassprobability) {
               pixelclass = clusters[classpos].classId_;
               pixelclassprobability = finalclassprobability;
            }
         }
      } else {
         // Sino le pongo un valor para la parte de la imagen que no
         // es valida.
         pixelclass = NDVPixelValue;
      }
      pDest[pixelposition] = pixelclass;
   }
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ClassMappingFunctionType, maxlikelihood);

/** Ctor */
MaxLikelihoodAlgorithm::MaxLikelihoodAlgorithm() :
        ClusterClassificationAlgorithm(wxT(ALGORITHM_VALUE_MAX_LIKELIHOOD)) {
}

/** Dtor */
MaxLikelihoodAlgorithm::~MaxLikelihoodAlgorithm() {
}


/**
 * Determina el tipo de dato a la entrada del algoritmo
 * @param[out] pDest datos clasificados
 * @param[in] pSource fuente de datos a clasificar
 * @param[in] Size cantidad de datos a clasificar
 */
bool MaxLikelihoodAlgorithm::Classify(int* pDest, std::vector<void*> pSource,
                                    size_t Size, const std::string &DataType) {
   ClassMappingFunctionType function = maxlikelihoodTypeMap[DataType];
   function(pDest, pSource, Size, GetNoClassPixelValue(), GetNDVPixelValue(),
            GetNoDataValue(), IsNoDataValueAvailable(), GetThreshold(), GetClusters());
   return true;
}

/**
 * verifica que las dimensiones de los parametros sean consistentes
 *  (misma cantidad de bandas)
 * @return True si las estadisticas de las clases tienen la misma dimension
 * @return True si la cantidad de clases es 0.
 * @return False alguna clase tiene estadisticas con dimension distinta o invalida.
 */
bool MaxLikelihoodAlgorithm::Check() const {
   return GetThreshold() >= 0 && GetThreshold() <= 100 && GetClusters()->Check();
}

}  // namespace

