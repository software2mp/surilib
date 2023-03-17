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

// Includes standard
#include <math.h>
#include <string>
#include <vector>
#include <limits>

// Includes suri
#include "MahalanobisAlgorithm.h"
#include "suri/DataTypes.h"
#include "suri/xmlnames.h"
#include "MathFunctions.h"
#include "logmacros.h"

// Defines
#define PI 3.14159265

namespace suri {
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, MahalanobisAlgorithm, 0)

/** Funcion clasificacion */
typedef void (*ClassMappingFunctionType)(int*, std::vector<void*>&, size_t,
                                         int NoClassPixelValue, int NDVPixelValue,
                                         double Ndv, bool NdvAvailable,
                                         double Threshold, Clusters* pClusters);


/**
 * Clasifica las bandas de entrada usando metodo de distancia minima
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
void mahalanobis(int* pDest, std::vector<void*> &pSource, size_t Size,
                 int NoClassPixelValue, int NDVPixelValue, double Ndv,
                 bool NdvAvailable, double Threshold, Clusters* pClusters) {
   int sourcesize = pSource.size();
   std::vector<T*> psrc;
   for (int i = 0; i < sourcesize; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   const std::vector<Clusters::ClusterData>& clusters = pClusters->GetClusterVector();
   int classcount = clusters.size();
//   int matrixsize = clusters[0].statistics_.mean_.size();
   int matrixsize = clusters[0].pStatistics_->GetBandCount();
   // Calculo cantidad de pixels en todas las clases.
   int classpixelssum = 0;
   for (int classpos = 0; classpos < classcount; classpos++)
      classpixelssum += clusters[classpos].pStatistics_->pPointCount_[0];

   // Inicializo matriz de covarianza
   std::vector<std::vector<double> > covariancematrix;
   for (int i = 0; i < matrixsize; i++) {
      covariancematrix.push_back(std::vector<double>(matrixsize));
      for (int j = 0; j < matrixsize; j++)
         covariancematrix[i][j] = 0;
   }

   // Calculo la matriz de covarianza comun a todas las clases
   for (int classpos = 0; classpos < classcount; classpos++) {
      // Se calcula la pooled variance de las matrices. Ver TCK #873
      double factor = (static_cast<double>(clusters[classpos].pStatistics_->pPointCount_[0]) - 1)
            / (classpixelssum - classcount);
      std::vector < std::vector<double> > classcovmatrix =
            clusters[classpos].pStatistics_->GetCovarianceMatrix();
      for (int i = 0; i < matrixsize; i++)
         for (int j = 0; j < matrixsize; j++)
            covariancematrix[i][j] = covariancematrix[i][j]
                  + factor * classcovmatrix[i][j];
   }

   // Calculo la matriz lu
   std::vector<std::vector<double> > lumatrix;
   CalculateLu(covariancematrix, lumatrix);

#ifdef __OLD_DEBUG__
   // Msgs con informacion de matriz de covarianza.
   for (int i = 0; i < matrixsize; i++)
      for (int j = 0; j < matrixsize; j++)
         REPORT_DEBUG("cov_mat_first_trainning_area[%d, %d]: %f", i, j,
                      clusters[0].statistics_.covarianceMatrix_[i][j]);

   REPORT_DEBUG("PIXEL COUNT first trainning area: %d", clusters[0].statistics_.count_);

   for (int i = 0; i < matrixsize; i++)
      for (int j = 0; j < matrixsize; j++)
         REPORT_DEBUG("mahalanobis_cov_mat[%d, %d]: %f", i, j, covariancematrix[i][j]);

   for (int i = 0; i < matrixsize; i++)
      for (int j = 0; j < matrixsize; j++)
         REPORT_DEBUG("descomposicion lu de mahalanobis_cov_mat[%d, %d]: %f", i, j,
                      lumatrix[i][j]);
#endif

   double threshold = Threshold;
   threshold = threshold * threshold;

   std::vector<double> pixelvalues(matrixsize);
   std::vector<double> partialinverse(matrixsize);
   std::vector<double> inverse(matrixsize);

   // Para cada pixel en la imagen
   for (size_t pixelposition = 0; pixelposition < Size; pixelposition++) {
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
      } else {
         invalidpixel = false;
      }

      // Si el pixel es valido hago el proc normal.
      if (invalidpixel == false) {
         double pixelclassdistance = std::numeric_limits<double>::max();

         // Para cada clase
         for (int classpos = 0; classpos < classcount; classpos++) {
            // Le resto a cada pixel la media
            for (int i = 0; i < matrixsize; i++)
               pixelvalues[i] = static_cast<double>(psrc[i][pixelposition])
                     - clusters[classpos].pStatistics_->pMean_[i];

            // Calculo v en y=Lv
            for (int i = 0; i < matrixsize; i++) {
               partialinverse[i] = pixelvalues[i];
               for (int j = 0; j < i; j++)
                  partialinverse[i] -= partialinverse[j] * lumatrix[i][j];
            }
            // Calculo x en v=Ux
            for (int i = matrixsize - 1; i >= 0; i--) {
               inverse[i] = partialinverse[i];
               for (int j = matrixsize - 1; j > i; j--)
                  inverse[i] -= inverse[j] * lumatrix[i][j];
               inverse[i] = inverse[i] / lumatrix[i][i];
            }

            // Calculo la probabilidad que el pixel sea de la clase classpos
            double classdistance = 0;
            for (int i = 0; i < matrixsize; i++)
               classdistance += inverse[i] * pixelvalues[i];

            // Comparo la ultima clase asignada y el threashold
            if (classdistance < threshold && classdistance < pixelclassdistance) {
               pixelclass = clusters[classpos].classId_;
               pixelclassdistance = classdistance;
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
INITIALIZE_DATATYPE_MAP(ClassMappingFunctionType, mahalanobis);


/** Ctor */
MahalanobisAlgorithm::MahalanobisAlgorithm() :
             ClusterClassificationAlgorithm(wxT(ALGORITHM_VALUE_MAHALANOBIS)) {
}

/** Dtor */
MahalanobisAlgorithm::~MahalanobisAlgorithm() {
}

/**
 * Llama algoritmo de clasificacion de tipo de dato correcto
 * @param[out] pDest datos clasificados
 * @param[in] pSource fuente de datos a clasificar
 * @param[in] Size cantidad de datos a clasificar
 * @param[in] Tipo de dato en imagen a clasificar
 */
bool MahalanobisAlgorithm::Classify(int* pDest, std::vector<void*> pSource,
                                    size_t Size, const std::string &DataType) {
   ClassMappingFunctionType function = mahalanobisTypeMap[DataType];
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
bool MahalanobisAlgorithm::Check() const {
   return GetThreshold() >= 0 && GetClusters()->Check();
}

} /* namespace suri */
