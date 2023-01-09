/** \file Geometry.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes suri
#include "Geometry.h"
#include "suri/VectorStyle.h"

namespace suri {


/** Ctor */
Geometry::Geometry(const Geometry &AGeometry) : pStyle_(NULL) {
   AssignSpatialReference(AGeometry.GetSpatialReference());
   SetStyle(AGeometry.GetStyle());
}

/** Ctor */
Geometry::Geometry(const std::string &SpatialReference) : pStyle_(NULL) {
   AssignSpatialReference(SpatialReference);
}

/** Dtor */
Geometry::~Geometry() {
   VectorStyle::Destroy(pStyle_);
}

/**
 * Indica si la geometria esta cerrada, solo valida si es poligono
 * @return bool que indica si la geometria esta cerrada
 */
bool Geometry::IsClosed() const {
   return false;
}
/**
 * Fuerza a que se cierre la geometria. Solo para poligonos
 * @return bool que indica si pudo cerrar los anillos.
 */
bool Geometry::CloseRings() {
   return false;
}

/**
 * Retorna el estilo configurado a la geometria
 * @return estilo de la geometria
 */
VectorStyle* Geometry::GetStyle() const {
   return pStyle_;
}
/**
 * Configura el estilo en la geometria
 * @param[in] pStyle Nuevo estilo de la geometria
 * \attention El estilo pasa a ser responsabilidad de esta clase
 */
void Geometry::SetStyle(VectorStyle *pStyle) {
   if (pStyle_)
      VectorStyle::Destroy(pStyle_);
   pStyle_ = pStyle;
}

/** Retorna el srs en el que estan los puntos de la geometria */
std::string Geometry::GetSpatialReference() const {
   return spatialReference_;
}

/** Retorna el srs en el que estan los puntos de la geometria */
void Geometry::AssignSpatialReference(const std::string &SpatialReference) {
   spatialReference_ = SpatialReference;
}

} /* namespace suri */
