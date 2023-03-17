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
