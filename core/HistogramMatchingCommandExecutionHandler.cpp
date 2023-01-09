/*! \file HistogramMatchingCommandExecutionHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandard
#include <map>

// Includes Suri
#include "suri/HistogramMatchingCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/Command.h"
#include "suri/VectorElement.h"
#include "suri/GenericTool.h"
#include "suri/ToolSupport.h"
#include "suri/MosaicProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "XmlMetadataHandler.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/HistogramMatchingProcess.h"

namespace suri {

namespace core {

/** Constructor **/
HistogramMatchingCommandExecutionHandler::HistogramMatchingCommandExecutionHandler(
      DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
HistogramMatchingCommandExecutionHandler::~HistogramMatchingCommandExecutionHandler(){
}


/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool HistogramMatchingCommandExecutionHandler::Execute(
      const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("HistogramMatching"))
      return false;

   // Verifico que haya solamente un raster seleccionado
   std::vector<RasterElement*> activerasters = ToolSupport::GetAllRaster(
         pDataView_->GetViewcontextManager());

   if (activerasters.size() != 1) {
         SHOW_WARNING(message_ONLY_ONE_RASTER_SELECTED);
         return false;
   }

   // Verifico que haya al menos dos raster en la lista de elementos
   std::vector<Element*> rasterelementlist = GetRasterElements(activerasters[0]);
   if (rasterelementlist.size() < 2) {
      SHOW_WARNING(message_HISTOGRAM_MATCHING_ERROR);
      return false;
   }

   std::vector<Element*> selectedraster;
   selectedraster.push_back(rasterelementlist[0]);

   Subset windowsubset;
   HistogramMatchingProcess process(selectedraster, windowsubset, pDataView_);
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   pPal->AddAttribute< std::vector<Element*> >(
         ProcessAdaptLayer::InputRasterElementsAttr, rasterelementlist);
   RunProcess(pDataView_, wxT("Ajuste de histograma"), &process);
   return true;
}


/**
 * Retorna un vector con los rasters de la lista de capas ubicando el raster seleccionado en [0]
 * @return rasterelementlist vector con los rasters de la lista de capas
 */
std::vector<Element*> HistogramMatchingCommandExecutionHandler::GetRasterElements(
      Element* pElement) {
   std::vector<Element*> rasterelementlist;
   rasterelementlist.push_back(pElement);
   LayerList* playerlist = ToolSupport::GetLayerList(pDataView_->GetViewcontextManager());
   int count = playerlist->GetElementCount();
   for (int i = 0; i < count; ++i) {
      Element* pelement = playerlist->GetElement(i);
      if (pelement->GetUrl() != pElement->GetUrl() && dynamic_cast<RasterElement*>(pelement)) {
         rasterelementlist.push_back(pelement);
      }
   }
   return rasterelementlist;
}

}  /** namespace core **/
}  /** namespace suri **/
