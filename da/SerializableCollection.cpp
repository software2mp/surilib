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

#include "suri/SerializableCollection.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri{

SerializableCollection::~SerializableCollection() {
   REPORT_DEBUG("D: SerializableCollection DESTRUCTOR");
   std::map<std::string, Serializable*>::iterator it;
   for (it = params_.begin(); it != params_.end(); ++it) {
      delete (*it).second;
   }
   params_.clear();
}

bool SerializableCollection::Contains(const std::string &KeyP) const{
   bool result = true;
   std::map<std::string, Serializable*>::const_iterator it;
   it = params_.find(KeyP);
   result = it != params_.end();
   return result;
   //return params_.find(KeyP) != params_.end();
}

void SerializableCollection::AddSerializer(const std::string& KeyP,
                                           Serializer* Serializer){
   if (Contains(KeyP)){
      delete this->serializerMap_[KeyP];
   }
   this->serializerMap_[KeyP] = Serializer;
}

/**
 *
 */
wxXmlNode* SerializableCollection::SerializeXml() const{
   wxXmlNode* xmlRoot = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(COLLECTION_NODE));
   std::map<std::string, Serializable*>::const_iterator it;
   for (it = params_.begin(); it != params_.end(); ++it) {
      wxXmlNode* xmlNode = SerializeParam((*it).first, (*it).second);
      if (xmlNode){
         xmlRoot->AddChild(xmlNode);
      }
   }
   return xmlRoot;
}

/**
 *
 */

SerializableCollection* SerializableCollection::DeserializeXml(wxXmlNode* pRoot){
   SerializableCollection* collection= new SerializableCollection();
   wxXmlNode* pChild = pRoot->GetChildren();
   while (pChild){
      wxString nodeType = pChild->GetName();
      if (nodeType.compare(COLLECTION_PARAM_NODE) == 0){
         collection->DeserializeParam(pChild);
      }
      pChild = pChild->GetNext();
   }
   return collection;
}

/**
 * Serializa un parámetro y devuelve una instancia nueva de wxXmlNode
 */
wxXmlNode* SerializableCollection::SerializeParam(const std::string& Name,
                          Serializable* pSerializable) const{
   wxXmlNode* paramNode = 0;
   Serializer* serializer = GetSerializer(Name);
   wxXmlNode* valueNode = serializer->Serialize(pSerializable);
   if (valueNode){
      paramNode = new wxXmlNode(0, wxXML_ELEMENT_NODE, COLLECTION_PARAM_NODE);
      paramNode->AddAttribute(wxT(PARAM_NAME_ATTRIBUTE), Name);
      std::string serName = serializer->GetName();
      if (! serName.empty()){
         paramNode->AddAttribute(wxT(PARAM_SERIALIZER_ATTRIBUTE),
                                 serializer->GetName());
      }
      paramNode->AddChild(valueNode);
   }
   return paramNode;
}

/**
 * Deserializa un parámetro y lo agrega al mapa
 */
void SerializableCollection::DeserializeParam(const wxXmlNode* pParam){
   wxString paramName = pParam->GetAttribute(wxT(PARAM_NAME_ATTRIBUTE), wxT(""));
   wxString serializerName = pParam->GetAttribute(
         wxT(PARAM_SERIALIZER_ATTRIBUTE), wxT(""));
   Serializable* paramValue = SerializableFactory::
         GetSerializable(pParam->GetChildren(), serializerName.mb_str());
   //Agrego el parámetro a la colección
   this->params_[paramName.mb_str()] = paramValue;
}

/**
 * Obtiene el serializador asociado a un parámetro.
 * En caso de que no haya ningún serializador, se devuelve el default
 */
Serializer* SerializableCollection::GetSerializer(const std::string& KeyP) const{
   std::map<std::string, Serializer*>::const_iterator it;
   const Serializer* s = &defaultSerializer_;
   it = this->serializerMap_.find(KeyP);
   if (it != this->serializerMap_.end()){
      s = it->second;
   }
   return const_cast<Serializer*>(s);
}

}
