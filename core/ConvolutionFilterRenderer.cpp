/*! \file ConvolutionFilterRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <algorithm>
#include <cmath>
#include <limits>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ConvolutionFilterRenderer.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Canvas.h"
#include "suri/World.h"
#include "MemoryCanvas.h"
#include "suri/XmlFunctions.h"

#include "wx/xml/xml.h"
#include "wx/string.h"

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::ConvolutionFilterRenderer);

/** namespace suri */
namespace suri {

/**
 * Este template se utiliza para aplicar filtro a la imagen.
 * Aplica matriz a cada pixel en pSrc y guarda resultado pDest.
 * \pre pDest debe tener tamanio imagen con pixeles en formato double.
 * \pre pSrc debe tener tamanio Size.
 * \post pDest tiene los datos luego de aplicar filtro.
 * @param[out] pDest puntero a los datos luego de aplicar el filtro.
 * @param[in] pSrc puntero a los datos a filtrar.
 * @param[in] pFilter matriz con doubles del filtro.
 * @param[in] ImageHeight altura de la imagen.
 * @param[in] ImageWidth ancho de la imagen.
 * @param[in] FilterHeight altura del filtro.
 * @param[in] FilterWidth ancho del filtro.
 */
template<typename T>
void kernelfilter(void* pDest, void* pSrc,
            std::vector<std::vector<double> > pFilter, int ImageWidth, int ImageHeight,
            int FilterWidth, int FilterHeight) {
   // Por alguna razon la matriz se estaba rotando. Ahora se aplica tal cual como
   // fue cargada.
   double** pmirroredfilter = new double*[FilterHeight];
   for (int i = 0; i < FilterHeight; ++i) {
      pmirroredfilter[i] = new double[FilterWidth];
      for (int j = 0; j < FilterWidth; ++j)
         pmirroredfilter[i][j] = pFilter[i][j];
   }

   // Aplico la matriz
   T* psrc = static_cast<T*>(pSrc);
   FILTER_OUTPUT_DATA_TYPE* pdest = static_cast<FILTER_OUTPUT_DATA_TYPE*>(pDest);

   /*
     La imagen de salida es mas chica que la de entrada (no tiene los bordes
     que se agregaron para aplicar la convolucion), esto significa que al
     aplicar el kernel y guardar el resultado en la salida,
     este queda en el pixel correspondiente al centro de la matriz.
     Ej: Para el primer pixel de la imagen de entrada se aplica el filtro desde
     [0, 0] hasta [FilterHeight-1, FilterWidth-1], al guardarlo en el pixel
     0 de la imagen de salida se esta guardando en la posicion que corresponde a
     [(FilterHeight-1)/2, (FilterWidth-1)/2] de la imagen de entrada
   */
   int imageendy = ImageHeight - FilterHeight + 1;
   int imageendx = ImageWidth - FilterWidth + 1;
   int n = 0;
   for (int i = 0; i < imageendy; i++) {
      for (int j = 0; j < imageendx; j++) {
         FILTER_OUTPUT_DATA_TYPE output = 0;
         for (int k = 0; k < FilterHeight; k++)
            for (int m = 0; m < FilterWidth; m++)
               output += psrc[(i + k) * ImageWidth + j + m] *
                                                   pmirroredfilter[k][m];
         pdest[n] = output;
         n++;
      }
   }

   // Elimino matriz temporal
   for (int i = 0; i < FilterWidth; i++)
      delete[] pmirroredfilter[i];
   delete[] pmirroredfilter;
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ConvolutionFilterRenderer::Parameters::FilterFunctionType,
                                                                     kernelfilter);

/**
 * Este template se utiliza para aplicar un filtro a la imagen.
 * Busca el maximo dentro del bloque que corresponde a cada pixel
 * \pre pDest debe tener tamanio imagen con pixeles en formato double.
 * \pre pSrc debe tener tamanio Size.
 * \post pDest tiene los datos luego de aplicar filtro.
 * @param[out] pDest puntero a los datos luego de aplicar el filtro.
 * @param[in] pSrc puntero a los datos a filtrar.
 * @param[in] pFilter matriz con doubles del filtro.
 * @param[in] ImageHeight altura de la imagen.
 * @param[in] ImageWidth ancho de la imagen.
 * @param[in] FilterHeight altura del filtro.
 * @param[in] FilterWidth ancho del filtro.
 */
template<typename T>
void mayorityfilter(void* pDest, void* pSrc,
            std::vector<std::vector<double> > pFilter, int ImageWidth, int ImageHeight,
            int FilterWidth, int FilterHeight) {
   T* psrc = static_cast<T*>(pSrc);
   T* pdest = static_cast<T*>(pDest);

   std::pair<typename std::map<T, int>::iterator, bool> ret;
   typename std::map<T, int>::iterator it;

   // Calculo minimo y maximo dependiendo del tipo
   int imageendy = ImageHeight - FilterHeight + 1;
   int imageendx = ImageWidth - FilterWidth + 1;
   int n = 0;
   for (int i = 0; i < imageendy; i++) {
      for (int j = 0; j < imageendx; j++) {
         std::map<T, int> valuecount;
         for (int k = 0; k < FilterHeight; k++) {
            for (int m = 0; m < FilterWidth; m++) {
               ret = valuecount.insert(std::pair<T, int>(
                                    psrc[(i + k) * ImageWidth + j + m], 1));
               if (!ret.second)
                  (ret.first)->second = (ret.first)->second + 1;
            }
         }
         int maxcount = 0;
         T maxvalue = 0;
         it = valuecount.begin();
         for (; it != valuecount.end(); it++)
            if (it->second > maxcount) {
               maxcount = it->second;
               maxvalue = it->first;
            }

         pdest[n] = maxvalue;
         n++;
      }
   }
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ConvolutionFilterRenderer::Parameters::FilterFunctionType,
                                                                    mayorityfilter);

/**
 * Este template se utiliza para aplicar un filtro a la imagen.
 * Busca el maximo dentro del bloque que corresponde a cada pixel
 * \pre pDest debe tener tamanio imagen con pixeles en formato double.
 * \pre pSrc debe tener tamanio Size.
 * \post pDest tiene los datos luego de aplicar filtro.
 * @param[out] pDest puntero a los datos luego de aplicar el filtro.
 * @param[in] pSrc puntero a los datos a filtrar.
 * @param[in] pFilter matriz con doubles del filtro.
 * @param[in] ImageHeight altura de la imagen.
 * @param[in] ImageWidth ancho de la imagen.
 * @param[in] FilterHeight altura del filtro.
 * @param[in] FilterWidth ancho del filtro.
 */
template<typename T>
void minorityfilter(void* pDest, void* pSrc,
            std::vector<std::vector<double> > pFilter, int ImageWidth, int ImageHeight,
            int FilterWidth, int FilterHeight) {
   T* psrc = static_cast<T*>(pSrc);
   T* pdest = static_cast<T*>(pDest);

   std::pair<typename std::map<T, int>::iterator, bool> ret;
   typename std::map<T, int>::iterator it;

   // Calculo minimo y maximo dependiendo del tipo
   int imageendy = ImageHeight - FilterHeight + 1;
   int imageendx = ImageWidth - FilterWidth + 1;
   int n = 0;
   for (int i = 0; i < imageendy; i++) {
      for (int j = 0; j < imageendx; j++) {
         std::map<T, int> valuecount;
         for (int k = 0; k < FilterHeight; k++) {
            for (int m = 0; m < FilterWidth; m++) {
               ret = valuecount.insert(std::pair<T, int>(
                                    psrc[(i + k) * ImageWidth + j + m], 1));
               if (!ret.second)
                  (ret.first)->second = (ret.first)->second + 1;
            }
         }
         int mincount = FilterWidth * FilterHeight + 1;
         T minvalue = 0;
         it = valuecount.begin();
         for (; it != valuecount.end(); it++)
            if (it->second < mincount) {
               mincount = it->second;
               minvalue = it->first;
            }

         pdest[n] = minvalue;
         n++;
      }
   }
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(ConvolutionFilterRenderer::Parameters::FilterFunctionType,
                                                                    minorityfilter);

/** Ctor */
ConvolutionFilterRenderer::ConvolutionFilterRenderer() {
}

/** Dtor */
ConvolutionFilterRenderer::~ConvolutionFilterRenderer() {
}

/**
 * Genera un nodo XML a partir de los parametros
 * @param[in] RenderParameters parametros con los que se generara el nodo
 * @return plutnode nodo XML con el filtro de convolucion
 */
wxXmlNode *ConvolutionFilterRenderer::GetXmlNode(
      const ConvolutionFilterRenderer::Parameters &RenderParameters) {
   wxXmlNode *pfilternode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(CONVOLUTION_NODE));

   // Creo nodo con tipo de algoritmo
   new wxXmlNode(new wxXmlNode(pfilternode, wxXML_ELEMENT_NODE,
           wxT(ALGORITHM_NAME_NODE)), wxXML_TEXT_NODE, wxEmptyString,
           RenderParameters.filterName_.c_str());

   // Creo nodo cantidad de columnas del filtro
   wxString width = wxString::Format(wxT("%d"), RenderParameters.filterWidth_);
   new wxXmlNode(new wxXmlNode(pfilternode, wxXML_ELEMENT_NODE,
           wxT(FILTER_WITDH)), wxXML_TEXT_NODE, wxEmptyString, width);

   // Creo nodo con cantidad de filas del filtro
   wxString height = wxString::Format(wxT("%d"), RenderParameters.filterHeight_);
   new wxXmlNode(new wxXmlNode(pfilternode, wxXML_ELEMENT_NODE,
           wxT(FILTER_HEIGHT)), wxXML_TEXT_NODE, wxEmptyString, height);

   // Aplico lut
   std::list<std::vector<std::vector<double> > >::const_iterator itfilters =
         RenderParameters.filters_.begin();
   for (; itfilters != RenderParameters.filters_.end(); itfilters++) {
      wxXmlNode *ptable = NULL;
      CreateTable(ptable, TABLE_NODE, RenderParameters.filterHeight_,
                  RenderParameters.filterWidth_);
      // Cargo valores en matriz desde xml
      for (int i = 0; i < RenderParameters.filterHeight_; i++) {
         std::vector<std::string> values;

         for (int j = 0; j < RenderParameters.filterWidth_; j++) {
            std::stringstream valuesstream;
            valuesstream << (*itfilters)[i][j];
            std::string valaux = valuesstream.str();
            values.push_back(valaux);
         }
         SetRow(ptable, i, values);
      }
      pfilternode->AddChild(ptable);
   }

   return pfilternode;
}

/**
 * Genera los parametros a partir de un nodo. Busca el nodo del filtro
 * en el nodo renderizacion del elemento y carga en parameters los
 * valores de la tabla.
 * @param[in] pNode del elemento
 * @return params parametros obtenidos del nodo
 */
ConvolutionFilterRenderer::Parameters ConvolutionFilterRenderer::GetParameters(
      const wxXmlNode *pNode) {
   Parameters params;

   wxXmlNode *pnode = NULL;
   // Obtengo nodo de renderizacion
   wxXmlNode *pdnode = pNode->GetChildren();
   wxXmlNode *pcnode = NULL;
   while (pdnode != NULL) {
      // Obtengo nodo renderizacion
      if (pdnode->GetName() == wxT(RENDERIZATION_NODE)) {
         pcnode = pdnode->GetChildren();
      }
      pdnode = pdnode->GetNext();
   }
   // Obtengo nodo lut
   while (pcnode != NULL) {
      if (pcnode->GetName() == wxT(CONVOLUTION_NODE) &&
            GetNodeByName(wxT(ALGORITHM_NAME_NODE), pcnode) != NULL) {
         pnode = pcnode;
         pcnode = NULL;
      } else {
         pcnode = pcnode->GetNext();
      }
   }

   if (pnode) {
      // Obtengo el nombre del algoritmo
      params.filterName_ = "";
      const wxXmlNode *pdatanode = GetNodeByName(wxT(ALGORITHM_NAME_NODE), pnode);
      if (pdatanode)
         params.filterName_ = pdatanode->GetNodeContent().c_str();

      pdatanode = GetNodeByName(wxT(FILTER_HEIGHT), pnode);
      if (pdatanode)
         params.filterHeight_ = StringToNumber<int>(pdatanode->GetNodeContent().c_str());

      pdatanode = GetNodeByName(wxT(FILTER_WITDH), pnode);
      if (pdatanode)
         params.filterWidth_ = StringToNumber<int>(pdatanode->GetNodeContent().c_str());

      // Recorro las tablas y las cargo
      pnode = pnode->GetChildren();
      while (pnode != NULL) {
         if (pnode->GetName() == wxT(TABLE_NODE)) {
            // Obtengo Tabla de nodo y la cargo en lista de params
            int width, height;
            GetDimension(pnode, width, height);
            if (params.filterWidth_ == width && params.filterHeight_ == height) {
               // Genero matriz
               std::vector<std::vector<double> > filter;
               LoadTable(pnode, filter);
               // Cargo el filtro a la lista
               params.filters_.push_back(filter);
            }
         }
         pnode = pnode->GetNext();
      }
   } else {
      REPORT_DEBUG("D: Se intento obtener parametros de LUT de elemento sin este nodo");
   }

   return params;
}

/**
 * Crea un renderizador a partir de un pElement
 * @param[in] pElement elemento a partir del cual se crea el renderizador
 * @param[in] pPreviousRenderer renderer anterior en el pipeline
 * @return pfilter nodo renderizacion
 * @return NULL si no existe el renderizador anterior o no existe el elemento
 */
Renderer *ConvolutionFilterRenderer::Create(Element *pElement,
                                            Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   // Si no existe nodo convolucion con una tabla retorno false
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(CreatedNode().c_str()) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(wxT(TABLE_NODE));

   wxXmlNode *pnode = pElement->GetNode(nodepath);
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML en el elemento \"%s\"", pElement->GetName().c_str());
      return NULL;
   }

   // Leo parametros de nodo
   Parameters params;
   params = GetParameters(pElement->GetNode(wxT("")));

   // Cargo el tipo de dato de entrada
   int x, y, b;
   std::string datatype;
   pPreviousRenderer->GetOutputParameters(x, y, b, datatype);
   if (b < static_cast<int>(params.filters_.size())) {
      REPORT_AND_FAIL_VALUE("D:Cantidad de Bandas es inferior a la cantidad de filtros",
                            NULL);
   }

   if (params.filterName_ == ALGORITHM_VALUE_KERNELFILTER)
      params.pFunction_ = kernelfilterTypeMap[datatype];
   if (params.filterName_ == ALGORITHM_VALUE_MINORITYFILTER)
      params.pFunction_ = minorityfilterTypeMap[datatype];
   if (params.filterName_ == ALGORITHM_VALUE_MAYORITYFILTER)
      params.pFunction_ = mayorityfilterTypeMap[datatype];

   if (!params.pFunction_) {
      REPORT_ERROR("D:Tipo de filtro o tipo de dato (%s) no manejado",
                                                 NULL, datatype.c_str());
      return NULL;
   }

   // Creo nuevo renderizador
   ConvolutionFilterRenderer *pfilter = new ConvolutionFilterRenderer;
   pfilter->parameters_.pFunction_ = params.pFunction_;
   if (!pfilter->SetParameters(params)) {
      delete pfilter;
      return NULL;
   }
   return pfilter;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del renderer
 */
