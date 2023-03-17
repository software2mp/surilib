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

#ifndef POLYNOMIALCOORDINATESTRANSFORMATION_H_
#define POLYNOMIALCOORDINATESTRANSFORMATION_H_

#include <vector>
#include <string>
#include <limits>

#include "suri/CoordinatesTransformation.h"
#include "suri/Coordinates.h"
#include "suri/Subset.h"
#include "suri/GroundControlPoint.h"

#include <ogr_spatialref.h>
#include <gdal.h>
#include <gdal_alg.h>


namespace suri {

/** Struct necesario */
typedef struct {
   /** Stub */
   GDALTransformerInfo sTI;
   /** Stub */
   double adfToGeoX[20];
   /** Stub */
   double adfToGeoY[20];
   /** Stub */
   double adfFromGeoX[20];
   /** Stub */
   double adfFromGeoY[20];
   /** Stub */
   int nOrder;
   /** Stub */
   int intbReversed;
   /** Stub */
   int nGCPCount;
   /** Stub */
   GDAL_GCP *pasGCPList;
} GCPTransformInfo;


/**
 * Par de coeficientes x' e y', que integraran una lista para definir un
 * polinomio.
 */
class PolynomialCoefficients {
public:
   /**
    * Constructor.
    */
   PolynomialCoefficients(const double &X = 0.0, const double &Y = 0.0) : x_(X), y_(Y) { }

   /**
    * Destructor.
    */
   virtual ~PolynomialCoefficients() { }

   double x_;  /*! Coeficiente en X */
   double y_;  /*! Coeficiente en Y */
};


/**
 * Representa un polinomio de aproximacion entre 2 conjuntos de
 * coordenadas (x,y)
 */
class PolynomialCoordinatesTransformation : public CoordinatesTransformation {
public:
   /**
    * Construye un nuevo objeto usando un orden y un conjunto de puntos de control
    * especificos.
    */
   PolynomialCoordinatesTransformation(
         const int& PolynomialOrder,
         const std::vector<GroundControlPoint>& GcpVector,
         const std::vector<PolynomialCoefficients>& Coefficients =
               std::vector<PolynomialCoefficients>());

   /**
    * Destructor.
    */
   virtual ~PolynomialCoordinatesTransformation();

   /**
    * Inicializa el objeto.
    */
   bool Initialize();


   /**
    * Obtiene el rmse del listado.
    */
   double GetRmse();

   /**
    * Realiza una transformacion polinomica de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un objeto Coordinates.
    */
   virtual int Transform(Coordinates &CoordinatesP, bool Inverse = false) const;

   /**
    * Realiza una transformacion polinomica de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un vector de Coordinates.
    */
   virtual int Transform(std::vector<Coordinates> &CoordinatesVector,
                         bool Inverse = false) const;

   /**
    * Realiza una transformacion polinomica de las coordenadas en el formato de
    * origen al formato de salida utilizando como salida un Subset.
    */
   virtual int Transform(Subset &SubsetP,
                         int CalculationPoints = DEFAULT_CALCULATION_POINTS,
                         bool Inverse = false) const;

   /**
    * Indica si es la transformacon identidad.
    */
   virtual bool IsIdentity() const;
   /** Compara si dos transformaciones son iguales **/
   virtual bool Equals(CoordinatesTransformation* pTransform) const;

private:
   int polynomialOrder_;  /** Orden del polinomio de interpolacion */
   bool isIdentity_;  /** es identidad */
   bool calculatedCoefficients_;  /** Coeficientes calculados */
   std::vector<GroundControlPoint> vGcp_;  /** Lista de GCPs */
   std::vector<PolynomialCoefficients> currentCoefficients_;  /** Lista de coefs. actuales */

   /**
    * Verifica si se pudo determinar el polinomio de transformacion.
    */
   bool IsOk() const;

   /**
    * Calcula los coeficientes actuales de los polinomios de interpolacion
    * x' e y', por medio del metodo de minimos cuadrados, a partir de los GCPs
    * cargados previamente
    */
   bool CalculateCoefficients();
   /** punteros a estructura que contiene los argumentos de la transformacion directa e inversa*/
   void* pCurrentTransformationDirectArgument_;
   void* pCurrentTransformationInverseArgument_;
};

}  // namespace suri

#endif  // POLYNOMIALCOORDINATESTRANSFORMATION_H_
