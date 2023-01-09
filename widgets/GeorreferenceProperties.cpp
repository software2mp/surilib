/*! \file GeorreferenceProperties.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "GeorreferenceProperties.h"

#include "RasterRenderer.h"
#include "SpatialReference.h"
#include "UniversalGraphicalComponentPart.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/BasicLibraryItemSelector.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/RasterElement.h"
#include "suri/RasterSpatialModel.h"

#include "resources.h"

#include "wx/xrc/xmlres.h"

#define SRS_LIBRARY_CODE "srs"

void InitGeorreferencePropertiesXmlResource();

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(GeorreferencePropertiesEvent, GeorreferenceProperties)
   IMPLEMENT_EVENT_CALLBACK(OnModelTextChangedHandler,
         OnModelTextChangedHandler(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

/**
 * Ctor. default.
 * @param[in] pElement elemento sobre el que se quieren mostrar las propiedades
 *             de georreferencia
 */
GeorreferenceProperties::GeorreferenceProperties(Element *pElement,
                                                 LibraryManager* pLibraryManager) :
      Part(true, false),
      NEW_EVENT_OBJECT(GeorreferencePropertiesEvent),
      pElement_(pElement), pDatasourceManager_(NULL), pDatasource_(NULL),
      pLibraryManager_(pLibraryManager), srsChanged_(false), modelChanged_(false),
      selectedPixelX_(0.0), selectedPixelY_(0.0), selectedNorth_(0.0),
      selectedModelP_(0.0), selectedModelL_(0.0), selectedModelX_(0.0),
      selectedModelY_(0.0), hasSrs_(false) {
   windowTitle_ = _(caption_GEO_MODEL_PROPERTIES);
   InitInternals();
}

/**
 * Ctor.
 */
GeorreferenceProperties::GeorreferenceProperties(
      DatasourceManagerInterface* pDatasourceManager, DatasourceInterface* pDatasource,
      LibraryManager* pLibraryManager) :
      Part(true, false),
      NEW_EVENT_OBJECT(GeorreferencePropertiesEvent),
      pElement_(NULL), pDatasourceManager_(pDatasourceManager),
      pDatasource_(pDatasource), pLibraryManager_(pLibraryManager), srsChanged_(false),
      modelChanged_(false), selectedPixelX_(0.0), selectedPixelY_(0.0), selectedNorth_(0.0),
      selectedModelP_(0.0), selectedModelL_(0.0), selectedModelX_(0.0), selectedModelY_(0.0),
      hasSrs_(false) {
   windowTitle_ = _(caption_GEO_MODEL_PROPERTIES);
   InitInternals();
}

/**
 * Dtor.
 */
GeorreferenceProperties::~GeorreferenceProperties() {
   DELETE_EVENT_OBJECT
   ;
}

/**
 * Inicializa los controles
 */
void GeorreferenceProperties::SetInitialValues() {
   RollbackChanges();
}

/**
 * Devuelve el estado del Part.
 * @return true / false si el part esta o no habilitado
 */
bool GeorreferenceProperties::IsEnabled() {
   return hasSrs_;
}

/**
 * Salva los cambios realizados en el Part.
 */
