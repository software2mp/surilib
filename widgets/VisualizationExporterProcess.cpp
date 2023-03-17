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

// Includes Estandar
#include <memory>

// Includes Suri
#include "suri/VisualizationExporterProcess.h"
#include "suri/DataViewManager.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "DataCastRenderer.h"
#include "MapElement.h"
#include "MapRenderer.h"

namespace suri {

/** Nombre del proceso **/
const std::string VisualizationExporterProcess::kProcessName = "VisualizationExporterProcess";

/** Constructor */
VisualizationExporterProcess::VisualizationExporterProcess(
      RasterElement* pRasterElement, std::vector<Element*> InputElements,
      const Subset &ViewerSubset, DataViewManager* pDataViewManager) :
            FileExporterProcess(InputElements, ViewerSubset, pDataViewManager),
            pRasterElement_(pRasterElement), InputElements_(InputElements),
            pReprojectionParameters_(new ReprojectionProcess::ReprojectionParameters()) {
   SetProcessName(kProcessName);
   RasterElement* praster = dynamic_cast<RasterElement*>(pRasterElement_);
   if (praster) {
      pReprojectionParameters_->rasterModelIn_ = praster->GetRasterModel();
      if (pReprojectionParameters_->rasterModelIn_.empty()) {
         DatasourceInterface* pdatasource =
               pDataViewManager->GetDatasourceManager()->GetDatasourceByElementId(
                     pRasterElement_->GetUid());
         pReprojectionParameters_->rasterModelIn_ =
               dynamic_cast<RasterElement*>(pdatasource->GetElement())->GetRasterModel();
      }
      if (!pReprojectionParameters_->pRasterSpatialModelIn_)
         pReprojectionParameters_->pRasterSpatialModelIn_ = RasterSpatialModel::Create(
               pReprojectionParameters_->rasterModelIn_);
      pReprojectionParameters_->srWktIn_ = pReprojectionParameters_->srWktOut_ =
            praster->GetSpatialReference().c_str();
      pReprojectionParameters_->algorithm_ = suri::TransformationFactory::kExact;
      suri::ParameterCollection params;
      params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn,
                                   pReprojectionParameters_->srWktIn_);
      params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut,
                                   pReprojectionParameters_->srWktOut_);
      suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();
      pReprojectionParameters_->pTranform_ = pfactory->Create(
            suri::TransformationFactory::kExact, params);
      pReprojectionParameters_->pWorldIn_ = pInputWorld_;
      praster->GetRasterSize(pReprojectionParameters_->width_, pReprojectionParameters_->height_);
   }
}

/** Destructor */
VisualizationExporterProcess::~VisualizationExporterProcess() {
   delete pReprojectionParameters_;
}

/**
 * Retorna el nodo datacast configurado con el tipo de dato de salida
 * @return Nodo que configura DataCastRenderer. El puntero es responsabilidad
 * del codigo invocante
 */
wxXmlNode* VisualizationExporterProcess::GetDataCastRenderizationNode() {
   DataCastRenderer::Parameters parameters;
   parameters.destinationDataType_ = DataInfo<unsigned char>::Name;
   return DataCastRenderer::GetXmlNode(parameters);
}

/**
 * Permite configurar la salida del proceso antes de correrlo
 * @param[in] pRasterElement puntero al elemento a configurar
 * @return bool que indica si tuvo exito
 */
bool VisualizationExporterProcess::ConfigureRaster(RasterElement *pRasterElement) {
   if (!FileExporterProcess::ConfigureRaster(pRasterElement))
      return false;

   if (pRasterElement->GetRasterProperties().DataType != DataInfo<unsigned char>::Name) {
      wxXmlNode *pdatacastnode = GetDataCastRenderizationNode();
      wxXmlNode *prnode = pRasterElement->GetNode(wxT(RENDERIZATION_NODE));
      if (!prnode) {
         delete pdatacastnode;
         return false;
      }

      if (pdatacastnode) {
         wxXmlNode* ppreviousrendernode = pRasterElement->GetNode(
         RENDERIZATION_NODE NODE_SEPARATION_TOKEN CANVAS_CACHE_NODE);
         ppreviousrendernode =
               ppreviousrendernode ? ppreviousrendernode : pRasterElement->GetNode(
               RENDERIZATION_NODE NODE_SEPARATION_TOKEN BAND_COMBINATION_NODE);
         pRasterElement->AddNode(prnode, pdatacastnode, ppreviousrendernode);
      }
   }
   return true;
}

