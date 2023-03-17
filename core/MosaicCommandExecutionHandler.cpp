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
#include "suri/MosaicCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/Command.h"
#include "suri/VectorElement.h"
#include "suri/GenericTool.h"
#include "suri/ToolSupport.h"
#include "suri/MosaicProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "XmlMetadataHandler.h"
#include "suri/ProcessAdaptLayer.h"
#include "DefaultViewcontext.h"

namespace suri {

namespace core {

/** Constructor **/
MosaicCommandExecutionHandler::MosaicCommandExecutionHandler(DataViewManager* pDataView) :
                                 pDataView_(pDataView) {
}

/** Destructor **/
MosaicCommandExecutionHandler::~MosaicCommandExecutionHandler(){
}


/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool MosaicCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id != pTool->GetId("Mosaic")) return false;
   LayerList* pLayerList = ToolSupport::GetLayerList(
         pDataView_->GetViewcontextManager());
   if (!pLayerList) return false;
   std::vector<Element*> elementList;
   std::vector<RasterElement*> activerasters = ToolSupport::GetAllRaster(
         pDataView_->GetViewcontextManager());
   // Verifico que haya al menos  2 rasters activos
   DefaultViewcontext* pDef = dynamic_cast<DefaultViewcontext*>
                              (pDataView_->GetViewcontextManager()->GetSelectedViewcontext());
   if (pDef) pDef->ResetSelection(); //fix para que no rompa la progressbar en linux
   if (!(2 <= activerasters.size())) {
      SHOW_WARNING(message_MOSAIC_START_ERROR);
      return false;
   }
   std::vector<RasterElement*>::const_iterator it = activerasters.begin();
   int bandcount = activerasters[0]->GetBandCount();
   wxString spatialreference = activerasters[0]->GetSpatialReference();
   for (; it != activerasters.end(); ++it) {
      // Verifico que tengan el mismo sistema de coordenadas
      if (spatialreference != (*it)->GetSpatialReference()) {
         SHOW_WARNING(message_MOSAIC_SPATIAL_ERROR);
         return false;
      }
      // Verifico que tengan la misma cantidad de bandas
      if (bandcount != (*it)->GetBandCount()) {
         SHOW_WARNING(message_MOSAIC_BAND_ERROR);
         return false;
      }
      elementList.push_back(dynamic_cast<Element*>(*it));
   }
   Subset a;
   MosaicProcess process(elementList, a, pDataView_);
   ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
   pPal->AddAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputRasterElementsAttr, elementList);
   SetAttributes(*pPal);
   RunProcess(pDataView_, wxT("Mosaico Georreferenciado"), &process);
   return true;
}


/** Configura atributos del proceso a traves del pal */
void MosaicCommandExecutionHandler::SetAttributes(ProcessAdaptLayer &Pal) {
}


}  /** namespace core **/
}  /** namespace suri **/
