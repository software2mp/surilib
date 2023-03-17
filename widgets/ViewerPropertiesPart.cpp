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
#include "suri/ViewerPropertiesPart.h"
#include "suri/DynamicViewerWidget.h"
#include "Viewer2DProperties.h"
#include "Viewer3DProperties.h"
#include "suri/LibraryManager.h"

// Includes Wx
#include "wx/choicebk.h"

// Defines

namespace suri {

// forwards
class DataViewManager;

ViewerPropertiesPart::ViewerPropertiesPart(DynamicViewerWidget* pViewer,
                                           LibraryManager* pLibManager,
                                           DataViewManager* pDataViewManager) :
      p2dProperties_(new Viewer2DProperties(pViewer, pLibManager)),
      p3dProperties_(new Viewer3DProperties(pViewer, pDataViewManager)), pViewer_(pViewer) {
}

ViewerPropertiesPart::~ViewerPropertiesPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool ViewerPropertiesPart::HasChanged() {
   wxChoicebook* pchoicebook = XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
   int page = pchoicebook->GetSelection();
   return page == 0 ? p2dProperties_->HasChanged() : p3dProperties_->HasChanged();
   return true;
}

/** Salva los cambios realizados en el Part. */
bool ViewerPropertiesPart::CommitChanges() {
   wxChoicebook* pchoicebook = XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
   int page = pchoicebook->GetSelection();
   return (page == 0) ? p2dProperties_->CommitChanges() : p3dProperties_->CommitChanges();
}

/** Restaura los valores originales del Part. */
bool ViewerPropertiesPart::RollbackChanges() {
   return false;
}

/** Inicializa el part */
void ViewerPropertiesPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void ViewerPropertiesPart::Update() {
}

/** Implementacion del metodo de creacion de ventana de Widget */
bool ViewerPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
         wxT("ID_VISUALIZATION_PROPERTIES_PART"));
   if (!pToolWindow_)
      return false;
   AddControl(p2dProperties_->GetWidget(), wxT("ID_2D_VISUALIZATION_PROPERTIES_PANEL"));
   AddControl(p3dProperties_->GetWidget(), wxT("ID_3D_VISUALIZATION_PROPERTIES_PANEL"));
   wxChoicebook* pchoicebook = XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
   DynamicViewerWidget* dynamicviewer = dynamic_cast<DynamicViewerWidget*>(pViewer_);
   int mode = dynamicviewer->GetVisualizationMode();
   if (mode < 2)
      pchoicebook->SetSelection(0);
   else
      pchoicebook->SetSelection(1);
   return true;
}
} /** namespace suri */
