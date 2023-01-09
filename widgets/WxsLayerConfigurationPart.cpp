/*! \file WxsCreteWxsLayerPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
