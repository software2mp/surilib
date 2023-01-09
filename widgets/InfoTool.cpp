/*! \file InfoTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "Query.h"
#include "resources.h"
#include "Viewer2DProperties.h"
#include "suri/messages.h"
#include "suri/Viewer2D.h"
#include "suri/InfoTool.h"
#include "suri/DataTypes.h"
#include "suri/ToolCommand.h"
#include "suri/EventCommand.h"
#include "suri/PartContainerWidget.h"
#include "MeassureWidget.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/ViewerPropertiesPart.h"

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
InfoTool::InfoTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "info";
   Initialize();
}

/**
 * Destructor
 */
InfoTool::~InfoTool() {
}

/**
 * Ejecuta un comando
 */
void InfoTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("HotlinkQuery")) {
      // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   } else if (id == GetId("Query")) {
      // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   } else if (id == GetId("QuickMeassure")) {
      Viewer2D* pviewer =
            dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
      if (!pviewer) {
         DynamicViewerWidget* pdynamic =
               dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer(DynamicViewerWidget::Raster));
      }
      if (pviewer) {
         if (pToolCommand->IsActive()) {
            pviewer->GetMeasureWidget()->ShowWidget();
         } else {
            pviewer->GetMeasureWidget()->HideWidget();
         }
      }
   } else if (id == GetId("ViewportProperties")) {
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      Part *pviewportproperties = NULL;
      if (dynamic_cast<DynamicViewerWidget*>(pviewer))
         pviewportproperties = new ViewerPropertiesPart(
               dynamic_cast<DynamicViewerWidget*>(pviewer),
               GetLibraryManager(), GetDataViewManager());

      if (pviewportproperties) {
         PartContainerWidget *pcontainer = new PartContainerWidget(pviewportproperties,
               _(caption_VIEWPORT_PROPERTIES));
         pcontainer->ShowModal();
      }
   }
}

/**
 * Crea un comando
 */
Command* InfoTool::CreateCommand(const std::string &CommandName) {
   Command* pcommand = NULL;
   if (CommandName == "Query") {
      pQuery_ = new Query(GetViewportManager(), GetViewcontextManager(),
                          GetLibraryManager());
      SetMouseEvent(pQuery_);
      SetPaintEvent(pQuery_);
      pcommand = new EventCommand(pQuery_, pQuery_, SuriObject::NullIntUuid,
                                  "ToggleButton|Menu", tool_QUERY, tooltip_PIXEL_QUERY,
                                  "ID_CONATINERMID|ID_PIXEL_QUERY_MENUTIEM");
   } else if (CommandName == "QuickMeassure") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-64);
      pcommand->SetDescription(tooltip_QUICK_MEASUREMENT);
      pcommand->SetDecoration(tool_MEASURE_WIDGET);
      pcommand->SetLocation("ID_CONATINERMID|ID_MEASSURE_MENUTIEM");
   } else if (CommandName == "ViewportProperties") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_VIEWPORT_LINK);
      pcommand->SetDecoration(icon_VIEWPORT_PROPERTIES);
      pcommand->SetLocation("ID_VIEWER_PROPERTIES_MENUTIEM");
   }
   return pcommand;
}

/**
 * Destruye un comando
 */
void InfoTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Crea el widget
 */
Widget* InfoTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /* namespace suri */
