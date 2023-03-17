/*! \file Serializer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
