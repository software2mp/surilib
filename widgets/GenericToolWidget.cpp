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

// Includes standard
// Includes Suri
#include "suri/GenericToolWidget.h"
#include "suri/ToolGroupManager.h"
#include "suri/Tool.h"
#include "suri/Button.h"
#include "suri/messages.h"
#include "suri/ViewerWidget.h"
#include "resources.h"
// Includes Wx
#include "wx/bmpbuttn.h"
#include "wx/xrc/xmlres.h"
// Includes App
#include "PointElementEditor.h"
#include "PixelVectorButton.h"
// Defines

namespace suri {
// Ctor
GenericToolWidget::GenericToolWidget(wxWindow *pParent,
                                       const std::string &WindowTitle,
                                       const std::string &WindowId) :
                                                         Widget(pParent) {
   windowTitle_ = WindowTitle;
   wantedWidth_ = 200;
   wantedHeight_ = 20;
   toolName_ = GetWindowTitle().c_str();
   windowId_ = WindowId;
}

// Dtor
GenericToolWidget::~GenericToolWidget() {
}

/** Creacion de la ventana */
bool GenericToolWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                     windowId_.c_str());
   InitializeToolbar(pToolWindow_, pViewer_, pList_);

   pViewer_->Link(this);

   return true;
}

/** Retorna la ventana */
wxWindow *GenericToolWidget::GetToolWindow() const {
   return (const_cast<GenericToolWidget*>(this))->GetWindow();
}

bool GenericToolWidget::IsEnabled() {
   return true;
}

}  // namespace suri