std::string ConvolutionFilterRenderer::CreatedNode() const {
   return wxT(CONVOLUTION_NODE);
}

/**
 * Antes de llamar a renderizar agranda el mundo con un borde usando
 * World::Transform, por lo tanto el canvas retornado tendra
 * pixeles de mas en los bordes.
 * Con los resultados aplica el filtro y achica el canvas
 * al tamanio original(sin bordes para filtro)
 * @param[in] pWorldWindow mundo con informacion del subset a renderizar
 * @param[in] pCanvas datos sobre los que se aplicara el filtro
 * @param[out] pCanvas datos obtenidos de aplicar el filtro
 * @param[in] pMask mascara
 */
bool ConvolutionFilterRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                       Mask* pMask) {
   bool prevrenderizationstatus = true;
   Subset auxwourld;
   pWorldWindow->GetWindow(auxwourld);

   World world(*pWorldWindow);
   // Claculo el nuevo viewport
   Subset windowsubset;
   int newviwportheight, newviwportwidth;
   pWorldWindow->GetViewport(newviwportwidth, newviwportheight);
   newviwportwidth += (parameters_.filterWidth_ - 1) / 2;
   newviwportheight += (parameters_.filterHeight_ - 1) / 2;
   Coordinates newviewportlr(newviwportwidth, newviwportheight);
   Coordinates newviewportul(-(parameters_.filterWidth_ - 1) / 2,
                             -(parameters_.filterHeight_ - 1) / 2);

   // Calculo la ventana del nuevo Viewport
   world.Transform(newviewportlr, windowsubset.lr_);
   world.Transform(newviewportul, windowsubset.ul_);

   // Modifico el window con nuevos valores
   world.SetWindow(windowsubset);
   world.SetViewport(newviwportwidth + (parameters_.filterWidth_ - 1) / 2,
                     newviwportheight + (parameters_.filterWidth_ - 1) / 2);

   // Cambio tipo de datoen canvas y llamo a Render
   Canvas *ptempcanvas = pCanvas;
   int canvassizex, canvassizey, bandcount = 0;
   std::string datatype;
   if (pPreviousRenderer_ && pCanvas) {
      pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey, bandcount, datatype);
      if (parameters_.filterName_ == ALGORITHM_VALUE_KERNELFILTER) {
         if (bandcount < static_cast<int>(parameters_.filters_.size())) {
            REPORT_AND_FAIL_VALUE(
                  "D:Cantidad de Bandas es inferior a la cantidad de filtros.", false);
         }

         pCanvas->SetDataType(datatype);
         // no puede cambiar el tipo de canvas
         if (pCanvas->GetDataType() != datatype) {
            ptempcanvas = new MemoryCanvas;
            REPORT_DEBUG("D:Se creo un canvas intermedio");
         }
      }
      prevrenderizationstatus = pPreviousRenderer_->Render(&world, ptempcanvas, pMask);
   } else {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar filtro.", false);
   }

   // Leo el tamanio del canvas retornado
   pCanvas->GetSize(canvassizex, canvassizey);
   if (canvassizex < 1 || canvassizey < 1) {
      REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
   }

   // leo el canvas retornado
   std::vector<int> bands(bandcount);
   std::vector<void*> data(bandcount);
   for (int b = 0; b < bandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];
   }
   pCanvas->Read(bands, data);

   // Calculo el tamanio del nuevo canvas(sin bordes)
   // NOTA: el +2 esta por error de redondeo en zoom.
   int nobordercanvassizex = canvassizex - (parameters_.filterWidth_ - 1),
         nobordercanvassizey = canvassizey - (parameters_.filterHeight_ - 1);

   // Si el canvas es el original cambio el tipo de dato al de la salida
   if (ptempcanvas == pCanvas &&
         parameters_.filterName_ == ALGORITHM_VALUE_KERNELFILTER) {
      pCanvas->SetDataType(DataInfo<FILTER_OUTPUT_DATA_TYPE>::Name);
   }
   pCanvas->SetBandCount(bandcount);
   pCanvas->SetSize(nobordercanvassizex, nobordercanvassizey);

   // Creo vector donde se guardan datos de salida temporales
   std::vector<void*> outdata(bandcount);
   for (int b = 0; b < bandcount; b++)
      outdata[b] = new unsigned char[nobordercanvassizex * nobordercanvassizey
            * pCanvas->GetDataSize()];

   std::list<std::vector<std::vector<double> > >::iterator itfilter =
         parameters_.filters_.begin();
   std::vector<std::vector<double> > filter;
   for (int i = 0; i < bandcount; i++, itfilter++) {
      if (itfilter != parameters_.filters_.end()) {
         filter = *itfilter;
      }
      parameters_.pFunction_(outdata[i], data[i],
                             filter, canvassizex, canvassizey, parameters_.filterWidth_,
                             parameters_.filterHeight_);
   }

   // Guardo cambios
   pCanvas->Write(bands, outdata);

   // libero el temporario
   for (int b = 0; b < pCanvas->GetBandCount(); b++) {
      delete[] static_cast<unsigned char*>(data[b]);
      delete[] static_cast<unsigned char*>(outdata[b]);
   }

   return true && prevrenderizationstatus;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]	pWorld: puntero a la ventana del mundo.
 * @param[in]	Ulx: uper-left x
 * @param[in]	Uly: uper-left y
 * @param[in]	Lrx: lower-right x
 * @param[in]	Lry: lower-right y
 */
