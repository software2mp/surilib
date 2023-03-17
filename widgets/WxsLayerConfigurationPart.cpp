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

#include "suri/WxsLayerConfigurationPart.h"

// Includes standard
#include <sstream>

// Includes Suri
#include "suri/messages.h"
#include "WxsDataSourcePart.h"
#include "WxsProperties.h"
#include "WxsVisualizationPart.h"
#include "WxsRenderer.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryManagerFactory.h"

// Includes Wx

// Includes App

// Defines
#define WXS_STYLE_PROPERTY "estilo"
#define WXS_FORMAT_PROPERTY "formato"
#define WXS_SERVICE_SRID_PROPERTY "idrefespacial"

namespace suri {
/** Ctor */
WxsLayerConfigurationPart::WxsLayerConfigurationPart(
      std::string* pWxsElementFactoryString, DataViewManager* pDataViewManager) :
      PartCollection(wxT("ID_WXS_CREATE_LAYER_PANEL"), _(caption_ADD_REMOTE_LAYER)) {

   LibraryManager* pManager = pDataViewManager->GetLibraryManager();

   LibraryClient* pClient = pManager ? new LibraryClient(pManager, LibraryManager::READONLY) : NULL;

   const Library* plib =
         pClient->GetLibraryByCode(LibraryManagerFactory::WmsLibraryCode);

   pItemSelector_ = new BasicLibraryItemSelector(
         this,
         plib->GetId(), pManager, "ID_LIBRARY_SELECTION_PANEL",
         "ID_LIBRARY_SELECTION_PANEL", false,
         UniversalGraphicalComponentPartInterface::ExtendedROWithFF);

   pWxsElementFactoryString_ = pWxsElementFactoryString;

   pWxsDataSourcePart_ = new WxsDataSourcePart();
   AddPart(pWxsDataSourcePart_, wxT("ID_WXS_DATA_SOURCE_PANEL"));

   pWxsProperties_ = new WxsProperties();
   AddPart(pWxsProperties_, wxT("ID_WXS_PROPERTIES_PANEL"));

   pWxsVisualizationPart_ = new WxsVisualizationPart();
   AddPart(pWxsVisualizationPart_, wxT("ID_WXS_VISUALIZATION_PANEL"));

   if (pItemSelector_) {
      AddPart(pItemSelector_, "ID_SR_SELECTOR_PANEL");
   }
}

/** Dtor */
WxsLayerConfigurationPart::~WxsLayerConfigurationPart() {
}

/** Inicializa parts y configura el string de la factoria */
void WxsLayerConfigurationPart::SetInitialValues() {
   PartCollection::SetInitialValues();
   UpdateFactoryString();
}

/**
 * Salva los cambios realizados en el Part.
 * @return resultado de la operacion
 */
bool WxsLayerConfigurationPart::CommitChanges() {
   if (!PartCollection::CommitChanges()) return false;
   UpdateFactoryString();
   return true;
}

/**
 * Actualiza el string que se pasa a la factoria de elementos con los
 * datos configurados en los parts
 */
void WxsLayerConfigurationPart::UpdateFactoryString() {
   WxsRenderer::Parameters params;
   pWxsProperties_->ConfigureParameters(params);
   pWxsVisualizationPart_->ConfigureParameters(params);

   std::ostringstream ss;
   ss << pWxsDataSourcePart_->GetWxsElementUrl();
   ss << "&service=wms";
   ss << "|ulx=" << params.extent_.ul_.x_;
   ss << "|uly=" << params.extent_.ul_.y_;
   ss << "|lrx=" << params.extent_.lr_.x_;
   ss << "|lry=" << params.extent_.lr_.y_;
   ss << "|" << WXS_STYLE_PROPERTY << "=" << params.style_;
   ss << "|" << WXS_SERVICE_SRID_PROPERTY << "=" << params.serviceSRId_;
   ss << "|" << WXS_FORMAT_PROPERTY << "=" << params.format_;

   *pWxsElementFactoryString_ = ss.str();
}

/** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
 * un boolean que indica si se trata de la primera seleccion (para que el part no piense
 * que posee cambios cuando en realidad es la seleccion inicial del CGU)
 * @param[in] pItem: item con la informacion del nodo
 */
void WxsLayerConfigurationPart::SetActiveItem(const LibraryItem* pItem) {
   if (pItem) {
      const LibraryItemAttribute* attr = pItem->GetAttribute("URL");
      pWxsDataSourcePart_->SetValue("ID_WMS_URL_TEXT", attr->GetValue());
   }
}

}  // namespace
