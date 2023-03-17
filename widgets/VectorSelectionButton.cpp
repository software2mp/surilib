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
#include "VectorSelectionButton.h"
#include "VectorDatasource.h"
#include "VectorEditorButtonTool.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "VectorEditorButtonTool.h"


// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor */
VectorSelectionButton::VectorSelectionButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                             const wxString &Bitmap,
                                             wxString ButtonTooltip,
                                             GuiGeometryCreator::GeometryType GeoType,
                                             DataViewManager* pDataViewManager,
                                             int Flags) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, GeoType, ButtonTooltip,
                         pDataViewManager->GetViewportManager(),
                         pDataViewManager->GetViewcontextManager(),
                         Flags),
      pFeatureSelection_(new FeatureSelection),
      pEventHandler_(NULL),
      pSelectionSource_(NULL), pDataViewManager_(pDataViewManager) {
}

VectorSelectionButton::~VectorSelectionButton() {
   // TODO Auto-generated destructor stub
}

/** Preparacion para el inicio de la selecion */
void VectorSelectionButton::Start() {
   ViewcontextInterface* pviewcontext = pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   if (!pviewcontext)
      return;
   VectorEditorButtonTool::BlockActivatedDatasources(pviewcontext, pDataViewManager_);

   if (IsActive()) {
      return;
   }

   /** Obtenemos la fuente de datos */
   VectorDatasource* pdatasource =
         dynamic_cast<VectorDatasource*>(VectorEditorButtonTool::GetSelectedDataSource(
               pviewcontext, pDataViewManager_));

   /** Si no logramos obtenerla, salimos */
   if ( !pdatasource ) {
      SHOW_ERROR("Error al intentar obtener el vector desde la fuente de datos.");
      End(); // Nos aseguramos que todo vuelva a 0.
      return;
   }

   pSelectionSource_ = new VectorDatasourceSelectionSource(pdatasource, NULL);
   pFeatureSelection_->Configure(pSelectionSource_);
   pEventHandler_ = new GeometrySelectionEvent(pDataViewManager_->GetViewportManager(),
                                               pDataViewManager_->GetViewcontextManager(),
                                          pFeatureSelection_);
   pViewer_->PushPaintEvent(pEventHandler_);
   pViewer_->PushMouseEvent(pEventHandler_);
}

/** Finalizamos la seleccion */
void VectorSelectionButton::End() {
   if (pEventHandler_ && pViewer_) {

      VectorEditorButtonTool::UnBlockActivatedDatasources(pDataViewManager_);

      pViewer_->PopPaintEvent(pEventHandler_);
      pViewer_->PopMouseEvent(pEventHandler_);

      delete pEventHandler_;
      pEventHandler_ = NULL;
   }
}

/** */
bool VectorSelectionButton::DoSetValue(bool State) {
   if (State)
      Start();
   else if (!State && IsActive())
      End();

   return ToggleButton::DoSetValue(State);
}
}  // namespace suri


