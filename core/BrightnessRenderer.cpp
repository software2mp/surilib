/*! \file BrightnessRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "BrightnessRenderer.h"

// Includes standard
#include <sstream>
#include <algorithm>
#include <limits>

// Includes suri
#include "suri/messages.h"
#include "MemoryCanvas.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"

// Includes wx
#include "wx/xml/xml.h"

/** Macro para registrar el renderer */
AUTO_REGISTER_RENDERER(suri::BrightnessRenderer);

/** namespace suri */
namespace suri {
/** Template brillo / contraste */
/**
 *  Este template se utiliza para realizar el cambio de brillo y contraste.
 *
 *  Utiliza la formula:
 *      DN' = Brillo+Contraste*DN
 *
 *  Guarda el dato cada Step posiciones de pSrc en pDest hasta el tamanio Size.
 * \pre pDest debe tener Size tamanio reservado
 * \pre pSrc debe tener Size*Step tamanio reservado
 * \post pDest tiene los datos de pSrc cada Step pasos
 * @param[out] pDest puntero a los datos "realzados"
 * @param[in] pSrc puntero a los datos a "realzar"
 * @param[in] Size Tamanio en T de pDest
 * @param[in] Brightness Valor de brillo a aplicar
 * @param[in] Contrast Valor de contraste a aplicar
 * \todo Here be interpolation
 * \todo: sacar cuando funcione la mascara
 */
template<typename T>
void brightness(void* pDest, void* pSrc, size_t Size, double Brightness,
                double Contrast) {
   T* pdest = static_cast<T*>(pDest);
   T* psrc = static_cast<T*>(pSrc);
   // \todo Here be interpolation
   for (size_t i = 0; i < Size; i++) {
      double newval = Contrast * (psrc[i] + Brightness) + Brightness;
      if (psrc[i] != 0) {
         // no permite overflow
         if (newval > std::numeric_limits<T>::max()) {
            pdest[i] = std::numeric_limits<T>::max();
         } else
         // no permite underflow
         if (newval < std::numeric_limits<T>::min()) {
            pdest[i] = std::numeric_limits<T>::min();
         } else {
            pdest[i] = static_cast<T>(newval);
         }
      } else {
         // \todo: sacar cuando funcione la mascara
         pdest[i] = psrc[i];
      }
   }
}
/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(BrightnessRenderer::Parameters::BrightnessFunc, brightness);

/**
 * Constructor
 */
BrightnessRenderer::BrightnessRenderer() {
}

/**
 * Destructor
 */
BrightnessRenderer::~BrightnessRenderer() {
}

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------

/**
 * Genera un nodo XML a partir de los parametros
 * \attention el nodo retornado(y su eliminacion) es responsabilidad de la clase invocante.
 * @param[in] RenderParameters parametros de brillo
 * @return plutnode nodo brillo o NULL
 */
wxXmlNode *BrightnessRenderer::GetXmlNode(const Parameters &RenderParameters) {
   wxXmlNode *plutnode = new wxXmlNode(
         NULL,
         wxXML_ELEMENT_NODE,
         wxT(BRIGHTNESS_NODE),
         wxT(""),
         new wxXmlProperty(wxT(NAME_PROPERTY),
                           wxT(NAME_PROPERTY_VALUE_LUT_BRIGHTNESS)));
   std::stringstream ss;
   ss << RenderParameters.contrast_;
   new wxXmlNode(
         new wxXmlNode(plutnode, wxXML_ELEMENT_NODE, wxT(LUT_CONTRAST_NODE), wxT(
               "")),
         wxXML_TEXT_NODE, wxT(""), wxT(ss.str() ));

   ss.str("");
   ss << RenderParameters.brightness_;
   new wxXmlNode(
         new wxXmlNode(plutnode, wxXML_ELEMENT_NODE, wxT(LUT_BRIGHTNESS_NODE), wxT(
               "")),
         wxXML_TEXT_NODE, wxT(""), wxT(ss.str() ));

   return plutnode;
}

/**
 * Obtiene el nodo de interes a partir de un elemento
 * \attention el nodo retornado(y su eliminacion) es responsabilidad de la clase invocante.
 * @param[in] pElement nodo del elemento
 * @return pnode nodo brillo o NULL
 */
wxXmlNode *BrightnessRenderer::GetXmlNode(const Element *pElement) {
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + NODE_SEPARATION_TOKEN
         + wxT(BrightnessRenderer().CreatedNode() );
   wxXmlNode *pnode = pElement->GetNode(nodepath);
   if (!pnode) {
      return NULL;
   }
   wxString luttype;
   pnode->GetPropVal(wxT(NAME_PROPERTY), &luttype);
   if (luttype != wxT(NAME_PROPERTY_VALUE_LUT_BRIGHTNESS)) {
      return NULL;
   }
   return pnode;
}

/**
 * Agrega/reemplaza el nodo renderizacion.
 * @param[in]	pNode: nodo renderizacion nuevo
 * @param[out]	pElement: elemento con el nodo renderizacion reemplazado.
 * @return true.
 */
bool BrightnessRenderer::SetXmlNode(Element *pElement, wxXmlNode *pNode) {
   wxString luttype;
   pNode->GetPropVal(wxT(NAME_PROPERTY), &luttype);
   if (luttype != wxT(NAME_PROPERTY_VALUE_LUT_BRIGHTNESS)) {
      REPORT_AND_RETURN_VALUE("D:Nodo %s invalido.", false, pNode->GetName().c_str());
   }

   wxXmlNode *pnode = GetXmlNode(pElement);
   if (!pnode) {
      REPORT_AND_FAIL_VALUE("D:No se encontro el nodo.", false);
   }
   pElement->AddNode(pnode->GetParent(), pNode, true);
   return true;
}

/**
 * Genera los parametros a partir de un nodo
 * @param[in] pNode nodo XML con los parametros
 * @return param variable con los parametros obtenidos del nodo XML
 */
BrightnessRenderer::Parameters BrightnessRenderer::GetParameters(
      const wxXmlNode *pNode) {
   Parameters param;
   if (!pNode) {
      REPORT_AND_FAIL_VALUE("D:Nodo vacio", param);
   }

   wxXmlNode *pnode = pNode->GetChildren();
   while (pnode) {
      wxXmlNode *ptemp = pnode->GetChildren();
      if (pnode->GetName() == wxT(LUT_BRIGHTNESS_NODE) && ptemp) {
         std::stringstream ss;
         ss << ptemp->GetContent();
         ss >> param.brightness_;
      }
      if (pnode->GetName() == wxT(LUT_CONTRAST_NODE) && ptemp) {
         std::stringstream ss;
         ss << ptemp->GetContent();
         ss >> param.contrast_;
      }
      pnode = pnode->GetNext();
   }

   return param;
}

// -------------------------- METODO DE CREACION ----------------------------
/**
 * Crea un renderizador a partir de un pElement
 * \attention el brightnessRenderer retornado(y su eliminacion) son responsabilidad del invocante
 * \todo reventar y llevar a deteccion on-render
 * @param[in] pElement nodo del elemento
 * @param[in] pPreviousRenderer renderizador anterior
 * @return lut renderizador de brillo
 * @return NULL si no existe el renderizador previo, o si no se puede obtener el
 *                nodo brillo del elemento.
 */
Renderer *BrightnessRenderer::Create(Element *pElement,
                                     Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   wxXmlNode *pnode = GetXmlNode(pElement);
   if (!pnode) {
      return NULL;
   }

   BrightnessRenderer *plut = new BrightnessRenderer;
   plut->parameters_ = GetParameters(pnode);
   int x, y, b;
   std::string datatype;
   pPreviousRenderer->GetOutputParameters(x, y, b, datatype);
   // \todo reventar y llevar a deteccion on-render
   for (int i = 0; i < b; i++)
      plut->parameters_.bands_.push_back(i);
   plut->parameters_.outputDataType_ = datatype;
   plut->parameters_.inputDataType_ = datatype;

   plut->parameters_.pFunction_ = brightnessTypeMap[plut->parameters_.outputDataType_];
   return plut;
}

/** Nombre del renderizador == al nombre del nodo */
std::string BrightnessRenderer::CreatedNode() const {
   return BRIGHTNESS_NODE;
}

/** Renderiza el elemento dado un World en un Canvas */
/**
 * Lee las bandas del canvas y utiliza pFunction_ del Parameters para
 * cambiarle el brillo y contraste deacuerdo al tipo de dato de entrada.
 * Finalmente guarda las bandas nuevamente en el canvas
 * No usa pMask
 * @param[in] pWorldWindow puntero al mundo
 * @param[out] pCanvas canvas para la renderizacion
 * @param[in] pMask mascara para los datos
 * @return true si se completa la renderizacion
 * @return false si falla el renderizador previo
 */
bool BrightnessRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                Mask* pMask) {
   bool prevrenderizationstatus = true;
   if (pPreviousRenderer_) {
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, pCanvas,
                                                           pMask);
   }
   // \todo se deberia hacer en Update, pero necesita a pPreviousRenderer_
