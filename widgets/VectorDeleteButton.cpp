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
#include "suri/GenericTool.h"
#include "VectorDeleteButton.h"
#include "GeometryDeleteEvent.h"
#include "GeometryElementEditor.h"
#include "suri/VectorEditionTable.h"
#include "suri/BufferedDriver.h"
#include "VectorEditionTaskEvent.h"
#include "suri/DataViewManager.h"
#include "VectorEditorButtonTool.h"

// Includes Wx
// Defines
// forwards


namespace suri {

// forwards
class ViewerWidget;

VectorDeleteButton::VectorDeleteButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                       const wxString &Bitmap,
                                       GuiGeometryCreator::GeometryType GeoType,
                                       wxString ButtonTooltip,
                                       DataViewManager* pDataViewManager, int Flags) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, GeoType, ButtonTooltip,
                         pDataViewManager->GetViewportManager(),
                         pDataViewManager->GetViewcontextManager(), Flags),
      pFeatureSelection_(pDataViewManager->GetFeatureSelection()),
      pDeleteEventHandler_(NULL), pPaintEventHandler_(NULL),
      pSelectionSource_(NULL),
      pDataViewManager_(pDataViewManager), pTask_(new VectorEditionTask), pTable_(NULL) {
   pDataViewManager_->GetTaskManager()->AppendTask(pTask_);
}

/** Destructor **/
VectorDeleteButton::~VectorDeleteButton() {
   delete pTask_;
}


/** Preparamos el inicio de la eliminacion de una geometria */
void VectorDeleteButton::Start() {
   if (!pDataViewManager_) {
      return;
   }

   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   if (!pviewcontext) return;

//   VectorEditorButtonTool::BlockActivatedDatasources(pviewcontext, pDataViewManager_);
   if (IsActive())
      return;


//   if (!pTask_ || !pDataViewManager_->GetTaskManager()->GetActiveTaskIds().empty())
//      return;

   VectorDatasource* pdatasource =
         dynamic_cast<VectorDatasource*>(VectorEditorButtonTool::GetSelectedDataSource(
               pviewcontext, pDataViewManager_));

   /** Obtenemos la fuente de datos */
   if (!pdatasource) {
      SHOW_ERROR("Error al intentar obtener el vector desde la fuente de datos.");
      End(); // Nos aseguramos que todo vuelva a 0.
      return;
   }

   // config tabla
   if (!VectorEditorButtonTool::ConfigureTable(pdatasource, &pTable_)) {
      SHOW_ERROR("Error al intentar configurar la tabla del shapefile.");
      End();
      return;
   }

   // config tarea
   if (!VectorEditorButtonTool::ConfigureTask(pTask_, pdatasource, pDataViewManager_,
                                              pFeatureSelection_, &pTable_)) {
      SHOW_ERROR("Error al intentar configurar la tarea.");
      End();
      return;
   }

//   VectorEditorButtonTool::BlockActivatedDatasources(pviewcontext, pDataViewManager_);

   pTask_->Start();
   pDeleteEventHandler_ = new GeometryDeleteEvent(
         pDataViewManager_->GetViewportManager(),
         pDataViewManager_->GetViewcontextManager(),
         pTask_);
   pPaintEventHandler_ = new VectorEditionTaskEvent(
         pDataViewManager_->GetViewportManager(),
         pDataViewManager_->GetViewcontextManager(), pTask_);
   pViewer_->PushPaintEvent(pPaintEventHandler_);
   pViewer_->PushMouseEvent(pDeleteEventHandler_);
}

/** Finaliza la eliminacion de geometria */
void VectorDeleteButton::End() {
//   VectorEditorButtonTool::UnBlockActivatedDatasources(pDataViewManager_);

   if (pPaintEventHandler_ && pDeleteEventHandler_ && pViewer_) {
      pTask_->End();

      // todo - Mejorar el codigo encerrado.
      //////////////////////////////////////////////////////////////////////////
      pViewer_->PopPaintEvent(pPaintEventHandler_);
      pViewer_->PopMouseEvent(pDeleteEventHandler_);
      ViewcontextInterface* pviewcontext =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      std::list<suri::NodePath> selectednodes =
            pDataViewManager_->GetSelectionManager()->GetSelectedNodes();
      std::list<suri::NodePath>::iterator it = selectednodes.begin();
      bool removeresult = true;
      suri::LayerInterface* player = NULL;
      for (; player == NULL && it != selectednodes.end() && removeresult; ++it)
         if (!it->AllowsChildren()) {
            if (pviewcontext) {
               player = pviewcontext->GetAssociatedLayer(
                     it->GetLastPathNode()->GetContent());
            }
         }
      if (player) {
         player->GetElement()->SetChanged();
         player->GetElement()->SendViewerUpdate();
      }
      //////////////////////////////////////////////////////////////////////////
   }
   delete pDeleteEventHandler_;
   pDeleteEventHandler_ = NULL;
   delete pPaintEventHandler_;
   pPaintEventHandler_ = NULL;
}


/** */
bool VectorDeleteButton::DoSetValue(bool State){
   if (State)
      Start();
   else if (!State && IsActive())
      End();

   return ToggleButton::DoSetValue(State);
}
} // suri namespace
