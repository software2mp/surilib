
/*! \file MapRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <cmath>
#include <vector>
#include <sstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <algorithm>

// Includes Suri
#include "MapRenderer.h"
#include "SpatialReference.h"
#include "suri/ViewerWidget.h"
#include "suri/LayerList.h"
#include "suri/World.h"
#include "suri/VectorElement.h"
#include "suri/Coordinates.h"
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/messages.h"
#include "suri/MapTool.h"
#include "resources.h"
#include "suri/CoordinatesTransformation.h"
#include "logmacros.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/RasterElement.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/VectorStyle.h"
#include "VectorRenderer.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/xrc/xmlres.h"
#include "wx/font.h"
#include "wx/colour.h"

// Includes App

// Defines
#define __TEST_SOURCE_CODE__

#define GET_RED_VALUE(M_color) (M_color >> 24) & 0xff
#define GET_GREEN_VALUE(M_color) (M_color >> 16) & 0xff
#define GET_BLUE_VALUE(M_color) (M_color >> 8) & 0xff
#define GET_ALPHA_VALUE(M_color) M_color & 0xff

namespace {

/**
 * Separa una cadena teniendo en cuanta el separador especificado.
 *
 * @param[in] Src Cadena que se quiere separar.
 * @param[in] Token Separado que se quiere utilizar.
 * @return Vector con cada una de las partes.
 */
std::vector<std::string> StringSplitEx(const std::string& Src, char Token) {
   std::vector<std::string> retvec;
   std::stringstream ss(Src);
   std::string value;
   while (getline(ss, value, Token))
      retvec.push_back(value);
   return retvec;
}

}  // namespace anonymous


/** namespace suri */
namespace suri {
const std::string MapRenderer::MapLegendNativeFontInfo = "app_map_legend_native_font_info";
const std::string MapRenderer::MapLegendTitleConfigTag = "app_map_legend_title_text";
const std::string MapRenderer::MapLegendBorderConfigTag = "app_map_legend_border";
const std::string MapRenderer::MapLegendBackgroundConfigTag = "app_map_legend_background_color";
const std::string MapRenderer::MapNorthIconConfigTag = "app_map_north_icon";

LayerList * MapRenderer::Parameters::pStaticList_ = NULL;

std::vector<int> MapRenderer::staticElementGroupVector_ = std::vector<int>();
/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(MapRenderer);

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
/**
 * genera el nodo "mapa" a partir de los parametros.
 * @param[in]	RenderParameters: parametros.
 * @return nodo "mapa" con los subnodos leyenda, grilla, norte, escala
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
wxXmlNode *MapRenderer::GetXmlNode(const Parameters &RenderParameters) {
   wxXmlNode *pmapnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("mapa"));  // no se traduce,
                                                                                // elemento de XML
   if (!pmapnode) {
      return NULL;
   }

   new wxXmlNode(
         pmapnode, wxXML_ELEMENT_NODE,
         wxT(LEGEND_NODE),
         wxT(""),
         NULL, // no se traduce, elemento de XML
         new wxXmlNode(
               pmapnode, wxXML_ELEMENT_NODE,
               wxT(GRID_NODE),
               wxT(""),
               NULL, // no se traduce, elemento de XML
               new wxXmlNode(
                     pmapnode, wxXML_ELEMENT_NODE,
                     wxT(NORTH_NODE),
                     wxT(""),
                     NULL, // no se traduce, elemento de XML
                     new wxXmlNode(
                           pmapnode, wxXML_ELEMENT_NODE,
                           wxT(DIMENSION_NODE),
                           wxT(""),
                           NULL, // no se traduce, elemento de XML
                           new wxXmlNode(
                                 pmapnode,
                                 wxXML_ELEMENT_NODE,
                                 wxT(X_NODE),
                                 wxT(""),
                                 NULL, // no se traduce, elemento de XML
                                 new wxXmlNode(
                                       pmapnode,
                                       wxXML_ELEMENT_NODE,
                                       wxT(Y_NODE),
                                       wxT(""),
                                       NULL, // no se traduce, elemento de XML
                                       new wxXmlNode(pmapnode, wxXML_ELEMENT_NODE,
                                                     wxT(SCALE_NODE), wxT(""))))))));
   wxXmlNode *pnode = pmapnode->GetChildren();
   wxString value = wxString::Format("%d", RenderParameters.legend_);
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxT(""), value));
   pnode = pnode->GetNext();
   value = wxString::Format("%d", RenderParameters.grid_);
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxT(""), value));
   pnode = pnode->GetNext();
   value = wxString::Format("%d", RenderParameters.north_);
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxT(""), value));
   pnode = pnode->GetNext();
   value = wxString::Format("%d", RenderParameters.dimension_);
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxT(""), value));
   pnode = pnode->GetNext();
   value = wxString::Format("%d", RenderParameters.sizex_);
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxT(""), value));
   pnode = pnode->GetNext();
   value = wxString::Format("%d", RenderParameters.sizey_);
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxT(""), value));
   pnode = pnode->GetNext();
   value = wxString::Format("%d", RenderParameters.scale_);
   pnode->AddChild(new wxXmlNode(pnode, wxXML_TEXT_NODE, wxT(""), value));
   return pmapnode;
}

/**
 * Obtiene el nodo de interes para la renderizacion a partir de un elemento
 * @param[in]	pElement: puntero al elemento.
 * @return nodo "mapa" o null en caso de error.
 * \attention no debe eliminarse el nodo devuelto.
 * \todo reemplazar : "mapa" por el define
 */
wxXmlNode *MapRenderer::GetXmlNode(const Element *pElement) {
   if (!pElement) {
      return NULL;
   }

   wxXmlNode *pnode = GetRenderizationNode(pElement);
   if (!pnode) {
      return NULL;
   }
   pnode = pnode->GetChildren();
   while (pnode) {
      if (pnode->GetName() == wxT("mapa")) {
         return pnode;
      }
      pnode = pnode->GetNext();
   }
   return NULL;
}

/**
 * Agrega/reemplaza el nodo renderizacion. Fuerza una actualizacion del elemento.
 * @param[in]	pNode: nodo renderizacion nuevo
 * @param[out]	pElement: elemento con el nodo renderizacion reemplazado.
 * @return true.
 */
bool MapRenderer::SetXmlNode(Element *pElement, wxXmlNode *pNode) {
   pElement->AddNode(GetRenderizationNode(pElement), pNode, true);
   pElement->SetChanged();
   pElement->SendViewerUpdate();
   return true;
}

/**
 * obtiene los parametros a partir de un nodo
 * \pre pNode debe ser nodo mapa.
 * @param[in]	pNode: nodo mapa.
 * @return parametros(objeto de tipo Parameters)
 */
MapRenderer::Parameters MapRenderer::GetParameters(const wxXmlNode *pNode) {
   if (!pNode) {
      return Parameters();
   }
   Parameters params;
   wxXmlNode *pnode = pNode->GetChildren();
   while (pnode) {
      // Aca se asume que el bool se guarda como un 0 o 1
      if (pnode->GetName() == wxT(LEGEND_NODE)) {  // No se traduce, elemento XML
         params.legend_ =
               pnode->GetChildren() ? pnode->GetChildren()->GetContent() != wxT("0") : false;
      }
      if (pnode->GetName() == wxT(GRID_NODE)) {  // No se traduce, elemento XML
         params.grid_ =
               pnode->GetChildren() ? pnode->GetChildren()->GetContent() != wxT("0") : false;
      }
      if (pnode->GetName() == wxT(NORTH_NODE)) {  // No se traduce, elemento XML
         params.north_ =
               pnode->GetChildren() ? pnode->GetChildren()->GetContent() != wxT("0") : false;
      }
      if (pnode->GetName() == wxT(DIMENSION_NODE)) {  // No se traduce, elemento XML
         params.dimension_ = StringToNumber<int>(pnode->GetChildren()->GetContent().c_str());
      }
      if (pnode->GetName() == wxT(X_NODE)) {  // No se traduce, elemento XML
         params.sizex_ = StringToNumber<int>(pnode->GetChildren()->GetContent().c_str());
      }
      if (pnode->GetName() == wxT(Y_NODE)) {  // No se traduce, elemento XML
         params.sizey_ = StringToNumber<int>(pnode->GetChildren()->GetContent().c_str());
      }
      if (pnode->GetName() == wxT(SCALE_NODE)) {  // No se traduce, elemento XML
         params.scale_ =
               pnode->GetChildren() ? pnode->GetChildren()->GetContent() != wxT("0") : false;
      }
      pnode = pnode->GetNext();
   }
   return params;
}

// -------------------------- METODO DE CREACION ----------------------------
/**
 * Crea un MapRenderer a partir de un Elemento
 * @param[in]	pElement: puntero a un elemento.
 * @param[in]	pPreviousRenderer: puntero al renderer anterior en la cadena.
 * @return puntero a un objeto de tipo MapRenderer.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
Renderer *MapRenderer::Create(Element *pElement, Renderer *pPreviousRenderer) const {
   if (!Parameters::pStaticList_) {
      return NULL;
   }

   wxXmlNode *pnode = GetXmlNode(pElement);
   if (!pnode) {
      return NULL;
   }
   MapRenderer *prenderer = new MapRenderer;
   prenderer->parameters_ = GetParameters(pnode);
   prenderer->parameters_.pList_ = Parameters::pStaticList_;

   prenderer->lengenddimension_ = prenderer->parameters_.dimension_;
   prenderer->scaledimension_ = prenderer->parameters_.dimension_;
   prenderer->parameters_.sizex_ = prenderer->parameters_.sizex_;
   prenderer->parameters_.sizey_ = prenderer->parameters_.sizey_;
   prenderer->northdimension_ = 500;

   prenderer->elementGroupVector_ = staticElementGroupVector_;
   return prenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del nodo que maneja el renderizador.
 */
std::string MapRenderer::CreatedNode() const {
   return wxT("mapa");
}

// ----------------------- METODOS DE RENDERIZACION -------------------------
/**
 * Renderiza el elemento dado un World en un Canvas
 * @param[in]	pWorldWindow: puntero a la ventana del mundo.
 * @param[in]	pCanvas: puntero al Canvas.
 * @param[in]	pMask: puntero a la mascara raster binaria.
 */
bool MapRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   wxDC *pdc = pCanvas->GetDC();
   if (!pdc) {
      REPORT_AND_FAIL_VALUE("D:DC nulo, abortando renderizacion de mapa", false);
   }
   if (parameters_.legend_) {
      DrawLegend(pdc, pCanvas->GetSizeX(), pCanvas->GetSizeY());
   }

