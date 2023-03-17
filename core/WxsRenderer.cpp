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

#include "WxsRenderer.h"

// Include standard
#include <sstream>
// Includes suri
#include "suri/Image.h"
#include "suri/Element.h"
#include "suri/World.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"
#include "suri/XmlFunctions.h"
#include "suri/Uri.h"

// Define
#define MAX_PARAMETER_SIZE 256

#define WXSRENDERER_NODE "wxs"
#define WXS_STYLE_PROPERTY "estilo"
#define WXS_FORMAT_PROPERTY "formato"
#define WXS_SERVICE_SRID_PROPERTY "idrefespacial"
#define DEFAULT_RGB_BANDS "0 1 2"
#define WMS_SERVICEID "wms"

// ! Macro para registrar el renderer
AUTO_REGISTER_RENDERER(suri::WxsRenderer);

// ! namespace suri
namespace suri {

//! ctor
WxsRenderer::WxsRenderer() {

}

//! dtor
WxsRenderer::~WxsRenderer() {
}

/** 
 * Actualiza parametros de RasterRenderer con los de WxsRenderer
 * @param[in] Parameters parametros de WxsRenderer
 */
void WxsRenderer::UpdateRasterRendererParameters(WxsRenderer::Parameters Parameters) {
   RasterRenderer::parameters_.imageUrl_ = Parameters.imageUrl_;
   RasterRenderer::parameters_.rasterModel_ = Parameters.rasterModel_;
   RasterRenderer::parameters_.spatialModel_ = Parameters.spatialModel_;
   RasterRenderer::parameters_.bandCombination_ = Parameters.bandCombination_;
   RasterRenderer::parameters_.readFunction_ = Parameters.readFunction_;
}

/** 
 * Genera nodo XML a partir de los WxsParameters
 * @param[in] Parametros con datos del nodo
 * @return wxXmlNode nodo del elemento.
 * \attention el wxXmlNode retornado(y su eliminacion) es responsabilidad
 * del invocante
 */
wxXmlNode *WxsRenderer::GetXmlNode(const Parameters &WxsParameters) {
   wxXmlNode *prenderernode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, WXSRENDERER_NODE);

   prenderernode->AddProperty(WXS_STYLE_PROPERTY, WxsParameters.style_);
   prenderernode->AddProperty(WXS_FORMAT_PROPERTY, WxsParameters.format_);
   prenderernode->AddProperty(WXS_SERVICE_SRID_PROPERTY, WxsParameters.serviceSRId_);

   return prenderernode;
}

/**
 * Obtiene los parametros a partir del nodo de un elemento
 * @param[in] pNode Nodo con datos
 * return parametros con datos del nodo
 */
WxsRenderer::Parameters WxsRenderer::GetParameters(const wxXmlNode *pNode, Option Metadata) {
   WxsRenderer::Parameters wxsparam;
   wxsparam.Init(RasterRenderer::GetParameters(pNode));

   wxsparam.bandCombination_ = split<int>(DEFAULT_RGB_BANDS);

   std::string path = RENDERIZATION_NODE;
   path = path + wxT("|") + WXSRENDERER_NODE;
   const wxXmlNode* pwxsrenderernode = GetNodeByName(path, pNode);
   if (pwxsrenderernode) {
      wxsparam.style_ = Metadata.GetOption(WXS_STYLE_PROPERTY);
      wxsparam.format_ = Metadata.GetOption(WXS_FORMAT_PROPERTY);
      wxsparam.serviceSRId_ = Metadata.GetOption(WXS_SERVICE_SRID_PROPERTY);
   }

   path = DIMENSION_NODE;
   path = path + wxT("|") + EXTENT_NODE;
   const wxXmlNode* pextentnode = GetNodeByName(path, pNode);
   if (pextentnode) {
      //ul_.x_
      const wxXmlNode* pvaluenode = GetNodeByName(
            path + wxT("|") + UL_NODE + wxT("|") + X_NODE, pNode);
      if (pvaluenode)
         wxsparam.extent_.ul_.x_ = StringToNumber<double>(Metadata.GetOption("ulx"));
      //ul_.y_
      pvaluenode = GetNodeByName(path + wxT("|") + UL_NODE + wxT("|") + Y_NODE, pNode);
      if (pvaluenode)
         wxsparam.extent_.ul_.y_ = StringToNumber<double>(Metadata.GetOption("uly"));
      //lr_.x_
      pvaluenode = GetNodeByName(path + wxT("|") + LR_NODE + wxT("|") + X_NODE, pNode);
      if (pvaluenode)
         wxsparam.extent_.lr_.x_ = StringToNumber<double>(Metadata.GetOption("lrx"));
      //lr_.y_
      pvaluenode = GetNodeByName(path + wxT("|") + LR_NODE + wxT("|") + Y_NODE, pNode);
      if (pvaluenode)
         wxsparam.extent_.lr_.y_ = StringToNumber<double>(Metadata.GetOption("lry"));
   }

   return wxsparam;
}

