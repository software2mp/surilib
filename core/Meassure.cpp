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

#include "Meassure.h"

// Includes standard

// Includes Suri
#include "suri/messages.h"

// Includes Wx

// Includes App

/** namespace suri */
namespace suri {
const char *Meassure::pUnitStrings_[] = { name_METER_UNIT, name_KILOMETER_UNIT,
                                          name_HECTARE_UNIT };
const char *Meassure::pAreaSuffix_[] = { label_SQUARE, label_SQUARE, "" };
const double Meassure::unitMultiplier_[] = { 1.0, 0.001, 0.01 };

/**
 * Obtiene la medicion.
 * @return string con la medicion, o un string con invalid_MEASSURE
 */
wxString Meassure::GetMeassurement() const {
   if (area_ >= 0) {
      return wxString::Format(
            format_MEASSURE_DISPLAY_fss,
            area_
                  * (unitMultiplier_[GetUnit()]
                        * (isArea_ ? unitMultiplier_[GetUnit()] : 1.0)),
            pUnitStrings_[GetUnit()], isArea_ ? _(pAreaSuffix_[GetUnit()]) : "");
   } else {
      return invalid_MEASSURE;
   }
}
}
