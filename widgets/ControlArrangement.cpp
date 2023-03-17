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

#include "suri/ControlArrangement.h"

// Includes standard

// Includes Suri

// Includes Wx

// Defines

// Forwards

/** namespace suri */
namespace suri {
/**
 *  Guarda los valores de los parametros que se utilizaran al agregar un control
 * en la clase Widget mediante el metodo AddControl.
 *
 * @param[in] Proportion Proporcion del espacio disponible en la ventana
 * @param[in] Position Posicion dentro del sizer.
 * @param[in] Flags Flags de wx para las opciones del sizer.
 * @param[in] Border Borde en pixels.
 * @param[in] CreateMiniFrame Flag para indicar que se desea que la ventana a
 *  agregar posea un Mini-Frame (solo para el caso en que no haya sido creada
 *  todavia con una llamaga a GetWindow() o CreateTool()).
 */
ControlArrangement::ControlArrangement(int Proportion, int Position, int Flags,
                                       int Border, bool CreateMiniFrame) :
      proportion_(Proportion), position_(Position), flags_(Flags), border_(Border),
      createMiniFrame_(CreateMiniFrame) {
}

ControlArrangement::~ControlArrangement() {
}

// -------------------------------- Getters --------------------------------
/** getter para proportion_ */
int ControlArrangement::GetProportion() const {
   return proportion_;
}

/** getter para postion_ */
int ControlArrangement::GetPosition() const {
   return position_;
}

/** getter para flags_ */
int ControlArrangement::GetFlags() const {
   return flags_;
}

/** getter para border */
int ControlArrangement::GetBorder() const {
   return border_;
}

/** getter para createMiniFrame_ */
bool ControlArrangement::GetMiniFrame() const {
   return createMiniFrame_;
}
} /* namespace suri */
