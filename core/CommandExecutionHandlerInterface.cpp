/*! \file CommandExecuteHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
// Includes Suri
#include "suri/Command.h"
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DataViewManager.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessInterface.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/ToolSupport.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class RasterElement;

namespace core {

/** Funcion auxiliar que una vez instanciado el proyecto configurada la capa de adaptacion
 *  y corre la ventana de configuracion del mismo para que luego corra el proceso
 *  @param[in] pDataView administrador de informacion de la aplicacion
 *  @param[in] Caption nombre que tendra la vetana de configuracion del proceso
 *  @param[in] Instancia del proceso que se correra una vez configurado
 *  @param[in] Elemento de entrada al proceso
 **/
void CommandExecutionHandlerInterface::RunProcess(DataViewManager* pDataView,
                                                  const std::string& Caption,
                                                  ProcessInterface* pProcess) {
   ProcessAdaptLayer* pPal=pProcess->GetProcessAdaptLayer();
   bool includemask = false;
   pPal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, includemask);
   pProcess->OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(pProcess,
                                                Caption, pDataView->GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         Element* pNewElement = NULL;
         pPal->GetAttribute<Element*>(
                     ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
         DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", pNewElement);
         pDataView->GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

/** Funcion auxiliar para obtener los parametros basicos de entrada a los procesos
 *  que operan sobre las bandas de un raster (el elemento de entrada y el subset)
 **/
void CommandExecutionHandlerInterface::LoadInputProcessParameters
   (DataViewManager* pDataView, RasterElement* &pRaster, Subset& ViewerWindow) {
   pRaster = ToolSupport::GetRaster(pDataView->GetViewcontextManager());
   if (pRaster != NULL) {
      ToolSupport::GetSubset(pDataView->GetViewportManager(), ViewerWindow, pRaster);
   }
}

}  /** namespace core **/
}  /** namespace suri **/
