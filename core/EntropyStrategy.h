/*! \file RegistrabilityStrategy.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REGISTRABILITYSTRATEGY_H_
#define REGISTRABILITYSTRATEGY_H_

// Includes Estandar
#include <vector>
// Includes Suri
#include "EntropyResult.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class EntropyStrategy {
public:
   /**
    * Destructor
    */
   virtual ~EntropyStrategy();
   /**
    * Calcula la entropia de una imagen
    */
   virtual std::vector<EntropyResult> CalculateEntropy() = 0;

};

class EntropyByTiles {
public:
   /**
    * Constructor
    */
   EntropyByTiles(float* const pData, int sizeX, int sizeY);
   /**
    * Calcula la entropia de la imagen recorriendola por baldosas
    * de tamanio tileSize
    */
   std::vector<EntropyResult> CalculateEntropy() const;
private:
   /**
    * Devuelve la entropia de una baldosa que comienza en tileX, tileY
    */
   EntropyResult GetEntropyForTile(int tileX, int tileY) const;
   /**
    * Imagen
    */
   float* pData_;
   /**
    * Tamanio de la imagen
    */
   int sizeX_, sizeY_;
   /**
    * Constante para el tamanio de la celda.
    */
   static const int kTileSize;
   static const double kHistogramBins;
};

} /** namespace suri */

#endif /* REGISTRABILITYSTRATEGY_H_ */
