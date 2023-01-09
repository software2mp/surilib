/*! \file Serializable.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


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
