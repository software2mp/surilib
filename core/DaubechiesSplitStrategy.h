/*! \file DaubechiesSplitStrategy.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
