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

#ifndef GUIGEOMETRYCREATORINTERFACE_H_
#define GUIGEOMETRYCREATORINTERFACE_H_

// Includes suri
#include "suri/Coordinates.h"

namespace suri {

/**
 * Interfaz que brinda VectorEditorTask para crear geometrias
 */
class GuiGeometryCreatorInterface {
public:
   /** Dtor */
   virtual ~GuiGeometryCreatorInterface() {}

   /** Agrega un punto al final de la geometria. */
   virtual bool AppendPoint(const Coordinates &PointCoordinate) = 0;
   /** Elimina el ultimo punto que agregue a la geometria */
   virtual bool RemoveLastPoint() = 0;
   /** Mueve el punto fantasma. */
   virtual bool MovePhantomPoint(const Coordinates &NewCoordinate) = 0;
   /** Posiciona el punto fantasma sobre el ultimo punto agregado */
   virtual bool HidePhantomPoint() = 0;
};

} /* namespace suri */
#endif /* GUIGEOMETRYCREATORINTERFACE_H_ */
