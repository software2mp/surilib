/*! \file StatisticsTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "resources.h"
#include "suri/ToolSupport.h"
#include "suri/messages.h"
#include "suri/LayerList.h"
#include "suri/ToolCommand.h"
#include "suri/RasterProcess.h"
#include "suri/Configuration.h"
#include "suri/RasterElement.h"
#include "suri/StatisticsTool.h"
#include "suri/PartContainerWidget.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/StatisticsExporterProcess.h"
#include "suri/ClassStatisticsProcess.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
StatisticsTool::StatisticsTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "statistics";
   Initialize();
}

/**
 * Destructor
 */
StatisticsTool::~StatisticsTool() {
   Finalize();
}

/**
 * Accion de "Ejecutar" de GlobalStatistics
 */
void StatisticsTool::ExecuteGlobalStatistics() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL)
      return;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   Widget* pstatisticsreport = NULL;
   StatisticsExporterProcess::OutputType output = ClassStatisticsProcess::HtmlPart;
   StatisticsExporterProcess process(praster, viewerwindow,
                                     GetDataViewManager(), pstatisticsreport,
                                     output);
   ProcessAdaptLayer* ppal = process.GetProcessAdaptLayer();
   bool showspectral = false;
   ppal->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showspectral);
   bool includemask = false;
   ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                              includemask);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget *pprocesswidget = new ProcessNotebookWidget(
         &process, caption_GLOBAL_STATISTICS, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      // Creo ventana que despliega reporte de estadisticas
      pstatisticsreport->CreateTool();
   }
}

/**
 * Accion de "Ejecutar" de ClassStatistics
 */
void StatisticsTool::ExecuteClassStatistics() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL)
      return;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   Widget* pstatisticsreport = NULL;
   StatisticsExporterProcess::OutputType output = ClassStatisticsProcess::HtmlPart;
   ClassStatisticsProcess process(praster, viewerwindow, GetDataViewManager(),
                                  pstatisticsreport, output);
   ProcessAdaptLayer* pal = process.GetProcessAdaptLayer();
   bool showspectral = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showspectral);
   bool includemask = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, includemask);
   bool classclasif = true;
   pal->AddAttribute<bool>(StatisticsExporterProcess::ClassClasificationKeyAttr, classclasif);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget *pprocesswidget = new ProcessNotebookWidget(
         &process, caption_GLOBAL_CLASS_STATISTICS, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      // Creo ventana que despliega reporte de estadisticas
      pstatisticsreport->CreateTool();
   }
}

/**
 * Accion de "Ejecutar" de ClassStatisticsExporter
 */
void StatisticsTool::ExecuteClassStatisticsExporter() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL)
      return;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   Widget* pstatisticsreport = NULL;
   StatisticsExporterProcess::OutputType output = ClassStatisticsProcess::CsvFile;
   ClassStatisticsProcess process(praster, viewerwindow, GetDataViewManager(),
                                  pstatisticsreport, output);
   ProcessAdaptLayer* pal = process.GetProcessAdaptLayer();
   bool showspectral = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showspectral);
   bool includemask = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, includemask);
   bool classclasif = true;
   pal->AddAttribute<bool>(StatisticsExporterProcess::ClassClasificationKeyAttr, classclasif);
   bool showcsvformat = true;
   pal->AddAttribute<bool>(ProcessAdaptLayer::CsvFormatPartKeyAttr, showcsvformat);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget *pprocesswidget = new ProcessNotebookWidget(
         &process, caption_GLOBAL_CLASS_STATISTICS_EXPORTER, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      // Ejecuto la creacion del csv
      SHOW_STATUS(message_STATISTICS_EXPORTER);
   }
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void StatisticsTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("GlobalStatistics")) {
      ExecuteGlobalStatistics();
   } else if (id == GetId("ClassStatistics")) {
      ExecuteClassStatistics();
   } else if (id == GetId("ClassStatisticsExporter")) {
      ExecuteClassStatisticsExporter();
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* StatisticsTool::CreateCommand(const std::string &CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "GlobalStatistics") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_STATISTICS_EXPORTER_BUTTON);
      pcommand->SetDecoration(tool_STATISTICS_EXPORTER);
      pcommand->SetLocation("ID_GLOBAL_STATISTICS_MENUTIEM");
   } else if (CommandName == "ClassStatistics") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_CLASS_STATISTICS_EXPORTER_BUTTON);
      pcommand->SetDecoration(tool_STATISTICS_EXPORTER);
      pcommand->SetLocation("ID_CLASS_STATISTICS_MENUTIEM");
   } else if (CommandName == "ClassStatisticsExporter") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_CLASS_STATISTICS_EXPORTER_BUTTON);
      pcommand->SetDecoration(tool_STATISTICS_EXPORTER);
      pcommand->SetLocation("ID_CLASS_STATISTICS_EXPORTER_MENUTIEM");
   }
   return pcommand;
}

/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 *
 */
void StatisticsTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 */
Widget* StatisticsTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /** namespace suri */
