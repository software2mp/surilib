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

#include <limits>

#include "NoDataValue.h"

namespace suri {

/**
 * Dtor.
 */
NoDataValue::~NoDataValue() {

}

/**
 * Establece el valor a utilizar para el area que no contiene datos.
 */
void NoDataValue::SetNoDataValue(double NoDataValue) {
   noDataValue_ = NoDataValue;
}

/**
 * Devuelve el valor a utilizar para el area que no contiene datos.
 */
double NoDataValue::GetNoDataValue() const {
   return noDataValue_;
}

/**
 * Establece el valor a utilizar para el area que no contiene datos para la
 * banda especificada.
 */
void NoDataValue::SetBandNdv(int Band, double Value) {
   bandsNdv_[Band] = Value;
}

/**
 * Devuelve el valor a utilizar para el area que no contiene datos para la
 * banda especificada.
 */
double NoDataValue::GetBandNdv(int Band) const {
   std::map<int, double>::const_iterator it = bandsNdv_.find(Band);

   if (it == bandsNdv_.end())
      return -std::numeric_limits<double>::max();

   return it->second;
}

/**
 * Indica si el valor para el area que no contiene datos fue establecido.
 */
bool NoDataValue::IsNoDataValueAvailable() const {
   return available_;
}

/**
 * Establece que el valor para el area que no contiene datos fue especificado.
 */
void NoDataValue::SetNoDataValueAvailable(bool Available) {
   available_ = Available;
}

/**
 * Indica si el valor para el area que no contiene datos fue establecido
 * para la banda especificada.
 */
bool NoDataValue::IsNdvAvailableForBand(int Band) const {
   std::map<int, double>::const_iterator it = bandsNdv_.find(Band);
   return it != bandsNdv_.end();
}

/**
 * Devuelve el mapa con todas las bandas y sus valores
 * no validos definidos.
 */
void NoDataValue::SetAllBandsNdv(const std::map<int, double>& BandsNdv) {
   bandsNdv_ = BandsNdv;
}

/**
 * Devuelve el mapa con todas las bandas y sus valores
 * no validos definidos.
 */
std::map<int, double> NoDataValue::GetAllBandsNdv() const {
   return bandsNdv_;
}

}  // namespace suri