#ifndef __PREVIOUS_RENDERER_IN_UPDATE__
   parameters_.bands_.clear();
   {
      int x, y, b;
      std::string datatype;
      pPreviousRenderer_->GetOutputParameters(x, y, b, datatype);
      // \todo reventar y llevar a deteccion on-render
      for (int i = 0; i < b; i++)
         parameters_.bands_.push_back(i);
   }
#endif   // __NO_PREVIOUS_RENDERER_IN_UPDATE__
   int x, y;
   pCanvas->GetSize(x, y);
   int size = x * y;
   std::vector<void*> data;
   for (size_t i = 0; i < parameters_.bands_.size(); i++)
      data.push_back(new unsigned char[size * pCanvas->GetDataSize()]);
   // leo del canvas
   pCanvas->Read(parameters_.bands_, data);

   // inicializo el canvas
   pCanvas->SetDataType(parameters_.outputDataType_);
   // busco el maximo indice de banda
   pCanvas->SetBandCount(
         *std::max_element(parameters_.bands_.begin(), parameters_.bands_.end()) + 1);
   pCanvas->SetSize(x, y);

   std::vector<void*> outdata;
   for (size_t i = 0; i < parameters_.bands_.size(); i++) {
      outdata.push_back(new unsigned char[size * SizeOf(parameters_.outputDataType_)]);
      parameters_.pFunction_(outdata[i], data[i], size, parameters_.brightness_,
                             parameters_.contrast_);
      delete[] static_cast<unsigned char*>(data[i]);
   }
   pCanvas->Write(parameters_.bands_, outdata);
   for (size_t i = 0; i < parameters_.bands_.size(); i++) {
      delete[] static_cast<unsigned char*>(outdata[i]);
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
void BrightnessRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                        double &Lrx, double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void BrightnessRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                            std::string &DataType) const {
   BandCount = parameters_.bands_.size();
   DataType = parameters_.inputDataType_;
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void BrightnessRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                             std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
   DataType = parameters_.outputDataType_;
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]	pElement: puntero al elemento.
 */
void BrightnessRenderer::Update(Element *pElement) {
   Parameters temp = GetParameters(GetXmlNode(pElement));
   parameters_.brightness_ = temp.brightness_;
   parameters_.contrast_ = temp.contrast_;
}
}
