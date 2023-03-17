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

#ifndef SERIALIZABLEVECTOR_H_
#define SERIALIZABLEVECTOR_H_


// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "suri/Serializable.h"
#include "suri/XmlFunctions.h"
// Includes Wx
#include <wx/sstream.h>
// Defines
// forwards

/**
 * Vector de serializables

 */

namespace suri{

template <class T>
class SerializableVector : public Serializable{
public:
   SerializableVector(){}
   SerializableVector(const std::vector<T>& value){
      this->value_ = value;
   }
   wxXmlNode* SerializeXml() const{
      wxXmlNode* vectorNode = new wxXmlNode(0, wxXML_ELEMENT_NODE, VECTOR_NODE);
      for (unsigned int i = 0; i < this->value_.size(); ++i){
         wxXmlNode* elementNode = this->value_[i].SerializeXml();
         vectorNode->AddChild(elementNode);
      }
      return vectorNode;
   }
   /**
    * @pre: es un vector de serializables
    * @pre: la clase a deserializar posee un método DeserializeXml
    */
   static SerializableVector* DeserializeXml(wxXmlNode* pRoot){
      std::vector<T> elementVector;
      wxXmlNode* pChild = pRoot->GetChildren();
      while (pChild != NULL){
         T* element;
         element = T::DeserializeXml(pChild);
         elementVector.push_back(*element);
         pChild = pChild->GetNext();
         if (!element){
            break;
         }
      }
      return new SerializableVector<T>(elementVector);
   }

   void* GetValue(){
        return &(this->value_);
   }

private:
   std::vector<T> value_;
};

}

#endif /* SERIALIZABLEVECTOR_H_ */
