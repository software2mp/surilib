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

// Includes Estandar
// Includes Suri
#include "suri/RangeItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
// Defines
#define NEG_INF_STR "-inf"
#define INF_STR "inf"

// forwards

namespace suri {

/**
 * Constructor
 * @param Name nombre del atributo
 * @param RangeName nombre del rango
 * @param Value valor del atributo
 */
RangeItemAttribute::RangeItemAttribute(const std::string& Name, const std::string& Value) :
      ComplexItemAttribute(Name, Value) {
}

/** Destructor **/
RangeItemAttribute::~RangeItemAttribute() {
}

/** Configura el valor de inicio del rango */
void RangeItemAttribute::SetStartValue(const double Value) {
   std::string strvalue = NumberToString<double>(Value);
   RangeDoubleToString(strvalue);
   attributes_[StartValuePosition]->SetValue(strvalue);
}

/** Configura el valor de fin del rango*/
void RangeItemAttribute::SetFinishValue(const double Value) {
   std::string strvalue = NumberToString<double>(Value);
   RangeDoubleToString(strvalue);
   attributes_[FinishValuePosition]->SetValue(strvalue);
}

/** Configura el valor del color del rango en hexa */
void RangeItemAttribute::SetHexaColor(const std::string& HexaColor) {
   attributes_[HexaColorPosition]->SetValue(HexaColor);
}

/** Devuelve el nombre del rango **/
std::string RangeItemAttribute::GetRangeName() const {
   return (attributes_.size() > 0) ? attributes_[RangeNamePosition]->GetValue() : "";
}

/** Devuelve el valor de inicio del rango*/
double RangeItemAttribute::GetStartValue() const {
   return
         (attributes_.size() > 1) ? RangeDoubleToString(
                                          attributes_[StartValuePosition]->GetValue()) :
                                    0;
}

/** Devuelve el valor de fin del rango*/
double RangeItemAttribute::GetFinishValue() const {
   return
         (attributes_.size() > 2) ? RangeDoubleToString(
                                          attributes_[FinishValuePosition]->GetValue()) :
                                    0;
}

/** Devuelve el valor del color del rango en hexa */
std::string RangeItemAttribute::GetHexaColor() const {
   return (attributes_.size() > 3) ? attributes_[HexaColorPosition]->GetValue() : "";
}

/**
 * Convierte un string que contiene un double teniendo en cuenta que los strings con inf
 * @param DoubleString string que representa un double
 * @return double asociado
 */
double RangeItemAttribute::RangeDoubleToString(const std::string& DoubleString) {
   double value = 0;
   bool inf = DoubleString.find(INF_STR) != std::string::npos;
   bool neginf = DoubleString.find(NEG_INF_STR) != std::string::npos;
   if (!inf && !neginf) {
      value = StringToNumber<double>(DoubleString);
   } else {
      value = std::numeric_limits<double>::infinity();
      if (neginf)
         value *= -1;
   }
   return value;
}
} /** namespace suri */
