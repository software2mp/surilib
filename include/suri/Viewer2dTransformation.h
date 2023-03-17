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
