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

#ifndef COMPLEXITEMATTRIBUTE_H_
#define COMPLEXITEMATTRIBUTE_H_

// Includes Estandar
#include <vector>
// Includes Suri
#include "LibraryItemAttribute.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *  Atributo para item de biblioteca de tabla de intervalos.
 *  Representa un atributo complejo que viene definido por mas de un valor.
 */
class ComplexItemAttribute : public suri::LibraryItemAttribute {
   /** ctor copia **/
   ComplexItemAttribute(const ComplexItemAttribute&);
public:
   /** Constructor **/
   explicit ComplexItemAttribute(
         const std::string& Name, const std::string& ComplexValue,
         const std::string& StartAttrChar = StartAttributeChar,
         const std::string& FinishAttrChar = FinishAttributeChar,
         const std::string& AttrToken = AttributesToken);
   /** Destructor **/
   virtual ~ComplexItemAttribute();
   /** Caracter delimitador del inicio del atributo **/
   static std::string StartAttributeChar;
   /** Caracter delimitador del final del atributo **/
   static std::string FinishAttributeChar;
   /** Token que separa los valores del atributo**/
   static std::string AttributesToken;
   /** Devuelve el valor asociado al indice especificado del atributo */
   virtual std::string GetAttribute(int Index) const;
   /** Agrega un atributo al vector **/
   void AddAttribute(LibraryItemAttribute* pAttribute);
   /** Devuelve el listado de items que posee el atributo complejo
    *  en un string solo de formato {attr1,atttr2,attr3,...,attrn}
    *  @return string **/
   virtual std::string GetSimpleAttributeValue() const;

protected:
   /** A partir de un valor de atributo complejo obtiene los atributos que lo componen **/
   void LoadAttributesFromString(const std::string& ComplexValue);
   std::vector<LibraryItemAttribute*> attributes_;
   std::string startAttr_, finishAttr_, attributesToken_;
};

} /** namespace suri */

#endif /* COMPLEXITEMATTRIBUTE_H_ */