/**
 * Usando el parameters obtenidos del pElement configuro el WxsRenderer creado.
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior(no se usa).
 * @return RasterRenderer Render creado usando el elemento
 * \attention el WxsRenderer retornado(y su eliminacion) es responsabilidad del invocante
 */
WxsRenderer *WxsRenderer::Create(Element *pElement, Renderer *pPreviousRenderer) const {
   wxXmlNode *pnode = pElement->GetNode(wxT(""));
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML en el elemento \"%s\"", pElement->GetName().c_str());
      return NULL;
   }

   WxsRenderer *prenderer = new WxsRenderer;
   prenderer->wxsParameters_ = GetParameters(pnode, pElement->GetMetadataMap());
   prenderer->UpdateRasterRendererParameters(prenderer->wxsParameters_);

   if (!prenderer->UpdateImage(NULL))
   REPORT_AND_FAIL_VALUE("D:RasterRenderer, error al abrir la imagen.", NULL);

   return prenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return Nombre del nodo del renderizador
 */
std::string WxsRenderer::CreatedNode() const {
   return WXSRENDERER_NODE;
}

//! Actualizacion de parametros para representar un elemento
/**
 * Abre una imgen wxs usando los parametros del renderizador y el mundo
 * que recibe por parametro.
 * Si tiene exito actualiza el atributo pImage_ y actualiza el RasterModel
 * en parameters_. Esto es necesario porque se lee la imagen en la menor
 * resolucion posible para optimizar el uso de ancho de banda, como para
 * distintos extent la dimension de la imagen obtenida es siempre la del cambas,
 * se modifica RasterModel para representar la nueva dimension de cada pixel.
 * @param[in]  pWorldWindow: puntero a la ventana que se quiere renderizar,
 * en caso de ser NULL se renderiza la imagen completa en dimension 256 x 256.
 * @return bool que indica si pudo actualizar la imagen correctamente.
 */
bool WxsRenderer::UpdateImage(const World* pWorldWindow) {

   Image* pimg = Image::Open(ConfigureXml(pWorldWindow));

   if (pimg) {
      Image::Close(pImage_);
      pImage_ = pimg;
      RasterRenderer::parameters_.rasterModel_ = pImage_->GetOption(
            Image::RasterMatrix);
   }

   return ValidateParameters(RasterRenderer::parameters_);
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]  pElement: puntero al elemento.
 */
void WxsRenderer::Update(Element *pElement) {
   Parameters wxsparameters = GetParameters(pElement->GetNode(), pElement->GetMetadataMap());

   if (wxsparameters.format_ != wxsParameters_.format_
         || wxsparameters.serviceSRId_ != wxsParameters_.serviceSRId_
         || wxsparameters.extent_ != wxsParameters_.extent_
         || wxsparameters.style_ != wxsParameters_.style_) changed_ = true;

   if (!wxsparameters.format_.empty() && !wxsparameters.serviceSRId_.empty()) {
      wxsParameters_ = wxsparameters;
      UpdateRasterRendererParameters(wxsParameters_);
   }
}

/**
 * Genera xml que requiere GDAL para abrir una imagen.
 * @param[in] pWorldWindow ventana que quiero renderizar. 
 * @return string con xml que reconoce GDAL para abrir la imagen.
 */
