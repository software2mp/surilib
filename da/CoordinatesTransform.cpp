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

// Includes standard
#include <math.h>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <cassert>
#include <limits>
#include <vector>
#include <string>

// Includes otros
#include <ogr_spatialref.h>

// Includes suri
#include "suri/CoordinatesTransform.h"
#include "SpatialReference.h"
#include "suri/Wkt.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] SRWktIn wkt de origen
 * @param[in] SRWktOut wkt de salida
 * \note por defecto construye la transformacion identidad
 */
CoordinatesTransform::CoordinatesTransform(const std::string &SRWktIn,
                                           const std::string &SRWktOut) {
   identityTransform_ = true;
   sRWktIn_ = SRWktIn;
   sRWktOut_ = SRWktOut;
   pTransform_ = NULL;
   pITransform_ = NULL;
}

/** Destructor */
CoordinatesTransform::~CoordinatesTransform() {
   if (!identityTransform_) {
      OCTDestroyCoordinateTransformation(pTransform_);
      OCTDestroyCoordinateTransformation(pITransform_);
   }
}

// --------------------- METODOS ESTATICOS DE CREACION/DESTRUCCION ----------- //

/**
 * Destruye el objeto
 * @param[in] pCTransform transformacion de coordenadas a destruir.
 */
void CoordinatesTransform::Destroy(CoordinatesTransform * &pCTransform) {
   if (pCTransform != NULL) {
      delete pCTransform;
      pCTransform = NULL;
   }
}

/**
 * Crea el objeto a partir de los WKTs
 *  Para transformar entre un PCS/GCS y otro PCS/GCS (forma 2)
 * @param[in] SRWktIn WKT del sistema de coordenadas de entrada
 * @param[in] SRWktOut WKT del sistema de coordenadas de salida
 * @return puntero a objeto modelo.
 * @return NULL si fallo.
 */
CoordinatesTransform * CoordinatesTransform::Create(const std::string &SRWktIn,
                                                    const std::string &SRWktOut) {
   // TODO(Gabriel - TCK #2344): Sacar cuando SR Raster tenga un Wkt Valido
   if (SpatialReference::IsPixelLineSpatialRef(SRWktIn)
         && SpatialReference::IsPixelLineSpatialRef(SRWktOut)) {
      return new CoordinatesTransform(SRWktIn, SRWktOut);
   }
   // Verifica los WKTs de entrada
   if (!Wkt::IsValid(SRWktIn) || !Wkt::IsValid(SRWktOut)) {
      return NULL;
   }

   // Verifica si los WKTs son los mismos y en caso positivo devuelve la identidad
   if (SRWktIn == SRWktOut) {
      return new CoordinatesTransform(SRWktIn, SRWktOut);
   }

   // Crea las referencias espaciales OGR
   OGRSpatialReference SRIn(SRWktIn.c_str());
   OGRSpatialReference SROut(SRWktOut.c_str());
   if (SRIn.Validate() == OGRERR_CORRUPT_DATA || SROut.Validate() == OGRERR_CORRUPT_DATA) {
      return NULL;
   }

   // Si OGR informa que los sistemas de referencia son iguales retorna identidad.
   if (SRIn.IsSame(&SROut)) {
      return new CoordinatesTransform(SRWktIn, SRWktOut);
   }

   // Crea la transformacion directa
   OGRCoordinateTransformation * ptransform = OGRCreateCoordinateTransformation(&SRIn,
                                                                                &SROut);
   if (ptransform == NULL) {
      return NULL;
   }

   // Crea la transformacion inversa
   OGRCoordinateTransformation * pinversetransform = OGRCreateCoordinateTransformation(
         &SROut, &SRIn);
   if (pinversetransform == NULL) {
      OCTDestroyCoordinateTransformation(ptransform);
      return NULL;
   }

   // Inicializa el objeto CoordinatesTransform con la transformacion identidad
   CoordinatesTransform * pcoordtransform = new CoordinatesTransform(SRWktIn, SRWktOut);
   pcoordtransform->identityTransform_ = false;
   pcoordtransform->pTransform_ = ptransform;
   pcoordtransform->pITransform_ = pinversetransform;

   return pcoordtransform;
}

