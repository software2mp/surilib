/*! \file Viewer2dTransformation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER2DTRANSFORMATION_H_
#define VIEWER2DTRANSFORMATION_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/CoordinatesTransformation.h"

// Includes Wx
// Defines

namespace suri {
// forwards
class World;

/*!
 *
 */
class Viewer2dTransformation : public CoordinatesTransformation {
   /** ctor copia **/
   Viewer2dTransformation(const Viewer2dTransformation&);

public:
   /** constructor **/
   explicit Viewer2dTransformation(World* pWorld);
   /** destructor **/
   virtual ~Viewer2dTransformation();
   /**
    * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
    * las coordenadas en el formato de origen al formato de salida utilizando como
    * salida un objecto Coordinates.
    */
   virtual int Transform(Coordinates &PointCoordinates, bool Inverse = false) const;

   /**
    * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
    * las coordenadas en el formato de origen al formato de salida utilizando como
    * salida un vector de Coordinates.
    */
   virtual int Transform(std::vector<Coordinates> &VectorCoordinates,
                         bool Inverse = false) const;

   /**
    * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
    * las coordenadas en el formato de origen al formato de salida utilizando como
    * salida un Subset.
    */
   virtual int Transform(Subset &SubsetTransformed, int CalculationPoints =
         DEFAULT_CALCULATION_POINTS,
                         bool Inverse = false) const;

   /**
    * Indica si es la transformacon identidad.
    */
   virtual bool IsIdentity() const;
   /** Compara si dos transformaciones son iguales **/
   virtual bool Equals(CoordinatesTransformation* pTransform) const;

private:
   /** mundo a utilizar en la transformacion **/
   World* pWorld_;
};

} /** namespace suri */

#endif /* VIEWER2DTRANSFORMATION_H_ */
