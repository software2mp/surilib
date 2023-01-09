/*! \file VectorEditionButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "VectorEditionButton.h"
#include "GuiGeometryCreator.h"
#include "GeometryEditionEvent.h"
#include "suri/GenericTool.h"
#include "GeometryElementEditor.h"
#include "suri/VectorEditionTable.h"
#include "VectorEditionTaskEvent.h"
#include "suri/DataViewManager.h"
#include "VectorEditorButtonTool.h"

namespace suri {

/** Ctor */
VectorEditionButton::VectorEditionButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                         const wxString &Bitmap,
                                         GuiGeometryCreator::GeometryType GeoType,
                                         wxString ButtonTooltip,
                                         DataViewManager* pDataViewManager, int Flags) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, GeoType, ButtonTooltip,
                         pDataViewManager->GetViewportManager(),
                         pDataViewManager->GetViewcontextManager(), Flags),
      pFeatureSelection_(pDataViewManager->GetFeatureSelection()), pEventHandler_(NULL),
      pPaintEventHandler_(
      NULL),
      pSelectionSource_(NULL), pDataViewManager_(pDataViewManager),
      pTask_(new VectorEditionTask), pTable_(NULL), error_(false), taskended_(false) {
   pDataViewManager_->GetTaskManager()->AppendTask(pTask_);
}

/** Destructor */
VectorEditionButton::~VectorEditionButton() {
}

/** Inicia la edicion de geometrias preexistentes */
void VectorEditionButton::Start() {
   if (!pDataViewManager_) {
      return;
   }
   ViewcontextInterface* pviewcontext =
         pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   if (!pviewcontext) return;

   VectorEditorButtonTool::BlockActivatedDatasources(pviewcontext, pDataViewManager_);
   if (IsActive()) {
      return;
   }
   if (!pTask_ || !pDataViewManager_->GetTaskManager()->GetActiveTaskIds().empty())
      return;

   VectorDatasource* pdatasource =
         dynamic_cast<VectorDatasource*>(VectorEditorButtonTool::GetSelectedDataSource(
               pviewcontext, pDataViewManager_));

   /** Obtenemos la fuente de datos */
   if (!pdatasource) {
      SHOW_ERROR("Error al intentar obtener el vector desde la fuente de datos.");
      End(); // Nos aseguramos que todo vuelva a 0.
      // Prendo el flag de error para que vuelva a su posicion inicial el boton.
      error_ = true;
      return;
   }

   // config tabla
   if (!VectorEditorButtonTool::ConfigureTable(pdatasource, &pTable_)) {
      SHOW_ERROR("Error al intentar configurar la tabla del shapefile.");
      // Prendo el flag de error para que vuelva a su posicion inicial el boton.
      error_ = true;
      End();
      return;
   }

   // config tarea
   if (!VectorEditorButtonTool::ConfigureTask(pTask_, pdatasource, pDataViewManager_,
                                              pFeatureSelection_, &pTable_)) {
      SHOW_ERROR("Error al intentar configurar la tarea.");
      End();
      return;
   }

   taskended_ = false;
   pTask_->Start();

   pDatasource_ = pdatasource;
   pEventHandler_ = new GeometryEditionEvent(pDataViewManager_->GetViewportManager(),
                                             pDataViewManager_->GetViewcontextManager(),
                                             pTask_, this);
   pPaintEventHandler_ = new VectorEditionTaskEvent(
         pDataViewManager_->GetViewportManager(),
         pDataViewManager_->GetViewcontextManager(), pTask_);

   pViewer_->PushPaintEvent(pPaintEventHandler_);
   pViewer_->PushMouseEvent(pEventHandler_);
}

/** Fin de la edicion de la geometria */
void VectorEditionButton::End() {
	VectorEditorButtonTool::UnBlockActivatedDatasources(pDataViewManager_);

   if (pEventHandler_ && pPaintEventHandler_ && pViewer_) {
      pTask_->End();
      pViewer_->PopPaintEvent(pPaintEventHandler_);
      pViewer_->PopMouseEvent(pEventHandler_);

      delete pEventHandler_;
      pEventHandler_ = NULL;
      delete pPaintEventHandler_;
      pPaintEventHandler_ = NULL;

      ViewcontextInterface* pviewcontext =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      if (!pviewcontext)
         return;
      pviewcontext->GetAssociatedLayer(pDatasource_->GetId())->GetElement()->SetChanged();
      pviewcontext->GetAssociatedLayer(pDatasource_->GetId())->GetElement()->SendViewerUpdate();
      pDatasource_ = NULL;
   }
}

bool VectorEditionButton::DoSetValue(bool State) {
   if (State) {
      Start();
      /** Hizo el Start y no llego a completarlo? Volvemos el valor del boton al inicial */
      if (error_)
         State = false;
      error_ = false;
   } else if (!State && IsActive())
      End();

   return ToggleButton::DoSetValue(State);
}

/** Indica si se encuentra activo el editor **/
bool VectorEditionButton::IsActive() const {
   return pEventHandler_ != NULL || pPaintEventHandler_ != NULL;
}
} /** namespace suri */
