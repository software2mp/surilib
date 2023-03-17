/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