   if (parameters_.grid_) {
      DrawGrid();
   }

   if (parameters_.north_) {
      DrawNorth(pdc, pWorldWindow, pCanvas->GetSizeX(), pCanvas->GetSizeY());
   }
   if (parameters_.scale_) {
      DrawScale(pdc, pWorldWindow, pCanvas->GetSizeX(), pCanvas->GetSizeY());
   }

   return true;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]	pWorld: puntero a la ventana del mundo.
 * @param[in]	Ulx: uper-left x
 * @param[in]	Uly: uper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 */
void MapRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                 double &Lrx, double &Lry) {
   // Puedo renderizar en el ambito del mundo
   pWorld->GetWorld(Ulx, Uly, Lrx, Lry);
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void MapRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                     std::string &DataType) const {
   SizeX = 0;
   SizeY = 0;
   BandCount = 3;
   DataType = DataInfo<unsigned char>::Name;
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void MapRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                      std::string &DataType) const {
   BandCount = 3;
   DataType = DataInfo<unsigned char>::Name;
}

/**
 * Objetivo: Dibuja un recuadro con las referencias.
 * Recorre la lista de elementos y para cada elemento referenciable obtiene el
 * bitmap de la leyenda. Concatena los bitmas de las leyendas y le concatena el
 * titulo de la ventana.
 * Dibuja un rectangulo y dentro dibuja el bitmap de la leyenda.
 * @param[in]	pDC dc para dibujar recuadro y referencias.
 * @param[in]	ViewerWidth ancho del viewer
 * @param[in]	ViewerHeight alto del viewer
 */
void MapRenderer::DrawLegend(wxDC *pDC, const int &ViewerWidth, const int &ViewerHeight) {
   // Posiciono correctamente la leyenda cuando exporto la visualizacion
   if (parameters_.dimension_ > 0 && lengenddimension_ > ViewerHeight) {
      lengenddimension_ -= ViewerHeight;
      return;
   }

   wxBitmap elementslegend;
   for (std::vector<int>::iterator it = elementGroupVector_.begin();
         it != elementGroupVector_.end(); it++)
      for (int i = 0; i < parameters_.pList_->GetElementCount(*it); i++) {
         Element *pelement = parameters_.pList_->GetElement(i, *it);
         if (pelement && pelement->IsActive()) {
            wxBitmap elementlegendbitmap;
            // Obtiene el bitmap de la leyenda del elemento
            CreateElementBitmapLegend(pelement, elementlegendbitmap);
            // Concatena la leyenda del elemento a la leyenda
            AppendBitmap(elementslegend, elementlegendbitmap,
                         parameters_.spacerHeight_);
         }
      }

   if (!elementslegend.IsOk()) {
      REPORT_AND_RETURN("D:No hay leyendas para dibujar.");
   }

   wxBitmap legendbitmap;
   std::string titletext = Configuration::GetParameter(MapRenderer::MapLegendTitleConfigTag, "");
   std::string nativeFontInfo = Configuration::GetParameter(MapRenderer::MapLegendNativeFontInfo, "");
   // Crea el tipo de letra para el titulo de la leyenda
   wxFont titlefont;
   titlefont.SetNativeFontInfo(nativeFontInfo);
   wxColor fontcolor(LEGEND_TITLE_FONT_COLOR_RED, LEGEND_TITLE_FONT_COLOR_GREEN,
                     LEGEND_TITLE_FONT_COLOR_BLUE);
   // Crea un bitmap con el titulo de la ventana de leyenda
   DrawMultilineText(
         _(titletext.empty() ? label_LEGEND : titletext.c_str()),
         parameters_.legendWidth_ - 2 * (SUB_RECT_SEPARATION + SPACE_TO_BORDER),
         titlefont, fontcolor, legendbitmap);
   // Concatena el titulo con la leyenda de los elementos
   AppendBitmap(legendbitmap, elementslegend, LEGEND_TITLE_SEPARATION);

   // Calcula el alto del recuadro: Alto del texto mas espacio a los bordes
   size_t heightlegend = 2 * (SUB_RECT_SEPARATION + SPACE_TO_BORDER)
         + (legendbitmap.GetHeight());

   // coordenadas para el recuadro.
   int coordx = 0, coordy = 0;
   GetCornerCoordinates(parameters_.legendCorner_, ViewerWidth, ViewerHeight,
                        parameters_.legendWidth_, heightlegend, coordx, coordy);
   // Dibuja el recuadro
   DrawRoundedRectangle(pDC, coordx, coordy, parameters_.legendWidth_, heightlegend);

   int bmpcoordsx = coordx + SPACE_TO_BORDER + SUB_RECT_SEPARATION;
   int bmpcoordsy = coordy + SPACE_TO_BORDER + SUB_RECT_SEPARATION;
   // Dibuja la leyenda dentro del recuadro
   pDC->DrawBitmap(legendbitmap, bmpcoordsx, bmpcoordsy, true);
}

