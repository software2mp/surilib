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

#ifndef NODATAVALUE_H_
#define NODATAVALUE_H_

#include <map>
#include <vector>

namespace suri {

class NoDataValue {
public:
   /**
    * Dtor.
    */
   virtual ~NoDataValue();

   /**
    * Establece el valor a utilizar para el area que no contiene datos.
    */
   void SetNoDataValue(double NoDataValue);

   /**
    * Devuelve el valor a utilizar para el area que no contiene datos.
    */
   double GetNoDataValue() const;

   /**
    * Establece el valor a utilizar para el area que no contiene datos para la
    * banda especificada.
    */
   void SetBandNdv(int Band, double Value);

   /**
    * Devuelve el valor a utilizar para el area que no contiene datos para la
    * banda especificada.
    */
   double GetBandNdv(int Band) const;

   /**
    * Indica si el valor para el area que no contiene datos fue establecido.
    */
   bool IsNoDataValueAvailable() const;

   /**
    * Establece que el valor para el area que no contiene datos fue especificado.
    */
   void SetNoDataValueAvailable(bool Available);

   /**
    * Indica si el valor para el area que no contiene datos fue establecido
    * para la banda especificada.
    */
   bool IsNdvAvailableForBand(int Band) const;

   /**
    * Devuelve el mapa con todas las bandas y sus valores
    * no validos definidos.
    */
   void SetAllBandsNdv(const std::map<int, double>& BandsNdv);

   /**
    * Devuelve el mapa con todas las bandas y sus valores
    * no validos definidos.
    */
   std::map<int, double> GetAllBandsNdv() const;

protected:
   /**
    * Ctor.
    */
   NoDataValue() : noDataValue_(0.0), available_(false) { }

private:
   double noDataValue_;  // valor no valido.
   bool available_;  // valor no valido establecido.
   std::map<int, double> bandsNdv_;
};

} // namespace suri

#endif  // NODATAVALUE_H_
