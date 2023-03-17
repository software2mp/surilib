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

// Includes estandard
#include <map>

// Includes Suri
#include "suri/RasterizationCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/Command.h"
#include "suri/VectorElement.h"
#include "suri/GenericTool.h"
#include "suri/ToolSupport.h"
#include "suri/RasterizationProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "XmlMetadataHandler.h"
#include "suri/ProcessAdaptLayer.h"

namespace suri {

namespace core {

/** Constructor **/
RasterizationCommandExecutionHandler::RasterizationCommandExecutionHandler
                                             (DataViewManager* pDataView) :pDataView_(pDataView) {
}

/** Destructor **/
RasterizationCommandExecutionHandler::~RasterizationCommandExecutionHandler(){
}


/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool RasterizationCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id != pTool->GetId("Rasterization")) return false;
   VectorDatasource* pVectorDatasource = ToolSupport::GetVectorDatasource(pDataView_->GetDatasourceManager(),
                                                                     pDataView_->GetViewcontextManager());
   if (!pVectorDatasource)
      return false;
   Subset a;
   std::vector<Element*> elementList;
   elementList.push_back(pVectorDatasource->GetElement());
   RasterizationProcess process(elementList, a, pDataView_);
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   pPal->AddAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputRasterElementsAttr, elementList);
   pPal->AddAttribute<VectorDatasource*>(
            ProcessAdaptLayer::RasterizationVectorDatasourceAttr, pVectorDatasource);
   Library* plibrary = pDataView_->GetLibraryManager()->GetLibraryByCode("srs",
                                                   LibraryManager::AccessKey(true));


   World* pworld = NULL;
   pPal->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                                      pworld);
   pPal->AddAttribute<World*>(ProcessAdaptLayer::OutputWorldKeyAttr, pworld);
   SuriObject::UuidType libid = plibrary->GetId();
   pPal->AddAttribute<SuriObject::UuidType>(
              ProcessAdaptLayer::LibraryIdKeyAttr, libid);
   SetAttributes(*pPal);
   RunProcess(pDataView_, wxT("Mosaico Georreferenciado"), &process);
   return true;
}


/** Configura atributos del proceso a traves del pal */
void RasterizationCommandExecutionHandler::SetAttributes(ProcessAdaptLayer &Pal) {
}


}  /** namespace core **/
}  /** namespace suri **/
