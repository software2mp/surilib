/*! \file RasterizationProcessCommandExecutionHandler.cpp */
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
#include "suri/RasterizationCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/Command.h"
#include "suri/VectorElement.h"
#include "suri/GenericTool.h"
#include "suri/ToolSupport.h"
#include "suri/RasterizationProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "XmlMetadataHandler.h"
#include "suri/ProcessAdaptLayer.h"

namespace suri {

namespace core {

/** Constructor **/
RasterizationCommandExecutionHandler::RasterizationCommandExecutionHandler
                                             (DataViewManager* pDataView) :pDataView_(pDataView) {
}

/** Destructor **/
RasterizationCommandExecutionHandler::~RasterizationCommandExecutionHandler(){
}


/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool RasterizationCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id != pTool->GetId("Rasterization")) return false;
   VectorDatasource* pVectorDatasource = ToolSupport::GetVectorDatasource(pDataView_->GetDatasourceManager(),
                                                                     pDataView_->GetViewcontextManager());
   if (!pVectorDatasource)
      return false;
   Subset a;
   std::vector<Element*> elementList;
   elementList.push_back(pVectorDatasource->GetElement());
   RasterizationProcess process(elementList, a, pDataView_);
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   pPal->AddAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputRasterElementsAttr, elementList);
   pPal->AddAttribute<VectorDatasource*>(
            ProcessAdaptLayer::RasterizationVectorDatasourceAttr, pVectorDatasource);
   Library* plibrary = pDataView_->GetLibraryManager()->GetLibraryByCode("srs",
                                                   LibraryManager::AccessKey(true));


   World* pworld = NULL;
   pPal->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                                      pworld);
   pPal->AddAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pworld);
   SuriObject::UuidType libid = plibrary->GetId();
   pPal->AddAttribute<SuriObject::UuidType>(
              ProcessAdaptLayer::LibraryIdKeyAttr, libid);
   SetAttributes(*pPal);
   RunProcess(pDataView_, wxT("Mosaico Georreferenciado"), &process);
   return true;
}


/** Configura atributos del proceso a traves del pal */
void RasterizationCommandExecutionHandler::SetAttributes(ProcessAdaptLayer &Pal) {
}


}  /** namespace core **/
}  /** namespace suri **/
