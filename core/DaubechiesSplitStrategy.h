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

#ifndef DAUBECHIESSPLITSTRATEGY_H_
#define DAUBECHIESSPLITSTRATEGY_H_

// Includes Estandar
#include <vector>
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

enum DaubechiesDirection {
   HORIZONTAL, VERTICAL, BOTH
};

class DaubechiesSplitStrategy {
public:
   /**
    * Destructor
    */
   virtual ~DaubechiesSplitStrategy();
   /**
    * Devuelve una instancia nueva de la estrategia seleccionada
    */
   static DaubechiesSplitStrategy* CreateSplitStrategy(DaubechiesDirection direction);

   virtual std::vector<void*> SplitBand(const void* inputBand, int inputWidth,
                                int inputHeight, int dataSize) const = 0;
   /**
    * Devuelve el total de bandas que tendra la salida
    */
   virtual int GetOutputBands(int inputBands) const = 0;
   /**
    * Devuelve el alto que tendra la salida
    */
   virtual int GetOutputHeight(int inputHeight) const = 0;
   /**
    * Devuelve el ancho que tendra la salida
    */
   virtual int GetOutputWidth(int inputWidth) const = 0;
};

/**
 * Estrategia para decimado horizontal
 */
class DaubechiesSplitHorizontal : public DaubechiesSplitStrategy {
public:
   virtual ~DaubechiesSplitHorizontal();
   /**
    * Toma el resultado de la transformada horizontal de una banda y separa
    * los coeficientes en 2 bandas
    * @param inputBands: banda a decimar
    * @param inputWidth: ancho de imagen de entrada
    * @param inputHeight: alto de imagen de entrada
    * @return imagenes filtradas y decimadas
    */
   std::vector<void*> SplitBand(const void* inputBand, int inputWidth,
                                int inputHeight, int dataSize) const;
   /**
    * Devuelve el total de bandas que tendra la salida
    */
   int GetOutputBands(int inputBands) const;
   /**
    * Devuelve el alto que tendra la salida
    */
   int GetOutputHeight(int inputHeight) const;
   /**
    * Devuelve el ancho que tendra la salida
    */
   int GetOutputWidth(int inputWidth) const;
};

/**
 * Estrategia para decimado vertical
 */
class DaubechiesSplitVertical : public DaubechiesSplitStrategy {
public:
   virtual ~DaubechiesSplitVertical();
   /**
    * Toma el resultado de la transformada vertical de una banda y separa
    * los coeficientes en 2 bandas
    * @param inputBands: banda a decimar
    * @param inputWidth: ancho de imagen de entrada
    * @param inputHeight: alto de imagen de entrada
    * @return imagenes filtradas y decimadas
    */
   std::vector<void*> SplitBand(const void* inputBand, int inputWidth,
                                int inputHeight, int dataSize) const;
   /**
    * Devuelve el total de bandas que tendra la salida
    */
   int GetOutputBands(int inputBands) const;
   /**
    * Devuelve el alto que tendra la salida
    */
   int GetOutputHeight(int inputHeight) const;
   /**
    * Devuelve el ancho que tendra la salida
    */
   int GetOutputWidth(int inputWidth) const;
};

/**
 * Estrategia para decimado horizontal y vertical
 */
class DaubechiesSplitBoth : public DaubechiesSplitStrategy {
public:
   virtual ~DaubechiesSplitBoth();
   /**
    * Toma el resultado de la transformada 2D de una banda y separa
    * los coeficientes en 4 bandas
    * @param inputBands: banda a decimar
    * @param inputWidth: ancho de imagen de entrada
    * @param inputHeight: alto de imagen de entrada
    * @return imagenes filtradas y decimadas
    */
   std::vector<void*> SplitBand(const void* inputBand, int inputWidth,
                                int inputHeight, int dataSize) const;
   /**
    * Devuelve el total de bandas que tendra la salida
    */
   int GetOutputBands(int inputBands) const;
   /**
    * Devuelve el alto que tendra la salida
    */
   int GetOutputHeight(int inputHeight) const;
   /**
    * Devuelve el ancho que tendra la salida
    */
   int GetOutputWidth(int inputWidth) const;
};

} /** namespace suri */

#endif /* DAUBECHIESSPLITSTRATEGY_H_ */
