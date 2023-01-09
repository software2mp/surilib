/*! \file WxsProperties.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "WxsProperties.h"

// Includes standard
#include <sstream>

// Includes Suri
#include "suri/messages.h"
#include "suri/Wkt.h"
#include "WxsElement.h"
#include "XrcWidget.h"

// Includes Wx

// Includes App

// Defines

// Valores default hasta que existan las bibliotecas.
#define WMS_FORMAT_INITIAL_VALUE "image/jpeg"
#define WMS_SPATIALREFERENCE_INITIAL_VALUE "EPSG:4326"
#define WMS_EXTENT_INITIAL_VALUE -180, 90, 180, -90

namespace suri {
/** Ctor */
WxsProperties::WxsProperties(bool Enable, bool Modified) :
      Part(wxT("ID_WXS_PROPERTIES_PANEL"), _(caption_WMS_PROPERTIES), Enable, Modified) {
   pWxsElement_ = NULL;

   parameters_.format_ = WMS_FORMAT_INITIAL_VALUE;
   parameters_.serviceSRId_ = WMS_SPATIALREFERENCE_INITIAL_VALUE;
   parameters_.extent_ = Subset(WMS_EXTENT_INITIAL_VALUE);
}

/** Ctor */
WxsProperties::WxsProperties(WxsElement* pWxsElement, bool Enable, bool Modified) :
      Part(wxT("ID_WXS_PROPERTIES_PANEL"), _(caption_WMS_PROPERTIES), Enable, Modified) {
   pWxsElement_ = pWxsElement;
   parameters_ = WxsRenderer::GetParameters(
         pWxsElement_->GetNode(), pWxsElement_->GetMetadataMap());
   widgetParameters_ = parameters_;
}

/** Dtor */
WxsProperties::~WxsProperties() {
}

/** Indica si el Part tiene cambios para salvar. */
bool WxsProperties::HasChanged() {
   return !(ReadParametersFromWidget() && widgetParameters_ == parameters_);
}

/** Salva los cambios realizados en el Part. */
bool WxsProperties::CommitChanges() {
   if (!ReadParametersFromWidget()) return false;

   parameters_ = widgetParameters_;
   if (pWxsElement_) {
      // Agrega el nodo combinacion al nodo renderizacion
      wxXmlNode *pwxsnode = WxsRenderer::GetXmlNode(parameters_);
      wxXmlNode *prenderernode = pWxsElement_->GetNode(RENDERIZATION_NODE);
      if (!prenderernode) return false;
      pWxsElement_->AddNode(prenderernode, pwxsnode);
      pWxsElement_->SetElementExtent(parameters_.extent_);
      pWxsElement_->SetChanged();
      pWxsElement_->SendViewerUpdate();
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool WxsProperties::RollbackChanges() {
   widgetParameters_ = parameters_;
   return UpdateParametersToWidget();
}

/** Inicializa el part */
void WxsProperties::SetInitialValues() {
   RollbackChanges();
}

/** Actualiza el estado de la parte */
void WxsProperties::Update() {
}

/**
 * Configura los parametros ingresados por usuario
 * @param[out] WxsParameters con datos configurados en commit.
 */
void WxsProperties::ConfigureParameters(WxsRenderer::Parameters &Parameters) {
   Parameters.serviceSRId_ = widgetParameters_.serviceSRId_;
   Parameters.format_ = widgetParameters_.format_;
   Parameters.extent_ = widgetParameters_.extent_;
}

/**
 * Configura los parametros con datos en control
 * @return bool que indica si hubo un error
 */
bool WxsProperties::ReadParametersFromWidget() {
   // Analizo si se modifico el part activo
   bool result = GetCtrlValue(GetWidget(), wxT("ID_SPATIAL_REFERENCE_COMBOBOX"),
                              widgetParameters_.serviceSRId_);
   result = result
         && GetCtrlValue(GetWidget(), wxT("ID_FORMAT_COMBOBOX"),
                         widgetParameters_.format_);

   double minx, miny, maxx, maxy;

   result = result && GetCtrlValue(GetWidget(), wxT("ID_BBOX_MIN_X_TEXT"), minx);
   result = result && GetCtrlValue(GetWidget(), wxT("ID_BBOX_MIN_Y_TEXT"), miny);
   result = result && GetCtrlValue(GetWidget(), wxT("ID_BBOX_MAX_X_TEXT"), maxx);
   result = result && GetCtrlValue(GetWidget(), wxT("ID_BBOX_MAX_Y_TEXT"), maxy);

   // TODO El subset se configura para que funcione con imagenes en el sistema
   // de refernecia EPSG:4326.
   // Si la fuente wms tiene los ejes de la imagen proyectada en un
   // sentido/direccion diferente, falla la renderizacion.
   widgetParameters_.extent_.lr_.x_ = maxx;
   widgetParameters_.extent_.lr_.y_ = miny;
   widgetParameters_.extent_.ul_.x_ = minx;
   widgetParameters_.extent_.ul_.y_ = maxy;

   return result;
}

/**
 * Actualiza el control con datos en los parametros
 * @return bool que indica si hubo un error
 */
bool WxsProperties::UpdateParametersToWidget() {
   bool result = SetCtrlValue(GetWidget(), wxT("ID_SPATIAL_REFERENCE_COMBOBOX"),
                              widgetParameters_.serviceSRId_);
   result = result
         && SetCtrlValue(GetWidget(), wxT("ID_FORMAT_COMBOBOX"),
                         widgetParameters_.format_);

   double minx, miny, maxx, maxy;
   maxx = widgetParameters_.extent_.lr_.x_;
   miny = widgetParameters_.extent_.lr_.y_;
   minx = widgetParameters_.extent_.ul_.x_;
   maxy = widgetParameters_.extent_.ul_.y_;

   result = result && SetCtrlValue(GetWidget(), wxT("ID_BBOX_MIN_X_TEXT"), minx);
   result = result && SetCtrlValue(GetWidget(), wxT("ID_BBOX_MIN_Y_TEXT"), miny);
   result = result && SetCtrlValue(GetWidget(), wxT("ID_BBOX_MAX_X_TEXT"), maxx);
   result = result && SetCtrlValue(GetWidget(), wxT("ID_BBOX_MAX_Y_TEXT"), maxy);

   return result;
}

}  // end namespace suri
