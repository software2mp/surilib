/*! \file WorldSerializer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
