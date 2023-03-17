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

// Includes Suri
#include "LatitudeLongitudeInputPart.h"

/** namespace suri */
namespace suri {
/** Ctor */
LatitudeLongitudeInputPart::LatitudeLongitudeInputPart() :
      Part(pCoordInput = new LatitudeLongitudeInputWidget()) {
}

/** Dtor */
LatitudeLongitudeInputPart::~LatitudeLongitudeInputPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool LatitudeLongitudeInputPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool LatitudeLongitudeInputPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool LatitudeLongitudeInputPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void LatitudeLongitudeInputPart::SetInitialValues() {
}

/** retorna la coordenada que ingresaron, informa si es valida */
bool LatitudeLongitudeInputPart::SetCoordinate(Coordinates LatLongCoord) {
   if (!pCoordInput->SetCoordinate(LatLongCoord)) {
      return false;
   }
   return true;
}

/** Actualiza el estado de la parte */
void LatitudeLongitudeInputPart::Update() {
}
}
