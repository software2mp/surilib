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
