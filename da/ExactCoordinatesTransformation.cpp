/** \file ExactCoordinatesTransformation.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/ExactCoordinatesTransformation.h"

namespace suri {

/**
 * Construye un objeto ExactCoordinatesTransformation
 *
 * @param[in] WktIn Referencia de entrada.
 * @param[in] WktIn Referencia de salida.
 * @param[in] IsIdentity Especifica si la transformacion es la identidad.
 * @param[in] PTransform Transformacion directa a utilizar.
 * @param[in] PITransform Transformacion inversa a utilizar.
 */
ExactCoordinatesTransformation::ExactCoordinatesTransformation(
      const std::string& WktIn,
      const std::string& WktOut,
      bool IsIdentity,
      OGRCoordinateTransformation* PTransform,
      OGRCoordinateTransformation* PITransform): isIdentity_(IsIdentity),
      wktIn_(WktIn), wktOut_(WktOut),
      pTransform_(PTransform),
      pITransform_(PITransform)  {
}

/**
 * Destructor (destruye los punteros de las transformaciones utilizadas)
 */
ExactCoordinatesTransformation::~ExactCoordinatesTransformation() {
   if (!IsIdentity()) {
      OCTDestroyCoordinateTransformation(pTransform_);
      OCTDestroyCoordinateTransformation(pITransform_);
      pTransform_ = NULL;
      pITransform_ = NULL;
   }
}

/**
 * Realiza una transformacion exacta de las coordenadas en el formato de
 * origen al formato de salida utilizando como salida un objeto Coordinates.
 *
 * @param[out] CoordinatesP Coordenadas transformadas.
 * @param[in] Inverse Indica si se debe hacer la transformacion inversa.
 * @return Cantidad de coordenadas transformadas (En este caso deberia devolver siempre 1).
 */
int ExactCoordinatesTransformation::Transform(Coordinates &CoordinatesP,
                                                bool Inverse) const {
   if (IsIdentity())
      return 1;

   if (Inverse) {
      return pITransform_->Transform(1, &(CoordinatesP.x_), &(CoordinatesP.y_));
   } else {
      return pTransform_->Transform(1, &(CoordinatesP.x_), &(CoordinatesP.y_));
   }
}

/**
 * Realiza una transformacion exacta de las coordenadas en el formato de
 * origen al formato de salida utilizando como salida un vector de Coordinates.
 *
 * @param[out] CoordinatesP Vector de coordenadas transformadas.
 * @param[in] Inverse Indica si se debe hacer la transformacion inversa.
 * @return Cantidad de coordenadas transformadas.
 */
int ExactCoordinatesTransformation::Transform(
      std::vector<Coordinates> &CoordinatesP,
      bool Inverse) const {

   size_t totalcoord = CoordinatesP.size();
   if (IsIdentity())
      return totalcoord;

   int result = 0;
   for (size_t ix = 0; ix < totalcoord; ++ix) {
      if (Transform(CoordinatesP[ix], Inverse) != 0) {
         ++result;
      }
   }

   return result;
}

/**
 * Realiza una transformacion exacta de las coordenadas en el formato de
 * origen al formato de salida utilizando como salida un Subset.
 *
 * @param[out] SubsetP Subset transformado.
 * @param[in] CalculationPoints Limite de coordenadas a calcular para el subset.
 * @param[in] Inverse Indica si se debe hacer la transformacion inversa.
 * @return Cantidad de coordenadas transformadas.
 */
int ExactCoordinatesTransformation::Transform(Subset &SubsetP,
                                                int CalculationPoints,
                                                bool Inverse) const {

   if (IsIdentity())
      return CalculationPoints * CalculationPoints;

   if (CalculationPoints < 2)
      return 0;

   // Resultado
   int result = 0;

   // Pasos
   double dx = (SubsetP.lr_.x_ - SubsetP.ul_.x_) / (CalculationPoints - 1);
   double dy = (SubsetP.lr_.y_ - SubsetP.ul_.y_) / (CalculationPoints - 1);

   // Limites
   double minx = std::numeric_limits<double>::max();
   double miny = std::numeric_limits<double>::max();
   double maxx = -std::numeric_limits<double>::max();
   double maxy = -std::numeric_limits<double>::max();

   // Calcula los limites en el SR de salida
   for (int ix = 0; ix < CalculationPoints; ++ix) {
      for (int jx = 0; jx < CalculationPoints; ++jx) {
         Coordinates point(SubsetP.ul_.x_ + ix * dx, SubsetP.ul_.y_ + jx * dy);
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
         SubsetP.ul_.x_ = maxx;
         SubsetP.lr_.x_ = minx;
      } else {
         SubsetP.ul_.x_ = minx;
         SubsetP.lr_.x_ = maxx;
      }
      if (dy < 0) {
         SubsetP.ul_.y_ = maxy;
         SubsetP.lr_.y_ = miny;
      } else {
         SubsetP.ul_.y_ = miny;
         SubsetP.lr_.y_ = maxy;
      }
   }

   return result;
}

/**
 * Indica si es la transformacon identidad.
 *
 * @return Devuelve true si es una transformacion identidad.
 */
bool ExactCoordinatesTransformation::IsIdentity() const {
   return isIdentity_;
}

/**
 * Devuelve la transformacion OGR directa o inversa.
 * @param[in] Inverse indica que debe devolver el metodo:
 * @return Transformacion inversa si Inverse == true
 * @return Transformacion si Inverse == false
 */
OGRCoordinateTransformation * ExactCoordinatesTransformation::GetOGRCT(bool Inverse) const {
   if (Inverse) {
      return pITransform_;
   } else {
      return pTransform_;
   }
}

/** Compara si dos transformaciones son iguales **/
/** Compara si dos transformaciones son iguales **/
bool ExactCoordinatesTransformation::Equals(CoordinatesTransformation* pTransform) const {
   ExactCoordinatesTransformation* ptransform =
         dynamic_cast<ExactCoordinatesTransformation*>(pTransform);
   if (!ptransform)
      return false;
   return (wktIn_.compare(ptransform->wktIn_) == 0 &&  wktOut_.compare(ptransform->wktOut_) == 0);
}
/** Devuelve el wkt de salida **/
std::string ExactCoordinatesTransformation::GetWktIn() const {
   return wktIn_;
}
/** Devuelve el wkt de llegada **/
std::string ExactCoordinatesTransformation::GetWktOut() const {
   return wktOut_;
}

} // namespace suri

/** @mainpage
 *
 * @page "suri::ExactCoordinatesTransformation" Mini-HowTo de como crear transformaciones.
 *
 * @code
 * Coordinates coords(Position);
 * suri::TransformationFactory* factory = suri::TransformationFactoryBuilder::Build();
 * suri::TransformationParameters params;
 * params["SRWktIn"] = &srcref;
 * params["SRWktOut"] = &destref;
 * suri::CoordinatesTransformation* transformation = factory->Create(suri::TransformationFactory::kExact, params);
 * if (transformation)
 *    transformation->Transform(coords);
 * suri::TransformationFactoryBuilder::Release(factory);
 * @endcode
 */