/**
 * Recibe dos bitmaps y al primero le concatena el segundo, por defecto se concatena
 * verticalmente.
 * Si se concatenan verticalmente, el bitmap devuelto tiene de ancho el maximo
 * entre los bitmaps recibidos, y de alto la suma de las alturas de los bitmaps.
 * Si se concatenan horizontalmente, el bitmap devuelto tiene de alto maximo
 * entre los bitmaps recibidos, y de ancho es la suma de los anchos de los bitmaps.
 * param[in/out] lhs bitmap sobre el que se concatena
 * param[in] rhs bitmap que se concatena
 * param[in] Separation separacion entre los bitmaps, en px
 * param[in] Vertical indica el sentido en que se concatena, por defecto true.
 * \ todo Se utiliza el color de fondo de la leyenda para crear la mascara. Pero
 *    se deberia obtener un color no usado por ninguno de los dos bitmaps que se
 *    concatenan.
 */
void MapRenderer::AppendBitmap(wxBitmap &lhs, wxBitmap rhs, int Separation,
                               bool Vertical) {
   if (!rhs.IsOk()) {
      return;
   }
   if (!lhs.IsOk()) {
      lhs = rhs;
      return;
   }

   wxBitmap aux = lhs;
   int bitmapheight = 0, bitmapwidth = 0, xcoord = 0, ycoord = 0;
   if (Vertical) {
      bitmapheight = aux.GetHeight() + rhs.GetHeight() + Separation;
      bitmapwidth = std::max(aux.GetWidth(), rhs.GetWidth());
      ycoord = aux.GetHeight() + Separation;
   } else {
      bitmapheight = std::max(aux.GetHeight(), rhs.GetHeight());
      bitmapwidth = aux.GetWidth() + rhs.GetWidth() + Separation;
      xcoord = aux.GetWidth() + Separation;
   }

   // Crea un bitmap del tamanio de la suma de los dos
   lhs = wxBitmap(bitmapwidth, bitmapheight);
   wxMemoryDC mdc;
   mdc.SelectObject(lhs);
   mdc.SetBackground(wxBrush(parameters_.backgroundColour_));
   mdc.Clear();
   mdc.DrawBitmap(aux, 0, 0, true);
   mdc.DrawBitmap(rhs, xcoord, ycoord, true);
   mdc.SelectObject(wxNullBitmap);
   // Crea una la mascara para que solo quede el texto (color del bitmap transparente)
   wxMask *mask = new wxMask(lhs, parameters_.backgroundColour_);
   lhs.SetMask(mask);
}

/**
 * Recibe un texto y devuelve un array con lineas de texto cuyo ancho (en px) no
 * supera al ancho recibido por parametro.
 * @param[in] SingleLineText texto a dividir
 * @param[in] MaxWidth ancho en pixeles maximo para cada linea
 * @param[in] TextFont tipo de letra, necesario para calcular el ancho del texto
 * @param[out] MultiLineText array con las lineas del texto
 * \ todo Mejorar la forma en que se corta el texto (ej. tratar de no cortar palabras).
 */
void MapRenderer::SingleToMultilineText(wxString SingleLineText, int MaxWidth,
                                        wxFont TextFont, wxArrayString &MultiLineText) {
   if (SingleLineText == "" || MaxWidth == 0) {
      return;
   }
   // Crea un memory dc y asigna el tipo de letra para medir el ancho del texto
   wxMemoryDC mdc;
   mdc.SetFont(TextFont);

   // Obtiene la cantidad de lineas necesarias ancho texto / MaxWidth
   float lines = std::max(
         1.0f,
         static_cast<float>(mdc.GetTextExtent(SingleLineText).GetWidth())
               / static_cast<float>(MaxWidth));
   // Obtiene
   int entirelines = static_cast<int>(lines);
   float partialline = std::max(0.0f, lines - entirelines);
   // Calculo la ultima linea de texto, proporcional a la parte decimal de la
   // cantidad de lineas.
   size_t lastlinestart = static_cast<size_t>(SingleLineText.Len()
         - ceil((partialline * SingleLineText.Len() / lines)));
   // Obtiene la ultima linea, y la elimina del texto
   wxString lasline = SingleLineText.Mid(lastlinestart);
   SingleLineText.Truncate(lastlinestart);

   int from = 0;
   int linelength = static_cast<int>(ceil(
         static_cast<float>(SingleLineText.Len()) / static_cast<float>(entirelines)));
   // Divide el texto en lineas y las guarda en un array
   for (int i = 0; i < entirelines; i++) {
      MultiLineText.Add(SingleLineText.Mid(from, linelength));
      from += linelength;
   }
   // Agrega la ultima linea
   if (lasline != "") {
      MultiLineText.Add(lasline);
   }
}

/**
 * Crea el texto de la leyenda para el elemento, contenido del nodo nombre mas
 * el contenido (si tiene) del nodo detalles entre parentesis.
 * @param[in] pElement elemento
 * @param[out] Legend texto de la leyenda
 */
void MapRenderer::CreateElementLegend(Element *pElement, wxString &Legend) {
   if (!pElement) {
      return;
   }
   Legend = pElement->GetName();
   wxString details = pElement->GetDetails();
   if (details != "") {
      Legend += " (" + details + ")";
   }
}

/**
 * Recibe un texto y dibuja en un bitmap el texto en multilinea, cada linea
 * no excede el ancho (en px) del limite recibido por parametro.
 * @param[in] Text Texto a dibujar
 * @param[in] TextWidth ancho maximo del bitmap (y de cada linea), en px
 * @param[in] TextFont tipo de letra con que se dibuja
 * @param[in] FontColour color para dibujar el texto.
 * @param[out] TextBitmap bitmap con el texto dibujado.
 */
void MapRenderer::DrawMultilineText(wxString Text, int TextWidth, wxFont TextFont,
                                    wxColor FontColour, wxBitmap &TextBitmap) {
   if (Text == "" || TextWidth == 0) {
      return;
   }
   wxArrayString multilinelegend;
   // Crea un memory dc y asigna el tipo de letra
   wxMemoryDC mdc;
   mdc.SetFont(TextFont);
   // Al ancho limite se le resta el ancho de un caracter, para asegurar que
   // el texto no exceda el bitmap
   int textwidth = TextWidth
         - static_cast<int>(CHAR_WIDTH_PLUS_FACTOR * mdc.GetCharWidth());
   // Transforma el texto de la leyenda en multilineas de ancho = textwidth
   SingleToMultilineText(Text, textwidth, TextFont, multilinelegend);
   if (multilinelegend.IsEmpty()) {
      return;
   }

   int textheigth = mdc.GetTextExtent(Text).GetHeight() + TEXT_LINE_SEPARATION;
   // El bitmap tiene el ancho recibido, y de alto = (lineas+separacion entre lineas)*alto del texto
   TextBitmap = wxBitmap(TextWidth, textheigth * multilinelegend.Count());

   // Asigna el bitmap al dc
   mdc.SelectObject(TextBitmap);
   // Pinta el fondo del bitmap para despues crear una mascara y que solo se dibuje el texto
   mdc.SetBackground(wxBrush(parameters_.backgroundColour_));
   mdc.Clear();

   // Asigna el color del texto de la leyenda, deberia ser parametro.
   mdc.SetTextForeground(FontColour);

   // Escribe las lineas de la leyenda
   int ycoord = 0;
   for (size_t j = 0; j < multilinelegend.Count(); j++) {
      mdc.DrawText(multilinelegend[j].Trim(false), 0, ycoord);
      ycoord += textheigth;
   }
   mdc.SelectObject(wxNullBitmap);
   // Crea una la mascara para que solo quede el texto (color del bitmap transparente)
   wxMask *mask = new wxMask(TextBitmap, parameters_.backgroundColour_);
   TextBitmap.SetMask(mask);
}