bool GeorreferenceProperties::CommitChanges() {
   RasterElement* praster = dynamic_cast<RasterElement*>(
               (pDatasource_) ? pDatasource_->GetElement() : pElement_);

   if (praster != NULL) {
      if (srsChanged_ && !selectedSrs_.empty()) {
         SaveSrsToElement(praster);
         SaveSrsToFile(praster);
      }

      if (modelChanged_) {
         SaveModelToElement(praster);
         SaveModelToFile(praster);
      }
   }

   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return true siempre
 */
bool GeorreferenceProperties::RollbackChanges() {
   LoadGeograficData();
   LoadProjectionData();
   LoadImageModel();
   LoadParametersTree();
   return true;
}

/**
 * Implementacion del metodo de creacion de ventana de Widget.
 */
bool GeorreferenceProperties::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, wxT("ID_GEORREFERENCE_PROPERTIES_PART"));

   Element* pelement = (pDatasource_) ? pDatasource_->GetElement() : pElement_;

   Library* plibrary = pLibraryManager_->GetLibraryByCode(
         SRS_LIBRARY_CODE, LibraryManager::AccessKey(true));

   RasterElement* praster = dynamic_cast<RasterElement*>(pelement);

   if (praster != NULL) {
      // El id ID_LIBRARY_SELECTION_PANEL esta repetido a proposito. No cambiar.
      BasicLibraryItemSelector* psrsselectionpart = new BasicLibraryItemSelector(
            this, plibrary->GetId(), pLibraryManager_, "ID_LIBRARY_SELECTION_PANEL",
            "ID_LIBRARY_SELECTION_PANEL", false,
            UniversalGraphicalComponentPart::RoWithFFAndNoLabel);

      AddControl(psrsselectionpart, wxT("ID_RS_LIBRARY_PANEL"));

      std::string aid = SpatialReference::GetAuthorityId(
            pelement->GetSpatialReference().mb_str());
      psrsselectionpart->SetActiveItem(plibrary->GetItemByPrincipal(aid));
   }

   SetupModelTextboxHandler("ID_GEO_PROP_PIXEL_X_TEXT");
   SetupModelTextboxHandler("ID_GEO_PROP_PIXEL_Y_TEXT");
   SetupModelTextboxHandler("ID_GEO_PROP_NORTH_ORIENTATION_TEXT");
   SetupModelTextboxHandler("ID_GEO_PROP_MODEL_P_TEXT");
   SetupModelTextboxHandler("ID_GEO_PROP_MODEL_L_TEXT");
   SetupModelTextboxHandler("ID_GEO_PROP_MODEL_X_TEXT");
   SetupModelTextboxHandler("ID_GEO_PROP_MODEL_Y_TEXT");

   return true;
}

/** Retorna el icono de la herramienta */
void GeorreferenceProperties::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_GEORREFERENCE_PROPERTIES, ToolBitmap);
}

/**
 * Carga controles de Sistema Geografico
 */
void GeorreferenceProperties::LoadGeograficData() {
   Element* pelement = (pDatasource_)?pDatasource_->GetElement():pElement_;
   std::string wkt = pelement->GetSpatialReference().c_str();
   GET_CONTROL( (*GetWidget()->GetWindow() ), wxT("ID_LBL_GEORREF_NAME"), wxStaticText)
      ->SetLabel(_(SpatialReference::GetGeoCoordinateSystemName(wkt).c_str() ));
   GET_CONTROL( (*GetWidget()->GetWindow() ), wxT("ID_LBL_GEORREF_UNITS"), wxStaticText)
      ->SetLabel(_(SpatialReference::GetGeoCoordinateSystemUnits(wkt).c_str() ));
   GET_CONTROL( (*GetWidget()->GetWindow() ), wxT("ID_LBL_GEORREF_DATUM"), wxStaticText)
      ->SetLabel(SpatialReference::GetGeoCoordinateSystemDatumName(wkt));
   GET_CONTROL( (*GetWidget()->GetWindow() ), wxT("ID_LBL_GEORREF_SPHEROID"), wxStaticText)
      ->SetLabel(_(SpatialReference::GetGeoCoordinateSystemSpheroidName(wkt).c_str() ));
}

/**
 * Carga controles de Sistema Proyectado
 */
void GeorreferenceProperties::LoadProjectionData() {
   Element *pelement = (pDatasource_) ? pDatasource_->GetElement() : pElement_;
   std::string wkt = pelement->GetSpatialReference().c_str();
   GET_CONTROL( (*GetWidget()->GetWindow() ), wxT("ID_LBL_PROJECTION_NAME"), wxStaticText)->SetLabel(
         _(SpatialReference::GetProjCoordinateSystemName(wkt).c_str()));
   GET_CONTROL( (*GetWidget()->GetWindow() ), wxT("ID_LBL_PROJECTION_UNITS"), wxStaticText)->SetLabel(
         _(SpatialReference::GetProjCoordinateSystemUnits(wkt).c_str()));
}

/**
 * Carga datos de modelo de imagen.
 */
