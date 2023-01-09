/*! \file SerializerFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SERIALIZERFACTORY_H_
#define SERIALIZERFACTORY_H_

// Includes Estandar
#include <string>
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

class Serializer;

class SerializerFactory {
public:
   static Serializer* GetSerializer(const std::string& name);
};

} /** namespace suri */

#endif /* SERIALIZERFACTORY_H_ */
