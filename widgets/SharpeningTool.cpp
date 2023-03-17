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

#include <vector>

#include "suri/SharpeningTool.h"
#include "resources.h"
#include "suri/ToolSupport.h"
#include "suri/messages.h"
#include "suri/ToolCommand.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/SharpeningProcess.h"

namespace suri {

/**
 * Ctor.
 */
SharpeningTool::SharpeningTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "sharpening";
   Initialize();
}

/**
 * Dtor.
 */
SharpeningTool::~SharpeningTool() {

}

/**
 * Ejecuta un comando (ToolInterface::Execute)
 */
void SharpeningTool::Execute(const Command *pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("Sharpening")) {
      ExecuteSharpening();
   }
}

/**
 * Metodo para la creacion de comandos.
 */
Command* SharpeningTool::CreateCommand(const std::string& CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "Sharpening") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("Fusion de bandas con pancromatica");
      pcommand->SetLocation("ID_SHARPENING_MENUITEM");
   }
   return pcommand;
}

/**
 * Metodo para la destruccion de comandos.
 */
void SharpeningTool::DestroyCommand(Command*& pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Metodo para la creacion de widgets de la herramienta.
 */
Widget* SharpeningTool::CreateWidget(const std::string& Widget) {
   return NULL;
}

/**
 * Ejecuta el procesos de fusion de bandas con pancromatica (HSV por el momento)
 */
void SharpeningTool::ExecuteSharpening() {
   std::vector<RasterElement*> rasters = ToolSupport::GetAllRaster(
         GetDataViewManager()->GetViewcontextManager());

   // Validar seleccion de 2 elementos (uno para baja reslocion y otro para alta resolucion)
   if (rasters.size() < 2) {
      SHOW_WARNING(_("Debe seleccionar al menos 2 elementos raster."));
      return;
   }

   // Validar la cantidad de bandas en total (al menos 4: tres para componer el
   // RVA y una para fusionar.
   int totalbandcount = 0;
   for (int ix = 0, lenix = rasters.size(); ix < lenix; ++ix) {
      totalbandcount += rasters[ix]->GetBandCount();
   }
   if (totalbandcount < 4) {
      SHOW_WARNING(_("Deben haber al menos 4 bandas disponibles."));
      return;
   }

   // La cosa parece estar en orden. Dale para adelante.
   std::vector<Element*> elements;
   for (int ix = 0, lenix = rasters.size(); ix < lenix; ++ix) {
      elements.push_back(dynamic_cast<Element*>(rasters[ix]));
   }

   Element* pelement = NULL;
   SharpeningProcess process(pelement, GetDataViewManager());
   ProcessAdaptLayer* pal = process.GetProcessAdaptLayer();
   pal->AddAttribute<std::vector<Element*> >(ProcessAdaptLayer::InputElementsKeyAttr,
                                            elements);
   process.OnProcessAdaptLayerSet();

   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_SHARPENING_TOOL, GetLibraryManager());

   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                     pelement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

}  // namespace suri
