/*! \file ViewportPropertiesCommandExecutionHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/ViewportPropertiesCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/DataViewManager.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/ViewerPropertiesPart.h"
#include "suri/PartContainerWidget.h"

namespace suri {
namespace core {

/** Constructor **/
ViewportPropertiesCommandExecutionHandler::ViewportPropertiesCommandExecutionHandler(
      DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
ViewportPropertiesCommandExecutionHandler::~ViewportPropertiesCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ViewportPropertiesCommandExecutionHandler::Execute(
      const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id == pTool->GetId("ViewportProperties"))
      ShowProperties();
   else
      return false;
   return true;
}

/** Metodo auxiliar encargado de mostrar las propiedades del visualizador **/
void ViewportPropertiesCommandExecutionHandler::ShowProperties() {
   ViewerWidget *pviewer = AS_VW(pDataView_->GetViewportManager()->GetSelectedViewport());
   Part *pviewportproperties = NULL;

   if (dynamic_cast<DynamicViewerWidget*>(pviewer))
      pviewportproperties = new ViewerPropertiesPart(
            dynamic_cast<DynamicViewerWidget*>(pviewer),
            pDataView_->GetLibraryManager(), pDataView_);

   if (pviewportproperties) {
      PartContainerWidget *pcontainer =
            new PartContainerWidget(pviewportproperties, _(caption_VIEWPORT_PROPERTIES));
      pcontainer->ShowModal();
   }
}

} /** namespace core */
} /** namespace suri */
