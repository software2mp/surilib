/*! \file QuickMeassureCommandExecutionHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/QuickMeassureCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/DataViewManager.h"
#include "suri/Viewer2D.h"
#include "suri/DynamicViewerWidget.h"
#include "MeassureWidget.h"

namespace suri {
namespace core {

/** Constructor **/
QuickMeassureCommandExecutionHandler::QuickMeassureCommandExecutionHandler(
      DataViewManager* pDataView, suri::MeassureWidget* pQuickMeasure) :
            pDataView_(pDataView), pQuickMeasure_(pQuickMeasure) {
}

/** Destructor **/
QuickMeassureCommandExecutionHandler::~QuickMeassureCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool QuickMeassureCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id == pTool->GetId("QuickMeassure")) {
      Viewer2D* pviewer =
            dynamic_cast<Viewer2D*>(pDataView_->GetViewportManager()->GetSelectedViewport());
      if (!pviewer) {
         DynamicViewerWidget* pdynamic =
               dynamic_cast<DynamicViewerWidget*>(pDataView_->GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer(DynamicViewerWidget::Raster));
      }
      if (pviewer) {
         if (pCommand->IsActive()) {
            pQuickMeasure_->ShowWidget();
         } else {
            pQuickMeasure_->HideWidget();
         }
      }
   }
   else
      return false;
   return true;
}

} /** namespace core */
} /** namespace suri */
