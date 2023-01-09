/*! \file SerializableVector.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
 * (Matías - TCK #8451) Hago un vector de instancias y no de punteros para
 * reforzar más el patrón RAII.
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
