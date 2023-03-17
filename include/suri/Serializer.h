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

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

// Includes standard
#include <string>

class wxXmlNode;

namespace suri {

class Serializable;

/**
 *
 */
class Serializer {
public:
   /**
    * Ctor.
    */
   Serializer() { }

   /**
    * Dtor.
    */
   virtual ~Serializer();

   /**
    * Serializa un objeto y lo devuelve como un xml. En caso de querer
    * serializar un objeto no serializable, se lo puede envolver con
    * la clase NonSerializable
    */
   virtual wxXmlNode* Serialize(Serializable* pSerializable) const = 0;

   /**
    * Deserializa un objeto nuevo a partir de un xml.
    */
   virtual Serializable* Deserialize(wxXmlNode* xmlNode) const = 0;

   /**
    * Devuelve el nombre de la clase, que se persistirá junto al objeto
    * serializado para poder revertir el proceso.
    */
   virtual std::string GetName() const = 0;
};

/**
 *
 */
class DefaultSerializer : public Serializer {
public:
   /**
    * Ctor.
    */
   DefaultSerializer() { }

   /**
    * Dtor.
    */
   virtual ~DefaultSerializer() { }

   /**
    * Serializa un objeto y lo devuelve como un xml, llamando a la función
    * SerializeXML propia de la clase del objeto pasado por parámetro. Los
    * objetos no serializables devuelven un nodo null
    */
   wxXmlNode* Serialize(Serializable* Serializable) const;

   /**
    * Devuelve un objeto null.
    */
   Serializable* Deserialize(wxXmlNode* xmlNode) const;

   /**
    * Devuelve un string vacío.
    */
   std::string GetName() const;
};

} /** namespace suri */

#endif /* SERIALIZER_H_ */
