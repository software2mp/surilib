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
#include <sstream>
// Includes Suri
#include "suri/UseFeatureSelection.h"
#include "HotLinkVectorLayer.h"
#include "suri/VectorElement.h"
// Includes Wx
// Defines
// forwards

namespace suri {
// Constantes
#define MEMORY_VECTOR_NAME             "shpmemory:Seleccion"
#define MEMORY_VECTOR_LAYER_NAME       "MEMORY_LAYER_NAME"
#define MEMORY_VECTOR_REF_FIELD_NAME   "ID"


/**
 * Crea un vector en memoria del tipo indicado.
 */
VectorEditor* UseFeatureSelection::CreateMemoryVector(
      const std::string& VectorName, std::map<std::string, std::string>& Geoms,
      const std::string& Srs) {
   // Creo un vector en memoria.
   suri::VectorEditor* pveditor = new suri::VectorEditor;
   pveditor->OpenVector(VectorName);
   pveditor->OpenLayer(MEMORY_VECTOR_LAYER_NAME, Srs, suri::Vector::Polygon);

   // Agrego el campo para guardar la referencia al poligono.
   pveditor->CreateField(MEMORY_VECTOR_REF_FIELD_NAME, suri::DataInfo<int>::Name);

   // Cargo todos los poligonos.
   std::map<std::string, std::string>::iterator it;
   for (it = Geoms.begin(); it != Geoms.end(); ++it) {
      long featureid = pveditor->OpenFeature();
      std::string outpolywkt;
      std::string inpolywkt = it->second;
      pveditor->SetGeometryByWkt(featureid, inpolywkt);
      pveditor->CloseFeature();
   }
   return pveditor;
}


/**
 * Devuelve un  HotLinkVectorLayer* con un elemento generado en memoria a partir de
 * la seleccion de pElement.
 */

LayerInterface*  UseFeatureSelection::CreateSelectionList(Element* pElement) {
   std::vector<std::string> selectedFeatures =
                                     dynamic_cast<VectorElement*>(pElement)->GetSelectedFeatures();
   std::map<std::string, std::string> geometries;
   VectorEditor editor;
   editor.OpenVector(std::string(pElement->GetUrl()));
   editor.OpenLayer(0);
   std::vector<std::string>::const_iterator it = selectedFeatures.begin();
   std::string vectorname = std::string(MEMORY_VECTOR_NAME) + " " +
                                             std::string(pElement->GetName());
   for (; it != selectedFeatures.end(); ++it) {
      std::stringstream ss;
      ss << *it;
      long featureId = 0;
      ss >> featureId;
      std::string geometry = editor.GetGeometryWkt(featureId);
      geometries[*it] = geometry;
   }
   CreateMemoryVector(vectorname, geometries, std::string(pElement->GetSpatialReference()));
   VectorDatasource* pmemdatasource = VectorDatasource::Create(vectorname);

   pSelection_.push_back(pmemdatasource);
   HotLinkVectorLayer* pVectorLayer = new  HotLinkVectorLayer
                                                   (pmemdatasource, pmemdatasource->GetElement());
   selectionMapFromLayer_[pVectorLayer->GetId()] = pmemdatasource;
   return pVectorLayer;
}

VectorDatasource* UseFeatureSelection::GetSelectedVectorDatasource(Element* pElement) {
   VectorElement* pelement = dynamic_cast<VectorElement*>(pElement);
   if (!pelement) return NULL;
   std::vector < std::string > selectedFeatures = pelement->GetSelectedFeatures();
   if (!selectedFeatures.empty()) {
      LayerInterface* player = CreateSelectionList(pelement);
      return GetVectorDatasourceFromLayer(player->GetId());
   }
   return NULL;
}
VectorDatasource* UseFeatureSelection::GetVectorDatasource
                                             (SuriObject::UuidType VectorDatasourceId) {
   VectorDatasource * returned = NULL;
   std::vector<VectorDatasource*>::const_iterator it = pSelection_.begin();
   for (; it != pSelection_.end(); ++it) {
      if ( (*it)->GetId() == VectorDatasourceId ){
         returned = (*it);
      }
   }
   return returned;
}

VectorDatasource* UseFeatureSelection::GetVectorDatasourceFromLayer
                                             (SuriObject::UuidType LayerId) {
   VectorDatasource * returned = NULL;
   std::map<SuriObject::UuidType, VectorDatasource*>::const_iterator it =
         selectionMapFromLayer_.find(LayerId);
   if (it != selectionMapFromLayer_.end()) {
      returned = selectionMapFromLayer_[LayerId];
   }
   return returned;
}

/**
 *Crea un LayerHtmlListItem dado un elmento con una seleccion , devuelve NULL  si el elemento
 *es NULL, o si el elemento no tiene una seleccion.
 */
ui::LayerHtmlListItem* UseFeatureSelection::GetItemSelection(Element* pElement) {
   VectorElement* pelement = dynamic_cast<VectorElement*>(pElement);
   if (!pelement) return NULL;
   std::vector < std::string > selectedFeatures = pelement->GetSelectedFeatures();
   if (!selectedFeatures.empty()) {
      LayerInterface* player = CreateSelectionList(pelement);
      return new ui::LayerHtmlListItem(player);
   }
   return NULL;
}
}
