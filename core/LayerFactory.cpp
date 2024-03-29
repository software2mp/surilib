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
#include <string>

// Includes Suri
#include "suri/LayerFactory.h"
#include "suri/DatasourceInterface.h"
#include "suri/LayerInterface.h"
#include "suri/Element.h"
#include "RasterLayer.h"
#include "VectorLayer.h"
#include "TerrainLayer.h"
#include "HtmlLayer.h"
#include "MultimediaLayer.h"
#include "HtmlElement.h"
#include "MultimediaElement.h"
#include "HotLinkVectorLayer.h"
#include "AnotationLayer.h"
#include "suri/VectorElement.h"
#include "wxmacros.h"
#include "suri/TerrainElement.h"
#include "suri/RasterElement.h"

// Includes Wx
#include "wx/filename.h"

// Defines
#define RASTER_DATASOURCE 'R'
#define WXS_DATASOURCE 'W'
#define TERRAIN_DATASOURCE 'T'
#define ANOTATION_DATASOURCE 'A'
#define HOTLINKVECTOR_DATASOURCE 'H'
#define VECTOR_DATASOURCE 'V'

// forwards

namespace suri {

/** Metodo auxiliar para la creacion de la capa a partir de un element
 * @param[in] pDatasource Fuente de datos asociada al layer que se quiere crear
 * @param[in] pElement elemento que va a encapsular la capa
 * @return una instancia de layerinterface, NULL  en caso contrario
 */
LayerInterface* LayerFactory::CreateLayerFromElement(
      DatasourceInterface *pDatasource, Element *pElement) {
   LayerInterface* player = NULL;
   if (pElement->GetClassId().empty()) {
	   if (dynamic_cast<HtmlElement*>(pElement)) {
		   player = new HtmlLayer(pDatasource, pElement);
	   } else if (dynamic_cast<MultimediaElement*>(pElement)) {
		   player = new MultimediaLayer(pDatasource, pElement);
	   }
	   return player;
   }
   switch (pElement->GetClassId()[0]) {
         case RASTER_DATASOURCE:
            player = new RasterLayer(pDatasource, pElement);
            break;
         case WXS_DATASOURCE:  // Cambiar a wsxl?
            // pLayer = new WxsLayer(Element);
            player = new VectorLayer(pDatasource, pElement);
            break;
         case TERRAIN_DATASOURCE:
            player = new TerrainLayer(pDatasource, pElement);
            break;
         case ANOTATION_DATASOURCE:
            player = new AnotationLayer(pDatasource, pElement);
            break;
         case HOTLINKVECTOR_DATASOURCE:
            player = new HotLinkVectorLayer(pDatasource, pElement);
            break;
         case VECTOR_DATASOURCE:
            player = new VectorLayer(pDatasource, pElement);
            break;

         default:
            break;
      }
   return player;
}

/** Metodo auxiliar para la copia de la capa activa de los ds vectoriales
 * @param[in] pElement elemento que posee seteada la capa activa
 * @param[out] pElementCopy elemento al que se desea setear la capa activa
 */
void LayerFactory::SetActiveLayerFromElement(Element* pElement, Element* pElementCopy) {
   // Se debe propagar la capa activa tambien
   // Se necesita hacer downcasting porque el metodo activelayer corresponde
   // solamente a los elementos vectoriales.
   VectorElement *pvectorcopy = dynamic_cast<VectorElement*>(pElementCopy);
   VectorElement *pvector = dynamic_cast<VectorElement*>(pElement);
   if (pvectorcopy && pvector) {
      pvectorcopy->SetActiveLayer(pvector->GetActiveLayer());
   }
}

/**
 * Metodo estatico para la creacion de una Layer a partir de un Datasource
 * @return LayerInterface* referencia la capa correspondiente a la fuente de datos
 * @return NULL en caso de no poder crear la capa correspondiente a la fuente de datos
 */
LayerInterface* LayerFactory::CreateLayer(DatasourceInterface* pDatasource) {
   // TODO(Gabriel - TCK #852): Integrar esta creacion con la abstractfactory
   // para LayerInterface
   Element* pelement = pDatasource->GetElement();
   LayerInterface* player = NULL;
   if (pelement) {
      Element* pelementcopy = Element::Create(pelement->GetClassId(),
                                              pelement->GetNode(),
                                              pDatasource->GetMetadataOption());
      if (!pelementcopy) {
         if (dynamic_cast<HtmlElement*>(pelement)) {
            pelementcopy = HtmlElement::Create(pelement->GetUrl().c_str());
         } else if (dynamic_cast<MultimediaElement*>(pelement)) {
            pelementcopy = MultimediaElement::Create(pelement->GetUrl().c_str());
            MultimediaElement* pphotods = dynamic_cast<MultimediaElement*>(pelement);
            MultimediaElement* pphotolayer = dynamic_cast<MultimediaElement*>(pelementcopy);
            pphotolayer->SetTitle(pphotods->GetTitle());
            pphotolayer->SetText(pphotods->GetText());
         }
         pelementcopy->SetName(pDatasource->GetName());
         pelementcopy->SetCopyRight(pelement->GetCopyRight());
         pelementcopy->SetDescription(pelement->GetDescription());
         double ndvalue = 0.0;
         bool ndvavailable = false;
         pelement->GetNoDataValue(ndvavailable, ndvalue);
         if (ndvavailable)
            pelementcopy->SetNoDataValue(ndvalue);
      }
      SetActiveLayerFromElement(pelement, pelementcopy);
      player = CreateLayerFromElement(pDatasource, pelementcopy);
      if (!player){
         delete pelementcopy;
      }
   }
   return player;
}

} /* namespace suri */
