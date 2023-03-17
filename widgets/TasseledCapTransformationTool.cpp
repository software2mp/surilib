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

#include "suri/TasseledCapTransformationTool.h"

#include "TasseledCapProcess.h"
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
TasseledCapTransformationTool::TasseledCapTransformationTool(
      DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "tasseledcaptransformation";
   Initialize();
}

/**
 * Dtor.
 */
TasseledCapTransformationTool::~TasseledCapTransformationTool() {

}

/**
 * Ejecuta un comando (ToolInterface::Execute)
 */
void TasseledCapTransformationTool::Execute(const Command *pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("TasseledCapTransformation")) {
      ExecuteTCTransformation();
   }
}

/**
 * Metodo para la creacion de comandos.
 */
Command* TasseledCapTransformationTool::CreateCommand(const std::string& CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "TasseledCapTransformation") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("Transformacion Tasseled-Cap");
      pcommand->SetLocation("ID_TCTRANSFORMATION_MENUITEM");
   }
   return pcommand;
}

/**
 * Metodo para la destruccion de comandos.
 */
void TasseledCapTransformationTool::DestroyCommand(Command*& pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Metodo para la creacion de widgets de la herramienta.
 */
Widget* TasseledCapTransformationTool::CreateWidget(const std::string& Widget) {
   return NULL;
}

/**
 * Ejecuta el proceso de transformacion tasseled-cap.
 */
void TasseledCapTransformationTool::ExecuteTCTransformation() {

//   SHOW_WARNING(_("La herramienta seleccionada no se encuentra disponible en este momento."));
//   return;

   Element* pinputelement = ToolSupport::GetRaster(
         GetDataViewManager()->GetViewcontextManager());

   // Validar seleccion de 2 elementos (uno para baja reslocion y otro para alta resolucion)
   if (pinputelement == NULL) {
      SHOW_WARNING(_("Debe seleccionar un elemento raster."));
      return;
   }



   // Data view manager.
   DataViewManager* pdvm = GetDataViewManager();
   // Crear el mundo con las propiedades del elemento recibido.
   World* pinputworld = new World;
   pinputworld->SetSpatialReference(pinputelement->GetSpatialReference().c_str());
   Subset elementsubset;
   pinputelement->GetElementExtent(elementsubset);
   pinputworld->SetWorld(elementsubset);
   pinputworld->SetWindow(elementsubset);
   Subset viewerwindow;
   ToolSupport::GetSubset(GetViewportManager(), viewerwindow, dynamic_cast<RasterElement*>(pinputelement));
   TasseledCapProcess process(pinputelement, viewerwindow, GetDataViewManager());
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   pPal->AddAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvm);//
   pPal->AddAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pinputworld);//
   // La cosa parece estar en orden. Dale para adelante.
   pPal->AddAttribute<Element*>(ProcessAdaptLayer::TasseledCapInputKeyAttr,
                                     pinputelement);//
   process.OnProcessAdaptLayerSet();
   ProcessNotebookWidget* pprocesswidget = new ProcessNotebookWidget(
         &process, caption_TCTRANSFORMATION_TOOL, GetLibraryManager());
   if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
      Element* pNewElement = NULL;
      pPal->GetAttribute<Element*>(
                  ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
      DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                     pNewElement);
      GetDatasourceManager()->AddDatasource(pdatasource);
   }
}

}  // namespace suri
