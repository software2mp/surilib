/*! \file Serializable.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
