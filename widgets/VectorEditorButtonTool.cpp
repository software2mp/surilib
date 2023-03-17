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
#include "VectorEditorButtonTool.h"
#include "suri/ViewcontextInterface.h"
#include "suri/DataViewManager.h"
#include "suri/VectorEditionTable.h"
#include "suri/BufferedDriver.h"
#include "suri/FeatureSelection.h"
#include "suri/Table.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor */
VectorEditorButtonTool::VectorEditorButtonTool() {
   // TODO Auto-generated constructor stub

}

/** Destructor */
VectorEditorButtonTool::~VectorEditorButtonTool() {
   // TODO Auto-generated destructor stub
}

/** Configura la fuente de datos */
DatasourceInterface* VectorEditorButtonTool::GetSelectedDataSource(
      ViewcontextInterface* pViewContext, DataViewManager* pDataViewManager) {
   std::list<suri::NodePath> selectednodes =
         pDataViewManager->GetSelectionManager()->GetSelectedNodes();
   std::list<suri::NodePath>::iterator it = selectednodes.begin();
   bool removeresult = true;
   suri::LayerInterface* player = NULL;
   for (; player == NULL && it != selectednodes.end() && removeresult; ++it)
      if (!it->AllowsChildren()) {
         if (pViewContext) {
            player = pViewContext->GetAssociatedLayer(
                  it->GetLastPathNode()->GetContent());
         }
      }
   DatasourceInterface* pdatasource = NULL;
   if (player) {
      SuriObject::UuidType dataid = player->GetAssociatedDatasourceId();
      pdatasource = pDataViewManager->GetDatasourceManager()->GetDatasource(dataid);
   }
   return pdatasource;
}

/** Configura la tabla para la tarea ( Task_ ) */
bool VectorEditorButtonTool::ConfigureTable(VectorDatasource* pDataSource, Table** pTable)  {
   *pTable = new VectorEditionTable(pDataSource);
   VectorEditorDriver* pdriver = new suri::VectorEditorDriver(pDataSource);
   BufferedDriver* pbuffereddriver = new suri::BufferedDriver(pdriver);

   // Si no se puede setear el driver, volvemos con false.
   if ( !(*pTable)->SetDriver(pbuffereddriver) )
      return pTable;

   (*pTable)->SetDataChangeNotifier(pbuffereddriver);

   return true;
}

/** Configura tarea */
bool VectorEditorButtonTool::ConfigureTask(VectorEditionTask* pVectorEditionTask,
                                           VectorDatasource* pDataSource,
                                           DataViewManager* pDataViewManager,
                                           FeatureSelection* pFeatureSelection,
                                           Table** pTable) {

   if (!pVectorEditionTask->SetTable(*pTable))
      return false;

   pVectorEditionTask->ConfigureVectorData(pDataSource);
   ViewerWidget *pviewer = AS_VW(
         pDataViewManager->GetViewportManager()->GetSelectedViewport());
   pVectorEditionTask->SetWorld(pviewer->GetWorld());

   if (!pVectorEditionTask->SetFeatureSelector(pFeatureSelection))
      return false;

   // Se logro configurar todo con normalidad, salimos OK.
   return true;
}

/** Se bloquean las fuentes activas */
void VectorEditorButtonTool::BlockActivatedDatasources(
      ViewcontextInterface* pViewContext, DataViewManager* pDataViewManager) {
   if (!pViewContext) return;

   LayerList* plist = pViewContext->GetLayerList();
   LayerList::ElementListType::iterator it = plist->GetElementIterator(true);
   Element* pcurrent = NULL;
   for (; it != plist->GetElementIterator(false); ++it) {
      pcurrent = *it;
      if (pcurrent->IsActive()) {
         LayerInterface* player = pViewContext->GetLayerByElementId(pcurrent->GetUid());
         if (player){
            SuriObject::UuidType dataid = player->GetAssociatedDatasourceId();
            pDataViewManager->GetDatasourceManager()->BlockDatasource(dataid);
         }
      }
   }
}

/** Se desbloquean las fuentes activas */
void VectorEditorButtonTool::UnBlockActivatedDatasources(DataViewManager* pDataViewManager) {
   pDataViewManager->GetDatasourceManager()->UnblockAllDatasource();
}

} // suri
