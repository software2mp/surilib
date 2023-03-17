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