/** Obtiene la lista de elementos que se a renderizar con el proceso */
LayerList *VisualizationExporterProcess::GetElementList() {
   // Los elementos que formen parte de la LayerList se copiaran ya que poseen eventos conectados
   LayerList *playerlist = new LayerList();
   RasterElement* praster = dynamic_cast<RasterElement*>(Element::Create(
         "RasterElement", pRasterElement_->GetNode(), pRasterElement_->GetMetadataMap()));

   // Si falla la creacion/configuracion del elemento retorna NULL
   if (!praster || !ConfigureRasterBands(praster) || !ConfigureRaster(praster)) {
      SHOW_ERROR(message_RASTER_CONFIGURATION_ERROR);
      delete playerlist;
      return NULL;
   }

   MapElement* pmap = NULL;
   for (size_t i = 0; i < InputElements_.size(); ++i) {
      Element* pelement = NULL;
      // TODO (Federico - 28/10/2014): La validacion se hace de esta forma ya que el Create
      // de MapElement no recibe como parametro el tipo de elemento. Deberia adaptarse para evitar
      // posibles errores. En este caso se asume que cualquier elemento que se encuentre en
      // LayerList y no posea ClassId es un MapElement.
      if (inputElements_[i]->GetClassId().empty())
         pmap = GetMapElement(InputElements_[i]);
      else
         pelement = Element::Create("VectorElement", inputElements_[i]->GetNode(),
                                    inputElements_[i]->GetMetadataMap());
      if (pelement) {
         pelement->Activate(true);
         playerlist->AddElement(pelement, 6);
      }
   }

   if (pmap) {
      MapRenderer::Parameters::pStaticList_ =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext()->GetLayerList();
      playerlist->AddElement(pmap);
   }

   praster->Activate(true);
   playerlist->AddElement(praster, 6);

   return playerlist;
}

/**
 * Devuelve un elemento de tipo mapa con la misma configuracion del que se encuentra activo
 * @param[in] pElement: elemento de tipo mapa activo
 * return pmap copia del elemento de tipo mapa activo
 */
MapElement* VisualizationExporterProcess::GetMapElement(Element* pElement) {
   MapElement* pmap = MapElement::Create();
   MapRenderer::Parameters params = MapRenderer::GetParameters(
         MapRenderer::GetXmlNode(pElement));
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::OutputSizeLinesKeyAttr, params.dimension_);
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::XMarginSizeKeyAttr, params.sizex_);
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::YMarginSizeKeyAttr, params.sizey_);
   MapRenderer::SetXmlNode(pmap, MapRenderer::GetXmlNode(params));
   pmap->Activate(true);
   std::vector<int> vecgroup;
   vecgroup.push_back(4);
   vecgroup.push_back(6);
   MapRenderer::staticElementGroupVector_.assign(vecgroup.begin(), vecgroup.end());
   return pmap;
}

/** Devuelve el Modelo Raster de llegada */
RasterSpatialModel* VisualizationExporterProcess::GetOutputRasterSpatialModel() {
   if (!pRasterModelOut_) {
       RasterElement* pelement = dynamic_cast<RasterElement*>(pRasterElement_);
       pRasterModelOut_ = RasterSpatialModel::Create(pelement->GetRasterModel());
   }
   return pRasterModelOut_;
}

