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
#include <string>
#include <vector>

// Includes Suri
#include "resources.h"
#include "suri/ToolSupport.h"
#include "suri/messages.h"
#include "suri/ToolCommand.h"
#include "suri/BandMathTool.h"
#include "suri/IndexProcess.h"
#include "suri/FilterProcess.h"
#include "suri/RasterProcess.h"
#include "suri/BandMathProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/DatasourceManagerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
BandMathTool::BandMathTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "bandmath";
   Initialize();
}

/**
 * Destructor
 */
BandMathTool::~BandMathTool() {
   Finalize();
}

/**
 * Accion de "Ejecutar" de ApplyFilter
 */
void BandMathTool::ExecuteApplyFilter() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL)
      return;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   Element* pelement = NULL;
   int flags = KERNEL_FILTER;
   FilterProcess process(praster, viewerwindow, flags, GetDataViewManager());
   ProcessAdaptLayer *pal=process.GetProcessAdaptLayer();
  // process->SetAdaptLayer(&pal);
   bool includemask = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                              includemask);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_FILTER_EXPORTER_TOOL, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", pelement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

/**
 * Accion de "Ejecutar" de Indexes
 */
void BandMathTool::ExecuteIndexes() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL)
      return;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   IndexProcess process(praster, viewerwindow, GetDataViewManager());
   ProcessAdaptLayer* pPal=process.GetProcessAdaptLayer();
   bool includemask = false;
   pPal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                              includemask);
   //pexporter->SetAdaptLayer(&pal);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_INDEX_EXPORTER_TOOL, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      Element* pNewElement = NULL;
      pPal->GetAttribute<Element*>(
                  ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
      DatasourceInterface* pdatasource = DatasourceInterface::
            Create("RasterDatasource", pNewElement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

/**
 * Accion de "Ejecutar" de BandMath
 */
void BandMathTool::ExecuteBandMath() const {
   RasterElement* praster = ToolSupport::GetRaster(GetViewcontextManager());
   if (praster == NULL)
      return;
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, praster);
   Element* pelement = NULL;
   BandMathProcess process(praster, viewerwindow, GetDataViewManager());
   ProcessAdaptLayer* pal=process.GetProcessAdaptLayer();
   bool includemask = false;
   pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                              includemask);
  // pexporter->SetAdaptLayer(&pal);
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(&process,
                                     caption_BAND_MATH_PART, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource", pelement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void BandMathTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("ApplyFilter")) {
      ExecuteApplyFilter();
   } else if (id == GetId("Indexes")) {
      ExecuteIndexes();
   } else if (id == GetId("BandMath")) {
      ExecuteBandMath();
   }
}

/**
 * Fabrica el comando si es uno de los predefinidos
 * @param[in]  CommandName: Nombre del comando a ejecutar
 * @return     Devuelve el comando creado si tuvo exito, sino NULL
 */
Command* BandMathTool::CreateCommand(const std::string &CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "ApplyFilter") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_RASTER_FILTER_BUTTON);
      pcommand->SetDecoration(tool_RASTER_FILTER);
      pcommand->SetLocation("ID_FILTER_RASTER_MENUTIEM");
   } else if (CommandName == "Indexes") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_RASTER_INDEX_BUTTON);
      pcommand->SetDecoration(tool_RASTER_INDEX);
      pcommand->SetLocation("ID_INDICES_MENUTIEM");
   } else if (CommandName == "BandMath") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_RASTER_BANDMATH_BUTTON);
      pcommand->SetDecoration(tool_RASTER_BANDMATH);
      pcommand->SetLocation("ID_BAND_MATH_MENUTIEM");
   }
   return pcommand;
}

/**
 * Destruye un comando previamente creado con CreateCommand
 * @param[in]  pCommand: Puntero a un Comando creado no NULL a destruir
 *
 */
void BandMathTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creador del Widget
 */
Widget* BandMathTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /** namespace suri */
