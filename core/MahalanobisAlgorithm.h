/** \file MahalanobisAlgorithm.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAHALANOBISALGORITHM_H_
#define MAHALANOBISALGORITHM_H_

// Includes standard
#include <string>
#include <vector>

// Includes suri
#include "ClusterClassificationAlgorithm.h"

namespace suri {

/** Clase que implementa el algoritmo mahalanobis para la clasificacion */
/**
 * Si se configura el ClassificationRenderer con una instancia de esta clase
 * se clasificara la imagen usando el metodo de mahalanobis.
 */
class MahalanobisAlgorithm : public suri::ClusterClassificationAlgorithm {
public:
   /** Ctor */
   MahalanobisAlgorithm();
   /** Dtor */
   virtual ~MahalanobisAlgorithm();

   /** Clasifica los pixels en pSource y asigna resultado a pDest */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                                                   const std::string &DataType);
   /** Verifica que las dimensiones de los parametros sean consistentes */
   virtual bool Check() const;

protected:
   /** Metodo que retorna un puntero al algoritmo de clasificacion */
   virtual MahalanobisAlgorithm* Initialize(const std::string &AlgorithmName) {
      return this;
   }
};

} /* namespace suri */
#endif /* MAHALANOBISALGORITHM_H_ */