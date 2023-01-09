/*! \file StatisticsToolCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

// Include estandard
#include <string>

// Includes Suri
#include "suri/StatisticsToolCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/Command.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/GenericTool.h"
#include "suri/RasterElement.h"
#include "suri/ToolSupport.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/StatisticsExporterProcess.h"
#include "suri/ClassStatisticsProcess.h"

namespace suri {

namespace core {

/** Funcion auxiliar para la configuracion de parametros de entrada de los procesos de estadisticas**/
void GetStatisticsInputParameters(DataViewManager* pDataView,
												RasterElement* &pRaster, Subset& ViewerWindow) {
	pRaster = ToolSupport::GetRaster(pDataView->GetViewcontextManager());
   ToolSupport::GetSubset(pDataView->GetViewportManager(), ViewerWindow, pRaster);
}


/** Funcion auxiliar para la configuracion de la capa de adaptacion de procesos para los 
 * procesos de estadisticas 
 * @param[in] ShowSpectral flag que determina si se debe mostrar la seleccion espectral
 * en el proceso
 * @param[in] IncludeMask flag que determina si se debe mostrar la configuracion de enmascaramiento
 * en el proceso
 * @param[in] ClassStatistics flag que determinas si se trata de una estadisticas clasificada
 * @param[in] Pal cada de adaptacion de proceso que se desea configurar
 * @param[out] Pal capa de adaptacion de proceso configurada 
 **/
void ConfigureProcessAdaptLayer(bool ShowSpectral, bool IncludeMask, bool ClassStatistics, 
										bool ShowCsvFormat, ProcessAdaptLayer& Pal) {
   Pal.AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, ShowSpectral);
   Pal.AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, IncludeMask);
   Pal.AddAttribute<bool>(StatisticsExporterProcess::ClassClasificationKeyAttr, ClassStatistics);
   Pal.AddAttribute<bool>(ProcessAdaptLayer::CsvFormatPartKeyAttr, ShowCsvFormat);
}

/** Constructor **/
GlobalStatisticsCommandExecutionHandler::GlobalStatisticsCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
GlobalStatisticsCommandExecutionHandler::~GlobalStatisticsCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool GlobalStatisticsCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("GlobalStatistics"))
		return false;	
	RasterElement* praster = NULL;
   Subset viewerwindow;
	GetStatisticsInputParameters(pDataView_, praster, viewerwindow);
	if (praster == NULL) {
		SHOW_ERROR(_(message_PROCESS_START_ERROR));
      return true;
	}
   Widget* pstatisticsreport = NULL;
   StatisticsExporterProcess::OutputType output = ClassStatisticsProcess::HtmlPart;
   StatisticsExporterProcess pstatics( praster, viewerwindow, pDataView_,
                                       pstatisticsreport, output);
   ProcessAdaptLayer* pal = pstatics.GetProcessAdaptLayer();
   ConfigureProcessAdaptLayer(false, false, false, false, *pal);
	pstatics.OnProcessAdaptLayerSet();
   ProcessNotebookWidget *pprocesswidget = new ProcessNotebookWidget(
         &pstatics, caption_GLOBAL_STATISTICS, pDataView_->GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      // Creo ventana que despliega reporte de estadisticas
      pstatisticsreport->CreateTool();
   }
	return true;
}

/** Constructor **/
ClassStatisticsCommandExecutionHandler::ClassStatisticsCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
ClassStatisticsCommandExecutionHandler::~ClassStatisticsCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ClassStatisticsCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("ClassStatistics"))
		return false;	
	RasterElement* praster = NULL;
   Subset viewerwindow;
	GetStatisticsInputParameters(pDataView_, praster, viewerwindow);
	if (praster == NULL) {
		SHOW_ERROR(_(message_PROCESS_START_ERROR));
      return true;
	}
   Widget* pstatisticsreport = NULL;
   StatisticsExporterProcess::OutputType output = ClassStatisticsProcess::HtmlPart;
   ClassStatisticsProcess pstatics(praster, viewerwindow, pDataView_,
                                   pstatisticsreport, output);
   ProcessAdaptLayer* pal=pstatics.GetProcessAdaptLayer();
   ConfigureProcessAdaptLayer(false, false, true, false, *pal);
	pstatics.OnProcessAdaptLayerSet();
   ProcessNotebookWidget *pprocesswidget = new ProcessNotebookWidget(
         &pstatics, caption_GLOBAL_CLASS_STATISTICS, pDataView_->GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      // Creo ventana que despliega reporte de estadisticas
      pstatisticsreport->CreateTool();
   }
	return true;
}

/** Constructor **/
ClassStatisticsExporterCommandExecutionHandler::ClassStatisticsExporterCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
ClassStatisticsExporterCommandExecutionHandler::~ClassStatisticsExporterCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ClassStatisticsExporterCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("ClassStatisticsExporter"))
		return false;	
	RasterElement* praster = NULL;
   Subset viewerwindow;
	GetStatisticsInputParameters(pDataView_, praster, viewerwindow);
	if (praster == NULL) {
		SHOW_ERROR(_(message_PROCESS_START_ERROR));
      return true;
	}
   Widget* pstatisticsreport = NULL;
   StatisticsExporterProcess::OutputType output = ClassStatisticsProcess::CsvFile;
   ClassStatisticsProcess process(
         praster, viewerwindow, pDataView_, pstatisticsreport, output);
   ProcessAdaptLayer* pal=process.GetProcessAdaptLayer();
   ConfigureProcessAdaptLayer(false, false, false, true, *pal);
   bool exportstats = true;
   pal->AddAttribute<bool>(StatisticsExporterProcess::ClassClasificationExporterKeyAttr, exportstats);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget *pprocesswidget = new ProcessNotebookWidget(
         &process, caption_GLOBAL_CLASS_STATISTICS_EXPORTER, pDataView_->GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      // Ejecuto la creacion del csv
      SHOW_STATUS(message_STATISTICS_EXPORTER);
   }
	return true;
}

}  /** namespace core **/
}  /** namespace suri **/
