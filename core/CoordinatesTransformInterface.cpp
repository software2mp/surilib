/*! \file CoordinatesTransformInterface.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/CoordinatesTransformInterface.h"
#include "suri/CoordinatesTransform.h"

// Includes Wx
// Defines
// forwards
namespace suri {
CoordinatesTransformInterface::CoordinatesTransformInterface() {
}

CoordinatesTransformInterface::~CoordinatesTransformInterface() {
}

/** Crea el objeto a partir del WKT */
/**
 *  Para transformar entre un PCS y su respectivo GCS (forma 1)
 *  @param[in] SRWkt WKT del sistema de coordenadas
 *  @return objeto CoordinatesTransform creado
 */
CoordinatesTransformInterface* CoordinatesTransformInterface::Create(
      const std::string& TransformationType, const std::string &SRWkt) {
   return CoordinatesTransform::Create(SRWkt);
}

/** Crea el objeto a partir de los WKTs */
/**
 * Para transformar entre un PCS/GCS y otro PCS/GCS (forma 2)
 * @param[in] SRWktIn WKT del sistema de coordenadas de entrada
 * @param[in] SRWktOut WKT del sistema de coordenadas de salida
 * @return objeto CoordinatesTransform creado
 */
CoordinatesTransformInterface* CoordinatesTransformInterface::Create(
      const std::string& TransformationType, const std::string &SRWktIn,
      const std::string &SRWktOut) {
   return CoordinatesTransform::Create(SRWktIn, SRWktOut);
}

/** Destruye el objeto */
void CoordinatesTransformInterface::Destroy(
      CoordinatesTransformInterface * &pCTransform) {
   if (pCTransform != NULL) {
      delete pCTransform;
      pCTransform = NULL;
   }
}

}  // namespace suri