/**
 * Recibe un elemento y crea un bitmap con la leyenda del elemento (preview de
 * la imagen del elemento + texto de la leyenda).
 * @param[in] pElement elemento
 * @param[out] Legend bitmap con la leyenda del elemento.
 */
void MapRenderer::CreateElementBitmapLegend(Element *pElement, wxBitmap &Legend) {
   if (!pElement) {
      return;
   }
   // Obtiene el texto para la leyenda del elemento (nombre + detalles)
   wxString legend;
   CreateElementLegend(pElement, legend);

   // El ancho del bitmap de la leyenda = ancho de leyenda - 2*separacion a los bordes -
   // ancho preview - separacion texto preview
   int textwidth = parameters_.legendWidth_
         - 2 * (SPACE_TO_BORDER + SUB_RECT_SEPARATION)
         - parameters_.previewWidth_ - TEXT_TO_ICON_SEPARATION;
   wxBitmap bitmaplegend;
   DrawMultilineText(legend, textwidth, parameters_.font_, parameters_.fontColour_,
                     bitmaplegend);

   // Obtiene el icono del elemento
   wxBitmap bmp;
   pElement->GetPreview(bmp, parameters_.previewWidth_, parameters_.previewHeight_);
   // Asegura que el bitmap recibido no exceda el tamanio previsto en los parametros
   wxBitmap preview = bmp.GetSubBitmap(
         wxRect(
               0,
               0,
               std::min(static_cast<int>(parameters_.previewWidth_), bmp.GetWidth()),
               std::min(static_cast<int>(parameters_.previewHeight_),
                        bmp.GetHeight())));
   // Concatena el bitmap del preview con el de la leyenda
   AppendBitmap(preview, bitmaplegend, TEXT_TO_ICON_SEPARATION, false);

   // Para los elementos raster trata de obtener la informacion
   // de clasificacion (si existe)
   if (dynamic_cast<RasterElement*>(pElement) != NULL) {
      std::vector<CLASSIFICATIONINFO> info = ExtractClassificationInfo(pElement);
      AddClassificationInfo(info, preview);

      if (info.size() == 0) {
         std::vector<COLORTABLEINFO> ctinfo = ExtractColorTableInfo(pElement);
         AddColorTableInfo(ctinfo, preview);
      }
   } else if (dynamic_cast<VectorElement*>(pElement) != NULL) {
      std::vector<VECTORCLASSINFO> vinfo = ExtractVectorClassesInfo(pElement);
      if (!vinfo.empty())
         AddVectorClassInfo(vinfo, preview);
   }
   Legend = preview;
}

/**
 * Objetivo: Dibuja una flecha apuntando al norte
 * @param[in]	pDC: dc para dibujar
 * @param[in]	pWorldWindow: puntero a la ventana del mundo.
 * @param[in]	ViewerWidth: ancho del viewer
 * @param[in]	ViewerHeight: alto del viewer
 */
void MapRenderer::DrawNorth(wxDC *pDC, const World *pWorldWindow, const int ViewerWidth,
                            const int ViewerHeight) {
   wxImageHandler * jpegLoader = new wxJPEGHandler();
   wxImage::AddHandler(jpegLoader);

   // Posiciono correctamente la leyenda cuando exporto la visualizacion
   if (parameters_.dimension_ > 0 && northdimension_ != ViewerHeight)
      return;
   northdimension_ -= ViewerHeight;
   // OBTIENE EL BITMAP
   wxBitmap bmpnortharrow;
   GET_BITMAP_RESOURCE(wxT(icon_NORTH_ARROW), bmpnortharrow);
   std::string northfilename = Configuration::GetParameter(MapRenderer::MapNorthIconConfigTag, "");
   if (!northfilename.empty())
      bmpnortharrow.LoadFile(northfilename, wxBITMAP_TYPE_ANY );
   wxImage imgnortharrow = bmpnortharrow.ConvertToImage();

   // coordenadas para el norte.
   int coordx = 0, coordy = 0;

   // Obtiene las coordenadas de la esquina
   GetCornerCoordinates(parameters_.northArrowCorner_, ViewerWidth, ViewerHeight,
                        imgnortharrow.GetWidth(), imgnortharrow.GetHeight(), coordx,
                        coordy);

   // Calcula el centro de la imagen
   int NorthArrowCenterX = coordx + imgnortharrow.GetWidth() / 2;
   int NorthArrowCenterY = coordy + imgnortharrow.GetWidth() / 2;

   // Si las coordenadas donde deberia ir el norte estan fuera del mundo
   // Busca el punto mas cercano
   Subset world;
   Coordinates coord(0, 0);

   // Transforma el punto donde deberia ir la imagen en coordenadas de mundo
   pWorldWindow->V2WTransform(coordx, coordy, coord.x_, coord.y_);

   pWorldWindow->GetWorld(world);
   // Si el punto central no esta dentro del mundo lo relocaliza
   if (!world.IsInside(coord)) {
      // relocaliza el punto
      coord = world.NearestPoint(coord);

      // ahora estoy en el limite del mundo, retransformo a coords de viewport
      pWorldWindow->W2VTransform(coord.x_, coord.y_, coordx, coordy);
      // le sumo la mitad del tamanio de la imagen para que este completamente dentro del mundo
      NorthArrowCenterX = coordx + imgnortharrow.GetWidth() / 2;
      NorthArrowCenterY = coordy + imgnortharrow.GetWidth() / 2;
   }

   double wx1, wy1, wx2, wy2;

   // TRANSFORMA DOS PUNTOS DEL VIEWPORT EN X-Y
   pWorldWindow->V2WTransform(NorthArrowCenterX, NorthArrowCenterY, wx1, wy1);

   pWorldWindow->V2WTransform(NorthArrowCenterX + MIN_NORTH_ARROW_POINT_DIFF,
                              NorthArrowCenterX + MIN_NORTH_ARROW_POINT_DIFF, wx2, wy2);

   // Obtiene la referencia espacial del mundo
   std::string inwkt = pWorldWindow->GetSpatialReference();



   // Configura el wkt de salida
   Wkt* psrinwkt = Wkt::Create(inwkt);
   std::string outwkt;
   if (psrinwkt->GetRootValue() != "PROJCS") {
#ifdef __DONT_ALLOW_GEOCS_IDENTITY__
      Wkt::Destroy(srinwkt);
      return NULL;
#else
      outwkt = psrinwkt->GetWktString("GEOGCS");
#endif
   } else {
      outwkt = psrinwkt->GetWktString("PROJCS|GEOGCS");
   }

   // Crea el objeto de transformacion
   Coordinates coord1(wx1, wy1);
   Coordinates coord2(wx2, wy2);
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                inwkt);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                outwkt);
   TransformationFactory* pfactory =
         TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pcoordtrans = pfactory->Create(TransformationFactory::kExact,
                                            params);
   TransformationFactoryBuilder::Release(pfactory);

   // SI NO PUEDE CREAR EL OBJETO RETORNA
   if (!pcoordtrans) {
      return;
   }

   // SI NO PUEDE TRANSFORMAR ESTOS DOS PUNTOS EN LATLONG RETORNA
   if (pcoordtrans->Transform(coord1) == 0
         || pcoordtrans->Transform(coord2) == 0) {
      return;
   }

   // Proyecta sobre eje X para que tenga direccion NORTE-SUR
   coord2.x_ = coord1.x_;

   // TRANSFORMA DE LATLONG A COORDENADAS DE MUNDO Y LUEGO A COORDENADAS DE VIEWPORT
   int sx1, sy1, sx2, sy2;
   // Rota la flecha en caso que el punto giratorio sea menor que el fijo
   // (apunte al sur)
   if (coord2.y_ < coord1.y_) {
      coord2.y_ = coord1.y_ + (coord1.y_ - coord2.y_);
   }

   // Se asegura que haya minimanmente MIN_NORTH_ARROW_POINT_DIFF grados entre
   // los puntos
   // Porque cuando la diferencia se aproxima a 0 comienza a tener error
   coord2.y_ = coord2.y_ + MIN_NORTH_ARROW_POINT_DIFF;

   // Se asegura que el punto movil no pase los 90 grados (polo norte)
   if (coord2.y_ > 90) {
      coord2.y_ = 90;
   }

   // TRASFORMA EL NUEVO LATLONG A COORDENADAS DE MUNDO
   pcoordtrans->Transform(coord1, true);
   pcoordtrans->Transform(coord2, true);

   pWorldWindow->W2VTransform(coord1.x_, coord1.y_, sx1, sy1);
   pWorldWindow->W2VTransform(coord2.x_, coord2.y_, sx2, sy2);

   delete pcoordtrans;

   // SOLO ROTA SI HAY ANGULO DE ROTACION
   double angle;

   // SI NO ESTA APUNTANDO HACIA ARRIBA O ABAJO (tangente da infinito)
   if (sx2 != sx1) {
      if (sx2 != sx1) {
         // calcula la tangente
         double tan = (static_cast<double>(sy1) - static_cast<double>(sy2))
               / (static_cast<double>(sx1) - static_cast<double>(sx2));
         // obtiene el angulo
         angle = M_PI / 2 - atan(tan);
         if (sx2 > sx1) {
            angle = angle + M_PI;
         }
      }
   } else {
      if (sy2 < sy1) {  // APUNTA HACIA ARRIBA
         angle = 0;
      } else {  // APUNTA HACIA ABAJO
         angle = M_PI;  // rota 180 grados
      }
   }

   imgnortharrow = imgnortharrow.Rotate(
         angle, wxPoint(imgnortharrow.GetWidth() / 2, imgnortharrow.GetHeight() / 2));

   pDC->DrawBitmap(wxBitmap(imgnortharrow),
                   NorthArrowCenterX - imgnortharrow.GetWidth() / 2,
                   NorthArrowCenterY - imgnortharrow.GetHeight() / 2, true);

