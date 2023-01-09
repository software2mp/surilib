/*! \file Serializer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes Suri
#include "suri/Serializer.h"
#include "suri/Serializable.h"

class wxXmlNode;

namespace suri {

/**
 *
 */
Serializer::~Serializer() {}

/**
 * Serializa un objeto y lo devuelve como un xml, llamando a la función
 * SerializeXML propia de la clase del objeto pasado por parámetro. Los
 * objetos no serializables devuelven un nodo null
 */
wxXmlNode* DefaultSerializer::Serialize(Serializable* Serializable) const {
   return Serializable->SerializeXml();
}

/**
 * Devuelve un objeto null.
 */
Serializable* DefaultSerializer::Deserialize(wxXmlNode* xmlNode) const {
   return 0;
}

/**
 * Devuelve un string vacío.
 */
std::string DefaultSerializer::GetName() const {
   return "";
}

} /** namespace suri */
