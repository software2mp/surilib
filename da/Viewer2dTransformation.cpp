/*! \file Viewer2dTransformation.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <limits>
#include <vector>

// Includes Suri
#include "suri/Viewer2dTransformation.h"
#include "suri/World.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** Constructor **/
Viewer2dTransformation::Viewer2dTransformation(World* pWorld) :
      pWorld_(pWorld) {
}

/** Destructor **/
Viewer2dTransformation::~Viewer2dTransformation() {
}

/**
 * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
 * las coordenadas en el formato de origen al formato de salida utilizando como
 * salida un objecto Coordinates.
 */
int Viewer2dTransformation::Transform(Coordinates &PointCoordinates,
                                      bool Inverse) const {
   if (pWorld_) {
      Coordinates coords;
      if (!Inverse)
         pWorld_->Transform(PointCoordinates, coords);
      else
         pWorld_->InverseTransform(PointCoordinates, coords);
      PointCoordinates = coords;
   } else {
      return 0;
   }
   return 1;
}

/**
 * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
 * las coordenadas en el formato de origen al formato de salida utilizando como
 * salida un vector de Coordinates.
 */
int Viewer2dTransformation::Transform(std::vector<Coordinates> &CoordinatesVector,
                                      bool Inverse) const {
   int count = 0;
   std::vector<Coordinates>::iterator it = CoordinatesVector.begin();
   for (; it != CoordinatesVector.end(); ++it) {
      count += Transform(*it, Inverse);
   }
   return count;
}

/**
 * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
 * las coordenadas en el formato de origen al formato de salida utilizando como
 * salida un Subset.
 */
int Viewer2dTransformation::Transform(Subset &SubsetTransformed, int CalculationPoints,
                                      bool Inverse) const {
   if (IsIdentity())
      return CalculationPoints * CalculationPoints;

   if (CalculationPoints < 2)
      return 0;

   // Resultado
   int result = 0;

   // Pasos
   double dx = (SubsetTransformed.lr_.x_ - SubsetTransformed.ul_.x_)
         / (CalculationPoints - 1);
   double dy = (SubsetTransformed.lr_.y_ - SubsetTransformed.ul_.y_)
         / (CalculationPoints - 1);

   // Limites
   double minx = std::numeric_limits<double>::max();
   double miny = std::numeric_limits<double>::max();
   double maxx = -std::numeric_limits<double>::max();
   double maxy = -std::numeric_limits<double>::max();

   // Calcula los limites en el SR de salida
   for (int ix = 0; ix < CalculationPoints; ++ix) {
      for (int jx = 0; jx < CalculationPoints; ++jx) {
         Coordinates point(SubsetTransformed.ul_.x_ + ix * dx,
                           SubsetTransformed.ul_.y_ + jx * dy);
         if (Transform(point, Inverse) != 0) {
            if (point.x_ < minx)
               minx = point.x_;

            if (point.y_ < miny)
               miny = point.y_;

            if (point.x_ > maxx)
               maxx = point.x_;

            if (point.y_ > maxy)
               maxy = point.y_;

            ++result;
         }
      }
   }

   // Genera el subset de salida respetando los ejes originales
   if (result > 0) {
      if (dx < 0) {
         SubsetTransformed.ul_.x_ = maxx;
         SubsetTransformed.lr_.x_ = minx;
      } else {
         SubsetTransformed.ul_.x_ = minx;
         SubsetTransformed.lr_.x_ = maxx;
      }
      if (dy < 0) {
         SubsetTransformed.ul_.y_ = maxy;
         SubsetTransformed.lr_.y_ = miny;
      } else {
         SubsetTransformed.ul_.y_ = miny;
         SubsetTransformed.lr_.y_ = maxy;
      }
   }

   return result;
}

/**
 * Indica si es la transformacon identidad.
 */
bool Viewer2dTransformation::IsIdentity() const {
   return false;
}

/** Compara si dos transformaciones son iguales **/
bool Viewer2dTransformation::Equals(CoordinatesTransformation* pTransform) const {
   return false;
}
} /** namespace suri */