/**
 * Crea el objeto a partir del WKT
 * Para transformar entre un PCS y su respectivo GCS (forma 1)
 * @param[in] SRWkt WKT del sistema de coordenadas de entrada de tipo PCS
 * @return puntero a objeto modelo.
 * @return NULL si fallo.
 */
CoordinatesTransform * CoordinatesTransform::Create(const std::string &SRWkt) {
   // Verifica el WKT de entrada
   if (!Wkt::IsValid(SRWkt)) {
      return NULL;
   }

   // Crea el WKT de salida (geografico)
   Wkt* psrinwkt = Wkt::Create(SRWkt);
   std::string geogswkt;
   if (psrinwkt->GetRootValue() != "PROJCS") {
#ifdef __DONT_ALLOW_GEOCS_IDENTITY__
      Wkt::Destroy(srinwkt);
      return NULL;
#else
      geogswkt = psrinwkt->GetWktString("GEOGCS");
#endif
   } else {
      geogswkt = psrinwkt->GetWktString("PROJCS|GEOGCS");
   }
   Wkt::Destroy(psrinwkt);
   return Create(SRWkt, geogswkt);
}

// ------------------- METODOS DE CONSULTA DE CONFIGURACION -----------------

/**
 * Devuelve el WKT del sistema de coordenadas de entrada
 * @return WKT del sistema de coordenadas de entrada
 */
std::string CoordinatesTransform::GetWktIn() const {
   return sRWktIn_;
}

/**
 * Devuelve el WKT del sistema de coordenadas de salida
 * @return WKT del sistema de coordenadas de salida
 */
std::string CoordinatesTransform::GetWktOut() const {
   return sRWktOut_;
}

/**
 * Indica si es la transformacon identidad
 * @return true si es la transformacion identidad.
 * @return false en caso contrario.
 */
bool CoordinatesTransform::IsIdentity() const {
   return identityTransform_;
}

/**
 * Devuelve la transformacion OGR directa o inversa.
 * @param[in] Inverse indica que debe devolver el metodo:
 * @return Transformacion inversa si Inverse == true
 * @return Transformacion si Inverse == false
 */
OGRCoordinateTransformation * CoordinatesTransform::GetOGRCT(bool Inverse) const {
   if (Inverse) {
      return pITransform_;
   } else {
      return pTransform_;
   }
}

// -------------------- METODOS DE TRANSFORMACION ---------------------------


/** Transforma de X-Y(in) a X-Y(out) */
int CoordinatesTransform::TransformCoordinates(Coordinates &InOut,
                                                  const std::string &SRWktIn,
                                                  const std::string &SRWktOut,
                                                  bool Inverse) {
   CoordinatesTransform *pct = NULL;
   if (SRWktOut.empty()) {
      pct = CoordinatesTransform::Create(SRWktIn);
   } else {
      pct = CoordinatesTransform::Create(SRWktIn, SRWktOut);
   }
   if (!pct)
      return -1;
   int points = pct->TransformCoordinates(InOut, Inverse);
   CoordinatesTransform::Destroy(pct);
   return points;
}

/** Transforma de X-Y(in) a X-Y(out) */
/**
 * @param[in] InOut Coordenadas en el sistema de referencia de entrada
 * @param[out] InOut Coordenadas en el sistema de referencia de salida
 * @param[in] Inverse Indica si se calcula la transformada inversa o directa
 * @return 1 en caso de exito
 * @return 0 en caso contrario
 * \todo Hacer metodos para 3D
 */
int CoordinatesTransform::TransformCoordinates(Coordinates &InOut,
                                                  bool Inverse) const {
   if (IsIdentity()) {
      return 1;
   }
   if (Inverse) {
      return pITransform_->Transform(1, &(InOut.x_), &(InOut.y_));
   } else {
      return pTransform_->Transform(1, &(InOut.x_), &(InOut.y_));
   }
}

/** Transforma de X-Y(in) a X-Y(out) */
int CoordinatesTransform::TransformCoordinates(std::vector<Coordinates> &InOut,
                                                  const std::string &SRWktIn,
                                                  const std::string &SRWktOut,
                                                  bool Inverse) {
   CoordinatesTransform *pct = NULL;
   if (SRWktOut.empty()) {
      pct = CoordinatesTransform::Create(SRWktIn);
   } else {
      pct = CoordinatesTransform::Create(SRWktIn, SRWktOut);
   }
   if (!pct)
      return -1;
   int points = pct->TransformCoordinates(InOut, Inverse);
   CoordinatesTransform::Destroy(pct);
   return points;
}