#ifdef __DRAW_NORTH_DEBUGGING_LINES__
   // DIBUJA EL NORTE CON LINEAS PARA DEBUG---------------------------------
   pDC->SetBrush(wxBrush(*wxGREEN_BRUSH) );
   pDC->SetPen(wxPen(*wxGREEN) );

   // LINEA AL NORTE
   pDC->DrawLine(sx1, sy1, sx2, sy2);

   // CIRCULO DE FLECHA
   pDC->DrawCircle(sx2, sy2, 3);

   // CIRCLULO PRINCIPAL
   int arrowlength = static_cast<int>(sqrt(pow((sx2-sx1), 2)+pow((sy2-sy1), 2)));
   pDC->SetBrush(wxBrush(*wxTRANSPARENT_BRUSH) );
   pDC->DrawCircle(sx1, sy1, arrowlength);
#endif
}

/**
 * Objetivo: Dibuja la escala
 * @param[in]	pDC: dc para dibujar
 * @param[in]	pWorldWindow: puntero a la ventana del mundo.
 * @param[in]	ViewerWidth: ancho del viewer
 * @param[in]	ViewerHeight: alto del viewer
 */
void MapRenderer::DrawScale(wxDC *pDC, const World *pWorldWindow, const int ViewerWidth,
                            const int ViewerHeight) {
   // Posiciono correctamente la leyenda cuando exporto la visualizacion
   if (parameters_.dimension_ > 0 && scaledimension_ > ViewerHeight) {
      scaledimension_ -= ViewerHeight;
      return;
   }

   // coordenadas para el norte.
   int coordx = 0, coordy = 0;
   GetCornerCoordinates(parameters_.scaleCorner_, ViewerWidth, ViewerHeight,
                        SCALE_INITIAL_SIZE, 20, coordx, coordy);

   // Obtiene la referencia espacial del mundo
   std::string wkt = pWorldWindow->GetSpatialReference();
   if (!SpatialReference::IsProjectedSpatialRef(wkt))
      return;
   double wx1, wy1, wx2, wy2;

   // TRANSFORMA DOS PUNTOS DEL VIEWPORT EN X-Y
   pWorldWindow->V2WTransform(coordx, coordy, wx1, wy1);

   pWorldWindow->V2WTransform(coordx + SCALE_INITIAL_SIZE, coordy, wx2, wy2);

   // ELIGE LA ESCALA QUE LE CORRESPONDA
   double scale = 1;
   bool found = false;
   double scalesarray[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   double i = 1;
   int j;
   double previousscale = scale;
   while (!found) {
      for (j = 0; j < 9; j++) {
         scale = i * scalesarray[j];
         if (wx2 - wx1 < scale) {
            found = true;
            break;
         }
         previousscale = scale;
      }
      i = i * 10;
   }
   // Comparo escala obtenida con la anterior para ver cual es menor
   if (((wx2 - wx1) - previousscale) < (scale - (wx2 - wx1))) {
      scale = previousscale;
   }

   // Asigna el pto 2 como pto1 + escala
   wx2 = wx1 + scale;

   int sx1, sy1, sx2, sy2;

   pWorldWindow->W2VTransform(wx1, wy1, sx1, sy1);
   pWorldWindow->W2VTransform(wx2, wy2, sx2, sy2);

   // Dibuja Rectangulos recuadro
   wxCoord textwidth;
   wxCoord textheight;
   pDC->GetTextExtent(wxString::Format(wxT("%.0f Km"), scale / 1000), &textwidth,
                      &textheight);
   int extrax = 15 + static_cast<int>(textwidth);
   int extray = 25 + static_cast<int>(textheight);
   GetCornerCoordinates(SCALE_CORNER, ViewerWidth, ViewerHeight,
                        sx2 - sx1 + extrax + 2 * SUB_RECT_SEPARATION,
                        extray + 2 * SUB_RECT_SEPARATION, coordx, coordy);

   // Cambio posicion de escala
   sx1 += SPACE_TO_BORDER + SUB_RECT_SEPARATION + 5;
   sx2 += SPACE_TO_BORDER + SUB_RECT_SEPARATION + 5;

   DrawScaleImage(pDC, sx1, sx2, sy1);
   // 0 de escala
   DrawShadedText(pDC, sx1, sy1 - 20, wxString::Format(wxT("%.0f"), 0.0).c_str());
   // Obtener las unidades del wkt en vez de hardcodearlas a KM o m
   if (scale < 1000) {
      DrawShadedText(pDC, sx1 + (sx2 - sx1) - 5, sy1 - 20,
                     wxString::Format(wxT("%.0f m"), scale).c_str());
   } else {
      DrawShadedText(pDC, sx1 + (sx2 - sx1) - 5, sy1 - 20,
                     wxString::Format(wxT("%.0f Km"), scale / 1000).c_str());
   }
}

/**
 * Dibuja un rectangulo de color con un subrectangulo negro
 * sin relleno en la posicion indicada.
 * @param[in]	pDC: wxDC sobre el que se dibuja.
 * @param[in]	coordx: coordenada x donde colocar el recuadro;
 * @param[in]  coordy: coordenada y donde colocar el recuadro;
 * @param[in]	ItemWidth: ancho del recuadro.
 * @param[in]	ItemHeight: alto del recuadro.
 */
void MapRenderer::DrawRoundedRectangle(wxDC *pDC, const int coordx, const int coordy,
                                       const int ItemWidth, const int ItemHeight) {
   // Dibujo cuadro de referencias.
   pDC->SetBrush(wxBrush(parameters_.backgroundColour_));
   pDC->SetPen(wxPen(wxColour(parameters_.backgroundColour_)));
   pDC->DrawRoundedRectangle(coordx, coordy, ItemWidth, ItemHeight, 10);
   // 2do recuadro
   std::string bordercolour = Configuration::GetParameter(MapRenderer::MapLegendBorderConfigTag, "");
   wxPen penaux(bordercolour.empty() ? wxColour(1, 1, 1, wxALPHA_OPAQUE) : wxColour(bordercolour), 1);
   pDC->SetPen(penaux);
   pDC->DrawRoundedRectangle(coordx + SUB_RECT_SEPARATION, coordy + SUB_RECT_SEPARATION,
                             ItemWidth - 2 * SUB_RECT_SEPARATION,
                             ItemHeight - 2 * SUB_RECT_SEPARATION, 10);
}

/**
 * Dibuja texto en la posicion indicada con una sobra negra
 * @param[in]	pDC: wxDC sobre el que se dibuja.
 * @param[in]	coordx: coordenada x donde colocar el texto;
 * @param[in]  coordy: coordenada y donde colocar el texto;
 * @param[in]	text: texto a escribir.
 */
void MapRenderer::DrawShadedText(wxDC *pDC, const int coordx, const int coordy,
                                 std::string text) {
   pDC->SetBackgroundMode(wxTRANSPARENT);
   pDC->SetTextForeground(wxColour(*wxBLACK));
   pDC->DrawText(wxT(text.c_str() ), wxCoord(coordx + 1), wxCoord(coordy + 1));
   pDC->SetTextForeground(wxColour(*wxWHITE));
   pDC->DrawText(wxT(text.c_str() ), wxCoord(coordx), wxCoord(coordy));
}

/**
 * Dibuja escala en coordenadas indicadas
 * @param[in]	pDC: wxDC sobre el que se dibuja.
 * @param[in]	coordx1: coordenada x donde comianza la escala;
 * @param[in]  coordx2: coordenada x donde termina la escala;
 * @param[in]  coordy: coordenada y donde colocar la escala;
 */
void MapRenderer::DrawScaleImage(wxDC *pDC, const int coordx1, const int coordx2,
                                 const int coordy) {
   // Dibujo escala
   pDC->SetPen(wxPen(*wxBLACK));
   pDC->SetBrush(wxBrush(wxColour(*wxWHITE), wxSOLID));
   pDC->DrawRectangle(coordx1, coordy, coordx2 - coordx1, 8);
   pDC->SetBrush(wxBrush(wxColour(*wxBLACK), wxSOLID));
   pDC->DrawRectangle(coordx1, coordy, (coordx2 - coordx1) / 4, 4);
   pDC->DrawRectangle(coordx1 + (coordx2 - coordx1) / 4, coordy + 4,
                      (coordx2 - coordx1) / 4, 4);
   pDC->DrawRectangle(coordx1 + (coordx2 - coordx1) / 2, coordy,
                      (coordx2 - coordx1) / 4, 4);
   pDC->DrawRectangle(coordx1 + 3 * (coordx2 - coordx1) / 4, coordy + 4,
                      (coordx2 - coordx1) / 4, 4);
}

/**
 * FUNCION DEPRECADA
 * \see MapTool::pGridElement_ es el elemento de tipo vectorial que dibuja
 *   las grillas.
 */
void MapRenderer::DrawGrid() {
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void MapRenderer::Update(Element *pElement) {
   LayerList *plistaux = parameters_.pList_;
   parameters_ = GetParameters(GetXmlNode(pElement));
   parameters_.pList_ = plistaux;
}
// -----------------------------------------------------------------------------
/**
 * Objetivo: devuelve las coordenadas de del viewer donde se debe dibujar el item.
 * @param[in]	corner: indice de la esquina del viewer.
 * @param[in]	ViewerWidth: ancho del viewer.
 * @param[in]	ViewerHeight: alto del viewer.
 * @param[in]	ItemWidth: ancho del item.
 * @param[in]	ItemHeight: alto del item.
 * @param[out]	X: coordenada X donde colocar el item;
 * @param[out] Y: coordenada Y donde colocar el item;
 */
void MapRenderer::GetCornerCoordinates(const int corner, const int ViewerWidth,
                                       const int ViewerHeight, const int ItemWidth,
                                       const int ItemHeight, int &X, int &Y) {
   switch (corner) {
      case 0:    // vertice inferior izquierdo.
         X = SPACE_TO_BORDER + parameters_.sizex_;
         Y = ViewerHeight - ItemHeight - SPACE_TO_BORDER - parameters_.sizey_;
         break;
      case 1:    // vertice superior izquierdo.
         X = Y = SPACE_TO_BORDER;
         break;
      case 3:    // vertice inferior derecho.
         X = ViewerWidth - ItemWidth - SPACE_TO_BORDER - parameters_.sizex_;
         Y = ViewerHeight - ItemHeight - SPACE_TO_BORDER - parameters_.sizey_;
         break;
      default:    // vertice superior derecho.
         X = ViewerWidth - ItemWidth - SPACE_TO_BORDER - parameters_.sizex_;
         Y = SPACE_TO_BORDER + parameters_.sizey_;
         break;
   }
}

/**
 * Objetivo: Corta la cadena Name a una longitud fija (Length).
 * @param[in]	Name: cadena a cortar.
 * @param[in]	Length: longitud donde debe cortarse la cadena.
 * @return: cadena cortada + "..." o la cadena sin alterar(Length < Name.Len)
 * \todo Este metodo es demasiado generico, considerar migrar fuera de la clase
 */
wxString MapRenderer::CutElementName(const wxString &Name, const size_t Length) {
   if (Name.IsEmpty() || Length == 0) {
      return wxT("");
   }

   if (Length < Name.Len()) {
      return Name.Mid(0, Length) + wxT("...");
   }

   return Name.Mid(0, Length);
}


/** Extrae informacion de estilos clasificados **/
std::vector<VECTORCLASSINFO> MapRenderer::ExtractVectorClassesInfo(Element* pElement) {
   std::vector<VECTORCLASSINFO> classes;
   wxXmlNode* pcatnode = pElement->GetNode(
         RENDERIZATION_NODE NODE_SEPARATION_TOKEN CATEGORIZED_LAYER_NODE);
   wxXmlNode* pclassnode = pcatnode ? pcatnode->GetChildren() : NULL;
   while (pclassnode) {
      wxXmlNode* pcnode = pclassnode->GetChildren();
      std::string condition, label, style;
      while (pcnode) {
         if (pcnode->GetName().compare(CATEGORY_CONDITION_NODE) == 0) {
            condition = pcnode->GetNodeContent().c_str();
         } else if (pcnode->GetName().compare(CATEGORY_STYLE_NODE) == 0) {
            style = pcnode->GetNodeContent().c_str();
         } else if (pcnode->GetName().compare(CATEGORY_LABEL_NODE) == 0) {
            label = pcnode->GetNodeContent().c_str();
         }
         pcnode = pcnode->GetNext();
      }
      if (!style.empty()) {
         VECTORCLASSINFO info;
         info.label = label.empty() ? condition : label;
         info.styleWkt = style;
         classes.push_back(info);
      }
      pclassnode = pclassnode->GetNext();
   }

   return classes;
}

/**
 * Extrae la informacion de clasificacion del elemento.
 *
 * @param[in] pElement Elemento del cual se desea extraer la informacion.
 * @return Vector con la informacion de clasificacion.
 */
std::vector<CLASSIFICATIONINFO> MapRenderer::ExtractClassificationInfo(Element* pElement) {
   bool done = false;
   std::vector<CLASSIFICATIONINFO> retvals;
   wxXmlNode* prootnode = pElement->GetNode(wxT(""));
   wxXmlNode* pnode = prootnode->GetChildren();
   while (pnode != NULL && !done) {
      if (pnode->GetName() == CLASSIFICATION_CLASSES_NODE) {
         wxXmlNode* pclassnode = pnode->GetChildren();
         while (pclassnode != NULL) {
            if (pclassnode->GetName().compare(CLASSIFICATION_CLASS_NODE) == 0) {
               CLASSIFICATIONINFO info;

               // Obtiene el indice
               wxString index;
               if (pclassnode->GetPropVal(CLASSIFICATION_CLASS_PROPERTY, &index)) {
                  info.index = StringToNumber<int>(index.c_str());
               }

               // Obtiene los datos del nodo
               wxXmlNode* pinfonode = pclassnode->GetChildren();
               while (pinfonode != NULL) {
                  if (pinfonode->GetName().compare(CLASSIFICATION_CLASS_NAME_NODE) == 0) {
                     info.name = pinfonode->GetNodeContent().c_str();
                  } else if (pinfonode->GetName().compare(CLASSIFICATION_CLASS_COLOR_NODE) == 0) {
                     std::stringstream ss;
                     ss << std::hex << pinfonode->GetNodeContent().c_str();
                     ss >> info.color;
                  } else if (pinfonode->GetName().compare(CLASSIFICATION_CLASS_ENCLOSURE_NODE) ==
                        0) {
                     info.enclosure = pinfonode->GetNodeContent().c_str();
                  }
                  pinfonode = pinfonode->GetNext();
               }
               retvals.push_back(info);
            }
            pclassnode = pclassnode->GetNext();
         }
         done = true;
      }
      pnode = pnode->GetNext();
   }
   return retvals;
}

/**
 * Extrae la informacion de la tabla de colores.
 *
 * @param[in] pElement Elemento del cual se desea extraer la informacion.
 * @return Vector con la informacion de la tabla de colores.
 */
std::vector<COLORTABLEINFO> MapRenderer::ExtractColorTableInfo(Element* pElement) {
   int component = 0;  // 0:rojo 1:verde 2:azul
   bool done = false;
   std::vector<COLORTABLEINFO> colors;
   wxXmlNode* prootnode = pElement->GetNode(wxT(""));
   wxXmlNode* pnode = prootnode->GetChildren();
   while (pnode != NULL && !done) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         wxXmlNode* prender = pnode->GetChildren();
         while (prender != NULL) {
            if (prender->GetName().compare(COLOR_TABLE) == 0) {
               wxXmlNode* ptable = prender->GetChildren();
               while (ptable != NULL) {
                  if (ptable->GetName().compare(TABLE_NODE) == 0) {
                     LoadColorTableInfo(colors, ptable, component);
                     ++component;
                  } else if (ptable->GetName().compare(COLOR_TABLE_LABELS) == 0) {
                     LoadColorTableInfoLabels(colors, ptable);
                  }
                  ptable = ptable->GetNext();
               }
            }
            prender = prender->GetNext();
         }
      }
      pnode = pnode->GetNext();
   }

   return colors;
}

