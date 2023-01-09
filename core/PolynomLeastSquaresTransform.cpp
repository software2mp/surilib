// <COPYRIGHT>

// Includes standard
#include <string>
// Includes Suri
#include "suri/PolynomLeastSquaresTransform.h"
// Includes Wx

// Includes App

// Includes Gdal
#include <gdal.h>
#include <gdal_alg.h>
// Defines

// Forwards

/** namespace suri */
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

CoefficientPair::CoefficientPair(const double &X, const double &Y) :
      x_(X), y_(Y) {
}
CoefficientPair::~CoefficientPair() {
}

/**
 * Constructor PolynomLeastSquaresTransform
 */
PolynomLeastSquaresTransform::PolynomLeastSquaresTransform() {
   calculatedCoefficients_ = false;
}

/**
 * Destructor
 */
PolynomLeastSquaresTransform::~PolynomLeastSquaresTransform() {
}

/**
 *Permite determinar si se pudo determinar el polinomio de transformacion.
 *@return bool indicando el exito de la determinacion del pol de aproximacion.
 */
bool PolynomLeastSquaresTransform::IsOk() {
   unsigned int minimumnumber = static_cast<unsigned int>(((polynomialOrder_ + 1)
         * (polynomialOrder_ + 2)) / 2); /* Indica el numero minimo de GCP y */
   // coefs
   if (calculatedCoefficients_) {
      return (currentGcps_.origin_.size() >= minimumnumber)
            && currentCoefficients_.size() == minimumnumber;
   } else {
      return (currentGcps_.origin_.size() >= minimumnumber);
   }
}

/**
 *Permite limpiar los coeficientes de la funcion de transformacion actual
 */
void PolynomLeastSquaresTransform::Clear() {
   currentCoefficients_.clear();
   currentGcps_.origin_.clear();
   currentGcps_.destination_.clear();
   polynomialOrder_ = 0;
   calculatedCoefficients_ = false;
}

/**
 * Permite cargar los GCPs para el calculo posterior de la funcion de
 * aproximacion.
 * @return bool indicando el exito de la carga.
 * @param[in] GcpListInput referencia al la !GCPList a ser seteada
 * como
 * la lista de puntos de control del polinomio.
 */

bool PolynomLeastSquaresTransform::LoadGcps(const GCPList &GcpListInput) {
   currentGcps_ = GcpListInput;
   calculatedCoefficients_ = false;
   return true;
}

/**
 * Permite calcular los coeficientes actuales de los polinomios de
 * interpolacion x' e y', por medio del metodo de minimos cuadrados, a
 * partir de
 *  los GCPs cargados previamente.
 * @return bool indicando el exito del calculo de coeficientes.
 * @param[in] NewPolynomialOrder Orden del polinomio a ser calculado
 */
bool PolynomLeastSquaresTransform::CalculateCoefficients(int NewPolynomialOrder) {
   polynomialOrder_ = NewPolynomialOrder;
   if (IsOk()) {
      currentCoefficients_.clear();
      GDAL_GCP gdalgcps[currentGcps_.origin_.size()];
      std::string id = "";
      std::string info = "";
      for (unsigned int i = 0; i < currentGcps_.origin_.size(); i++) {
         gdalgcps[i].pszId = const_cast<char*>(id.c_str());
         gdalgcps[i].pszInfo = const_cast<char*>(info.c_str());
         gdalgcps[i].dfGCPPixel = currentGcps_.origin_[i].x_;
         gdalgcps[i].dfGCPLine = currentGcps_.origin_[i].y_;
         gdalgcps[i].dfGCPX = currentGcps_.destination_[i].x_;
         gdalgcps[i].dfGCPY = currentGcps_.destination_[i].y_;
         gdalgcps[i].dfGCPZ = 0.0;
      }
      GCPTransformInfo *gdalpolynomresult =
            (GCPTransformInfo*) GDALCreateGCPTransformer(currentGcps_.origin_.size(),
                                                         gdalgcps, NewPolynomialOrder,
                                                         0);
      if (gdalpolynomresult == NULL) {
         return false;
      } else {
         for (int i = 0; i < (((polynomialOrder_ + 1) * (polynomialOrder_ + 2)) / 2);
               i++) {
            currentCoefficients_.push_back(
                  CoefficientPair(gdalpolynomresult->adfToGeoX[i],
                                  gdalpolynomresult->adfToGeoY[i]));
         }
         calculatedCoefficients_ = true;
      }
   }
   return true;
}