/** Transforma de X-Y(in) a X-Y(out) */
/**
 * @param[in] InOut Vector de coordenadas en el sistema de referencia de entrada
 * @param[out] InOut Vector de coordenadas en el sistema de referencia de salida
 * @param[in] Inverse Indica si se calcula la transformada inversa o directa
 * @return cantidad de puntos del vector que pudo transformar
 * \todo Hacer metodos para 3D
 */
int CoordinatesTransform::TransformCoordinates(std::vector<Coordinates> &InOut,
                                               bool Inverse) const {
   size_t totalcoord = InOut.size();
   if (IsIdentity()) {
      return totalcoord;
   }
   int result = 0;
   for (size_t i = 0; i < totalcoord; i++)
      if (TransformCoordinates(InOut[i], Inverse) != 0) {
         ++result;
      }
   return result;
}


/** Transforma de Subset(in) a Subset(out) */
int CoordinatesTransform::TransformCoordinates(Subset &InOut,
                                      const std::string &SRWktIn,
                                      const std::string &SRWktOut,
                                      int CalculationPoints, bool Inverse) {
   CoordinatesTransform *pct = NULL;
   if (SRWktOut.empty()) {
      pct = CoordinatesTransform::Create(SRWktIn);
   } else {
      pct = CoordinatesTransform::Create(SRWktIn, SRWktOut);
   }
   if (!pct)
      return -1;
   int points = pct->TransformCoordinates(InOut, CalculationPoints, Inverse);
   CoordinatesTransform::Destroy(pct);
   return points;
}

/** Transforma de Subset(in) a Subset(out) */
/**
 * @param[in] InOut Subset en el sistema de referencia de entrada
 * @param[out] InOut Subset en el sistema de referencia de salida
 * @param[in] Inverse Indica si se calcula la transformada inversa o directa
 * @param[in] CalculationPoints cantidad de puntos a transformar
 * @return cantidad de puntos que pudo transformar
 * \todo Hacer metodos para 3D
 */
int CoordinatesTransform::TransformCoordinates(Subset &InOut,
                                                        int CalculationPoints,
                                                        bool Inverse) const {
   if (IsIdentity()) {
      return CalculationPoints * CalculationPoints;
   }

   if (CalculationPoints < 2) {
      return 0;
   }

   // Resultado
   int result = 0;
   // Pasos
   double dx = (InOut.lr_.x_ - InOut.ul_.x_) / (CalculationPoints - 1);
   double dy = (InOut.lr_.y_ - InOut.ul_.y_) / (CalculationPoints - 1);
   // Limites
   double minx = std::numeric_limits<double>::max();
   double miny = std::numeric_limits<double>::max();
   double maxx = -std::numeric_limits<double>::max();
   double maxy = -std::numeric_limits<double>::max();
   // Calcula los limites en el SR de salida
   for (int i = 0; i < CalculationPoints; i++) {
      for (int j = 0; j < CalculationPoints; j++) {
         Coordinates point(InOut.ul_.x_ + i * dx, InOut.ul_.y_ + j * dy);
         if (TransformCoordinates(point, Inverse) != 0) {
            if (point.x_ < minx) {
               minx = point.x_;
            }
            if (point.y_ < miny) {
               miny = point.y_;
            }
            if (point.x_ > maxx) {
               maxx = point.x_;
            }
            if (point.y_ > maxy) {
               maxy = point.y_;
            }
            ++result;
         }
      }
   }

   // Genera el subset de salida respetando los ejes originales
   if (result > 0) {
      if (dx < 0) {
         InOut.ul_.x_ = maxx;
         InOut.lr_.x_ = minx;
      } else {
         InOut.ul_.x_ = minx;
         InOut.lr_.x_ = maxx;
      }
      if (dy < 0) {
         InOut.ul_.y_ = maxy;
         InOut.lr_.y_ = miny;
      } else {
         InOut.ul_.y_ = miny;
         InOut.lr_.y_ = maxy;
      }
   }
   return result;
}
} /*namespace suri*/

