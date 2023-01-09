/*! \file GcpDetectionSubprocess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
#include <string>
// Includes Suri
#include "FileCanvas.h"
#include "MovingWindowController.h"
#include "Mask.h"
#include "suri/ElementVectorSerializer.h"
#include "suri/GcpDetectionSubprocess.h"
#include "suri/DataTypes.h"
#include "suri/RasterElement.h"
#include "WaveletModulusRenderer.h"
#include "suri/XmlFunctions.h"
// Includes Wx
#include "wx/xml/xml.h"
// Defines
#define DAUBECHIES_OUTPUT 3

/**
 * Obtiene el valor NoDataValue de un elemento (si existe)
 */
void GetNoDataValue(const suri::Element* pElement, bool& Available, double& NoDataValue) {
   Available = false;
   NoDataValue = 0.0;
   wxXmlNode* root = pElement->GetNode("");
   std::string str = suri::Xml2String(root);
   wxXmlNode* pdatanotvalidvalue = pElement->GetNode(
   PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue) {
      pdatanotvalidvalue = pdatanotvalidvalue->GetChildren();
      if (pdatanotvalidvalue->GetType() == wxXML_TEXT_NODE) {
         wxString ctt = pdatanotvalidvalue->GetContent();
         Available = true;
         NoDataValue = suri::StringToNumber<double>(ctt.mb_str());
      }
   }
}

/**
 * Crea un nodo XML para correr un filtro Daubechies
 */
void ConfigureDaubechiesProcess(suri::Element* pElement, const std::vector<int>& bands) {
   // Bandas que se van a renderizar
   std::stringstream bandcomb;
   suri::WaveletModulusRenderer::Parameters params;
   std::vector<int>::const_iterator it;
   int i = 0;
   for (it = bands.begin(); it != bands.end(); ++it) {
      bandcomb << *it << " ";
      params.insertPair(i * DAUBECHIES_OUTPUT + 1, i * DAUBECHIES_OUTPUT + 2);
      ++i;
   }
   // Creo un nodo a partir del nodo combinacion del elemento
   wxXmlNode *pRasterNode = pElement->AddNode(pElement->GetNode(wxT("")),
   RENDERIZATION_NODE);

   pElement->AddNode(pRasterNode, BAND_COMBINATION_NODE, bandcomb.str());
   pElement->AddNode(pRasterNode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                             suri::DataInfo<float>::Name, true);
   pElement->AddNode(pRasterNode, DAUBECHIES_NODE);
   // Creo el nodo de algebra de bandas de modulo wavelet
   wxXmlNode *waveletXml = suri::WaveletModulusRenderer::GetXmlNode(params);
   pElement->AddNode(pRasterNode, waveletXml);
   // Por ultimo me quedo con los mas representativos
   wxXmlNode *pThresholdNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(THRESHOLD_NODE));
   pThresholdNode->AddAttribute(THRESHOLD_PERCENT_PROPERTY, "96");
   pElement->AddNode(pRasterNode, pThresholdNode);
}

void ConfigureDaubechiesProcess2(suri::Element* pElement, const std::vector<int>& bands) {
   // Bandas que se van a renderizar
   std::stringstream bandcomb;
   suri::WaveletModulusRenderer::Parameters params;
   std::vector<int>::const_iterator it;
   int i = 0;
   for (it = bands.begin(); it != bands.end(); ++it) {
      bandcomb << *it << " ";
      params.insertPair(i * DAUBECHIES_OUTPUT + 1, i * DAUBECHIES_OUTPUT + 2);
      ++i;
   }
   // Creo un nodo a partir del nodo combinacion del elemento
   wxXmlNode *pRasterNode = pElement->AddNode(pElement->GetNode(wxT("")),
   RENDERIZATION_NODE);

   pElement->AddNode(pRasterNode, BAND_COMBINATION_NODE, bandcomb.str());
   pElement->AddNode(pRasterNode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                             suri::DataInfo<float>::Name, true);
   pElement->AddNode(pRasterNode, DAUBECHIES_NODE);
   // Creo el nodo de algebra de bandas de modulo wavelet
   wxXmlNode *waveletXml = suri::WaveletModulusRenderer::GetXmlNode(params);
   pElement->AddNode(pRasterNode, waveletXml);

}

void ConfigureEntropyProcess2(suri::RasterElement* pElement, const std::vector<int>& bands) {
   // Bandas que se van a renderizar
   std::stringstream bandcomb;
   std::vector<int>::const_iterator it;
   for (it = bands.begin(); it != bands.end(); ++it) {
      bandcomb << *it << " ";
   }
   // Creo un nodo a partir del nodo combinacion del elemento
   wxXmlNode *pRasterNode = pElement->AddNode(pElement->GetNode(wxT("")),
   RENDERIZATION_NODE);

   pElement->AddNode(pRasterNode, BAND_COMBINATION_NODE, bandcomb.str());

   pElement->AddNode(pRasterNode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                             suri::DataInfo<float>::Name, true);

   wxXmlNode *pEntropyNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(ENTROPY_NODE));
   pElement->AddNode(pRasterNode, pEntropyNode);
   wxXmlNode *pDecimateNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(DECIMATION_NODE));
   pElement->AddNode(pRasterNode, pDecimateNode);
}