/**
 * Agrega informacion nueva al vector.
 *
 * @param[in|out] CTInfo Vector al que agregar info o al que actualizarle la info.
 * @param[in] pTable Nodo tabla del cual sacar la info.
 * @param[in] Component Color que se esta leyendo (0:rojo 1:verde 2:azul)
 */
void MapRenderer::LoadColorTableInfo(std::vector<COLORTABLEINFO>& CTInfo,
                                     wxXmlNode* pTable, int Component) {
   int rowpos = 0;  // 0:intevalos 1:valores de colores
   std::vector<std::string> boundaries;
   std::vector<std::string> values;

   // Cargo los valores de los limites y los valores para la componente de color
   wxXmlNode* prow = pTable->GetChildren();
   while (prow != NULL) {
      if (prow->GetName().compare(ROW_NODE) == 0) {
         if (rowpos == 0) {
            // limites
            boundaries = StringSplitEx(prow->GetNodeContent().c_str(), ' ');
         } else if (rowpos == 1) {
            // valor de la componente
            values = StringSplitEx(prow->GetNodeContent().c_str(), ' ');
         }
         ++rowpos;
      }
      prow = prow->GetNext();
   }

   for (int ix = 0, lenix = boundaries.size(); ix < lenix; ++ix) {
      if (Component == 0) {
         // rojo
         COLORTABLEINFO info;
         info.boundary = boundaries[ix];
         info.color = 0; info.color = atoi(values[ix].c_str()) << 24;
         CTInfo.push_back(info);
      } else if (Component == 1) {
         // verde
         COLORTABLEINFO info;
         info.boundary = boundaries[ix];
         std::vector<COLORTABLEINFO>::iterator it;
         it = find(CTInfo.begin(), CTInfo.end(), info);
         if (it != CTInfo.end()) {
            it->color |= atoi(values[ix].c_str()) << 16;
         }
      } else if (Component == 2) {
         // verde
         COLORTABLEINFO info;
         info.boundary = boundaries[ix];
         std::vector<COLORTABLEINFO>::iterator it;
         it = find(CTInfo.begin(), CTInfo.end(), info);
         if (it != CTInfo.end()) {
            it->color |= atoi(values[ix].c_str()) << 8;
         }
      }
   }
}

