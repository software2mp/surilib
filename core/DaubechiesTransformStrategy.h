/*! \file DaubechiesTransformStrategy.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DAUBECHIESTRANSFORMSTRATEGY_H_
#define DAUBECHIESTRANSFORMSTRATEGY_H_

// Includes Estandar
#include <string>
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Aplica interfaz para las distintas transformadas Daubechies
 */

class DaubechiesTransformStrategy {
public:
   /**
    * Constructor
    */
   DaubechiesTransformStrategy(void* data, int width, int height,
                               const std::string& dataType);

   /**
    * Destructor
    */
   virtual ~DaubechiesTransformStrategy();
   /**
    * Aplica la transformada de Daubechies D4 in-place, en sentido horizontal
    */
   virtual float* Transform() = 0;
   static double D4ScalingCoefficients[8];
   static double D4WaveletCoefficients[8];
   typedef void (*ReadFunctionType)(int offsetX, void* data,
                                    int width, int height, double* vector);

protected:
   /**
    * Lee un vector de longitud 8 para aplicar la transformada de forma
    * deslizante
    */
   ReadFunctionType readSubVectorFunction_;
   void* data_;
   int width_;
   int height_;
   std::string dataType_;
};

class DaubechiesTransformHorizontal : public DaubechiesTransformStrategy {
public:
   /**
    * Constructor
    */
   DaubechiesTransformHorizontal(void* data, int width, int height,
                                 const std::string& dataType);
   /**
    * Tomo un vector horizontal y transformo pixel a pixel
    */
   float* Transform();
};

class DaubechiesTransformVertical : public DaubechiesTransformStrategy {
public:
   /**
    * Constructor
    */
   DaubechiesTransformVertical(void* data, int width, int height,
                               const std::string& dataType);
   /**
    * Tomo vectores de forma vertical y transformo haciendo un barrido horizontal
    * de a 2 lineas por vez.
    */
   float* Transform();
};

class DaubechiesTransform2D : public DaubechiesTransformStrategy {
public:
   /**
    * Constructor
    */
   DaubechiesTransform2D(void* data, int width, int height,
                               const std::string& dataType);
   /**
    * Transformo de forma vertical y luego horizontal
    */
   float* Transform();
};

} /** namespace suri */

#endif /* DAUBECHIESTRANSFORMSTRATEGY_H_ */
