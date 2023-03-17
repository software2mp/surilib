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

#ifndef LIBRARYITEMATTRIBUTE_H_
#define LIBRARYITEMATTRIBUTE_H_

// Includes Estandar
#include <string>

// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un atributo de un item de biblioteca
 */
class LibraryItemAttribute {
   LibraryItemAttribute(const LibraryItemAttribute&);

public:
   typedef enum {
      SimpleAttr = 0,
      ComplexAttr = 1,
      XmlAttr = 2
   }
   AttributeTypeEnum;

   static const std::string PrincipalAttrName;
   static const std::string ExtendedAttrName;

   /** Constructor **/
   LibraryItemAttribute(const std::string& Name, const std::string& Value = " ",
                        AttributeTypeEnum AttributeType = SimpleAttr);
   /** Destructor **/
   virtual ~LibraryItemAttribute();
   /** Enum para definir los tipos de atributos **/

   /**
    * Devuelve el nombre asociado al atributo
    * @return string con el nombre del atributo
    */
   std::string GetName() const;
   /**
    * Devuelve el tipo asociado al atributo
    * @return string con el tipo del atributo
    */
   AttributeTypeEnum GetType() const;
   /**
    * Devuelve el valor asociado al atributo
    * @return string con el valor del atributo
    */
   virtual std::string GetValue() const;
   /** Asocia un valor al atributo */
   virtual void SetValue(std::string Value);
   /** Compara si dos atributos son iguales **/
   virtual bool Equals(const LibraryItemAttribute& Attribute) const;

private:
   /** Nombre del atributo **/
   std::string name_;
   /** Tipo del atributo **/
   AttributeTypeEnum type_;
   /** Valor del atributo **/
   std::string value_;
};

} /** namespace suri */

#endif /* LIBRARYITEMATTRIBUTE_H_ */