/**
 * Carga las etiquetas para cada intervalo.
 *
 * @param[in|out] CTInfo Vector al que agregar info o al que actualizarle la info.
 * @param[in] pTable Nodo tabla del cual sacar la info.
 */
void MapRenderer::LoadColorTableInfoLabels(std::vector<COLORTABLEINFO>& CTInfo,
                                           wxXmlNode* pTable) {
   wxXmlNode* prow = pTable->GetChildren();
   while (prow != NULL) {
      if (prow->GetName().compare(COLOR_TABLE_LABEL) == 0) {
         wxString boundary;
         if (prow->GetPropVal(wxT(COLOR_TABLE_BOUNDARY), &boundary)) {
            COLORTABLEINFO info;
            info.boundary = boundary.c_str();
            std::vector<COLORTABLEINFO>::iterator it;
            it = find(CTInfo.begin(), CTInfo.end(), info);
            if (it != CTInfo.end()) {
               it->name = prow->GetNodeContent().c_str();
            }
         }
      }
      prow = prow->GetNext();
   }
}

/** Agrega la informacion de la tabla de colores si esta disponible. */
void MapRenderer::AddVectorClassInfo(std::vector<VECTORCLASSINFO>& Classes,
                                     wxBitmap& DestBM) {
   int textwidth = parameters_.legendWidth_
            - 2 * (SPACE_TO_BORDER + SUB_RECT_SEPARATION);
   for (int ix = 0, lenix = Classes.size(); ix < lenix; ++ix) {
      VectorStyle* pvstyle = VectorStyle::Create(Classes[ix].styleWkt);
      if (!pvstyle)
         return;

      wxBitmap textclassbm, classbm, stylebm;

      // Identacion de bmp
      wxMemoryDC mdc;
      wxBitmap emptybmp(parameters_.previewWidth_, parameters_.previewHeight_);
      mdc.SelectObject(emptybmp);
      mdc.SetBackground(wxBrush(*wxWHITE));
      mdc.Clear();
      mdc.SelectObject(wxNullBitmap);
      AppendBitmap(classbm, emptybmp, TEXT_TO_ICON_SEPARATION, false);

      // bmp del estilo
      VectorRenderer::PreviewStyle(pvstyle, stylebm, parameters_.previewWidth_, parameters_.previewHeight_);
      AppendBitmap(classbm, stylebm, TEXT_TO_ICON_SEPARATION, false);

      wxString classstr(Classes[ix].label);
      DrawMultilineText(classstr, textwidth, parameters_.font_, parameters_.fontColour_,
                        textclassbm);
      AppendBitmap(classbm, textclassbm, TEXT_TO_ICON_SEPARATION, false);

      AppendBitmap(DestBM, classbm, TEXT_TO_ICON_SEPARATION);

      VectorStyle::Destroy(pvstyle);
   }
}