/**
 * Crea un nodo XML para correr un proceso de entropia
 */
void ConfigureEntropyProcess(suri::RasterElement* pElement, const std::vector<int>& bands) {
   // Bandas que se van a renderizar
   std::stringstream bandcomb;
   std::vector<int>::const_iterator it;
   for (it = bands.begin(); it != bands.end(); ++it) {
      bandcomb << *it << " ";
   }
   // Creo un nodo a partir del nodo combinacion del elemento
   wxXmlNode *pRasterNode = pElement->AddNode(pElement->GetNode(wxT("")),
   RENDERIZATION_NODE);

   pElement->AddNode(pRasterNode, BAND_COMBINATION_NODE, bandcomb.str());

   pElement->AddNode(pRasterNode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                             suri::DataInfo<float>::Name, true);

   wxXmlNode *pEntropyNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(ENTROPY_NODE));
   pElement->AddNode(pRasterNode, pEntropyNode);
   wxXmlNode *pDecimateNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(DECIMATION_NODE));
   pElement->AddNode(pRasterNode, pDecimateNode);
   wxXmlNode *pThresholdNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(THRESHOLD_NODE));
   pElement->AddNode(pRasterNode, pThresholdNode);
}

namespace suri {

const std::string GcpDetectionSubprocess::kOutputFormat = "GTiff";

/**
 * Constructor
 */
GcpDetectionSubprocess::GcpDetectionSubprocess(
   Element* pElement,  const std::vector<int>& Bands){
   this->bands_ = Bands;
   this->pInputElement_ =static_cast<RasterElement*>(Element::Create(
         "RasterElement", pElement->GetNode(), pElement->GetMetadataMap()));
   this->tempFilePath_.assign(wxFileName::CreateTempFileName(_("")).c_str());
   this->pGcpCanvas_ = CreateFileCanvas(
      this->pInputElement_, this->tempFilePath_ + "Gcp.tif");
   this->pGcpCanvas2_ = CreateFileCanvas(
      this->pInputElement_, this->tempFilePath_ + "Gcp2.tif");
   this->pEntropyCanvas_ = CreateFileCanvas(
      this->pInputElement_, this->tempFilePath_ + "Entropy.tif");
   this->pEntropyCanvas2_ = CreateFileCanvas(
      this->pInputElement_, this->tempFilePath_ + "Entropy2.tif");
   this->pEntropyCastCanvas_ = CreateFileCanvas(
      this->pInputElement_, this->tempFilePath_ + "EntropyCast.tif");
   this->pFilteredGcpCanvas_ = CreateFileCanvas(
      this->pInputElement_, this->tempFilePath_ + "FilteredGcp.tif");

   this->pEntropyCastCanvas_->SetDataType(DataInfo<unsigned char>::Name);
   this->pFilteredGcpCanvas_->SetDataType(DataInfo<float>::Name);
}

/**
 * Destructor
 */
GcpDetectionSubprocess::~GcpDetectionSubprocess() {
   /*wxRemoveFile(this->tempFilePath_);
   wxRemoveFile(this->tempFilePath_ + "Entropy.tif");
   wxRemoveFile(this->tempFilePath_ + "EntropyCast.tif");*/
}

RasterElement* GcpDetectionSubprocess::RunProcess() {
   std::string entropyCanvasPath = this->pEntropyCanvas_->GetFileName();
   std::string entropyCanvasCastPath = this->pEntropyCastCanvas_->GetFileName();
   std::string gcpCanvasPath = this->pGcpCanvas_->GetFileName();

   // Renderizo entropia
   ConfigureEntropyProcess(this->pInputElement_, this->bands_);
   RenderElement(this->pInputElement_, this->pEntropyCanvas_);
   // Cierro la imagen
   delete pEntropyCanvas_;
   ConfigureEntropyProcess2(this->pInputElement_, this->bands_);
   RenderElement(this->pInputElement_, this->pEntropyCanvas2_);
   // Cierro la imagen
   delete pEntropyCanvas2_;

   ConfigureDaubechiesProcess(this->pInputElement_, this->bands_);
   // Renderizo transformacion daubechies, con umbral y enmascaro
   RenderElement(this->pInputElement_, this->pGcpCanvas_);
   // Libero y cierro la imagen
   delete pGcpCanvas_;
   
   ConfigureDaubechiesProcess2(this->pInputElement_, this->bands_);
   // Renderizo transformacion daubechies, con umbral y enmascaro
   RenderElement(this->pInputElement_, this->pGcpCanvas2_);
   // Libero y cierro la imagen
   delete pGcpCanvas2_;

   // Levanto el archivo de entropia, convierto a uchar
   RasterElement* pEntropyMaskCastElement = RasterElement::Create(entropyCanvasPath);
   wxXmlNode *pRasterNode = pEntropyMaskCastElement->
                           AddNode(pEntropyMaskCastElement->GetNode(wxT("")),
                           RENDERIZATION_NODE);
   pEntropyMaskCastElement->AddNode(pRasterNode, BAND_COMBINATION_NODE, "0");
   pEntropyMaskCastElement->AddNode(pRasterNode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                             suri::DataInfo<unsigned char>::Name, true);
   // Lo renderizo en un archivo uchar (ver si se puede evitar crear este canvas)
   RenderElement(pEntropyMaskCastElement, this->pEntropyCastCanvas_);
   // Libero recursos y flusheo canvas
   delete pEntropyMaskCastElement;
   delete this->pEntropyCastCanvas_;

   // Levanto la mascara de entropia otra vez, esta vez ya es uchar
   RasterElement* pEntropyMaskElement = RasterElement::Create(entropyCanvasCastPath);
   // Preparo mascara
   pRasterNode = pEntropyMaskElement->AddNode(pEntropyMaskElement->GetNode(wxT("")),
                  RENDERIZATION_NODE);
   // Una sola banda puede enmascarar por vez
   pEntropyMaskElement->AddNode(pRasterNode, BAND_COMBINATION_NODE, "0");

   // Vuelvo a renderizar ya que las mascaras parecen no llevarse bien con
   // los renderers que crean muchas bandas
   RasterElement* pGcpElement = RasterElement::Create(gcpCanvasPath);
   pRasterNode = pGcpElement->AddNode(pGcpElement->GetNode(wxT("")),
                  RENDERIZATION_NODE);
   pGcpElement->AddNode(pRasterNode, BAND_COMBINATION_NODE, "0");
   // Una sola banda puede enmascarar por vez
   pGcpElement->AddNode(pRasterNode, DATA_CAST_NODE, wxT(""), OUTPUT_PROPERTY,
                             suri::DataInfo<float>::Name, true);
   std::string filteredCanvasFilename = this->pFilteredGcpCanvas_->GetFileName();
   RenderElement(pGcpElement, this->pFilteredGcpCanvas_, pEntropyMaskElement);
   // Libero recursos y flusheo
   delete this->pFilteredGcpCanvas_;
   delete pEntropyMaskElement;
   RasterElement* pGcpFilteredElement = RasterElement::Create(
      filteredCanvasFilename);
   return pGcpFilteredElement;
}

/**
 * Crea el canvas de salida para ver los puntos candidatos
 */
FileCanvas* GcpDetectionSubprocess::CreateFileCanvas(RasterElement* element,
                                                   const std::string& filepath) const {
   Option option;
   option.SetOption("Geo", element->GetSpatialReference().mb_str());
   option.SetOption("Format", GcpDetectionSubprocess::kOutputFormat);
   int width, height;
   element->GetRasterSize(width, height);
   // La decimacion la hago aca porque el renderpipeline no puede
   width /= 2;
   height /= 2;
   Subset viewport(Coordinates(), Coordinates(width, height));
   Subset worldextent;
   element->GetElementExtent(worldextent);

   RasterSpatialModel *prm = RasterSpatialModel::Create(viewport.ul_, viewport.lr_,
                                                        worldextent.ul_,
                                                        worldextent.lr_);
   // Agrego info del modelo geoespacial
   if (prm) {
      option.SetOption("Matrix", prm->GetWkt());
   }
   RasterSpatialModel::Destroy(prm);

   // Inicializo la salida
   FileCanvas *pOutputCanvas = new FileCanvas(filepath,
                                              width, height, option);
   bool hasNoDataValue = false;
   double noDataValue = 0.0;
   GetNoDataValue(element, hasNoDataValue, noDataValue);
   // Configuro canvas
   pOutputCanvas->SetNoDataValueAvailable(hasNoDataValue);
   pOutputCanvas->SetNoDataValue(noDataValue);
   pOutputCanvas->SetSize(width, height);
   pOutputCanvas->SetBandCount(element->GetBandCount());
   pOutputCanvas->SetDataType(DataInfo<float>::Name);
   return pOutputCanvas;
}

/**
 * Ejecuta el pipeline de renderizacion
 */
void GcpDetectionSubprocess::RenderElement(RasterElement* pElement,
                                             Canvas* pCanvas, Element* pMask) {
   
   MovingWindowController mwController;
   LayerList *pList = new LayerList();
   pList->AddElement(pElement);
   if (pMask) {
      pMask->Activate();
      LayerList *pMaskList = new LayerList();
      pMaskList->AddElement(pMask);
      mwController.SetMaskList(pMaskList);
   }

   World* pWorld = pElement->CreateWorld();

   mwController.SetRenderizationList(pList);
   mwController.SetWorld(pWorld);
   mwController.SetOutputCanvas(pCanvas);
   mwController.SetBestBufferSize();

   pElement->Activate(true);

   if (!mwController.Render()) {
      throw message_RASTER_OPERATION_ERROR;
   }
   delete pWorld;
   // TODO matiaslafroce: pasarle una copia a pList, así la borra sin problemas
   // LEAK! delete pList;
}
}
