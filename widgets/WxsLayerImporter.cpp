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
#include <map>
#include <utility>

// Includes Suri
#include "WxsLayerImporter.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryClient.h"
#include "suri/LibraryManager.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/DataViewManager.h"
#include "suri/BasicLibraryItemSelector.h"
#include "suri/Option.h"

// Includes Wx
#include "wx/treectrl.h"

// Defines
#define WXS_STYLE_PROPERTY "estilo"
#define WXS_FORMAT_PROPERTY "formato"
#define WXS_SERVICE_SRID_PROPERTY "idrefespacial"
#define URL_LAYERS_FIELD_NAME "layers"
#define WXS_ULX_PROPERTY "ulx"
#define WXS_ULY_PROPERTY "uly"
#define WXS_LRX_PROPERTY "lrx"
#define WXS_LRY_PROPERTY "lry"
#define WXS_BLOCK_SIZE_X "blocksizex"
#define WXS_BLOCK_SIZE_Y "blocksizey"
#define WXS_TILEMATRIX "tilematrix"
#define WXS_TILEMATRIXSET "tilematrixset"
#define WXS_MATRIX_WIDTH "matrixwidth"
#define WXS_MATRIX_HEIGHT "matrixheight"
#define WXS_URL "url"

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(WxsLayerImporterEvent, WxsLayerImporter)
   IMPLEMENT_EVENT_CALLBACK(OnButtonOk, OnButtonOk(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonCancel, OnButtonCancel(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRefreshDataRequest, OnRefreshDataRequest(event),
         wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLayerSelectionChange, OnLayerSelectionChange(event),
         wxTreeEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSrsSelectionChange, OnSrsSelectionChange(event),
         wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor **/
WxsLayerImporter::WxsLayerImporter(DataViewManager* pDataViewManager,
                                   std::string& FactoryString, Option& Metadata) :
      NEW_EVENT_OBJECT(WxsLayerImporterEvent), pDataViewManager_(pDataViewManager),
      pItemSelector_(NULL), factoryString_(FactoryString), metadata_(Metadata) {
   LibraryManager* plmgr = pDataViewManager->GetLibraryManager();
   windowTitle_ = _(caption_ADD_REMOTE_LAYER);
   LibraryClient client(plmgr, LibraryManager::READONLY);
   const Library* plib = client.GetLibraryByCode(LibraryManagerFactory::WmsLibraryCode);
   if (plib) {
      pItemSelector_ = new BasicLibraryItemSelector(
            this, plib->GetId(), plmgr, "ID_LIBRARY_SELECTION_PANEL",
            "ID_LIBRARY_SELECTION_PANEL", false,
            UniversalGraphicalComponentPartInterface::ReadOnlyWithFullFledged);
   }
}

/** Destructor **/
WxsLayerImporter::~WxsLayerImporter() {
   DELETE_EVENT_OBJECT;
}
/** Indica si el Part tiene cambios para salvar. */
bool WxsLayerImporter::HasChanged() {
   return false;
}
/** Salva los cambios realizados en el Part. */
bool WxsLayerImporter::CommitChanges() {
   return false;
}
/** Restaura los valores originales del Part. */
bool WxsLayerImporter::RollbackChanges() {
   return false;
}
/** Inicializa el part */
void WxsLayerImporter::SetInitialValues() {
}
/** Implementacion del metodo de creacion de ventana de Widget */
bool WxsLayerImporter::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_WXS_LAYER_IMPORTER_PANEL"));
   if (!pToolWindow_) return false;
   GET_CONTROL(*pToolWindow_, "ID_CANCEL_BTN", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(WxsLayerImporterEvent::OnButtonCancel), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_CANCEL_BTN", wxButton)->SetLabel(label_EXIT);
   GET_CONTROL(*pToolWindow_, "ID_OK_BTN", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(WxsLayerImporterEvent::OnButtonOk), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_REFRESH_WMS_DATA_BTN", wxBitmapButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(WxsLayerImporterEvent::OnRefreshDataRequest), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_SPATIAL_REFERENCE_COMBOBOX", wxComboBox)->Connect(
         wxEVT_COMMAND_COMBOBOX_SELECTED,
         wxCommandEventHandler(WxsLayerImporterEvent::OnSrsSelectionChange), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_,
         "ID_LAYERS_TREECTRL", wxTreeCtrl)->Connect(
         wxEVT_COMMAND_TREE_SEL_CHANGED,
         wxTreeEventHandler(WxsLayerImporterEvent::OnLayerSelectionChange), NULL,
         pEventHandler_);
   if (pItemSelector_) {
      AddControl(pItemSelector_->GetWidget(), "ID_SR_SELECTOR_PANEL");
   }
   RefreshData();
   return true;
}

/** Metodo que se llama cuando se presiona el boton Aceptar */
void WxsLayerImporter::OnButtonOk(wxCommandEvent &Event) {
   wxTreeCtrl* ptreectrl = XRCCTRL(*pToolWindow_, "ID_LAYERS_TREECTRL", wxTreeCtrl);
   std::string layertitle = ptreectrl->GetItemText(ptreectrl->GetSelection()).c_str();
   std::map<std::string, WxsLayersInformation::WxsLayerNode*>::iterator it =
         layermap_.find(layertitle);
   if (it == layermap_.end()) {
      return;
   }
   WxsLayersInformation::WxsLayerNode linfo = (*it->second);

   std::string url = GetUrl();

   if (url.find("?") != std::string::npos)
      url += "&";
   else
      url += "?";
   metadata_.SetOption(WXS_URL, url);

   std::ostringstream ss;
   ss << url;
   std::string layers = linfo.name_.empty() ? linfo.title_ : linfo.name_;
   metadata_.SetOption("title", linfo.title_);

   ss << URL_LAYERS_FIELD_NAME << "=" << layers;
   ss << "&service=" << serviceType_;

   std::string srs = GetSpatialReference();
   metadata_.SetOption(WXS_ULX_PROPERTY, GetMinX());
   metadata_.SetOption(WXS_ULY_PROPERTY, GetMaxY());
   metadata_.SetOption(WXS_LRX_PROPERTY, GetMaxX());
   metadata_.SetOption(WXS_LRY_PROPERTY, GetMinY());
   metadata_.SetOption(WXS_STYLE_PROPERTY, GetStyle());
   metadata_.SetOption(WXS_SERVICE_SRID_PROPERTY, srs);
   metadata_.SetOption(WXS_FORMAT_PROPERTY, GetFormat());
   metadata_.SetOption(URL_LAYERS_FIELD_NAME, layers);
   GetTileMatrix(srs);

   factoryString_ = ss.str();
   EndModal (wxID_OK);
}

/** Retorna el valor ulx */
std::string WxsLayerImporter::GetMinX() {
   return XRCCTRL(*pToolWindow_, "ID_BBOX_MIN_X_TEXT", wxTextCtrl)->GetValue().c_str();
}

/** Retorna el valor lrx */
std::string WxsLayerImporter::GetMaxX() {
   return XRCCTRL(*pToolWindow_, "ID_BBOX_MAX_X_TEXT", wxTextCtrl)->GetValue().c_str();
}

/** Retorna el valor lry */
std::string WxsLayerImporter::GetMinY() {
   return XRCCTRL(*pToolWindow_, "ID_BBOX_MIN_Y_TEXT", wxTextCtrl)->GetValue().c_str();
}

/** Retorna el valor uly */
std::string WxsLayerImporter::GetMaxY() {
   return XRCCTRL(*pToolWindow_, "ID_BBOX_MAX_Y_TEXT", wxTextCtrl)->GetValue().c_str();
}

/** Retorna del sistema de referencia */
std::string WxsLayerImporter::GetSpatialReference() {
   wxComboBox* psrscombo = XRCCTRL(*pToolWindow_, "ID_SPATIAL_REFERENCE_COMBOBOX",
                                   wxComboBox);
   return psrscombo->GetString(psrscombo->GetSelection()).c_str();
}

/** Retorna el estilo */
std::string WxsLayerImporter::GetStyle() {
   wxComboBox* pstylecombo = XRCCTRL(*pToolWindow_, "ID_STYLE_COMBOBOX", wxComboBox);
   return pstylecombo->GetString(pstylecombo->GetSelection()).c_str();
}

/** Retorna el formato */
std::string WxsLayerImporter::GetFormat() {
   wxComboBox* pformatcombo = XRCCTRL(*pToolWindow_, "ID_FORMAT_COMBOBOX", wxComboBox);
   return pformatcombo->GetString(pformatcombo->GetSelection()).c_str();
}

/** Retorna la url */
std::string WxsLayerImporter::GetUrl() {
   wxString textCtrlUrl =
         XRCCTRL(*pToolWindow_, "ID_WMS_URL_TEXT", wxTextCtrl)->GetValue();
   std::string url = textCtrlUrl.c_str();
   /** Se descartan todos los caracteres que vengan despues de '?' */
   return url.substr(0, textCtrlUrl.find("?"));
}

/**
 * Retorna el parametro TileMatrix de un TileMatrixSet
 * @param[in] Srs: sistema de referencia del TileMatrixSet
 */
void WxsLayerImporter::GetTileMatrix(std::string Srs) {
   if (serviceType_ != "wmts") return;

   for (size_t i = 0; i < getCapabilitesResponse_.tileMatrixSets_.size(); ++i) {
      if (getCapabilitesResponse_.tileMatrixSets_[i].identifier_.compare(Srs) == 0) {
         std::string tilematrixid =
               getCapabilitesResponse_.tileMatrixSets_[i].tileMatrixVector_[0].identifier_;
         std::string tilematrixsetid =
               getCapabilitesResponse_.tileMatrixSets_[i].identifier_;
         int tilewidth =
               getCapabilitesResponse_.tileMatrixSets_[i].tileMatrixVector_[0].tileWidth_;
         int tileheight =
               getCapabilitesResponse_.tileMatrixSets_[i].tileMatrixVector_[0].tileHeight_;
         int matrixwidth =
               getCapabilitesResponse_.tileMatrixSets_[i].tileMatrixVector_[0].matrixWidth_;
         int matrixheight =
               getCapabilitesResponse_.tileMatrixSets_[i].tileMatrixVector_[0].matrixHeight_;
         metadata_.SetOption(WXS_TILEMATRIX, tilematrixid);
         metadata_.SetOption(WXS_TILEMATRIXSET, tilematrixsetid);
         metadata_.SetOption(WXS_BLOCK_SIZE_X, NumberToString<int>(tilewidth));
         metadata_.SetOption(WXS_BLOCK_SIZE_Y, NumberToString<int>(tileheight));
         metadata_.SetOption(WXS_MATRIX_WIDTH, NumberToString<int>(matrixwidth));
         metadata_.SetOption(WXS_MATRIX_HEIGHT, NumberToString<int>(matrixheight));
         break;
      }
   }
}

/** Metodo que se llama cuando se presiona el boton Cancelar */
void WxsLayerImporter::OnButtonCancel(wxCommandEvent &Event) {
   EndModal (wxID_CANCEL);
}

/** Metodo que se llama cuando se presiona el boton de refresh */
void WxsLayerImporter::OnRefreshDataRequest(wxCommandEvent &Event) {
   RefreshData();
}

void WxsLayerImporter::RefreshData() {
   std::string wmsurl =
         XRCCTRL(*pToolWindow_, "ID_WMS_URL_TEXT", wxTextCtrl)->GetValue().c_str();
   WxsCapabilitesInformation capabilities;
   WxsGetCapabilities::GetCapabilitiesRequest(wmsurl, capabilities, serviceType_);
   getCapabilitesResponse_ = capabilities;
   UpdateAvailableLayers();
}

/** Funcion recursiva que carga el arbol **/
void WxsLayerImporter::LoadLayerTree(WxsLayersInformation::WxsLayerNode& Current,
                                     wxTreeItemId& ParentId, wxTreeCtrl* pTreeCtrl,
                                     bool IsRoot) {
   wxTreeItemId id;
   if (!IsRoot) {
      id = pTreeCtrl->AppendItem(ParentId, Current.title_);
   } else {
      id = pTreeCtrl->AddRoot(Current.title_);
   }
   layermap_.insert(std::make_pair(Current.title_, &Current));
   std::vector<WxsLayersInformation::WxsLayerNode>::iterator it =
         Current.childLayers_.begin();
   for (; it != Current.childLayers_.end(); ++it) {
      LoadLayerTree(*it, id, pTreeCtrl);
   }
}

/** Metodo que actualiza las capas disponibles asociadas al servicio
 *  wms ingresado  **/
void WxsLayerImporter::UpdateAvailableLayers() {
   layermap_.clear();
   wxTreeCtrl* ptreectrl = XRCCTRL(*pToolWindow_, "ID_LAYERS_TREECTRL", wxTreeCtrl);
   wxTreeItemId id;
   ptreectrl->DeleteAllItems();
   LoadLayerTree(getCapabilitesResponse_.layerInfo_.rootlayer_, id, ptreectrl, true);
   ptreectrl->ExpandAll();
}

/** Metodo encargado de cargar la informacion correspondiente en la interfaz
 * grafica de la capa que se pasa por parametro
 *  @param[in] LayerId id de capa
 */
void WxsLayerImporter::LoadLayerInfo(const std::string& LayerId) {
   std::map<std::string, WxsLayersInformation::WxsLayerNode*>::iterator it =
         layermap_.find(LayerId);
   if (it == layermap_.end()) {
      return;
   }
   WxsLayersInformation::WxsLayerNode linfo = (*it->second);
   std::string descp = linfo.title_;
   descp.append("-");
   descp.append(linfo.abstract_);
   GET_CONTROL(*pToolWindow_, "ID_WMS_SOURCE_DESCRIPTION_TEXT", wxTextCtrl)->SetValue(
         descp.c_str());
   wxComboBox* psrscombo = XRCCTRL(*pToolWindow_, "ID_SPATIAL_REFERENCE_COMBOBOX",
                                   wxComboBox);
   psrscombo->Clear();
   if (!linfo.srsList_.empty()) {
      std::vector<std::string>::const_iterator it = linfo.srsList_.begin();
      for (; it != linfo.srsList_.end(); ++it) {
         psrscombo->Append((*it).c_str());
      }
      psrscombo->SetSelection(0);
   }
   wxComboBox* pformatcombo = XRCCTRL(*pToolWindow_, "ID_FORMAT_COMBOBOX", wxComboBox);
   pformatcombo->Clear();
   wxComboBox* pstylecombo = XRCCTRL(*pToolWindow_, "ID_STYLE_COMBOBOX", wxComboBox);
   pstylecombo->Clear();
   std::vector<WxsLayersInformation::WxsStyleNode>::iterator sit =
         linfo.styles_.begin();
   for (; sit != linfo.styles_.end(); ++sit) {
      pformatcombo->Append((*sit).legendurl_.format_);
      pstylecombo->Append((*sit).title_.empty() ? (*sit).name_ : (*sit).title_);
   }
   pformatcombo->SetSelection(0);
   pstylecombo->SetSelection(0);

   if (!linfo.boundingBox_.empty()) {
      LoadBoundingBoxInfo(linfo.boundingBox_[0]);
   }
}

/** Metodo que se llama cuando se actualiza la capa seleccionada **/
void WxsLayerImporter::OnLayerSelectionChange(wxTreeEvent& Event) {
   wxTreeCtrl* ptreectrl = XRCCTRL(*pToolWindow_, "ID_LAYERS_TREECTRL", wxTreeCtrl);
   std::string layertitle = ptreectrl->GetItemText(ptreectrl->GetSelection()).c_str();
   LoadLayerInfo(layertitle);
   lastLayer_ = layertitle;
}

/** Metodo que se llama cuando se actualiza el sistema de referencia **/
void WxsLayerImporter::OnSrsSelectionChange(wxCommandEvent &Event) {
   wxComboBox* pcombo = XRCCTRL(*pToolWindow_, "ID_SPATIAL_REFERENCE_COMBOBOX",
                                wxComboBox);
   std::string srs = pcombo->GetString(pcombo->GetSelection()).c_str();
   std::map<std::string, WxsLayersInformation::WxsLayerNode*>::iterator it =
         layermap_.find(lastLayer_);
   if (it == layermap_.end()) {
      return;
   }
   WxsLayersInformation::WxsLayerNode linfo = (*it->second);

   size_t pos = pcombo->GetSelection();
   if (linfo.boundingBox_.size() > pos) {
      LoadBoundingBoxInfo(linfo.boundingBox_[pos]);
   }

}

/**
 * Metodo que carga la informacion de bounding box de la capa en los
 *  campos correspondientes
 */
void WxsLayerImporter::LoadBoundingBoxInfo(
      WxsLayersInformation::BoundingBox& BoundingBox) {
   GET_CONTROL(*pToolWindow_, "ID_BBOX_MIN_X_TEXT", wxTextCtrl)->SetValue(
         BoundingBox.minX_);
   GET_CONTROL(*pToolWindow_, "ID_BBOX_MAX_X_TEXT", wxTextCtrl)->SetValue(
         BoundingBox.maxX_);
   GET_CONTROL(*pToolWindow_, "ID_BBOX_MIN_Y_TEXT", wxTextCtrl)->SetValue(
         BoundingBox.minY_);
   GET_CONTROL(*pToolWindow_, "ID_BBOX_MAX_Y_TEXT", wxTextCtrl)->SetValue(
         BoundingBox.maxY_);
}

/** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
 * un boolean que indica si se trata de la primera seleccion (para que el part no piense
 * que posee cambios cuando en realidad es la seleccion inicial del CGU)*/
void WxsLayerImporter::SetActiveItem(const LibraryItem* pItem) {
   const LibraryItemAttribute* pattr = pItem ? pItem->GetAttribute("URL") : NULL;
   if (pattr) {
      XRCCTRL(*pToolWindow_, "ID_WMS_URL_TEXT", wxTextCtrl)->SetValue(
            pattr->GetValue().c_str());
   }
}
} /** namespace  suri **/
