/*! \file SerializerFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/SerializerFactory.h"

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/WorldSerializer.h"
#include "suri/ElementVectorSerializer.h"
// Includes Wx
// Defines
// forwards

namespace suri {

Serializer* SerializerFactory::GetSerializer(const std::string& name){
   if (name.compare(WorldSerializer::SerializerName) == 0){
      return new WorldSerializer();
   } else if (name.compare(ElementVectorSerializer::SerializerName) == 0){
      return new ElementVectorSerializer();
   }
   // TODO: Agregar serializador default
   return 0;
}

} /** namespace suri */
