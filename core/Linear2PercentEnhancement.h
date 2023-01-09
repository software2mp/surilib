/*! \file Linear2PercentEnhancement.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LINEAR2PERCENTENHANCEMENT_H_
#define LINEAR2PERCENTENHANCEMENT_H_

// Includes suri
#include "LinearEnhancement.h"

/** namespace suri */
namespace suri {
/** Clase que crea un realce (Lut) de tipo lineal 2% */
/**
 * Calse que aplica el realce lineal 2% para crear una Lut.
 * Utiliza el histograma de valores de la imagen para excluir los valores que
 * quedan en los extremos de la distribucion (2% a cada lado).
 */
class Linear2PercentEnhancement : public LinearEnhancement {
public:
   /** Ctor. */
   Linear2PercentEnhancement();
   /** Dtor. */
   virtual ~Linear2PercentEnhancement();
private:
   /** Devuelve los parametros estadisticos relevantes para la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const;
   /** Obtiene el minimo y el maximo relevantes de una banda para la funcion de realce */
   virtual void GetLimits(const Statistics &Stats, size_t BandNumber, double &Min,
                          double &Max) const;
};
}

#endif /* LINEAR2PERCENTENHANCEMENT_H_ */
