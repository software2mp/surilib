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
#include "suri/ToggleButton.h"
#include "suri/VectorEditor.h"
#include "suri/ViewerWidget.h"
#include "PolygonElementEditor.h"
#include "PixelVectorButton.h"
#include "resources.h"
// Includes Wx
// Defines

// todo provisorio mover a messages.h
#define tooltip_SELECT_VECTOR "Seleccionar vector"
#define tooltip_EDIT_TABLE "Editar tabla de vector"
#define tooltip_EDIT_GEOMETRY "Editar geometria de vector"

#include "suri/VectorTool.h"

namespace suri {
/** Ctor */
VectorTool::VectorTool(ViewerWidget *pViewer, int AnotationGroup, int VectorGroup,
                       VectorEditor* pVectorEditor, wxWindow *pParent) :
      Widget(pParent), pVectorEditor_(pVectorEditor), anotationGroup_(AnotationGroup),
      vectorGroup_(VectorGroup) {
   if( Configuration::GetParameter("app_short_name", "") == wxT("SoPI") )
      windowTitle_ = caption_DRAW_TOOL;
   else
      windowTitle_ = caption_DRAW_CREATION_TOOL;
   wantedWidth_ = 200;
   wantedHeight_ = 20;
   toolName_ = GetWindowTitle().c_str();
}

/** Dtor */
VectorTool::~VectorTool() {
}

/**
 *  Creacion de la ventana
 *  @return bool true en caso de exito, false en caso contrario.
 */
bool VectorTool::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_VECTOR_TOOLBAR"));
   InitializeToolbar(pToolWindow_, pViewer_, pList_);

   pViewer_->Link(this);

   return true;
}

/**
 * Retorna la ventana
 * @return wxWindow * Ventana
 */
wxWindow *VectorTool::GetToolWindow() const {
   return (const_cast<VectorTool*>(this))->GetWindow();
}

/**
 * Devuelve el estado del Tool.
 * @return bool true si esta habilitado, false en caso contrario
 */
bool VectorTool::IsEnabled() {
   return true;
}
}