/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso */
void VisualizationExporterProcess::DoOnProcessAdaptLayerSet() {
   FileExporterProcess::DoOnProcessAdaptLayerSet();

   // Elementos recibidos
   inputElements_ = InputElements_;

   // Guarda la referencia espacial del elemento
   spatialReference_ = GetInputElement()->GetSpatialReference().c_str();

   RasterElement *pelement = dynamic_cast<RasterElement*>(GetInputElement());
   if (pelement) {
      World* pworld = NULL;
      pAdaptLayer_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld);
      pworld->GetWindow(elementSubset_);
      pRasterModelIn_ = RasterSpatialModel::Create(pelement->GetRasterModel());
      bandCount_ = pelement->GetBandCount();
   }

   /** Crear el mundo con las propiedades del elemento recibido */
   pInputWorld_->SetSpatialReference(spatialReference_);
   pInputWorld_->SetWorld(elementSubset_);
   pInputWorld_->SetWindow(elementSubset_);

   if (pAdaptLayer_) {
      bool notneededattr = false;
      pAdaptLayer_->AddAttribute<bool>(ReprojectionProcess::ReprojectionKeyAtttr, notneededattr);
      pAdaptLayer_->AddAttribute<bool>(ReprojectionProcess::AskGcpFileAttr, notneededattr);
      pAdaptLayer_->AddAttribute<ReprojectionProcess::ReprojectionParameters*>(
            ReprojectionProcess::ReprojectionParamsKeyAttr, pReprojectionParameters_);
      pAdaptLayer_->AddAttribute<Subset>(ProcessAdaptLayer::InputSubsetWindowKeyAttr, elementSubset_);
      pAdaptLayer_->AddAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pOutputWorld_);
   }
}

/**
 * Crea un elemento con el resultado de la renderizacion.
 * return bool que indica si pudo generar elemento
 */
bool VisualizationExporterProcess::ConfigureOutput() {
   if (!FileExporterProcess::ConfigureOutput())
      return false;
   /**
    * Si el elemento de entrada posee una banda simulo una combinacion para formar una
    * imagen de tres bandas para poder conservar el color de los vectores (de lo contrario
    * se renderizarian en escala de grises)
    */
   std::vector<int> selectedbands;
   pAdaptLayer_->GetAttribute<std::vector<int> >(
         ProcessAdaptLayer::SelectedBandsKey, selectedbands);
   Element* pNewElement = NULL;
   pAdaptLayer_->GetAttribute<Element*>(
               ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
   if (selectedbands.size() < 3) {
      wxXmlNode* prnode = pNewElement->GetNode(RENDERIZATION_NODE);
      pNewElement->AddNode(prnode, BAND_COMBINATION_NODE, wxT("0 1 2"));
   }

   return true;
}

/**
 * Obtiene el Filename y Opciones de la interfaz grafica, verifica su validez y
 * los setea a la propiedad interna de FileRenderization.
 * @return false filename invalido
 * @return false espacio en disco insuficiente.
 */
bool VisualizationExporterProcess::ConfigureProcess() {
   if (!FileExporterProcess::ConfigureProcess())
      return false;

   // Configura el nombre y formato de archivo de salida
   std::string filename;
   Option option;
   GetSelectedOutput(filename, option);
   option.SetOption("DataInfo", DataInfo<unsigned char>::Name);
   option.SetOption("BandCount", "3");

   FileRenderization *pfilerender = dynamic_cast<FileRenderization*>(pRenderizationObject_);
   pfilerender->SetFilename(filename);
   pfilerender->SetOption(option);

   return true;
}

/** Configura el subset espacial */
World *VisualizationExporterProcess::GetWorld() {
   Subset inputsubset = GetInputSubset();
   pInputWorld_->SetWindow(inputsubset);
   // El mundo de salida se configura a traves de OutputPageConfigPart
   if (pAdaptLayer_) {
      pAdaptLayer_->GetAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pOutputWorld_);
   }
   return pOutputWorld_;
}
} /** namespace suri */
