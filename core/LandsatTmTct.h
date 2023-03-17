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

#ifndef LANDSATTMTCT_H_
#define LANDSATTMTCT_H_

#include <map>
#include <string>

#include "TasseledCapTransformation.h"
#include"suri/ParameterCollection.h"

#define LANDSATTMTCT_REQUIRED_INPUT_BANDS    6

namespace suri {
namespace raster {
namespace transformation {

/**
 * Representa una trasformacion para LandSat-4 TM (ND).
 */
class LandsatTmTct : public TasseledCapTransformation {
public:
   /**
    * Ctor.
    */
   explicit LandsatTmTct(const std::vector<int>& OutBands);

   /**
    * Dtor.
    */
   virtual ~LandsatTmTct();

   /**
    * Metodo para transformar los datos de entrada.
    */
   virtual bool Transform(const std::string& InputDataType, std::vector<void*>& InputData,
                          int InputDataLength, std::vector<void*>& OutputData);

   /**
    * Devuelve la cantidad de bandas requeridas en la entrada.
    */
   virtual int GetRequiredInputBands();

   /**
    * Crea una instancia de la transformacion tasseled-cap usando
    * coeficientes de LandSat-4 TM (ND).
    */
   static TasseledCapTransformation* Create(suri::ParameterCollection& Params);

   /**
    * Obtiene las bandas de salida con la descripcion para LandSat-4 TM (ND).
    */
   static std::map<int, std::string> GetOutputBands();
};

}  // namespace transformation
}  // namespace raster
}  // namespace suri

#endif  // LANDSATTMTCT_H_