std::string WxsRenderer::ConfigureXml(const World* pWorldWindow) {
   std::string retval = wxT("");

   Uri uri(wxsParameters_.imageUrl_);
   std::map<std::string, std::string> queryparameters = StringToMap(
         uri.GetQuery(), Uri::QUERY_SEPARATOR);

   // Calculo el uri de la imagen
   std::string url;
   uri.GetDatasource(url);
   bool hasquery = false;

   std::map<std::string, std::string>::iterator it = queryparameters.begin();
   for (; it != queryparameters.end(); it++) {
      if (!CaseInsensitiveCompare(it->first, "service")
            && !CaseInsensitiveCompare(it->first, "layers")) {
         if (!hasquery) {
            url += Uri::QUERY_STARTER;
            hasquery = true;
         } else
            url += Uri::QUERY_SEPARATOR;
         url = url + it->first + "=" + it->second;
      }
   }

   // Me quedo con la inteseccion del extent wxs y la ventana
   int height = 256, width = 256;
   Subset window(wxsParameters_.extent_);
   if (pWorldWindow) {
      Subset worldwindow;
      pWorldWindow->GetWindow(worldwindow);
      window = Intersect(worldwindow, window);

      pWorldWindow->GetViewport(width, height);

      // En caso que la interseccion sea menor que la ventana actualizo dimension
      // de imagen pedida para mantener proporcion.
      Dimension dimwindow(window);
      Dimension dimworldwindows(worldwindow);
      width = SURI_CEIL(width*(dimwindow.GetWidth()/dimworldwindows.GetWidth()));
      height = SURI_CEIL(height*(dimwindow.GetHeight()/dimworldwindows.GetHeight()));
   }

   std::ostringstream ss;
   if (queryparameters["service"] == WMS_SERVICEID) {
      ss << "<GDAL_WMS> \
         <Service name=\"WMS\"> \
         <Version>1.1.1</Version> \
         <ServerUrl>";
      ss << url;
      ss << "</ServerUrl> \
         <SRS>";
      ss << wxsParameters_.serviceSRId_;
      ss << "</SRS> \
         <ImageFormat>";
      ss << wxsParameters_.format_;
      ss << "</ImageFormat> \
         <Layers>";
      ss << queryparameters["layers"];
      ss << "</Layers> \
             <Styles>";
      ss << wxsParameters_.style_;
      ss << "</Styles>\
         </Service> \
         <DataWindow> \
         <UpperLeftX>";
      ss << window.ul_.x_;
      ss << "</UpperLeftX> \
         <UpperLeftY>";
      ss << window.ul_.y_;
      ss << "</UpperLeftY> \
         <LowerRightX>";
      ss << window.lr_.x_;
      ss << "</LowerRightX> \
         <LowerRightY>";
      ss << window.lr_.y_;
      ss << "</LowerRightY> \
         <SizeX>";
      ss << width;
      ss << "</SizeX> \
         <SizeY>";
      ss << height;
      ss << "</SizeY> \
         </DataWindow> \
         <OverviewCount>12</OverviewCount> \
         <BlockSizeX>512</BlockSizeX> \
         <BlockSizeY>512</BlockSizeY> \
         <Projection>";
      ss << wxsParameters_.serviceSRId_;
      ss << "</Projection> \
         <BandsCount>3</BandsCount> \
         <ClampRequests>false</ClampRequests> \
         </GDAL_WMS>";
      retval = ss.str();
   }

   return retval;
}

//--------------------------- Metodo de Parameters -----------------------------
/**
 * Inicializa instancia con Parameters de RasterRenderer
 * @param[in] Parameters parametros de RasterRenderer
 */
void WxsRenderer::Parameters::Init(const RasterRenderer::Parameters& RasterParameters) {
   imageUrl_ = RasterParameters.imageUrl_;
   bandCombination_ = RasterParameters.bandCombination_;
   readFunction_ = RasterParameters.readFunction_;
   rasterModel_ = RasterParameters.rasterModel_;
   spatialModel_ = RasterParameters.spatialModel_;
}

/**
 * Operador de comparacion entre dos Parameters
 * @param[in] WxsParameters parametros contra los que comparo
 * @return bool que indica el resultado de la comparacion.
 */
bool WxsRenderer::Parameters::operator==(
      const WxsRenderer::Parameters& WxsParameters) const {
   return imageUrl_ == WxsParameters.imageUrl_
         && bandCombination_ == WxsParameters.bandCombination_
         && readFunction_ == WxsParameters.readFunction_
         && rasterModel_ == WxsParameters.rasterModel_
         && spatialModel_ == WxsParameters.spatialModel_
         && style_ == WxsParameters.style_ && format_ == WxsParameters.format_
         && serviceSRId_ == WxsParameters.serviceSRId_
         && extent_ == WxsParameters.extent_;
}

/**
 * Operador de copia entre dos WxsRenderer::Parametros
 * @param[in] WxsParameters parametros contra los que comparo
 * @return *this.
 */
WxsRenderer::Parameters& WxsRenderer::Parameters::operator=(
      const WxsRenderer::Parameters& WxsParameters) {
   imageUrl_ = WxsParameters.imageUrl_;
   bandCombination_ = WxsParameters.bandCombination_;
   readFunction_ = WxsParameters.readFunction_;
   rasterModel_ = WxsParameters.rasterModel_;
   spatialModel_ = WxsParameters.spatialModel_;
   style_ = WxsParameters.style_;
   format_ = WxsParameters.format_;
   serviceSRId_ = WxsParameters.serviceSRId_;
   extent_ = WxsParameters.extent_;
   return *this;
}

}

