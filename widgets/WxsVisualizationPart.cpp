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

#include "WxsVisualizationPart.h"

// Includes standard

// Includes Suri
#include "WxsElement.h"
#include "WxsRenderer.h"
#include "XrcWidget.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// Includes App

// Defines

namespace suri {

/** Ctor */
WxsVisualizationPart::WxsVisualizationPart(bool Enable, bool Modified) :
      Part(wxT("ID_WXS_VISUALIZATION_PANEL"), _(caption_WMS_VISUALIZATION), Enable,
           Modified) {
   pWxsElement_ = NULL;
}

/** Ctor */
WxsVisualizationPart::WxsVisualizationPart(WxsElement* pWxsElement, bool Enable,
                                           bool Modified) :
      Part(wxT("ID_WXS_VISUALIZATION_PANEL"), _(caption_WMS_VISUALIZATION), Enable,
           Modified) {
   pWxsElement_ = pWxsElement;
   parameters_ = WxsRenderer::GetParameters(
         pWxsElement_->GetNode(), pWxsElement_->GetMetadataMap());
   widgetParameters_ = parameters_;
}

/** Dtor */
WxsVisualizationPart::~WxsVisualizationPart() {
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return bool que indica si hubo un cambio
 */
bool WxsVisualizationPart::HasChanged() {
   return !(ReadParametersFromWidget() && parameters_ == widgetParameters_);
}

/**
 * Actualiza los cambios en parameters. Si pElement_!=NULL
 * actualiza el nodo wxs del elemento.
 * @return indica si pudo guardar los valores.
 */
bool WxsVisualizationPart::CommitChanges() {
   if (!ReadParametersFromWidget()) return false;

   parameters_ = widgetParameters_;
   if (pWxsElement_) {
      // Agrega el nodo combinacion al nodo renderizacion
      wxXmlNode *pwxsnode = WxsRenderer::GetXmlNode(parameters_);
      wxXmlNode *prenderernode = pWxsElement_->GetNode(RENDERIZATION_NODE);
      if (!prenderernode) return false;
      pWxsElement_->AddNode(prenderernode, pwxsnode);
      pWxsElement_->SetChanged();
      pWxsElement_->SendViewerUpdate();
   }
   return true;
}

/**
 * Remplaza el contenido de los controles con los valores
 * en parameters_.
 * @return bool que indica si pudo actualizar los controles
 */
bool WxsVisualizationPart::RollbackChanges() {
   widgetParameters_ = parameters_;
   return UpdateParametersToWidget();
}

/** Inicializa el part */
void WxsVisualizationPart::SetInitialValues() {
   RollbackChanges();
}

/** Actualiza el estado de la parte */
void WxsVisualizationPart::Update() {
}

/**
 * Configura los parametros ingresados por usuario
 * @param[out] WxsParameters con datos configurados en commit.
 */
void WxsVisualizationPart::ConfigureParameters(WxsRenderer::Parameters &Parameters) {
   Parameters.style_ = widgetParameters_.style_;
}

/**
 * Configura los parametros con datos en control
 * @return bool que indica si hubo un error
 */
bool WxsVisualizationPart::ReadParametersFromWidget() {
   return GetCtrlValue(GetWidget(), wxT("ID_STYLE_COMBOBOX"), widgetParameters_.style_);
}

/**
 * Actualiza el control con datos en los parametros
 * @return bool que indica si hubo un error
 */
bool WxsVisualizationPart::UpdateParametersToWidget() {
   return SetCtrlValue(GetWidget(), wxT("ID_STYLE_COMBOBOX"), widgetParameters_.style_);;
}

}  // end namespace suri