/**
 * Agrega la informacion de clasificacion si esta esta disponible.
 *
 * @param[in] CInfo Vector con la informacion de clasificacion a mostrar.
 * @param[out] DestBM Bitmap en el cual dibujar la informacion.
 */
void MapRenderer::AddClassificationInfo(std::vector<CLASSIFICATIONINFO>& CInfo,
                                        wxBitmap& DestBM) {
   int textwidth = parameters_.legendWidth_
            - 2 * (SPACE_TO_BORDER + SUB_RECT_SEPARATION)
            - parameters_.previewWidth_ - TEXT_TO_ICON_SEPARATION;
   for (int ix = 0, lenix = CInfo.size(); ix < lenix; ++ix) {
      wxBitmap classbm;

      // Dejo un espacio (TODO: ver la manera mejorar esta parte)
      wxMemoryDC mdc;
      wxBitmap emptybmp(parameters_.previewWidth_, parameters_.previewHeight_);
      mdc.SelectObject(emptybmp);
      mdc.SetBackground(wxBrush(*wxWHITE));
      mdc.Clear();
      mdc.SelectObject(wxNullBitmap);
      AppendBitmap(classbm, emptybmp, TEXT_TO_ICON_SEPARATION, false);

      // Dibujo el icono con el color de la clase
      wxBitmap colorbmp(parameters_.previewWidth_, parameters_.previewHeight_);
      mdc.SelectObject(colorbmp);
      mdc.SetBackground(wxBrush(wxColor(GET_RED_VALUE(CInfo[ix].color),
                                        GET_GREEN_VALUE(CInfo[ix].color),
                                        GET_BLUE_VALUE(CInfo[ix].color))));
      mdc.Clear();
      mdc.SelectObject(wxNullBitmap);
      AppendBitmap(classbm, colorbmp, TEXT_TO_ICON_SEPARATION, false);

      // Agrego el texto
      wxBitmap textbm;
      wxString testwxs(CInfo[ix].name);
      DrawMultilineText(testwxs, textwidth, parameters_.font_, parameters_.fontColour_,
                        textbm);
      AppendBitmap(classbm, textbm, TEXT_TO_ICON_SEPARATION, false);

      AppendBitmap(DestBM, classbm, TEXT_TO_ICON_SEPARATION);
   }
}

/**
 * Agrega la informacion de la tabla de colores si esta disponible.
 *
 * @param[in] CTInfo Vector con la informacion de la tabla de colores a mostrar.
 * @param[out] DestBM Bitmap en el cual dibujar la informacion.
 */
void MapRenderer::AddColorTableInfo(std::vector<COLORTABLEINFO>& CTInfo,
                                    wxBitmap& DestBM) {
   int textwidth = parameters_.legendWidth_
               - 2 * (SPACE_TO_BORDER + SUB_RECT_SEPARATION)
               - parameters_.previewWidth_ - TEXT_TO_ICON_SEPARATION;

   for (int ix = 0, lenix = CTInfo.size(); ix < lenix; ++ix) {
      wxBitmap colortablebm;

      // Dejo un espacio (TODO: ver la manera mejorar esta parte)
      wxMemoryDC mdc;
      wxBitmap emptybmp(parameters_.previewWidth_, parameters_.previewHeight_);
      mdc.SelectObject(emptybmp);
      mdc.SetBackground(wxBrush(*wxWHITE));
      mdc.Clear();
      mdc.SelectObject(wxNullBitmap);
      AppendBitmap(colortablebm, emptybmp, TEXT_TO_ICON_SEPARATION, false);

      // Dibujo el icono con el color de la clase
      // TODO: dibujar un rectangulo negro cuando el colo sea blanco (?)
      wxBitmap colorbmp(parameters_.previewWidth_, parameters_.previewHeight_);
      mdc.SelectObject(colorbmp);
      mdc.SetBackground(wxBrush(wxColor(GET_RED_VALUE(CTInfo[ix].color),
                                        GET_GREEN_VALUE(CTInfo[ix].color),
                                        GET_BLUE_VALUE(CTInfo[ix].color))));
      mdc.Clear();
      mdc.SelectObject(wxNullBitmap);
      AppendBitmap(colortablebm, colorbmp, TEXT_TO_ICON_SEPARATION, false);

      // Formateo el texto (asumo que vienen ordenados de menor a mayor
      // TODO: verificar si esta bien asumir el orden
      std::string text;
      {
         char auxtext[64] = { 0 };
         if (ix == 0) {
            sprintf(auxtext, "<= %s ", CTInfo[ix].boundary.c_str());
         } else if (ix > 0 && ix < lenix - 1) {
            sprintf(auxtext, "(%s : %s] ", CTInfo[ix - 1].boundary.c_str(),
                    CTInfo[ix].boundary.c_str());
         } else {
            sprintf(auxtext, "> %s ", CTInfo[ix - 1].boundary.c_str());
         }
         text = auxtext;
      }

      // Agrego el texto
      wxBitmap textbm;
      wxString testwxs(text);
      testwxs += _(CTInfo[ix].name.c_str());
      DrawMultilineText(testwxs, textwidth, parameters_.font_, parameters_.fontColour_,
                        textbm);
      AppendBitmap(colortablebm, textbm, TEXT_TO_ICON_SEPARATION, false);

      AppendBitmap(DestBM, colortablebm, TEXT_TO_ICON_SEPARATION);
   }
}

}
