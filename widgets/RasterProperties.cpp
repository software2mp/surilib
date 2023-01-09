/*! \file RasterProperties.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandar
#include <sstream>
#include <cmath>
#include <vector>
#include <string>

// Includes Suri
#include "RasterProperties.h"
#include "BandDriver.h"
#include "ColorTableRenderer.h"
#include "DefaultTable.h"
#include "RasterDatasource.h"
#include "RasterFeaturesWidget.h"
#include "TablePart.h"
#include "TablePartConfigurator.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/BufferedDriver.h"
#include "suri/LayerInterface.h"
#include "suri/ParameterCollection.h"
#include "suri/RasterSpatialModel.h"
#include "resources.h"

// Includes Wx
#include "wx/textctrl.h"
#include "wx/xrc/xmlres.h"

void InitRasterPropertiesXmlResource();

namespace {

void SetNodeTextContent(wxXmlNode* pNode, const wxString& Text) {
   if (pNode == NULL) return;

   wxXmlNode* pchild = pNode->GetChildren();
   while (pchild) {
      if (pchild->GetType() == wxXML_TEXT_NODE) {
         pchild->SetContent(Text);
         break;
      }
      pchild = pchild->GetNext();
   }
}

} // namespace anonimo

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(RasterPropertiesEvent, RasterProperties)
   IMPLEMENT_EVENT_CALLBACK(OnNotDataValueCheck, OnNotDataValueCheck(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnNotValueValidEventHandler, OnNotValueValidEventHandler(event),
         wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDimensionChangedHandler,
         OnDimensionChangedHandler(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnBandsCountChangedHandler,
         OnBandsCountChangedHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor.
 * @param[in] pElement elemento del raster del que quiero las propiedades
 */
RasterProperties::RasterProperties(RasterElement* pElement) :
      NEW_EVENT_OBJECT(RasterPropertiesEvent), pElement_(pElement), pDatasource_(NULL),
      pLayer_(NULL), dimModified_(false), pDataViewManager_(NULL),
      pRasterFeaturesWidget_(NULL), pIOParams_(new ParameterCollection),
      rawDataModified_(false), pTablePart_(NULL), bandsDataModified_(false), pTable_(NULL) {
   windowTitle_ = _(caption_RASTER_MODEL_PROPERTIES);
   minNotValidValueLimit_ = 0.0;
   maxNotValidValueLimit_ = 0.0;
}

/**
 * Ctor.
 * @param[in] pElement elemento del raster del que quiero las propiedades
 */
RasterProperties::RasterProperties(RasterDatasource* pDatasource,
                                   LayerInterface* pLayer) :
      NEW_EVENT_OBJECT(RasterPropertiesEvent), pElement_(NULL),
      pDatasource_(pDatasource), pLayer_(pLayer), dimModified_(false),
      pDataViewManager_(NULL), pRasterFeaturesWidget_(NULL),
      pIOParams_(new ParameterCollection), rawDataModified_(false), pTablePart_(NULL),
      bandsDataModified_(false), pTable_(NULL) {
   windowTitle_ = _(caption_RASTER_MODEL_PROPERTIES);
   minNotValidValueLimit_ = 0.0;
   maxNotValidValueLimit_ = 0.0;
}

/**
 * Ctor.
 */
RasterProperties::RasterProperties(RasterDatasource* pDatasource,
                                   LayerInterface* pLayer,
                                   DataViewManager* pDataViewManager) :
      NEW_EVENT_OBJECT(RasterPropertiesEvent), pElement_(NULL),
      pDatasource_(pDatasource), pLayer_(pLayer), dimModified_(false),
      pDataViewManager_(pDataViewManager), pRasterFeaturesWidget_(NULL),
      pIOParams_(new ParameterCollection), rawDataModified_(false), pTablePart_(NULL),
      bandsDataModified_(false), pTable_(NULL) {
   windowTitle_ = _(caption_RASTER_MODEL_PROPERTIES);
   minNotValidValueLimit_ = 0.0;
   maxNotValidValueLimit_ = 0.0;
}

/**
 * Dtor.
 */
RasterProperties::~RasterProperties() {
   DELETE_EVENT_OBJECT;

   if (pIOParams_ != NULL) {
      delete pIOParams_;
      pIOParams_ = NULL;
   }
}

