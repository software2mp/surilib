/*! \file WorldSerializer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WORLDSERIALIZER_H_
#define WORLDSERIALIZER_H_

#include "Serializer.h"

// Includes Estandar
#include <string>
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

class WorldSerializer : public Serializer {
public:
   wxXmlNode* Serialize(Serializable* pSerializable) const;
   Serializable* Deserialize(wxXmlNode* xmlNode) const;
   std::string GetName() const;
   static std::string SerializerName;
};

} /** namespace suri */

#endif /* WORLDSERIALIZER_H_ */
