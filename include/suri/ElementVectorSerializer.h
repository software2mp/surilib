/*! \file RasterElementSerializer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ELEMENTVECTORSERIALIZER_H_
#define ELEMENTVECTORSERIALIZER_H_

// Includes Estandar
// Includes Suri
#include "suri/Serializer.h"
// Includes Wx
// Defines
// forwards
class wxXmlNode;

namespace suri {

class Serializable;

class ElementVectorSerializer : public Serializer {
public:
   wxXmlNode* Serialize(Serializable* pSerializable) const;
   Serializable* Deserialize(wxXmlNode* xmlNode) const;
   std::string GetName() const;
   static std::string SerializerName;
};

} /** namespace suri */

#endif /* ELEMENTVECTORSERIALIZER_H_ */