void GeorreferenceProperties::LoadImageModel() {
   wxString lbl;

   RasterElement* pelement = dynamic_cast<RasterElement*>(
         pDatasource_ ? pDatasource_->GetElement() : pElement_);

   wxWindow* pwin = GetWidget()->GetWindow();

   if (pelement == NULL) {
      GET_CONTROL(*pwin, wxT("ID_GEO_PROP_PANEL_RASTER_MODEL"), wxPanel)->Hide();
      return;
   }

   RasterSpatialModel::Parameters smparameters = RasterSpatialModel::WktToParameters(
         pelement->GetRasterModel());

   lbl = wxString::Format(wxT("%.2f"), smparameters.pixelSizes_[0]);
   GET_CONTROL(*pwin, wxT("ID_GEO_PROP_PIXEL_X_TEXT"), wxTextCtrl)->ChangeValue(lbl);

   lbl = wxString::Format(wxT("%.2f"), smparameters.pixelSizes_[1]);
   GET_CONTROL(*pwin, wxT("ID_GEO_PROP_PIXEL_Y_TEXT"), wxTextCtrl)->ChangeValue(lbl);

   lbl = wxString::Format(wxT("%.2f"), smparameters.rotation_);
   GET_CONTROL(*pwin, wxT("ID_GEO_PROP_NORTH_ORIENTATION_TEXT"), wxTextCtrl)->ChangeValue(lbl);

   lbl = wxString::Format(wxT("%.2f"), smparameters.tiePointPl_.x_);
   GET_CONTROL(*pwin, wxT("ID_GEO_PROP_MODEL_P_TEXT"), wxTextCtrl)->ChangeValue(lbl);

   lbl = wxString::Format(wxT("%.2f"), smparameters.tiePointPl_.y_);
   GET_CONTROL(*pwin, wxT("ID_GEO_PROP_MODEL_L_TEXT"), wxTextCtrl)->ChangeValue(lbl);

   lbl = wxString::Format(wxT("%.2f"), smparameters.tiePointXy_.x_);
   GET_CONTROL(*pwin, wxT("ID_GEO_PROP_MODEL_X_TEXT"), wxTextCtrl)->ChangeValue(lbl);

   lbl = wxString::Format(wxT("%.2f"), smparameters.tiePointXy_.y_);
   GET_CONTROL(*pwin, wxT("ID_GEO_PROP_MODEL_Y_TEXT"), wxTextCtrl)->ChangeValue(lbl);
}

/**
 * Carga el arbol de parametros
 */
void GeorreferenceProperties::LoadParametersTree() {
   return;
}

/**
 * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
 * un boolean que indica si se trata de la primera seleccion (para que el part no piense
 * que posee cambios cuando en realidad es la seleccion inicial del CGU)
 */
void GeorreferenceProperties::SetActiveItem(const LibraryItem* pItem) {
   Element* pelement = (pDatasource_) ? pDatasource_->GetElement() : pElement_;
   std::string elementaid = SpatialReference::GetAuthorityId(
         pelement->GetSpatialReference().mb_str());

   const LibraryItemAttribute* pattr = pItem ? pItem->GetAttribute("WKT") : NULL;
   std::string itemaid = pattr ? SpatialReference::GetAuthorityId(pattr->GetValue()) : "";

   // La verificacion contra ":" es para las imagenes raster que vienen
   // sin sistema de referencia.
   if (IsEnabled() && elementaid.compare(":") != 0 && elementaid.compare(itemaid) != 0) {
      srsChanged_ = true;
      selectedSrs_ = pattr ? pattr->GetValue() : "";
   } else {
      srsChanged_ = false;
      selectedSrs_ = "";
   }
}

/**
 * Maneja aviso de que alguno de los textboxes del modelo
 * cambio.
 */
