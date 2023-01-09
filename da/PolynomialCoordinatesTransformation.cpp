/** \file PolynomialCoordinatesTransformation.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/PolynomialCoordinatesTransformation.h"

// Includes Wx
// Defines
// forwards
namespace suri {

/**
 * Construye un nuevo objeto usando un orden y un conjunto de puntos de control
 * especificos.
 *
 * @param[in] PolynomialOrder Orden de transformacion.
 * @param[in] vGcp Puntos de control que se van a utilizar para el calculo
 * de los coeficientes del polinomio de transformacion.
 */
PolynomialCoordinatesTransformation::PolynomialCoordinatesTransformation(
      const int& PolynomialOrder, const std::vector<GroundControlPoint>& GcpVector,
      const std::vector<PolynomialCoefficients>& Coefficients) :
      polynomialOrder_(PolynomialOrder), vGcp_(GcpVector), currentCoefficients_(Coefficients),
      pCurrentTransformationDirectArgument_(NULL), pCurrentTransformationInverseArgument_(NULL) {
   isIdentity_ = false;
   calculatedCoefficients_ = false;
}

/**
 * Destructor.
 */
PolynomialCoordinatesTransformation::~PolynomialCoordinatesTransformation() {
   if (pCurrentTransformationDirectArgument_)
      GDALDestroyGCPTransformer(pCurrentTransformationDirectArgument_);
   if (pCurrentTransformationInverseArgument_)
      GDALDestroyGCPTransformer(pCurrentTransformationInverseArgument_);
}

/**
 * Inicializa el objeto.
 *
 * @return true si se realizo la inicializacion con exito.
 */
bool PolynomialCoordinatesTransformation::Initialize() {
   bool retvalue = true;
   if (currentCoefficients_.empty())
      retvalue = CalculateCoefficients();
   else
      calculatedCoefficients_ = true;
   return retvalue;
}

/**
 * Obtiene el rmse del listado.
 *
 * @return Diferencia calculada.
 */
double PolynomialCoordinatesTransformation::GetRmse() {
   if (IsOk()) {
      double quadraticerrorsum = 0.0;
      std::vector<Coordinates>
      temporalcoordinatelist(GroundControlPoint::ExtractSourceCoordinates(vGcp_));

      Transform(temporalcoordinatelist);

      for (size_t ix = 0, len = temporalcoordinatelist.size(); ix < len; ++ix) {
         quadraticerrorsum += sqrt(
               pow(temporalcoordinatelist[ix].x_ - vGcp_[ix].GetDestination().x_, 2)
                     + pow(temporalcoordinatelist[ix].y_
                                 - vGcp_[ix].GetDestination().y_, 2));
      }
      return (quadraticerrorsum / (double) vGcp_.size());
   }
   return 0.0;
}

/**
 * Realiza una transformacion polinomica de las coordenadas en el formato de
 * origen al formato de salida utilizando como salida un objeto Coordinates.
 *
 * @param[out] CoordinatesP Coordenadas transformadas.
 * @param[in] Inverse Indica si se debe hacer la transformacion inversa.
 * @return Cantidad de coordenadas transformadas.
 */
int PolynomialCoordinatesTransformation::Transform(Coordinates &CoordinatesP,
                                                   bool Inverse) const {
   if (IsOk()) {
      std::vector<Coordinates> auxvector;
      auxvector.push_back(CoordinatesP);
      if (Transform(auxvector, Inverse)) {
         CoordinatesP = auxvector[0];
         return 1;
      }
   }
   return 0;
}

/**
 * Realiza una transformacion polinomica de las coordenadas en el formato de
 * origen al formato de salida utilizando como salida un vector de Coordinates.
 *
 * @param[out] CoordinatesP Vector de coordenadas transformadas.
 * @param[in] Inverse Indica si se debe hacer la transformacion inversa.
 * @return Cantidad de coordenadas transformadas.
 */
int PolynomialCoordinatesTransformation::Transform(std::vector<Coordinates> &CoordinatesVector,
                      bool Inverse) const {
   if (IsOk() &&  pCurrentTransformationDirectArgument_ != NULL
         && pCurrentTransformationInverseArgument_ != NULL) {
      std::vector<Coordinates> origin(CoordinatesVector);
      CoordinatesVector.clear();
      
      for (size_t index = 0, len = origin.size(); index < len; ++index) {
#ifdef __UNUSED_CODE__
         double temporalcoordinatex = 0.0;
         double temporalcoordinatey = 0.0;
         int currentcoefficientindex = 0;

         for (int ix = 0; ix <= polynomialOrder_; ++ix) {
            for (int jx = 0; jx <= ix; ++jx) {
               temporalcoordinatex += currentCoefficients_[currentcoefficientindex].x_
                     * pow(origin[index].x_, ix - jx) * pow(origin[index].y_, jx);
               temporalcoordinatey += currentCoefficients_[currentcoefficientindex].y_
                     * pow(origin[index].x_, ix - jx) * pow(origin[index].y_, jx);
               ++currentcoefficientindex;
            }
         }
         
         CoordinatesP.push_back(Coordinates(temporalcoordinatex, temporalcoordinatey));
#endif
         double x = origin.at(index).x_, y =
               origin.at(index).y_, z = origin.at(index).z_;
         int result = 0;
         int desttosrc = !Inverse ? TRUE : FALSE;
         if (!Inverse) {
            GDALGCPTransform(pCurrentTransformationDirectArgument_,
                             desttosrc, 1, &x, &y, &z, &result);
         } else {
            GDALGCPTransform(pCurrentTransformationInverseArgument_,
                             desttosrc, 1, &x, &y, &z, &result);
         }
         if (result != 0) {
            Coordinates transfcoord(x, y, z);
            CoordinatesVector.push_back(transfcoord);
         }
      }
      return CoordinatesVector.size();
   }
   return 0;
}

