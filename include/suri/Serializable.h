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

#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_


// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "suri/xmlnames.h"
// Includes Wx
// Defines
// forwards

class wxXmlNode;

namespace suri{

class Serializable{
public:
   /**
    * Devuelve una versión xml del objeto
    */
   virtual wxXmlNode* SerializeXml() const = 0;
   /**
    * Genera el objeto a través de los parámetros persistidos en Serialize
    */
   virtual ~Serializable();
   /**
    * Es un asco que exista una función así, pero dado que
    * SerializableCollection es una bolsa de gatos, con esto puedo unwrappear
    * NonSerializables
    * Creo que lo ideal es que esa clase no exista, pero eso hay que
    * convertirlo con tiempo.
    */
   virtual void* GetValue();
   static double DeserializeDouble(wxXmlNode* pNode, const std::string& key);
   static int DeserializeInt(wxXmlNode* pNode, const std::string& key);
   static std::string DeserializeString(wxXmlNode* pNode, const std::string& key);
};

/**
 * Wrappers de tipos nativos
 */

class SerializableInt : public Serializable{
public:
   SerializableInt();
   SerializableInt(int value);
   wxXmlNode* SerializeXml() const;
   static SerializableInt* DeserializeXml(wxXmlNode* pRoot);
   void* GetValue();
private:
   int value_;
};

class SerializableString : public Serializable{
public:
   SerializableString();
   SerializableString(const std::string& value);
   wxXmlNode* SerializeXml() const;
   static SerializableString* DeserializeXml(wxXmlNode* pRoot);
   void* GetValue();
private:
   std::string value_;
};

/**
 * Null serializer genérico
 */

template <class T>
class NonSerializable : public Serializable{
public:
   NonSerializable(T& value) : value_(value){}
   wxXmlNode* SerializeXml() const{
	   return 0;
   }
   void* GetValue(){
	   return &this->value_;
   }
private:
   T value_;
};

}
#endif /* SERIALIZABLE_H_ */
