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

#ifndef POLYNOMLEASTSQUARESTRANSFORM_H_
#define POLYNOMLEASTSQUARESTRANSFORM_H_

// Includes standard
#include <vector>
// Includes Suri
#include "suri/PolynomLeastSquaresTransform.h"
// Includes Wx

// Includes App

// Includes Gdal

// Defines

// Forwards

/** namespace suri */
#include "suri/Coordinates.h"
#include "suri/WarpTransform.h"

namespace suri {
/** Par de coeficientes x' e y', que integraran una lista para definir un */
/** polinomio */
class CoefficientPair {
public:
   /** Constructor */
   CoefficientPair(const double &X = 0.0, const double &Y = 0.0);
   /** Destructor */
   virtual ~CoefficientPair();
   double x_; /*! Coeficiente en X */
   double y_; /*! Coeficiente en Y */
};

typedef std::vector<CoefficientPair> XYCoefficientListType;
/**
 * Lista de GCPs a ser utilizados
 */
class GCPList {
public:
   CoordinateListType origin_; /*! Lista de coordenadas de origen */
   CoordinateListType destination_; /*! Lista de coordenadas de destino */
};

/**
 * Una clase que representa un polinomio de aproximacion entre 2 conjuntos de
 * coordenadas x,y.
 */

class PolynomLeastSquaresTransform : public WarpTransform {
public:

   PolynomLeastSquaresTransform();
   virtual ~PolynomLeastSquaresTransform();
   /** True si se pudo determinar el polinomio de transformacion */
   bool IsOk();
   /** Limpia el calculo del polinomio y reinicializa la clase */
   void Clear();
   /** Carga un listado de puntos de control */
   bool LoadGcps(const GCPList &GcpListInput);
   /** Calcula los coeficientes actuales de los polinomios de interpolacion */
   /** x' e y', por medio del metodo de minimos cuadrados, a partir de los GCPs */
   /** cargados previamente */
   bool CalculateCoefficients(int NewPolinomialOrder);
   /** Obtiene el rmse del listado */
   double GetRmse();
   /** Permite obtener los coeficientes actuales de los polinomios de */
   /** interpolacion x' e y', representado como una lista de coordenadas. */
   bool GetCoefficients(XYCoefficientListType &ActualCoefficients);
   /** Permite setear directamente los coeficientes actuales de los */
   /** polinomios de interpolacion x' e y'. */
   bool SetCoefficients(const XYCoefficientListType &NewCoefficients,
                        int NewPolinomialOrder);
   /** Mapea la lista de coordenadas de entrada en la de salida, */
   /** aplicando el polinomio de transformacion calculado previamente */
   bool ApplyTransform(const CoordinateListType &Origin,
                       CoordinateListType &Destination);
   /** Mapea la coordenada de entrada a la coordenada de salida, */
   /** aplicando el polinomio de transformacion calculado previamente. */
   bool ApplyTransform(const Coordinates &Origin, Coordinates &Destination);

private:
   int polynomialOrder_; /*! Orden del polinomio de interpolacion */
   GCPList currentGcps_; /*! Lista de GCPs */
   XYCoefficientListType currentCoefficients_; /*! Lista de coefs. actuales */
   bool calculatedCoefficients_; /*! Coeficientes calculados */
};
}  // namespace suri

#endif /* POLYNOMLEASTSQUARESTRANSFORM_H_ */

