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
