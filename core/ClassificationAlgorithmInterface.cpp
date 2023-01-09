/*! \file ClassificationAlgorithmInterface.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>

#include "ClassificationAlgorithmInterface.h"
#include "suri/AbstractFactory.h"

#define NDV_PIXEL_VALUE 255

namespace suri {

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION_COMMON(ClassificationAlgorithmInterface, 0)

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(ClassificationAlgorithmInterface, 1,
                           DeclareParameters_1(const std::string&))

/** Ctor */
ClassificationAlgorithmInterface::ClassificationAlgorithmInterface() :
      ndvPixelValue_(NDV_PIXEL_VALUE) {

}

/** Dtor */
ClassificationAlgorithmInterface::~ClassificationAlgorithmInterface() {

}

/**
 * Configura el valor que el algoritmo asigna a los pixels con valor no valido.
 */
void ClassificationAlgorithmInterface::SetNDVPixelValue(int NDVPixelValue) {
   ndvPixelValue_ = NDVPixelValue;
}

/**
 * Retorna el valor que el algoritmo asigna a los pixels valor no valido.
 */
int ClassificationAlgorithmInterface::GetNDVPixelValue() const {
   return ndvPixelValue_;
}

}  // namespace suri
