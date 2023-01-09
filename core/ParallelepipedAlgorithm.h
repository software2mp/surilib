/** \file ParallelepipedAlgorithm.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef PARALLELEPIPEDALGORITHM_H_
#define PARALLELEPIPEDALGORITHM_H_

// Includes standard
#include <string>
#include <vector>
// Includes Suri
#include "ClusterClassificationAlgorithm.h"

/** namespace suri */
namespace suri {
/** Clase que implementa el algoritmo paralelepipedo para la clasificacion */
/**
 * Si se configura el ClassificationRenderer con una instancia de esta clase
 * se clasificara la imagen usando el metodo de paralelepipedo.
 */
class ParallelepipedAlgorithm : public ClusterClassificationAlgorithm {
   /** Ctor. de Copia. */
   ParallelepipedAlgorithm(const ParallelepipedAlgorithm &ParallelepipedAlgorithm);

public:
   /** Ctor */
   ParallelepipedAlgorithm();
   /** Dtor */
   virtual ~ParallelepipedAlgorithm();
   /** Clasifica los pixels en pSource y asigna resultado a pDest */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                                                   const std::string &DataType);
   /** Verifica que las dimensiones de los parametros sean consistentes */
   virtual bool Check() const;

protected:
   /** Metodo que retorna un puntero al algoritmo de clasificacion */
   virtual ParallelepipedAlgorithm* Initialize(const std::string &AlgorithmName) {
      return this;
   }
};
}  // namespace


#endif /* PARALLELEPIPEDALGORITHM_H_ */