/**
 * Crea la ventana
 * @return informa si pudo crear la herramienta
 */
bool RasterProperties::CreateToolWindow() {
   modified_ = false;
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_RASTER_PROPERTIES_PART"));
   GET_CONTROL(*pToolWindow_,
         "ID_NOT_VALID_VALUE_CHECKBOX", wxCheckBox)->Connect(
         wxEVT_COMMAND_CHECKBOX_CLICKED,
         wxCommandEventHandler(RasterPropertiesEvent::OnNotDataValueCheck), NULL,
         pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_NOT_VALID_VALUE", wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(RasterPropertiesEvent::OnNotValueValidEventHandler),
         NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_,
         "ID_RASTER_PROPERTIES_BANDS_TEXT", wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(RasterPropertiesEvent::OnBandsCountChangedHandler), NULL,
         pEventHandler_);

   GET_CONTROL(*pToolWindow_,
         "ID_RASTER_PROPERTIES_PIXELS_TEXT", wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(RasterPropertiesEvent::OnDimensionChangedHandler), NULL,
         pEventHandler_);

   GET_CONTROL(*pToolWindow_,
         "ID_RASTER_PROPERTIES_LINES_TEXT", wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(RasterPropertiesEvent::OnDimensionChangedHandler), NULL,
         pEventHandler_);

   /** Valores permitidos para ingreso de datos */
   wxString decimalfilter[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"),
                                wxT("7"), wxT("8"), wxT("9"), wxT("0"), wxT("-"), wxT("N"),
                                wxT("a"), wxT("I"), wxT("F"), wxT(".")};
   wxTextValidator notvalidvaluefilter = wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
   wxArrayString numericvalues = wxArrayString(16, decimalfilter);
   notvalidvaluefilter.SetIncludes(numericvalues);

   GET_CONTROL(*pToolWindow_, wxT("ID_NOT_VALID_VALUE"),
         wxTextCtrl)->SetValidator(notvalidvaluefilter);

   LoadNoValidValue();
   LoadNotValidValueCheckBox();
   return true;
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap bitmap que se asocia a la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void RasterProperties::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_RASTER_PROPERTIES, ToolBitmap);
}

/** Inicializa los controles */
void RasterProperties::SetInitialValues() {
   RollbackChanges();
}

/**
 * Restaura los valores originales del Part.
 * @return informa si pudo hacer Rollback
 */
bool RasterProperties::RollbackChanges() {
   LoadDimensionData();
   LoadOrganizationData();
   LoadIdentificationData();
   LoadBandsData();
   LoadNoValidValue();
   LoadNotValidValueCheckBox();
   LoadDnValueLimits();
   LoadFeatures();

   return true;
}

void RasterProperties::RestoreValues() {
   wxCheckBox* pnotValidValue_check = XRCCTRL(*pToolWindow_,
                                              "ID_NOT_VALID_VALUE_CHECKBOX",
                                              wxCheckBox);
   pnotValidValue_check->SetValue(lastDataValueChecked_);
   lastNotValidValue_ = notValidValue_;
}

/**
 * Carga datos de dimension.
 */
void RasterProperties::LoadDimensionData() {
   wxString lbl;

   RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());

   bool isRaw = (playerraster->GetTypeAsString().compare("raw") == 0);

   lbl = wxString::Format(wxT("%d"), playerraster->GetBandCount());
   GET_CONTROL(*pToolWindow_, wxT("ID_RASTER_PROPERTIES_BANDS_TEXT"), wxTextCtrl)->ChangeValue(
         lbl);
   if (isRaw)
      GET_CONTROL(*pToolWindow_, wxT("ID_RASTER_PROPERTIES_BANDS_TEXT"), wxTextCtrl)->Enable();

   int rasterwidth = 0, rasterheight = 0;
   playerraster->GetRasterSize(rasterwidth, rasterheight);

   lbl = wxString::Format(wxT("%d"), rasterwidth);
   GET_CONTROL(*pToolWindow_, wxT("ID_RASTER_PROPERTIES_PIXELS_TEXT"), wxTextCtrl)->ChangeValue(
         lbl);

   lbl = wxString::Format(wxT("%d"), rasterheight);
   GET_CONTROL(*pToolWindow_, wxT("ID_RASTER_PROPERTIES_LINES_TEXT"), wxTextCtrl)->ChangeValue(
         lbl);
}