void GeorreferenceProperties::OnModelTextChangedHandler(wxCommandEvent &Event) {
   RasterElement* pelement = dynamic_cast<RasterElement*>(
         pDatasource_ ? pDatasource_->GetElement() : pElement_);

   RasterSpatialModel::Parameters smparameters = RasterSpatialModel::WktToParameters(
         pelement->GetRasterModel());

   modelChanged_ = false;

   double value = 0.0;

   value = StringToNumber<double>(GetTextboxValue("ID_GEO_PROP_PIXEL_X_TEXT"));
   if (selectedPixelX_ != value) {
      modelChanged_ = true;
      selectedPixelX_ = value;
   }

   value = StringToNumber<double>(GetTextboxValue("ID_GEO_PROP_PIXEL_Y_TEXT"));
   if (selectedPixelY_ != value) {
      modelChanged_ = true;
      selectedPixelY_ = value;
   }

   value = StringToNumber<double>(GetTextboxValue("ID_GEO_PROP_NORTH_ORIENTATION_TEXT"));
   if (selectedNorth_ != value) {
      modelChanged_ = true;
      selectedNorth_ = value;
   }

   value = StringToNumber<double>(GetTextboxValue("ID_GEO_PROP_MODEL_P_TEXT"));
   if (selectedModelP_ != value) {
      modelChanged_ = true;
      selectedModelP_ = value;
   }

   value = StringToNumber<double>(GetTextboxValue("ID_GEO_PROP_MODEL_L_TEXT"));
   if (selectedModelL_ != value) {
      modelChanged_ = true;
      selectedModelL_ = value;
   }

   value = StringToNumber<double>(GetTextboxValue("ID_GEO_PROP_MODEL_X_TEXT"));
   if (selectedModelX_ != value) {
      modelChanged_ = true;
      selectedModelX_ = value;
   }

   value = StringToNumber<double>(GetTextboxValue("ID_GEO_PROP_MODEL_Y_TEXT"));
   if (selectedModelY_ != value) {
      modelChanged_ = true;
      selectedModelY_ = value;
   }
}

/**
 *
 */
void GeorreferenceProperties::SetupModelTextboxHandler(const std::string& Id) {
   GET_CONTROL(*pToolWindow_,
         Id.c_str(), wxTextCtrl)->Connect(
         wxEVT_COMMAND_TEXT_UPDATED,
         wxCommandEventHandler(GeorreferencePropertiesEvent::OnModelTextChangedHandler),
         NULL, pEventHandler_);
}

/**
 * Obtiene el valor del textbox.
 */
std::string GeorreferenceProperties::GetTextboxValue(const std::string& Id) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, Id.c_str(), wxTextCtrl);
   return ptext->GetValue().mb_str();
}

/**
 * Inicializa valores internos.
 */
void GeorreferenceProperties::InitInternals() {
   Element* pelement = (pDatasource_) ? pDatasource_->GetElement() : pElement_;
   // Si fue deshabilitado por un SetEnabled, le hace honor
   hasSrs_ =
         Part::IsEnabled()
               && (SpatialReference::IsGeographic(pelement->GetSpatialReference().c_str())
                     || SpatialReference::IsProjected(pelement->GetSpatialReference().c_str()));
}

/** */
void GeorreferenceProperties::SaveSrsToElement(RasterElement* pRaster) {
   pRaster->SetSpatialReference(selectedSrs_);
}

/** */
void GeorreferenceProperties::SaveSrsToFile(RasterElement* pRaster) {
   std::string filepath = RasterRenderer::GenerateImageId(pRaster->GetUrl().c_str());
   Image* pimage = Image::Open(filepath, Image::WriteOnly);
   if (pimage) {
      pimage->SetSrs(pRaster->GetSpatialReference().mb_str());
      Image::Close(pimage);
   }
}

/** */
void GeorreferenceProperties::SaveModelToElement(RasterElement* pRaster) {
   RasterSpatialModel::Parameters smparameters = RasterSpatialModel::WktToParameters(
         pRaster->GetRasterModel());

   smparameters.pixelSizes_[0] = selectedPixelX_;
   smparameters.pixelSizes_[1] = selectedPixelY_;
   smparameters.rotation_ = selectedNorth_;
   smparameters.tiePointPl_.x_ = selectedModelP_;
   smparameters.tiePointPl_.y_ = selectedModelL_;
   smparameters.tiePointXy_.x_ = selectedModelX_;
   smparameters.tiePointXy_.y_ = selectedModelY_;

   smparameters.SetDirty();

   pRaster->SetRasterModel(RasterSpatialModel::ParametersToWkt(smparameters));
}

/** */
void GeorreferenceProperties::SaveModelToFile(RasterElement* pRaster) {
   Image* pimage = Image::Open(RasterRenderer::GenerateImageId(pRaster->GetUrl().c_str()),
                               Image::WriteOnly);

   if (pimage) {
      RasterSpatialModel* pmodel = RasterSpatialModel::Create(
            pRaster->GetRasterModel());

      double geotrans[6] = { 0.0 };
      pmodel->GetGeoTransform(geotrans);

      pimage->SetGeoTransform(geotrans);

      Image::Close(pimage);
   }
}

}  // namespace suri
