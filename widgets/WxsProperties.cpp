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
