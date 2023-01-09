/*! \file ManualEnhancementCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/ManualEnhancementCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/GenericTool.h"
#include "suri/Command.h"
#include "suri/DatasourceManagerInterface.h"
#include "RasterDatasource.h"
#include "suri/ManualEnhancementPart.h"
#include "suri/ToolSupport.h"
#include "RasterLayer.h"
#include "suri/NotebookWidget.h"

namespace suri {

namespace core {

/** Constructor **/
ManualEnhancementCommandExecutionHandler::
			ManualEnhancementCommandExecutionHandler(DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
ManualEnhancementCommandExecutionHandler::~ManualEnhancementCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
*/
bool ManualEnhancementCommandExecutionHandler::Execute(const Command* pCommand,
																GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("ManualEnhancement"))
		return false;
	RasterDatasource* praster = ToolSupport::GetRasterDatasource(
										pDataView_->GetDatasourceManager(), pDataView_->GetViewcontextManager());
	LayerInterface* player = praster ? pDataView_->GetViewcontextManager()->
								GetSelectedViewcontext()->GetAssociatedLayer(praster->GetId()) : NULL;
	if (!praster || !player) {
		SHOW_ERROR("Para activar la herramienta debe seleccionar una imagen");
		return true;
	}
	ui::ManualEnhancementPart* phistpart =
											new ui::ManualEnhancementPart(dynamic_cast<RasterLayer*>(player));
   NotebookWidget* pnotebookwidget = new NotebookWidget(
         _(phistpart->GetWindowTitle()), wxNB_LEFT, NotebookWidget::Listbook,
         SUR_BTN_OK | SUR_BTN_CANCEL | SUR_BTN_APPLY);
	pnotebookwidget->CreateTool();
	pnotebookwidget->InsertPart(phistpart);
	pnotebookwidget->ShowModal();
	return true;
}


}  /** namespace core **/
}  /*di* namespace suri **/
