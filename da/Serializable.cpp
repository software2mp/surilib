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
#include <string>
// Includes Suri
#include "suri/AuxiliaryFunctions.h"
#include "suri/Serializable.h"
#include "suri/XmlFunctions.h"
// Includes Wx
#include <wx/xml/xml.h>
// Defines
// forwards

namespace suri{

Serializable::~Serializable(){}

void* Serializable::GetValue(){
	return this;
}

double Serializable::DeserializeDouble(wxXmlNode* pNode, const std::string& key){
   double value = 0;
   if (pNode->GetName().compare(key.c_str()) == 0) {
      value = StringToNumber<double>(pNode->GetNodeContent().c_str());
   }
   return value;
}

int Serializable::DeserializeInt(wxXmlNode* pNode, const std::string& key){
   int value = 0;
   if (pNode->GetName().compare(key.c_str()) == 0) {
      value = StringToNumber<int>(pNode->GetNodeContent().c_str());
   }
   return value;
}

std::string Serializable::DeserializeString(wxXmlNode* pNode,
		const std::string& key){
   std::string value = "";
   if (pNode->GetName().compare(key.c_str()) == 0) {
      value = pNode->GetNodeContent().mb_str();
   }
   return value;
}

// Wrapper String

SerializableString::SerializableString(){}

SerializableString::SerializableString(const std::string& value): value_(value){}

wxXmlNode* SerializableString::SerializeXml() const{
   wxXmlNode* node = CreateKeyValueNode(
          STRING_NODE, this->value_);
   return node;
}

SerializableString* SerializableString::DeserializeXml(wxXmlNode* pRoot){
   std::string strAux(Serializable::DeserializeString(pRoot, STRING_NODE));
   return new SerializableString(strAux);
}

void* SerializableString::GetValue(){
   return &this->value_;
}

//Wrapper int

SerializableInt::SerializableInt(): value_(0){}

SerializableInt::SerializableInt(int value): value_(value){}

wxXmlNode* SerializableInt::SerializeXml() const{
   wxXmlNode* node = CreateKeyValueNode(
          BAND_POSITION, NumberToString<int>(this->value_));
   return node;
}

SerializableInt* SerializableInt::DeserializeXml(wxXmlNode* pRoot){
   int num = Serializable::DeserializeInt(pRoot, STRING_NODE);
   return new SerializableInt(num);
}

void* SerializableInt::GetValue(){
   return &this->value_;
}

}
