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

#ifndef RANGEITEMATTRIBUTE_H_
#define RANGEITEMATTRIBUTE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "ComplexItemAttribute.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *  Atributo para item de biblioteca de tabla de intervalos.
 *  Representa un rango que viene definido por el nombre del rango,
 *  el valor de inicio y el valor de fin del mismo.
 */
class RangeItemAttribute : public suri::ComplexItemAttribute {
   /** ctor copia **/
   RangeItemAttribute(const RangeItemAttribute&);

public:
   /** constructor **/
   explicit RangeItemAttribute(const std::string& Name, const std::string& Value);
   /** destructor **/
   virtual ~RangeItemAttribute();
   /** Configura el valor de inicio del rango*/
   void SetStartValue(const double Value);
   /** Configura el valor de fin del rango*/
   void SetFinishValue(const double Value);
   /** Configura el valor del color del rango en hexa */
   void SetHexaColor(const std::string& HexaColor);
   /** Devuelve el nombre del rango **/
   std::string GetRangeName() const;
   /** Devuelve el valor de inicio del rango*/
   double GetStartValue() const;
   /** Devuelve el valor de fin del rango*/
   double GetFinishValue() const;
   /** Devuelve el valor del color del rango en hexa */
   std::string GetHexaColor() const;
   /** Enum con la definicion de posiciones del valores de atributos dentro del rango **/
   typedef enum {
      RangeNamePosition = 0,
      StartValuePosition = 1,
      FinishValuePosition = 2,
      HexaColorPosition = 3
   } AtributeValuesPosition;
   /**
    * Convierte un string que contiene un double teniendo en cuenta que los strings con inf
    * @param DoubleString string que representa un double
    * @return double asociado
    */
   static double RangeDoubleToString(const std::string& DoubleString);

private:
   /** nombre del rango **/
   std::string rangeName_;
   // TODO: Si se replica lo de spectral no hace falta los atributos.
   /** valor de inicio del rango */
   double startValue_;
   /** valor de fin del rango **/
   double finishValue_;
   /** color del rango en hexa **/
   std::string hexaColor_;
};

} /** namespace suri */

#endif /* RANGEITEMATTRIBUTE_H_ */
