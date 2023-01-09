/*! \file QueryCommandExecutionHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/QueryCommandExecutionHandler.h"
#include "Query.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/DataViewManager.h"
#include "suri/ViewerWidget.h"

namespace suri {
namespace core {

/** Constructor */
QueryCommandExecutionHandler::QueryCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView), pQuery_(new Query(pDataView->GetViewportManager(),
                                               pDataView->GetViewcontextManager(),
                                               pDataView->GetLibraryManager())) {
}

/** Destructor */
QueryCommandExecutionHandler::~QueryCommandExecutionHandler() {
   delete pQuery_;
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool QueryCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id != pTool->GetId("Query"))
      return false;
   std::vector<SuriObject::UuidType> ids = pDataView_->GetViewportManager()->GetViewportIds();
   ViewerWidget* pviewer = NULL;
   for (size_t i = 0; i < ids.size(); ++i) {
      pviewer = dynamic_cast<ViewerWidget*>(pDataView_->GetViewportManager()->GetViewport(ids[i]));
      if (pTool->IsActive(pCommand)) {
         pviewer->PushMouseEvent(pQuery_);
         pviewer->PushPaintEvent(pQuery_);
      } else {
         pviewer->PopMouseEvent(pQuery_);
         pviewer->PopPaintEvent(pQuery_);
      }
   }
   return true;
}

} /** namespace core */
} /** namespace suri */
