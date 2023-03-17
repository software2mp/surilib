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

// Includes Estandar
#include <cstring>
#include <vector>
// Includes Suri
#include "DaubechiesSplitStrategy.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Destructor
 */
DaubechiesSplitStrategy::~DaubechiesSplitStrategy() {}

DaubechiesSplitStrategy* DaubechiesSplitStrategy::CreateSplitStrategy(
      DaubechiesDirection direction) {
   switch (direction) {
      case HORIZONTAL:
         return new DaubechiesSplitHorizontal();
      case VERTICAL:
         return new DaubechiesSplitVertical();
      case BOTH:
         return new DaubechiesSplitBoth();
   }
   return 0;
}

/**
 * Funcion auxiliar para dividir una unica fila
 */
void SplitLine(const void* inputBand, void* outputBandA, void* outputBandB,
                int outputWidth, int inputOffset, int outputOffset, int dataSize) {
   for (int j = 0; j < outputWidth; ++j) {
      memcpy(static_cast<unsigned char*>(outputBandA) + outputOffset,
             static_cast<const unsigned char*>(inputBand) + inputOffset,
             dataSize);

      inputOffset += dataSize;
      memcpy(static_cast<unsigned char*>(outputBandB) + outputOffset,
                   static_cast<const unsigned char*>(inputBand) + inputOffset,
                   dataSize);
      inputOffset += dataSize;
      outputOffset += dataSize;
   }
}

/**
 * Destructor
 */
DaubechiesSplitHorizontal::~DaubechiesSplitHorizontal() {}

/**
 * Devuelve el total de bandas que tendra la salida
 */
int DaubechiesSplitHorizontal::GetOutputBands(int inputBands) const {
   return inputBands * 2;
}

/**
 * Devuelve el alto que tendra la salida
 */
int DaubechiesSplitHorizontal::GetOutputHeight(int inputHeight) const {
   return inputHeight;
}

/**
 * Devuelve el ancho que tendra la salida
 */
int DaubechiesSplitHorizontal::GetOutputWidth(int inputWidth) const {
   return inputWidth / 2;
}

/**
 * Toma el resultado de la transformada horizontal de una banda y separa
 * los coeficientes en 2 bandas
 * @param inputBands: banda a decimar
 * @param inputWidth: ancho de imagen de entrada
 * @param inputHeight: alto de imagen de entrada
 * @return imagenes filtradas y decimadas
 */
std::vector<void*> DaubechiesSplitHorizontal::SplitBand(const void* inputBand,
                                                        int inputWidth,
                                                        int inputHeight,
                                                        int dataSize) const {
   // Ancho de salida
   int outputWidth = inputWidth / 2;
   // Bandas de salida
   std::vector<void*> outputBands;
   // Agrego 2 bandas nuevas
   for (int i = 0; i < 2; ++i) {
      outputBands.push_back(new unsigned char[inputHeight * outputWidth * dataSize]);
   }
   // Offset al pixel que se esta leyendo
   int inputOffset = 0;
   // Offset al pixel que se esta escribiendo
   int outputOffset = 0;
   // Recorro fila por fila
   for (int i = 0; i < inputHeight; ++i) {
      SplitLine(inputBand, outputBands[0], outputBands[1],
                outputWidth, inputOffset, outputOffset, dataSize);
      outputOffset += outputWidth  * dataSize;
      inputOffset += inputWidth  * dataSize;
   }
   return outputBands;
}

/**
 * Destructor
 */
DaubechiesSplitVertical::~DaubechiesSplitVertical() {}

/**
 * Devuelve el total de bandas que tendra la salida
 */
int DaubechiesSplitVertical::GetOutputBands(int inputBands) const {
   return inputBands * 2;
}

/**
 * Devuelve el alto que tendra la salida
 */
int DaubechiesSplitVertical::GetOutputHeight(int inputHeight) const {
   return inputHeight / 2;
}

/**
 * Devuelve el ancho que tendra la salida
 */
int DaubechiesSplitVertical::GetOutputWidth(int inputWidth) const {
   return inputWidth;
}

std::vector<void*> DaubechiesSplitVertical::SplitBand(const void* inputBand,
                                                      int inputWidth,
                                                      int inputHeight,
                                                      int dataSize) const {
   // Alto de salida
   int outputHeight = (inputHeight + 1) / 2;
   // Bandas de salida
   std::vector<void*> outputBands;
   for (int i = 0; i < 2; ++i)
         outputBands.push_back(new unsigned char[outputHeight * inputWidth * dataSize]);
   // Offset al pixel que se esta leyendo
   int inputOffset = 0;
   // Offset al pixel que se esta escribiendo
   int outputOffset = 0;
   // Bandas de salida
   unsigned char *outputBandA, *outputBandB;
   outputBandA = static_cast<unsigned char*>(outputBands[0]);
   outputBandB = static_cast<unsigned char*>(outputBands[1]);
   // Recorro fila por fila
   for (int i = 0; i < inputHeight; ++i) {
      // Copio a una banda
      memcpy(outputBandA + outputOffset,
             static_cast<const unsigned char*>(inputBand) + inputOffset,
             inputWidth * dataSize);
      inputOffset += inputWidth * dataSize;
      // Bajo una linea
      ++i;
      // Copio a la otra banda (Si quedan lineas)
      if (i < inputHeight) {
         memcpy(outputBandB + outputOffset,
             static_cast<const unsigned char*>(inputBand) + inputOffset,
             inputWidth  * dataSize);
      }
      inputOffset += inputWidth * dataSize;
      outputOffset += inputWidth * dataSize;
   }
   return outputBands;
}

/**
 * Destructor
 */
DaubechiesSplitBoth::~DaubechiesSplitBoth() {}

/**
 * Devuelve el total de bandas que tendra la salida
 */
int DaubechiesSplitBoth::GetOutputBands(int inputBands) const {
   return inputBands * 4;
}

/**
 * Devuelve el alto que tendra la salida
 */
int DaubechiesSplitBoth::GetOutputHeight(int inputHeight) const {
   return inputHeight / 2;
}

/**
 * Devuelve el ancho que tendra la salida
 */
int DaubechiesSplitBoth::GetOutputWidth(int inputWidth) const {
   return inputWidth / 2;
}

std::vector<void*> DaubechiesSplitBoth::SplitBand(const void* inputBand,
                                                  int inputWidth,
                                                  int inputHeight,
                                                  int dataSize) const {
   // Alto de salida
   int outputHeight = (inputHeight + 1) / 2;
   // Ancho de salida
   int outputWidth = inputWidth / 2;
   // Bandas de salida
   std::vector<void*> outputBands;
   // Agrego 2 bandas nuevas
   for (int i = 0; i < 4; ++i) {
      outputBands.push_back(new unsigned char[outputHeight * outputWidth * dataSize]);
   }
   // Offset al pixel que se esta leyendo
   int inputOffset = 0;
   // Offset al pixel que se esta escribiendo
   int outputOffset = 0;
   // Recorro fila por fila
   for (int i = 0; i < inputHeight; ++i) {
      SplitLine(inputBand, outputBands[0], outputBands[1],
                outputWidth, inputOffset, outputOffset, dataSize);
      inputOffset += inputWidth * dataSize;
      // Bajo una linea
      ++i;
      // Copio a la otra banda (Si quedan lineas)
      if (i < inputHeight) {
         SplitLine(inputBand, outputBands[2], outputBands[3],
                outputWidth, inputOffset, outputOffset, dataSize);
         outputOffset += outputWidth * dataSize;
         inputOffset += inputWidth * dataSize;
      }
   }
   return outputBands;
}

} /** namespace suri */
