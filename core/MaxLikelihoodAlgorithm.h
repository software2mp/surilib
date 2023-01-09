/*! \file MaxLikelihoodAlgorithm.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MAXLIKELIHOODALGORITHM_H_
#define MAXLIKELIHOODALGORITHM_H_

// Includes standard
#include <string>
#include <vector>
// Includes Suri
#include "ClusterClassificationAlgorithm.h"
// Includea wx
// Defined
// forward

/** namespace suri */
namespace suri {
/** Clase que implementa el algoritmo MaxLikelihood para la clasificacion */
/**
 * Si se configura el ClassificationRenderer con una instancia de esta clase
 * se clasificara la imagen usando el metodo de maxima verosimilitud.
 */
class MaxLikelihoodAlgorithm : public ClusterClassificationAlgorithm {
   /** Ctor. de Copia. */
   MaxLikelihoodAlgorithm(const MaxLikelihoodAlgorithm &MaxLikAlgorithm);

public:
   /** Ctor */
   MaxLikelihoodAlgorithm();
   /** Dtor */
   virtual ~MaxLikelihoodAlgorithm();
   /** Clasifica los pixels en pSource y asigna resultado a pDest */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                                                   const std::string &DataType);
   /** Verifica que las dimensiones de los parametros sean consistentes */
   virtual bool Check() const;

protected:
   /** Metodo que retorna un puntero al algoritmo de clasificacion */
   virtual MaxLikelihoodAlgorithm* Initialize(const std::string &FileName) {
      return this;
   }
};
}  // namespace

#endif /* MAXLIKELIHOODALGORITHM_H_ */