/** carga datos de organizacion */
void RasterProperties::LoadOrganizationData() {

}

/** carga datos de identificacion */
void RasterProperties::LoadIdentificationData() {

}

/** carga datos de Bandas */
void RasterProperties::LoadBandsData() {
   if (pTablePart_ == NULL) {
      RasterElement* praster = dynamic_cast<RasterElement*>(pDatasource_ ?
                                    pDatasource_->GetElement() : pLayer_->GetElement());

      pTable_ = new DefaultTable();
      BandDriver* pdriver = new BandDriver(praster);
      BufferedDriver* pbuffer = new BufferedDriver(pdriver);
      pTable_->SetDriver(pbuffer);
      pTable_->Attach(this);

      suri::TablePartConfigurator tableconf;
      suri::Option opt = suri::TablePartConfigurator::GetBandTableEditorConfig();
      pTablePart_ = tableconf.Initialize(pDataViewManager_, pTable_, opt);
      AddControl(pTablePart_, wxT("ID_RASTER_PROPERTIES_TABLE_CONTAINER"));
   }
}

std::string RasterProperties::LoadDataType() {
   RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());
   wxXmlNode* pdatatypeNode = playerraster->GetNode(PATH_DATA_TYPE_NODE_PIPES);
   return pdatatypeNode->GetNodeContent().c_str();
}

/** Carga el estado del checkbox de valid value**/
void RasterProperties::LoadNotValidValueCheckBox() {
   RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());
   wxTextCtrl *pnotvalidvaluetext = XRCCTRL(*pToolWindow_, "ID_NOT_VALID_VALUE",
                                            wxTextCtrl);
   dataType_ = LoadDataType();
   wxXmlNode* pdatanotvalidvalue = playerraster->GetNode(
         PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   
   if (pdatanotvalidvalue) {
      dataValueChecked_ = true;
      lastDataValueChecked_ = true;
      pnotvalidvaluetext->Enable(true);
   } else {
      dataValueChecked_ = false;
      lastDataValueChecked_ = false;
      pnotvalidvaluetext->Enable(false);
   }
   GET_CONTROL(*pToolWindow_,
         wxT("ID_NOT_VALID_VALUE_CHECKBOX"), wxCheckBox)->SetValue(dataValueChecked_);
}

/** carga datos de Modelo de Mapa */
void RasterProperties::LoadDnValueLimits() {
   RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());
   wxXmlNode* pDataTypeNode = playerraster->GetNode(PATH_DATA_TYPE_NODE_PIPES);
   std::string datatype = pDataTypeNode->GetNodeContent().c_str();
   GetDatatypeLimits(datatype, minNotValidValueLimit_, maxNotValidValueLimit_);
}

/**
 * Carga las caracteristicas de la imagen raster.
 */
void RasterProperties::LoadFeatures() {
   RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());

   if (playerraster != NULL && pRasterFeaturesWidget_ == NULL) {
      bool isRaw = (playerraster->GetTypeAsString().compare("raw") == 0);

      RasterElement::RasterPropertiesType props = playerraster->GetRasterProperties();

      pIOParams_->AddValue<std::string>(RasterFeaturesWidget::kFeatureDataType,
                                        GetFullDataTypeName(props.DataType));

      if (isRaw) {
         LoadRawMetadata(playerraster->GetNode(""));
      }

      pRasterFeaturesWidget_ = new RasterFeaturesWidget;
      pRasterFeaturesWidget_->SetIOParams(pIOParams_);
      pRasterFeaturesWidget_->SetChangedFlag(&rawDataModified_);
      AddControl(pRasterFeaturesWidget_, wxT("ID_RASTER_PROPERTIES_FEATURES_PANEL"));

      if (!isRaw) pRasterFeaturesWidget_->Disable();
   }
}

