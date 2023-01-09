/** \file MinimumDistanceAlgorithm.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <limits>
#include <string>
#include <vector>

// Includes suri
#include "MinimumDistanceAlgorithm.h"
#include "suri/DataTypes.h"
#include "suri/xmlnames.h"

namespace suri {
/** Macro para registrar el tipo de algoritmo de clasificacion de forma automatica */
AUTO_REGISTER_CLASS(ClassificationAlgorithmInterface, MinimumDistanceAlgorithm, 0)

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
void mindistance(int* pDest, std::vector<void*> &pSource, size_t Size,
                 int NoClassPixelValue, int NDVPixelValue, double Ndv,
                 bool NdvAvailable, double Threshold, Clusters* pClusters) {
   int sourcesize = pSource.size();
   std::vector<T*> psrc;
   for (int i = 0; i < sourcesize; i++)
      psrc.push_back(static_cast<T*>(pSource[i]));

   const std::vector<Clusters::ClusterData>& clusters =
                                          pClusters->GetClusterVector();
   int classcount = clusters.size();
//   int matrixsize = clusters[0].statistics_.mean_.size();
   int matrixsize = clusters[0].pStatistics_->GetBandCount();
   double threshold = Threshold * Threshold;

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
            double diftomean, distance = 0;
            for (int i = 0; i < matrixsize; i++) {
#ifdef __OLD_STATS_CALC__
               diftomean = static_cast<double>(psrc[i][pixelposition])
                     - clusters[classpos].statistics_.mean_[i];
#endif
               diftomean = static_cast<double>(psrc[i][pixelposition])
                     - clusters[classpos].pStatistics_->pMean_[i];
               distance += diftomean * diftomean;
            }

            // Comparo la ultima clase asignada y el threashold
            if (distance < threshold && distance < pixelclassdistance) {
               pixelclass = clusters[classpos].classId_;
               pixelclassdistance = distance;
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
INITIALIZE_DATATYPE_MAP(ClassMappingFunctionType, mindistance);

/** Ctor */
MinimumDistanceAlgorithm::MinimumDistanceAlgorithm()  :
          ClusterClassificationAlgorithm(wxT(ALGORITHM_VALUE_MIN_DISTANCE)) {
}

/** Dtor */
MinimumDistanceAlgorithm::~MinimumDistanceAlgorithm() {
}

/**
 * Determina el tipo de dato a la entrada del algoritmo
 * @param[out] pDest datos clasificados
 * @param[in] pSource fuente de datos a clasificar
 * @param[in] Size cantidad de datos a clasificar
 */
bool MinimumDistanceAlgorithm::Classify(int* pDest, std::vector<void*> pSource,
                                    size_t Size, const std::string &DataType) {
   ClassMappingFunctionType function = mindistanceTypeMap[DataType];
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
bool MinimumDistanceAlgorithm::Check() const {
   return GetThreshold() >= 0 && GetClusters()->Check();
}

} /* namespace suri */
