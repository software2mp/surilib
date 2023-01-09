/*! \file Viewer3dTransformation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER3DTRANSFORMATION_H_
#define VIEWER3DTRANSFORMATION_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/CoordinatesTransformation.h"
#include "suri/Viewer3D.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *
 */
class Viewer3dTransformation : public CoordinatesTransformation {
public:
   /** constructor **/
   explicit Viewer3dTransformation(Viewer3D* pViewer);
   /** destructor **/
   virtual ~Viewer3dTransformation();
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
   /** visualizador 3d sobre el cual se hace la transformacion **/
   Viewer3D* pViewer_;
};

} /** namespace suri */

#endif /* VIEWER3DTRANSFORMATION_H_ */