void RasterProperties::LoadNoValidValue() {
   RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());
   wxString lbl;
   std::string dataType;
   dataType_ = LoadDataType();
   wxXmlNode* pdatanotvalidvalue = playerraster->GetNode(
         PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue) {
      lbl = pdatanotvalidvalue->GetNodeContent();
      lastNotValidValue_ = lbl;
      notValidValue_ = lbl;
      GET_CONTROL(*pToolWindow_,
                  wxT("ID_NOT_VALID_VALUE"), wxTextCtrl)->SetValue(lbl);
   }
}

/** */
void RasterProperties::OnNotDataValueCheck(wxCommandEvent &Event) {
   wxCheckBox* pnotvalidvaluecheck = XRCCTRL(*pToolWindow_,
                                             "ID_NOT_VALID_VALUE_CHECKBOX", wxCheckBox);
   wxTextCtrl *pnotvalidvaluetext = XRCCTRL(*pToolWindow_, "ID_NOT_VALID_VALUE",
                                            wxTextCtrl);
   dataValueChecked_ = pnotvalidvaluecheck->IsChecked();
   if (dataValueChecked_) {
      // Para este caso si se pone en true el checkbox, se crea un nodo en el xml del proyecto
      // para guardar el valor del textbox
      pnotvalidvaluetext->Enable(true);
      if (notValidValue_.IsEmpty()) {
         notValidValue_ = "0";
      }
      pnotvalidvaluetext->SetValue(notValidValue_);
   } else {
      pnotvalidvaluetext->Enable(false);
   }
   lastDataValueChecked_ = dataValueChecked_;
   modified_ = true;
}

void RasterProperties::OnNotValueValidEventHandler(wxCommandEvent &Event) {
   wxTextCtrl *pnotvalidvaluetext = XRCCTRL(*pToolWindow_, "ID_NOT_VALID_VALUE",
                                            wxTextCtrl);
   notValidValue_.clear();
   notValidValue_ = pnotvalidvaluetext->GetValue();
   if (lastNotValidValue_ != notValidValue_) {
      lastNotValidValue_ = notValidValue_;
      modified_ = true;
   } else {
      modified_ = false;
   }
}

/**
 * Evento para el cambio de dimensiones (ancho y alto)
 */
void RasterProperties::OnDimensionChangedHandler(wxCommandEvent& Event) {
   dimModified_ = true;
}

/**
 * Evento para el cambio en la cantidad de bandas.
 */
void RasterProperties::OnBandsCountChangedHandler(wxCommandEvent& Event) {
   dimModified_ = true;

   Table* ptable = pTablePart_->GetTable();
   wxTextCtrl* ptxt = XRCCTRL(*pToolWindow_, "ID_RASTER_PROPERTIES_BANDS_TEXT",
                              wxTextCtrl);

   int bandscount = StringToNumber<int>(ptxt->GetValue().c_str());

   if (bandscount <= 0) {
      ptxt->ChangeValue(NumberToString<int>(ptable->GetRows()));
      return;
   }

   if (bandscount > 0 && bandscount < ptable->GetRows()) {
      for (int ix = ptable->GetRows(); ix >= bandscount; --ix) {
         ptable->DeleteRow(ix);
      }
   } else if (bandscount > 0 && bandscount > ptable->GetRows()) {
      for (int ix = ptable->GetRows(); ix < bandscount; ++ix) {
         std::stringstream ss;
         ss << "Banda #" << (ix + 1);
         ptable->AppendRow();

         std::string name = ss.str();
         ptable->SetCellValue(0, ix, name);

         std::string validity = "1";
         ptable->SetCellValue(1, ix, validity);
      }
   }
}

/** Condiciones para la invalidacion de datos de entrada **/
bool RasterProperties::HasValidData() {
   errorMessage_ = wxT("");
   // si la configuracion del valor no valido esta deshabilitada
   // no tiene sentido validarla (este caso se da cuando el tipo
   // de dato es double)
   if (IsNaNEnabled() && dataValueChecked_
         && (!IsbetweenNotValidValueLimts(notValidValue_) || notValidValue_.IsEmpty())) {
      errorMessage_ = message_INVALID_NO_DATA_VALUE;
      return false;
   }
   return true;
}

/**
 * Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable).
 */
void RasterProperties::Update(NotificationInterface* pNotification) {
   bandsDataModified_ = true;
}

