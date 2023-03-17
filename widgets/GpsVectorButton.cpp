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

// Includes suri
#include "GpsVectorButton.h"
#include "suri/ViewerWidget.h"
#include "suri/Configuration.h"
#include "suri/FileManagementFunctions.h"
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/CoordinatesTransform.h"
#include "suri/messages.h"
#include "VectorElementEditor.h"
#include "MarkButton.h"
#include "DeleteButton.h"
#include "CloseButton.h"
#include "resources.h"
#include "suri/DataViewManager.h"
#include "RasterDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/CoordinatesNavigationInputInterface.h"

// Includes app

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pViewer viewer
 * @param[in] Bitmap nombre del recurso (icono) asociado al boton
 * @param[in] pVectorElementEditor elemento para edicion
 * @param[in] pDataViewManager Administrador de info de la aplicacion
 * @param[in] ButtonTooltip tooltip para el boton
 * @param[in] Flags indica que botones deben mostrarse en el navigator
 */
GpsVectorButton::GpsVectorButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                 const wxString &Bitmap,
                                 VectorElementEditor* pVectorElementEditor,
                                 DataViewManager* pDataViewManager,
                                 wxString ButtonTooltip, int Flags) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, pVectorElementEditor,
                         ButtonTooltip),
      pMarkButton_(NULL), pDeleteButton_(NULL), pCloseButton_(NULL), flag_(Flags),
      pDataViewManager_(pDataViewManager) {
}

/**
 * dtor
 */
GpsVectorButton::~GpsVectorButton() {
}

/**
 * Setea el estado del boton. Ingresa/Saca botones de NavigationWidget. Los
 * botones son agregar punto, eliminar punto y cerrar vector
 * @param[in] State estado en el que va a estar el boton
 */
bool GpsVectorButton::DoSetValue(bool State) {
   if (!pViewer_->GetNavigator()->GetWidget()) {
      return false;
   }

   if (State) {
      // Verifico que no halla vectores en edicion
      if (pList_->GetEditedCount() > 0) {
         bool isinlist = pList_->GetElement(
               pVectorElementEditor_->GetCurrentElement()) !=
               NULL;
         if (!(isinlist && pList_->GetEditedCount() == 1
               && pVectorElementEditor_->GetCurrentElement()->IsEdited())) {
            // Remplazar para que cierre el vector en vez de dar error
            REPORT_ERROR(_(message_ELEMENT_IN_EDITION));
            state_ = false;
            ToggleButton::DoSetValue(false);
            return false;
         }
      }
      // En caso que se active el boton dos veces sin desactivarse
      RemoveButtonsFromNavigationWidget();

      // Agrego boton Mark a NavigationWidget
      if ((flag_ & BUTTON_MARK)
            || (!(flag_ & BUTTON_DELETE) && !(flag_ & BUTTON_CLOSE))) {
         pMarkButton_ = new MarkButton(
               pViewer_->GetNavigator()->GetWidget()->GetToolWindow(), pViewer_,
               pVectorElementEditor_);
         pViewer_->GetNavigator()->GetWidget()->AddButtonTool(pMarkButton_,
                                                              ToolGroupManager::Last);
      }
      if (flag_ & BUTTON_DELETE) {
         // Agrego boton de Delete a NavigationWidget
         pDeleteButton_ = new DeleteButton(
               pViewer_->GetNavigator()->GetWidget()->GetToolWindow(),
               pVectorElementEditor_);
         pViewer_->GetNavigator()->GetWidget()->AddButtonTool(pDeleteButton_,
                                                              ToolGroupManager::Last);
      }
      if (flag_ & BUTTON_CLOSE) {
         // Agrego boton de cerrar vector a NavigationWidget
         pCloseButton_ = new CloseButton(
               pViewer_->GetNavigator()->GetWidget()->GetToolWindow(),
               pVectorElementEditor_);
         pViewer_->GetNavigator()->GetWidget()->AddButtonTool(pCloseButton_,
                                                              ToolGroupManager::Last);
      }
      DatasourceManagerInterface* pdsmgr =
            pDataViewManager_  ? pDataViewManager_->GetDatasourceManager() : NULL;
      ViewcontextManagerInterface* pvcmgr =
            pDataViewManager_  ? pDataViewManager_->GetViewcontextManager() : NULL;
      if (pdsmgr && pvcmgr) {
         RasterDatasource* praster = ToolSupport::GetRasterDatasource(pdsmgr, pvcmgr);
         if (praster && pViewer_) {
            CoordinatesNavigationInputInterface* pcoordpart =
                  pViewer_->GetNavigator()->GetWidget()->GetInputWidget();
            pcoordpart->SetSpatialReference(praster->GetSpatialReference());
            pcoordpart->EnablePixelLineNavigation(false);
            // El ingreso de coordenadas (para Seymour) siempre se hace en
            // grados, minutos y segundos. Esta la posibilidad se usar la
            // version decimal de las coordenadas. Va cableado WGS84.
            pcoordpart->SetSpatialReference(
                  std::string(LATLON_SPATIAL_REFERENCE));

         }
      }

   } else {
      RemoveButtonsFromNavigationWidget();
   }

   return ToggleButton::DoSetValue(State);
}

/**
 * Primero saca los botones de NavigationWidget y luego los elimina.
 */
void GpsVectorButton::RemoveButtonsFromNavigationWidget() {
   // Saco los botones del NavigationWidget
   // Saco Mark
   if (pMarkButton_) {
      pViewer_->GetNavigator()->GetWidget()->RemoveButtonFromTool(pMarkButton_);
   }
   // Saco Delete
   if (pDeleteButton_) {
      pViewer_->GetNavigator()->GetWidget()->RemoveButtonFromTool(pDeleteButton_);
   }
   // Saco Close
   if (pCloseButton_) {
      pViewer_->GetNavigator()->GetWidget()->RemoveButtonFromTool(pCloseButton_);
   }

   // Los deletes los hago despues de cerrar todos los botones por setminsize
   // en destructor de widget. Si no lo hiciera asi cambia tamano minimo.
   if (pMarkButton_) {
      // Elimino la ventana del boton en forma segura.
      pMarkButton_->GetWindow()->Destroy();
      pMarkButton_ = NULL;
   }
   // Saco Delete
   if (pDeleteButton_) {
      // Elimino la ventana del boton en forma segura.
      pDeleteButton_->GetWindow()->Destroy();
      pDeleteButton_ = NULL;
   }
   // Saco Close
   if (pCloseButton_) {
      // Elimino la ventana del boton en forma segura.
      pCloseButton_->GetWindow()->Destroy();
      pCloseButton_ = NULL;
   }
}
}  // namespace suri

