/*! \file VectorFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORFACTORY_H_
#define VECTORFACTORY_H_

// Includes suri
#include "suri/Vector.h"
#include "TFactory.h"

/** namespace suri */
namespace suri {
/** Defino un TFactory que crea vectores */
typedef TFactory<Vector> VectorFactory;
/** Defino un singleton para VectorFactory */
typedef TSingleton<VectorFactory> TheVectorFactory;

void RegisterAllVectors();
}

#endif /* VECTORFACTORY_H_ */
