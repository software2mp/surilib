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
