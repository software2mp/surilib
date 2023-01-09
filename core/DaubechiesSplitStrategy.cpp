/*! \file DaubechiesSplitStrategy.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
