/*! \file RasterElementSerializer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/ElementVectorSerializer.h"

// Includes Estandar
#include <vector>
// Includes Suri
#include "suri/RasterElement.h"
#include "suri/xmlnames.h"
#include "suri/XmlFunctions.h"
// Includes Wx
#include "wx/xml/xml.h"
// Defines
// forwards

namespace suri {

typedef std::vector<Element*> elementVector_t;

std::string ElementVectorSerializer::SerializerName = "ElementVectorSerializer";

wxXmlNode* ElementVectorSerializer::Serialize(Serializable* pSerializable) const{
   NonSerializable<elementVector_t>* pAux =
         static_cast<NonSerializable<elementVector_t>*>(pSerializable);
   elementVector_t pElementVect =
         *static_cast<elementVector_t*>(pAux->GetValue());
   wxXmlNode* xmlRoot = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(ELEMENTS_NODE));
   for (unsigned int i = 0; i < pElementVect.size(); ++i){
      Element* pElement = pElementVect[i];
      wxXmlNode* xmlChild = CreateKeyValueNode(ELEMENT_NODE,
                                               pElement->GetUrl().mb_str());
      xmlRoot->AddChild(xmlChild);
   }
   return xmlRoot;
}

Serializable* ElementVectorSerializer::Deserialize(wxXmlNode* xmlNode) const{
   elementVector_t elemVector;
   wxXmlNode* pChild = xmlNode->GetChildren();
   while (pChild){
      std::string filename = Serializable::DeserializeString(
            pChild, ELEMENT_NODE);
      elemVector.push_back(Element::Create("RasterElement", filename));
      pChild = pChild->GetNext();
   }
   NonSerializable<elementVector_t>* pAux =
         new NonSerializable<elementVector_t>(elemVector);
   return pAux;
}

std::string ElementVectorSerializer::GetName() const{
   return SerializerName;
}

} /** namespace suri */
