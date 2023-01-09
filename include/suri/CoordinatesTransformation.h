/** \file CoordinatesTransformation.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COORDINATESTRANSFORMATION_H_
#define COORDINATESTRANSFORMATION_H_

// Standards
#include <vector>

/**
 * Puntos a tener en cuenta en una transformacion de coordenadas
 */
#define DEFAULT_CALCULATION_POINTS  300

namespace suri {

// Forward Declarations
class Coordinates;
class Subset;

/**
 * Clase base para los tipo de tranformacion de coordenadas particulares
 * creados con el TransformationFactory.
 */
class CoordinatesTransformation {
public:
   /**
    * Destructor
    */
   virtual ~CoordinatesTransformation() { }

   /**
    * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
    * las coordenadas en el formato de origen al formato de salida utilizando como
    * salida un objecto Coordinates.
    */
   virtual int Transform(Coordinates &CoordinatesP, bool Inverse = false) const = 0;

   /**
    * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
    * las coordenadas en el formato de origen al formato de salida utilizando como
    * salida un vector de Coordinates.
    */
   virtual int Transform(std::vector<Coordinates> &CoordinatesP,
                         bool Inverse = false) const = 0;

   /**
    * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
    * las coordenadas en el formato de origen al formato de salida utilizando como
    * salida un Subset.
    */
   virtual int Transform(Subset &SubsetP,
                         int CalculationPoints = DEFAULT_CALCULATION_POINTS,
                         bool Inverse = false) const = 0;

   /**
    * Indica si es la transformacon identidad.
    */
   virtual bool IsIdentity() const = 0;
   /** Compara si dos transformaciones son iguales **/
   virtual bool Equals(CoordinatesTransformation* pTransform) const=0;

protected:
   /**
    * No deberia ser instanciable directamente.
    */
   CoordinatesTransformation() { }
};

}  // namespace suri

#endif  // COORDINATESTRANSFORMATION_H_
