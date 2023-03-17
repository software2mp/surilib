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

#ifndef EXACTCOORDINATESTRANSFORMATION_H_
#define EXACTCOORDINATESTRANSFORMATION_H_

#include <vector>
#include <string>
#include <limits>

#include <ogr_spatialref.h>

#include "suri/CoordinatesTransformation.h"
#include "suri/Coordinates.h"
#include "suri/Subset.h"

/** namespace suri */
namespace suri {

/**
 * Implementa los metodos correspondientes de CoordinatesTransformation
 * para poder generar una trasnformacion exacta.
 */
class ExactCoordinatesTransformation : public CoordinatesTransformation {
public:
   /**
    * Construye un objeto ExactCoordinatesTransformation
    */
   ExactCoordinatesTransformation(const std::string& WktIn,
                                    const std::string& WktOut,
                                    bool IsIdentity = true,
                                    OGRCoordinateTransformation* PTransform = NULL,
                                    OGRCoordinateTransformation* PITransform = NULL);

   /**
    * Destructor
    */
   virtual ~ExactCoordinatesTransformation();

   /**
    * Realiza una transformacion exacta de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un objeto Coordinates.
    */
   virtual int Transform(Coordinates &CoordinatesP, bool Inverse = false) const;

   /**
    * Realiza una transformacion exacta de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un vector de Coordinates.
    */
   virtual int Transform(std::vector<Coordinates> &CoordinatesP,
                         bool Inverse = false) const;

   /**
    * Realiza una transformacion exacta de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un Subset.
    */
   virtual int Transform(Subset &SubsetP,
                         int CalculationPoints = DEFAULT_CALCULATION_POINTS,
                         bool Inverse = false) const;

   /**
    * Indica si es la transformacon identidad.
    */
   virtual bool IsIdentity() const;
   /** Devuelve el la transformacion OGR directa o inversa */
   OGRCoordinateTransformation * GetOGRCT(bool Inverse = false) const;
   /** Compara si dos transformaciones son iguales **/
   virtual bool Equals(CoordinatesTransformation* pTransform) const;
   /** Devuelve el wkt del sr de salida **/
   std::string GetWktIn() const;
   /** Devuelve el wkt del sr de llegada **/
   std::string GetWktOut() const;

private:
   bool isIdentity_; /** es identidad */
   std::string wktIn_; /** Referencia de entrada */
   std::string wktOut_; /** Referencia de salida */
   OGRCoordinateTransformation* pTransform_; /** Puntero a la transformacion directa */
   OGRCoordinateTransformation* pITransform_; /** Puntero a la transformacion inversa */
};

}  // namespace suri

#endif  // EXACTCOORDINATESTRANSFORMATION_H_