/**
 * Realiza una transformacion polinomica de las coordenadas en el formato de
 * origen al formato de salida utilizando como salida un Subset.
 *
 * @param[out] SubsetP Subset transformado.
 * @param[in] CalculationPoints Limite de coordenadas a calcular para el subset.
 * @param[in] Inverse Indica si se debe hacer la transformacion inversa.
 * @return Cantidad de coordenadas transformadas.
 */
int PolynomialCoordinatesTransformation::Transform(Subset &SubsetP,
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
 */
bool PolynomialCoordinatesTransformation::IsIdentity() const {
   return isIdentity_;
}

/**
 * Verifica si se pudo determinar el polinomio de transformacion.
 *
 * @return true si se puede determinar.
 */
bool PolynomialCoordinatesTransformation::IsOk() const {
   // Indica el numero minimo de GCP y coefs
   unsigned int minimumnumber = static_cast<unsigned int>(((polynomialOrder_ + 1)
         * (polynomialOrder_ + 2)) / 2);

   if (calculatedCoefficients_) {
      return (vGcp_.size() >= minimumnumber)
            && currentCoefficients_.size() == minimumnumber;
   } else {
      return (vGcp_.size() >= minimumnumber);
   }
}

/**
 * Calcula los coeficientes actuales de los polinomios de interpolacion
 * x' e y', por medio del metodo de minimos cuadrados, a partir de los GCPs
 * cargados previamente.
 *
 * @param[in] NewPolynomialOrder Orden del polinomio a ser calculado.
 * @return bool indicando el exito del calculo de coeficientes.
 */
bool PolynomialCoordinatesTransformation::CalculateCoefficients() {
   if (IsOk()) {
      currentCoefficients_.clear();
      GDAL_GCP* pgdalgcps = new GDAL_GCP[vGcp_.size()];
      std::string id = "";
      std::string info = "";
      for (size_t ix = 0, len = vGcp_.size(); ix < len; ++ix) {
         pgdalgcps[ix].pszId = const_cast<char*>(id.c_str());
         pgdalgcps[ix].pszInfo = const_cast<char*>(info.c_str());
         pgdalgcps[ix].dfGCPPixel = vGcp_[ix].GetDestination().x_;
         pgdalgcps[ix].dfGCPLine = vGcp_[ix].GetDestination().y_;
         pgdalgcps[ix].dfGCPX = vGcp_[ix].GetSource().x_;
         pgdalgcps[ix].dfGCPY = vGcp_[ix].GetSource().y_;
         pgdalgcps[ix].dfGCPZ = 0.0;
      }

#ifdef __UNUSED_CODE__
      GCPTransformInfo* gdalpolynomresult =
            reinterpret_cast<GCPTransformInfo*>(GDALCreateGCPTransformer(vGcp_.size(),
                                                                         gdalgcps,
                                                                        polynomialOrder_,
                                                                        0));
      delete[] gdalgcps;


      if (gdalpolynomresult == NULL) {
         return false;
      } else {
         int count = ((polynomialOrder_ + 1) * (polynomialOrder_ + 2)) / 2;
         for (int ix = 0; ix < count; ++ix) {
            currentCoefficients_.push_back(
                  PolynomialCoefficients(gdalpolynomresult->adfToGeoX[ix],
                                  gdalpolynomresult->adfToGeoY[ix]));
         }
         calculatedCoefficients_ = true;
      }
#endif
      if (pCurrentTransformationDirectArgument_)
            GDALDestroyGCPTransformer(pCurrentTransformationDirectArgument_);

      pCurrentTransformationDirectArgument_ = GDALCreateGCPTransformer(vGcp_.size(),
                                                                 pgdalgcps,
                                                                 polynomialOrder_,
                                                                 0);
      if (pCurrentTransformationInverseArgument_)
            GDALDestroyGCPTransformer(pCurrentTransformationInverseArgument_);

      pCurrentTransformationInverseArgument_ = GDALCreateGCPTransformer(vGcp_.size(),
                                                                 pgdalgcps,
                                                                 polynomialOrder_,
                                                                 1);
   } else {
      return false;
   }
   return true;
}
/** Compara si dos transformaciones son iguales **/
bool PolynomialCoordinatesTransformation::Equals(
      CoordinatesTransformation* pTransform) const {
   return false;
}
}  // namespace suri
