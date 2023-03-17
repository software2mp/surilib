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
#include "suri/ComplexItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** caracter delimitador del inicio del atributo **/
std::string ComplexItemAttribute::StartAttributeChar = "{";
/** caracter delimitador del final del atributo **/
std::string ComplexItemAttribute::FinishAttributeChar = "}";
/** token que separa los valores del atributo**/
std::string ComplexItemAttribute::AttributesToken = ",";

ComplexItemAttribute::ComplexItemAttribute(
      const std::string& Name, const std::string& ComplexValue,
      const std::string& StartAttrChar,
      const std::string& FinishAttrChar,
      const std::string& AttrToken) :
      LibraryItemAttribute(Name, ComplexValue), startAttr_(StartAttrChar),
      finishAttr_(FinishAttrChar), attributesToken_(AttrToken) {
   LoadAttributesFromString(ComplexValue);
}

ComplexItemAttribute::~ComplexItemAttribute() {
   /** destruir el array **/
}
/** Devuelve el valor asociado al indice especificado del atributo */
std::string ComplexItemAttribute::GetAttribute(int Index) const {
   if (Index > 0 && (size_t)Index >= attributes_.size())
      return "";
   return attributes_.at(Index)->GetValue();
}

/** Agrega un atributo al vector **/
void ComplexItemAttribute::AddAttribute(LibraryItemAttribute* pAttribute) {
   attributes_.push_back(pAttribute);
}
/** Devuelve el listado de items que posee el atributo complejo
 *  en un string solo de formato {attr1,atttr2,attr3,...,attrn}
 *  @return string **/
std::string ComplexItemAttribute::GetSimpleAttributeValue() const {
   std::string value;
   value.append(startAttr_);
   std::vector<LibraryItemAttribute*>::const_iterator it = attributes_.begin();
   for (; it != attributes_.end(); ++it) {
      value.append((*it)->GetValue());
      value.append(attributesToken_);
   }
   // sacar el ultimo caracter porque queda una , de mas
   value = value.substr(0, value.size()-1);
   value.append(finishAttr_);
   return value;
}

/** A partir de un valor de atributo complejo obtiene los atributos que lo componen **/
void ComplexItemAttribute::LoadAttributesFromString(const std::string& ComplexValue) {
   std::string defaultattrname = "defaultname";
   if (ComplexValue.size() > 0 && ComplexValue.at(0) == startAttr_.at(0)
         && ComplexValue.at(ComplexValue.size() - 1) == finishAttr_.at(0)) {
      std::vector < std::string > values = tokenizer(ComplexValue, attributesToken_);
      for (unsigned int i = 0; i < values.size(); ++i) {
         std::string value;
         if (i == 0) {
            // Saltea el primer char '{'
            value = values.at(i).substr(1);
         } else if (i == values.size() - 1) {
            // Evita la copia del char de cierre del atributo '}'
            value = values.at(i).substr(0, values.at(i).size() - 1);
         } else {
            value = values.at(i);
         }
         std::string attrname = defaultattrname + NumberToString(i);
         attributes_.push_back(new LibraryItemAttribute(attrname, value));
      }
   }
}
} /** namespace suri */
