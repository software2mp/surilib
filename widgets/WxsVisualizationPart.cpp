/*! \file WxsVisualizationPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
