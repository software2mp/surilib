/*! \file PrincipalComponentAnalysisTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/PrincipalComponentAnalysisTool.h"

#include "HtmlReportWidget.h"
#include "PrincipalComponentAnalysisProcess.h"

#include "suri/DataViewManager.h"
#include "suri/Element.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/ToolCommand.h"
#include "suri/ToolSupport.h"
#include "suri/World.h"

namespace suri {

/**
 * Ctor.
 */
PrincipalComponentAnalysisTool::PrincipalComponentAnalysisTool(
      DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "pcanalysis";
   Initialize();
}

/**
 * Dtor.
 */
PrincipalComponentAnalysisTool::~PrincipalComponentAnalysisTool() {

}

/**
 * Ejecuta un comando (ToolInterface::Execute)
 */
void PrincipalComponentAnalysisTool::Execute(const Command *pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("PCAnalysis")) {
      ExecutePCAnalysis();
   }
}

/**
 * Metodo para la creacion de comandos.
 */
Command* PrincipalComponentAnalysisTool::CreateCommand(const std::string& CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "PCAnalysis") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("Analisis de componentes principales");
      pcommand->SetLocation("ID_PCANALYSIS_MENUITEM");
   }
   return pcommand;
}

/**
 * Metodo para la destruccion de comandos.
 */
void PrincipalComponentAnalysisTool::DestroyCommand(Command*& pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Metodo para la creacion de widgets de la herramienta.
 */
Widget* PrincipalComponentAnalysisTool::CreateWidget(const std::string& Widget) {
   return NULL;
}

/**
 * Ejecuta el proceso de transformacion tasseled-cap.
 */
void PrincipalComponentAnalysisTool::ExecutePCAnalysis() {
   Element* pinputelement = ToolSupport::GetRaster(
         GetDataViewManager()->GetViewcontextManager());

   // Validar seleccion de 2 elementos (uno para baja reslocion y otro para alta resolucion)
   if (pinputelement == NULL) {
      SHOW_WARNING(_("Debe seleccionar un elemento raster."));
      return;
   }

   //ProcessAdaptLayer pal;///

   // Data view manager.
   DataViewManager* pdvm = GetDataViewManager();///
   /////

   // La cosa parece estar en orden. Dale para adelante.
   /*pal.AddAttribute<Element*>(ProcessAdaptLayer::PCAnalysisInputKeyAttr,
                              pinputelement);*///

   // Crear el mundo con las propiedades del elemento recibido.
   World* pinputworld = new World;
   pinputworld->SetSpatialReference(pinputelement->GetSpatialReference().c_str());
   Subset elementsubset;
   pinputelement->GetElementExtent(elementsubset);
   pinputworld->SetWorld(elementsubset);
   pinputworld->SetWindow(elementsubset);

   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow,
                          dynamic_cast<RasterElement*>(pinputelement));

   HtmlReportWidget* pReportWidget = new HtmlReportWidget(_(caption_STATISTICS_REPORT));
   Element* poutputelement = NULL;
   PrincipalComponentAnalysisProcess process(pinputelement, poutputelement,
                                               viewerwindow, GetDataViewManager(),
                                               pReportWidget);

   ProcessAdaptLayer *pal = process.GetProcessAdaptLayer();
   pal->AddAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvm);
   pal->AddAttribute<Element*>(ProcessAdaptLayer::PCAnalysisInputKeyAttr,
                                 pinputelement);
   pal->AddAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pinputworld);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_PCANALYSIS_TOOL, GetLibraryManager());

   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      Element* poutputelement = NULL;
      pal->GetAttribute(ProcessAdaptLayer::OutputElementKeyAttr, poutputelement);
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                     poutputelement);
      GetDatasourceManager()->AddDatasource(pdatasource);
      pReportWidget->CreateTool();
   }
}

} /** namespace suri */
