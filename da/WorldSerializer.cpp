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

#include "suri/WorldSerializer.h"

// Includes Estandar
// Includes Suri
#include "suri/Serializable.h"
#include "SpatialReference.h"
#include "suri/World.h"
#include "suri/xmlnames.h"
#include "suri/XmlFunctions.h"
// Includes Wx
#include "wx/xml/xml.h"
// Defines
// forwards

namespace suri {

std::string WorldSerializer::SerializerName = "WorldSerializer";

wxXmlNode* WorldSerializer::Serialize(Serializable* pSerializable) const{
   NonSerializable<World*>* pAux = static_cast<NonSerializable<World*>*>(pSerializable);
   World* pWorld = *static_cast<World**>(pAux->GetValue());
   wxXmlNode* xmlRoot = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(WORLD_NODE));
   //std::string wkt = SpatialReference::GetAuthorityId(pWorld->GetSpatialReference());
   std::string wkt = pWorld->GetSpatialReference();
   wxXmlNode* child = CreateKeyValueNode(SPATIAL_REFERENCE_NODE, wkt);
   xmlRoot->AddChild(child);
   Subset subset;
   pWorld->GetWindow(subset);
   xmlRoot->AddChild(subset.SerializeXml());
   return xmlRoot;
}

Serializable* WorldSerializer::Deserialize(wxXmlNode* pRoot) const{
   World* pWorld = new World();
   NonSerializable<World*>* pAux = new NonSerializable<World*>(pWorld);
   wxXmlNode* pChild = pRoot->GetChildren();
   // sistema de referencia de entrada
   // TODO(Gabriel - TCK #7139): Buscar el epsg en la biblioteca
   std::string spatialRef = Serializable::DeserializeString(pChild, SPATIAL_REFERENCE_NODE);
   pChild = pChild->GetNext();
   Subset* subset = Subset::DeserializeXml(pChild);
   pWorld->SetSpatialReference(spatialRef);
   pWorld->SetWindow(*subset);
   delete subset;
   return pAux;
}

std::string WorldSerializer::GetName() const{
   return SerializerName;
}

} /** namespace suri */
