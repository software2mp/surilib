/*! \file MosaicCommandExecutionHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes estandard
#include <map>

// Includes Suri
#include "suri/MosaicCommandExecutionHandler.h"
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
#include "DefaultViewcontext.h"

namespace suri {

namespace core {

/** Constructor **/
MosaicCommandExecutionHandler::MosaicCommandExecutionHandler(DataViewManager* pDataView) :
                                 pDataView_(pDataView) {
}

/** Destructor **/
MosaicCommandExecutionHandler::~MosaicCommandExecutionHandler(){
}


/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool MosaicCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id != pTool->GetId("Mosaic")) return false;
   LayerList* pLayerList = ToolSupport::GetLayerList(
         pDataView_->GetViewcontextManager());
   if (!pLayerList) return false;
   std::vector<Element*> elementList;
   std::vector<RasterElement*> activerasters = ToolSupport::GetAllRaster(
         pDataView_->GetViewcontextManager());
   // Verifico que haya al menos  2 rasters activos
   DefaultViewcontext* pDef = dynamic_cast<DefaultViewcontext*>
                              (pDataView_->GetViewcontextManager()->GetSelectedViewcontext());
   if (pDef) pDef->ResetSelection(); //fix para que no rompa la progressbar en linux
   if (!(2 <= activerasters.size())) {
      SHOW_WARNING(message_MOSAIC_START_ERROR);
      return false;
   }
   std::vector<RasterElement*>::const_iterator it = activerasters.begin();
   int bandcount = activerasters[0]->GetBandCount();
   wxString spatialreference = activerasters[0]->GetSpatialReference();
   for (; it != activerasters.end(); ++it) {
      // Verifico que tengan el mismo sistema de coordenadas
      if (spatialreference != (*it)->GetSpatialReference()) {
         SHOW_WARNING(message_MOSAIC_SPATIAL_ERROR);
         return false;
      }
      // Verifico que tengan la misma cantidad de bandas
      if (bandcount != (*it)->GetBandCount()) {
         SHOW_WARNING(message_MOSAIC_BAND_ERROR);
         return false;
      }
      elementList.push_back(dynamic_cast<Element*>(*it));
   }
   Subset a;
   MosaicProcess process(elementList, a, pDataView_);
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   pPal->AddAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputRasterElementsAttr, elementList);
   SetAttributes(*pPal);
   RunProcess(pDataView_, wxT("Mosaico Georreferenciado"), &process);
   return true;
}


/** Configura atributos del proceso a traves del pal */
void MosaicCommandExecutionHandler::SetAttributes(ProcessAdaptLayer &Pal) {
}


}  /** namespace core **/
}  /** namespace suri **/
