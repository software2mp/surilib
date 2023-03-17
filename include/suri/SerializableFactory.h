/*! \file SerializableFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SERIALIZABLEFACTORY_H_
#define SERIALIZABLEFACTORY_H_


// Includes Estandar
#include <string>
// Includes Suri
// Includes Wx
// Defines
// forwards

class wxXmlNode;

namespace suri{

class Serializable;

/**
 * Clase que crea instancias de Serializable a partir de los XML
 */
class SerializableFactory{
public:
   /**
    * Crea una instancia de un serializable a partir de un string con la
    * representación xml del objeto
    * @pre: El xml debe estar bien formado.
    * @param[in]: String con una representación válida del objeto Serializable
    * @return: puntero a una instancia de Serializable
    */
   static Serializable* GetSerializable(const std::string& xml);
   static Serializable* GetSerializable(wxXmlNode* pRoot,
                                        const std::string& serializer = "");
private:
   /**
    * Crea una instancia de un serializable a partir del nombre del tag
    * de la raiz.
    */
   static Serializable* GetSerializableByName(wxXmlNode* pRoot);
   /**
    * Crea una instancia de un serializador a partir del atributo de la raiz
    * Luego utiliza este serializador para crear una instancia de Serializable.
    */
   static Serializable* GetSerializableBySerializer(wxXmlNode* pRoot,
                                                    const std::string& name);
   /**
    * Genera un vector de serializables a partir del tipo del primer hijo
    * @pre: El xml debe estar bien formado.
    */
   static Serializable* GetSerializableVector(wxXmlNode* pRoot);
};

}

#endif /* SERIALIZABLEFACTORY_H_ */

