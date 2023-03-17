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

#ifndef SPECTRALSIGNITEMATTRIBUTE_H_
#define SPECTRALSIGNITEMATTRIBUTE_H_

// Includes Estandar
// Includes Suri
#include "ComplexItemAttribute.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *  Atributo para item de biblioteca de tabla de intervalos.
 *  Representa una firma espectral que viene definida por longitud de onda,
 *  amplitud y reflectancia.
 */
class SpectralSignItemAttribute : public suri::ComplexItemAttribute {
public:
   /** Constructor */
   explicit SpectralSignItemAttribute(const std::string& Name, const std::string& Value);
   /** Destructor */
   virtual ~SpectralSignItemAttribute();
   /** Enum con la definicion de posiciones del valores de atributos dentro del rango **/
   typedef enum {
      WavelengthValuePosition = 0,
      AmplitudeValuePosition = 1,
      RelevanceValuePosition = 2,
      WavelengthTypePosition = 3,
      MagnitudeTypePosition = 4
   } AtributeValuesPosition;
   /** Configura el valor de longitud de onda */
   void SetWavelengthValue(const double Value);
   /** Configura el valor de amplitud */
   void SetAmplitudeValue(const double Value);
   /** Configura el valor de relevancia */
   void SetReflectanceValue(const double Value);
   /** Configura el tipo de longitud de onda */
   void SetWavelengthType(const std::string Value);
   /** Configura el tipo de magnitud */
   void SetMagnitudeType(const std::string Value);
   /** Retorna el valor de longitud de onda */
   double GetWavelengthValue() const;
   /** Retorna el valor de amplitud */
   double GetAmplitudeValue() const;
   /** Retorna el valor de relevancia */
   double GetReflectanceValue() const;
   /** Retorna el tipo de longitud de onda */
   std::string GetWavelengthType() const;
   /** Retorna el tipo de magnitud */
   std::string GetMagnitudeType() const;
};

} /** namespace suri */

#endif /* SPECTRALSIGNITEMATTRIBUTE_H_ */
