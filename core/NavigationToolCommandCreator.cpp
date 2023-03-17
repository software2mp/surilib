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
#include "suri/NavigationToolCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/DragEvent.h"
#include "suri/DataViewManager.h"
#include "suri/GenericTool.h"
#include "suri/EventCommand.h"
#include "suri/MultiZoom.h"

// Includes Wx
// Defines
// forwards

namespace suri {
namespace core {

/** Constructor */
ViewportLinkCommandCreator::ViewportLinkCommandCreator() {
}

/** Destructor */
ViewportLinkCommandCreator::~ViewportLinkCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* ViewportLinkCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                     ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "ViewportLink") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_VIEWPORT_LINK);
      pcommand->SetDecoration(tool_VIEWPORT_LINK);
      pcommand->SetLocation("ID_SINCRONIZE_VIEWERS_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
NavigationCommandCreator::NavigationCommandCreator() {
}

/** Destructor */
NavigationCommandCreator::~NavigationCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* NavigationCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                   ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "Navigation") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-128);
      pcommand->SetDescription(tooltip_NAVIGATION_WIDGET);
      pcommand->SetDecoration(tool_NAVIGATION_WIDGET);
      pcommand->SetLocation("ID_CONATINERMID|ID_NAVIGATION_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
FullExtentCommandCreator::FullExtentCommandCreator() {
}

/** Destructor */
FullExtentCommandCreator::~FullExtentCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* FullExtentCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                   ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "FullExtent") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(tooltip_ZOOM_FULL_EXTENT);
      pcommand->SetDecoration(button_ZOOM_FULL_EXTENT);
      pcommand->SetLocation("ID_CONATINERMID|ID_ZOOM_FULL_EXTENT_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
LayerExtentCommandCreator::LayerExtentCommandCreator() {
}

/** Destructor */
LayerExtentCommandCreator::~LayerExtentCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* LayerExtentCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                   ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "LayerExtent") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(tooltip_ZOOM_LAYER_EXTENT);
      pcommand->SetDecoration(button_ZOOM_LAYER_EXTENT);
      pcommand->SetLocation("ID_CONATINERMID|ID_ZOOM_TO_LAYER_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
Zoom1on1CommandCreator::Zoom1on1CommandCreator() {
}

/** Destructor */
Zoom1on1CommandCreator::~Zoom1on1CommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* Zoom1on1CommandCreator::DoCreateCommand(const std::string& CommandName,
                                                 ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "Zoom1on1") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(tooltip_ZOOM_ONE_TO_ONE);
      pcommand->SetDecoration(button_ZOOM_ONE_TO_ONE);
      pcommand->SetLocation("ID_CONATINERMID|ID_ZOOM_1_1_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
SaveViewCommandCreator::SaveViewCommandCreator() {
}

/** Destructor */
SaveViewCommandCreator::~SaveViewCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* SaveViewCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                 ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "SaveView") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(caption_SAVE_VISUALIZATION);
      pcommand->SetDecoration(button_SAVE_VIEW);
      pcommand->SetLocation("ID_CONATINERMID|ID_SAVE_VISIALIZATION_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
DragCommandCreator::DragCommandCreator(DataViewManager* pDataViewManager, GenericTool* pTool) :
      pDataView_(pDataViewManager), pTool_(pTool), pDrag_(NULL) {
}

/** Destructor */
DragCommandCreator::~DragCommandCreator() {
   delete pDrag_;
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* DragCommandCreator::DoCreateCommand(const std::string& CommandName,
                                             ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "Drag") {
      pDrag_ = new DragEvent(pDataView_->GetViewportManager(), pDataView_->GetViewcontextManager());
      pTool_->SetMouseEvent(pDrag_);
      pcommand = new EventCommand(pDrag_, NULL, SuriObject::NullIntUuid, "ToggleButton|Menu",
                                  tool_HAND, tooltip_DRAG, "ID_CONATINERMID|ID_SCROLL_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
MultiZoomCommandCreator::MultiZoomCommandCreator(
      DataViewManager* pDataViewManager, GenericTool* pTool) :
            pDataView_(pDataViewManager), pTool_(pTool), pZoom_(NULL){
}

/** Destructor */
MultiZoomCommandCreator::~MultiZoomCommandCreator() {
   delete pZoom_;
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* MultiZoomCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                  ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "MultiZoom") {
      pZoom_ = new MultiZoom(pDataView_->GetViewportManager(), pDataView_->GetViewcontextManager());
      pTool_->SetMouseEvent(pZoom_);
      pTool_->SetPaintEvent(pZoom_);
      pcommand = new EventCommand(pZoom_, pZoom_, SuriObject::NullIntUuid, "ToggleButton|Menu",
                                  tool_ZOOM, tooltip_ZOOM, "ID_CONATINERMID|ID_ZOOM_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
ZoomInCommandCreator::ZoomInCommandCreator() {
}

/** Destructor */
ZoomInCommandCreator::~ZoomInCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* ZoomInCommandCreator::DoCreateCommand(const std::string& CommandName,
                                               ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "ZoomIn") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription("ZoomIn");
      pcommand->SetDecoration(tool_ZOOM_IN);
      pcommand->SetLocation("ID_ZOOM_IN_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
ZoomOutCommandCreator::ZoomOutCommandCreator() {
}

/** Destructor */
ZoomOutCommandCreator::~ZoomOutCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* ZoomOutCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "ZoomOut") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription("ZoomOut");
      pcommand->SetDecoration(tool_ZOOM_OUT);
      pcommand->SetLocation("ID_ZOOM_OUT_MENUTIEM");
   }
   return pcommand;
}

/** Constructor */
SphericalCameraCommandCreator::SphericalCameraCommandCreator() {
}

/** Destructor */
SphericalCameraCommandCreator::~SphericalCameraCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* SphericalCameraCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                        ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "SphericalCamera") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetGroupId(-128);
      pcommand->SetDescription(tooltip_SPHERICAL_CAMERA);
      pcommand->SetDecoration(tool_SPHERICAL_CAMERA);
      pcommand->SetLocation("ID_SPHERICAL_CAM_MENUITEM");
   }
   return pcommand;
}

/** Constructor */
EyeCameraCommandCreator::EyeCameraCommandCreator() {
}

/** Destructor */
EyeCameraCommandCreator::~EyeCameraCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* EyeCameraCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                        ToolInterface* pTool) {
   Command *pcommand = NULL;
   if (CommandName == "EyeCamera") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetGroupId(-128);
      pcommand->SetDescription(tooltip_EYE_CAMERA);
      pcommand->SetDecoration(tool_EYE_CAMERA);
      pcommand->SetLocation("ID_EYE_CAM_MENUTIEM");
   }
   return pcommand;
}

} /** namespace core */
} /** namespace suri */
