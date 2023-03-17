/*! \file SerializableFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
// Includes Suri
#include "suri/SerializableCollection.h"
#include "suri/SerializableFactory.h"
#include "suri/SerializableVector.h"
#include "suri/SerializerFactory.h"
#include "suri/xmlnames.h"
#include "suri/ProcessAdaptLayer.h"
#include "BandInfo.h"
#include "suri/Subset.h"
// Includes Wx
#include <wx/sstream.h>
#include <wx/xml/xml.h>
// Defines
// forwards

namespace suri{

/**
 * Crea una instancia de un serializable a partir de un string con la
 * representación xml del objeto
 * @pre: El xml debe estar bien formado.
 * @param[in]: String con una representación válida del objeto Serializable
 * @return: puntero a una instancia de Serializable
 */

Serializable* SerializableFactory::GetSerializable(const std::string& xml){
   wxStringInputStream ss(xml);
   wxXmlDocument doc(ss);
   wxXmlNode* pRoot = doc.GetRoot();
   return GetSerializable(pRoot);
}

Serializable* SerializableFactory::GetSerializable(wxXmlNode* pRoot,
                                                   const std::string& serializer){
   if (!serializer.empty()){
      return GetSerializableBySerializer(pRoot, serializer);
   }
   return GetSerializableByName(pRoot);
}

Serializable* SerializableFactory::GetSerializableBySerializer(wxXmlNode* pRoot,
                                                      const std::string& name){
   Serializer* serializer = SerializerFactory::GetSerializer(name);
   Serializable* serializable = serializer->Deserialize(pRoot);
   delete serializer;
   return serializable;
}

Serializable* SerializableFactory::GetSerializableByName(wxXmlNode* pRoot){
   wxString nodeType = pRoot->GetName();
   if (nodeType.compare(BAND_NODE) == 0){
      return BandInfo::DeserializeXml(pRoot);
   } else if (nodeType.compare(COLLECTION_NODE) == 0){
      return SerializableCollection::DeserializeXml(pRoot);
   } else if (nodeType.compare(STRING_NODE) == 0){
      return SerializableString::DeserializeXml(pRoot);
   } else if (nodeType.compare(SUBSET_NODE) == 0){
      return Subset::DeserializeXml(pRoot);
   } else if (nodeType.compare(PROCESS_ADAPT_LAYER_NODE) == 0){
      return ProcessAdaptLayer::DeserializeXml(pRoot);
   } else if (nodeType.compare(VECTOR_NODE) == 0){
      return SerializableFactory::GetSerializableVector(pRoot);
   }
   throw "Unknown Serializable type";
}

/**
 * Genera un vector de serializables a partir del tipo del primer hijo
 * @pre: El xml debe estar bien formado.
 */
Serializable* SerializableFactory::GetSerializableVector(wxXmlNode* pRoot){
   wxString childType = "";
   wxXmlNode* pChild = pRoot->GetChildren();
   if (pChild){
      childType = pChild->GetName();
   }
   if (childType.compare(BAND_NODE) == 0){
      return SerializableVector<BandInfo>::DeserializeXml(pRoot);
   }
   return 0;
}

}
