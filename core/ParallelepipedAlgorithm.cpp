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
#include <limits>
#include <string>
#include <vector>

// Includes Suri
#include "ParallelepipedAlgorithm.h"
#include "suri/StatisticsFunctions.h"
#include "suri/DataTypes.h"
#include "MathFunctions.h"
#include "suri/xmlnames.h"
// Includea wx
// Defines
#define NOCLASS 0
#define PI 3.14159265

// forward

/** namespace suri */
namespace suri {
// Declarations
/** Macro para registrar el tipo de algoritmo de clasificacion de forma automatica */
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, ParallelepipedAlgorithm, 0)

/** Funcion clasificacion */
typedef void (*ClassMappingFunctionType)(int*, std::vector<void*>&, size_t,
                                         int NoClassPixelValue, int NDVPixelValue,
                                         double Ndv, bool NdvAvailable,
                                         double Threshold, Clusters* pClusters);

/**
 * Clasifica las bandas de entrada usando metodo del paralelepipedo
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
void parallelepiped(int* pDest, std::vector<void*> &pSource, size_t Size,
                    int NoClassPixelValue, int NDVPixelValue, double Ndv,
                    bool NdvAvailable, double Threshold, Clusters* pClusters) {
   int bandcount = pSource.size();
   std::vector<T*> psrc;
   for (int i = 0; i < bandcount; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   const std::vector<Clusters::ClusterData>& clusters = pClusters->GetClusterVector();
   int classcount = clusters.size();

   // Calculo el rango de cada clase en cada banda
   double *plowerlimits = new double[classcount*bandcount];
   double *pupperlimits = new double[classcount*bandcount];
   for (int classpos = 0; classpos < classcount; classpos++) {
      for (int bandpos = 0; bandpos < bandcount; bandpos++) {
         plowerlimits[classpos * bandcount + bandpos] =
                              clusters[classpos].pStatistics_->pMean_[bandpos] -
                              sqrt(clusters[classpos].pStatistics_->pAccumVariance_[bandpos]) *
                              Threshold;
         pupperlimits[classpos * bandcount + bandpos] =
                              clusters[classpos].pStatistics_->pMean_[bandpos] +
                              sqrt(clusters[classpos].pStatistics_->pAccumVariance_[bandpos]) *
                              Threshold;
      }
   }

   // Para cada pixel veo si cae en el rango de todas las bandas.
   // Para cada pixel en la imagen
   for (size_t pixelposition = 0; pixelposition < Size; pixelposition++) {
      int pixelclass = NoClassPixelValue;

      bool invalidpixel = true;

      // Salida para pixel no valido.
      // (valor no valido en todas las bandas involucradas)
      // Ver TCK #7325
      if (NdvAvailable) {
         for (int i = 0; i < bandcount && invalidpixel; ++i) {
            if (static_cast<double>(psrc[i][pixelposition]) != CLASSIFICATION_NDV)
               invalidpixel = false;
         }
      } else {
         invalidpixel = false;
      }

      // Si el pixel es valido hago el proc normal.
      if (invalidpixel == false) {
         for (int classpos = 0; classpos < classcount; classpos++) {
            int testedclass = clusters[classpos].classId_;
            for (int bandpos = 0; bandpos < bandcount; bandpos++) {
               if (psrc[bandpos][pixelposition]
                     < plowerlimits[classpos * bandcount + bandpos]
                     || psrc[bandpos][pixelposition]
                           > pupperlimits[classpos * bandcount + bandpos]) {
                  testedclass = NoClassPixelValue;
                  break;
               }
            }
            if (testedclass != NoClassPixelValue) pixelclass = testedclass;
         }
      } else {
         // Sino le pongo un valor para la parte de la imagen que no
         // es valida.
         pixelclass = NDVPixelValue;
      }
      pDest[pixelposition] = pixelclass;
   }

   delete[] plowerlimits;
   delete[] pupperlimits;
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ClassMappingFunctionType, parallelepiped);

/** Ctor */
ParallelepipedAlgorithm::ParallelepipedAlgorithm() :
        ClusterClassificationAlgorithm(wxT(ALGORITHM_VALUE_PARALLELEPIPED)) {
}

/** Dtor */
ParallelepipedAlgorithm::~ParallelepipedAlgorithm() {
}


/**
 * Determina el tipo de dato a la entrada del algoritmo
 * @param[out] pDest datos clasificados
 * @param[in] pSource fuente de datos a clasificar
 * @param[in] Size cantidad de datos a clasificar
 */
bool ParallelepipedAlgorithm::Classify(int* pDest, std::vector<void*> pSource,
                                    size_t Size, const std::string &DataType) {
   ClassMappingFunctionType function = parallelepipedTypeMap[DataType];
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
bool ParallelepipedAlgorithm::Check() const {
   return GetThreshold() >= 0 && GetClusters()->Check();
}

}  // namespace