/**
 * Guarda los cambios realizados.
 */
bool RasterProperties::CommitChanges() {
   if (!HasValidData()) {
      SHOW_ERROR(_(errorMessage_));
      FLUSH();
      return false;
   }
   // Verifico que alguna banda se encuentre marcada como valida
   bool invalidbands = true;
   for (int r = 0; invalidbands && r < pTable_->GetRows(); ++r) {
      std::string validity;
      pTable_->GetCellValue(BandDriver::kColumnValidity, r, validity);
      invalidbands &= StringToNumber<int>(validity) == 0;
   }
   if (invalidbands) {
      SHOW_ERROR(_(message_ALL_BANDS_INVALID_ERROR));
      return false;
   }
   RasterElement* praster = dynamic_cast<RasterElement*>(
         pDatasource_ ? pDatasource_->GetElement() : pElement_);

   if (dimModified_) {
      RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());
      SaveDimToElement(playerraster);
   }

   if (rawDataModified_) {
      RasterElement* playerraster = dynamic_cast<RasterElement*>(pLayer_->GetElement());
      SaveRawMetadataToElement(playerraster);
   }

   if (modified_) {
      // Selecciono el padre
      wxXmlNode *pparentnode = praster->GetNode(PATH_PROPERTIES_NODE_PIPES);
      if (dataValueChecked_ == true && pparentnode) {
         praster->AddNode(pparentnode, NOT_VALID_DATA_VALUE, notValidValue_);
         if (pLayer_) {
            wxXmlNode *playerparentnode = pLayer_->GetElement()->GetNode(
                  PATH_PROPERTIES_NODE_PIPES);
            pLayer_->GetElement()->AddNode(playerparentnode, NOT_VALID_DATA_VALUE, notValidValue_);
         }
      } else if (dataValueChecked_ == false && pparentnode) {
         wxXmlNode *pdatanotvalidvalue = praster->GetNode(
               PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
         praster->RemoveNode(pparentnode, pdatanotvalidvalue);
         if (pLayer_) {
            wxXmlNode *playerparentnode = pLayer_->GetElement()->GetNode(
                  PATH_PROPERTIES_NODE_PIPES);
            wxXmlNode *pdatanotvalidvalue = pLayer_->GetElement()->GetNode(
                  PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
            pLayer_->GetElement()->RemoveNode(playerparentnode, pdatanotvalidvalue);
         }
      }
   }

   if (bandsDataModified_) {
      pTablePart_->CommitChanges();
        RasterElement* pdsraster = dynamic_cast<RasterElement*>(pDatasource_->GetElement());
      std::vector<SuriObject::UuidType> layers =
           pDataViewManager_->GetViewcontextManager()->GetAssociatedLayer(pDatasource_->GetId());
      for (size_t j = 0; j < layers.size(); ++j) {
         LayerInterface* player = pDataViewManager_->GetViewcontextManager()->GetLayer(layers[j]);
         RasterElement* playerraster = dynamic_cast<RasterElement*>(player->GetElement());
         for (int i = 0; i < pdsraster->GetBandCount(); ++i) {
            std::string bandname;
            pdsraster->GetBandName(bandname, i);
            playerraster->SetBandName(bandname, i);
            int valid = 1;
            pdsraster->GetBandValidity(valid, i);
            playerraster->SetBandValidity(valid, i);
         }
      }
   }

   if (modified_ || dimModified_ || rawDataModified_ || bandsDataModified_) {
      praster->SetChanged();
      praster->SendViewerUpdate();
   }

   modified_ = false;
   dimModified_ = false;
   rawDataModified_ = false;
   bandsDataModified_ = false;

   return true;
}

/**
 * Indica si el Part tiene cambios para salvar.
 */
bool RasterProperties::HasChanged() {
   return modified_ || dimModified_ || rawDataModified_ || bandsDataModified_;
}

/**
 * Verifica si el valor dado esta dentro de los valores permitidos.
 */
bool RasterProperties::IsbetweenNotValidValueLimts(const wxString &NotValidValue) {
   double val = 0.0;

   if (!NotValidValue.ToDouble(&val))
      return false;

   bool isnaninf = std::isnan(val) || std::isinf(val);
   bool hasnaninf = HasNaN(dataType_) || HasINF(dataType_);

   if (isnaninf && !hasnaninf)
      return false;

   if (isnaninf && hasnaninf)
      return true;

   if (val < minNotValidValueLimit_ || maxNotValidValueLimit_ < val) {
      return false;
   }

   return true;
}

/**
 * Verifica si esta habilitada la carga de valor no valido.
 */
bool RasterProperties::IsNaNEnabled() {
   wxCheckBox* pchk = XRCCTRL(*pToolWindow_, "ID_NOT_VALID_VALUE_CHECKBOX", wxCheckBox);
   bool chkenabled = pchk->IsEnabled();

   wxTextCtrl* ptxt = XRCCTRL(*pToolWindow_, "ID_NOT_VALID_VALUE", wxTextCtrl);
   bool txtenabled = ptxt->IsEnabled();

   return chkenabled && txtenabled;
}

/**
 * Guarda los cambios de dimensiones al elemento.
 */
void RasterProperties::SaveDimToElement(RasterElement* pRaster) {
   int width = 0, height = 0;

   wxTextCtrl* ptxt = XRCCTRL(*pToolWindow_, "ID_RASTER_PROPERTIES_PIXELS_TEXT",
                              wxTextCtrl);
   width = StringToNumber<int>(ptxt->GetValue().c_str());

   ptxt = XRCCTRL(*pToolWindow_, "ID_RASTER_PROPERTIES_LINES_TEXT", wxTextCtrl);
   height = StringToNumber<int>(ptxt->GetValue().c_str());

   pRaster->SetRasterSize(width, height);
}

/**
 * Carga la metadatos para elementos raster crudos.
 */
void RasterProperties::LoadRawMetadata(wxXmlNode* pNode) {
   if (pNode == NULL) return;

   wxXmlNode* pchild = pNode->GetChildren();
   while (pchild) {
      if (pchild->GetName().Cmp(FILE_NODE) == 0) {
         wxXmlNode* prawmeta = pchild->GetChildren();
         while (prawmeta && prawmeta->GetName().Cmp(RAW_METADATA))
            prawmeta = prawmeta->GetNext();
         GetRasterMetadata(prawmeta);
      }

      pchild = pchild->GetNext();
   }
}

/**
 * Metodo auxiliar para extraer informacion de metadata en el xml.
 */
void RasterProperties::GetRasterMetadata(wxXmlNode* pMetadataNode) {
   wxXmlNode* pcurrent = pMetadataNode ? pMetadataNode->GetChildren() : NULL;
   while (pcurrent) {
      std::string key = pcurrent->GetName().c_str();
      std::string value = pcurrent->GetNodeContent().c_str();
      pIOParams_->AddValue<std::string>(key, value);
      pcurrent = pcurrent->GetNext();
   }
}

/**
 * Metodo auxiliar para establecer informacion de metadata en el xml.
 */
void RasterProperties::SetRasterMetadata(wxXmlNode* pMetadataNode) {
   wxXmlNode* pcurrent = pMetadataNode ? pMetadataNode->GetChildren() : NULL;
   while (pcurrent) {
      std::string key = pcurrent->GetName().c_str();
      std::string value;
      pIOParams_->GetValue<std::string>(key, value);
      if (key.compare(RasterFeaturesWidget::kFeatureDataType) == 0) {
         value = GetShortDataTypeName(value);
      }
      SetNodeTextContent(pcurrent, value);
      pcurrent = pcurrent->GetNext();
   }
}

/**
 * Guarda los metadatos para elementos raster crudos.
 */
void RasterProperties::SaveRawMetadataToElement(RasterElement* pRaster) {
   wxXmlNode* pnode = pRaster->GetNode("");

   if (pnode == NULL) return;

   wxXmlNode* pchild = pnode->GetChildren();
   while (pchild) {
      if (pchild->GetName().Cmp(FILE_NODE) == 0) {
         wxXmlNode* prawmeta = pchild->GetChildren();
         while (prawmeta && prawmeta->GetName().Cmp(RAW_METADATA))
            prawmeta = prawmeta->GetNext();
         SetRasterMetadata(prawmeta);
      }

      pchild = pchild->GetNext();
   }
}

}  // namespace suri