/**
 * Permite calcular el RMSE (Root Mean Squared Error) para los GCPS y el
 * polinomio actual.
 * @return double Rmse
 */
double PolynomLeastSquaresTransform::GetRmse() {
   if (IsOk()) {
      CoordinateListType temporalcoordinatelist;
      temporalcoordinatelist.resize(currentGcps_.origin_.size());
      double quadraticerrorsum = 0.0;
      ApplyTransform(currentGcps_.origin_, temporalcoordinatelist);
      for (unsigned int i = 0; i < currentCoefficients_.size(); ++i) {
         quadraticerrorsum += sqrt(
               pow(temporalcoordinatelist[i].x_ - currentGcps_.destination_[i].x_, 2)
                     + pow(temporalcoordinatelist[i].y_
                                 - currentGcps_.destination_[i].y_,
                           2));
      }
      return (quadraticerrorsum / (double) currentGcps_.origin_.size());
   } else {
      return 0.0;
   }
}

/**
 * Permite obtener los coeficientes actuales de los polinomios de interpolacion
 * x' e y', representado como una lista de coordenadas.
 * @return bool indicando el exito de la copia de coeficientes.
 * @param[out] ActualCoefficients: Coeficientes resultantes de la aproximacion
 * polinomial.
 */

bool PolynomLeastSquaresTransform::GetCoefficients(
      XYCoefficientListType &ActualCoefficients) {
   ActualCoefficients = currentCoefficients_;
   return true;
}

/**
 * Permite setear directamente los coeficientes actuales de los polinomios de
 * interpolacion x' e y'
 * @return bool indicando el exito de la copia de coeficientes
 * @param[in] NewCoefficients:  Coeficientes de la aproximacion
 * polinomial a ser seteados.
 * @param[out] NewPolynomialOrder nuevo orden del polinomio
 */

bool PolynomLeastSquaresTransform::SetCoefficients(
      const XYCoefficientListType &NewCoefficients, int NewPolynomialOrder) {
   Clear();
   currentCoefficients_ = NewCoefficients;
   polynomialOrder_ = NewPolynomialOrder;
   calculatedCoefficients_ = false;
   return true;
}

/**
 * Mapea la lista de coordenadas de entrada en la de salida, aplicando el
 * polinomio de transformacion calculado previamente
 * @return bool indicando el exito de la aplicacion del polinomio de
 * transformacion.
 * @param[in] Origin: Lista de cordenadas origen.
 * @param[out] Destination: Lista de coordenadas a aproximar por la aplicacion
 * del polinomio.
 */

bool PolynomLeastSquaresTransform::ApplyTransform(const CoordinateListType &Origin,
                                                  CoordinateListType &Destination) {
   if (IsOk()) {
      Destination.clear();
      double temporalcoordinatex;
      double temporalcoordinatey;
      int currentcoefficientindex;

      for (unsigned int index = 0; index < Origin.size(); index++) {
         temporalcoordinatex = 0.0;
         temporalcoordinatey = 0.0;
         currentcoefficientindex = 0;
         for (int i = 0; i <= polynomialOrder_; ++i) {
            for (int j = 0; j <= i; ++j) {
               temporalcoordinatex += currentCoefficients_[currentcoefficientindex].x_
                     * pow(Origin[index].x_, i - j) * pow(Origin[index].y_, j);
               temporalcoordinatey += currentCoefficients_[currentcoefficientindex].y_
                     * pow(Origin[index].x_, i - j) * pow(Origin[index].y_, j);
               ++currentcoefficientindex;
            }
         }
         Destination[index].x_ = temporalcoordinatex;
         Destination[index].y_ = temporalcoordinatey;
      }
      return true;
   } else {
      return false;
   }
}

/**
 * Mapea la coordenada de entrada a la coordenada de salida, aplicando el
 * polinomio de transformacion calculado previamente
 * @return bool indicando el exito de la aplicacion del polinomio de
 * aproximacion
 * @param[in] Origin: Coordenada de origen
 * @param[out] Destination: Coordenada de destino
 */

bool PolynomLeastSquaresTransform::ApplyTransform(const Coordinates &Origin,
                                                  Coordinates &Destination) {
   if (!IsOk()) {
      return false;
   } else {
      CoordinateListType temporalorigin;
      CoordinateListType temporaldestination;
      temporalorigin.push_back(Origin);
      if (ApplyTransform(temporalorigin, temporaldestination)) {
         Destination = temporaldestination[0];
         return true;
      }
      return false;
   }
}
}  // namespace suri