void ConvolutionFilterRenderer::GetBoundingBox(const World *pWorld, double &Ulx,
                                               double &Uly, double &Lrx, double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void ConvolutionFilterRenderer::GetInputParameters(int &SizeX, int &SizeY,
                                                   int &BandCount,
                                                   std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }

   BandCount = std::max(BandCount, static_cast<int>(parameters_.filters_.size()));
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void ConvolutionFilterRenderer::GetOutputParameters(int &SizeX, int &SizeY,
                                                    int &BandCount,
                                                    std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }

   // Los filtros de mayoria y minoria no modifican el tipo de dato de salida
   if (parameters_.filterName_ == ALGORITHM_VALUE_KERNELFILTER)
      DataType = DataInfo<FILTER_OUTPUT_DATA_TYPE>::Name;
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void ConvolutionFilterRenderer::Update(Element *pElement) {
   Parameters params;
   params = GetParameters(pElement->GetNode(wxT("")));
   if (!SetParameters(params)) {
      REPORT_DEBUG("D:Error en los parametros");
   }
}

/**
 * Cambia el contenido de parametes interno. El unico
 * parametro que no modifica es la funcion. El filtro debe tener un numero
 * impar de columnas y filas.
 * @param[in] Params nuevos valores que debe tomar parameters
 * @return false si el ancho / alto de los filtros es menor o igual a cero o par
 * @return true en otro caso
 */
bool ConvolutionFilterRenderer::SetParameters(const Parameters &Params) {
   // Si tiene numero impar de columnas o filas no hace nada
   if (Params.filterWidth_ < 0 || Params.filterWidth_ < 0
         || (Params.filterWidth_ % 2) == 0 || (Params.filterHeight_ % 2) == 0) {
      return false;
   }

   // Guardo los filtros
   parameters_.filters_ = Params.filters_;
   parameters_.filterWidth_ = Params.filterWidth_;
   parameters_.filterHeight_ = Params.filterHeight_;
   parameters_.filterName_ = Params.filterName_;

   return true;
}
}

