/*! \file VectorSelectionButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "VectorSelectionButton.h"
#include "VectorDatasource.h"
#include "VectorEditorButtonTool.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "VectorEditorButtonTool.h"


// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor */
VectorSelectionButton::VectorSelectionButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                             const wxString &Bitmap,
                                             wxString ButtonTooltip,
                                             GuiGeometryCreator::GeometryType GeoType,
                                             DataViewManager* pDataViewManager,
                                             int Flags) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, GeoType, ButtonTooltip,
                         pDataViewManager->GetViewportManager(),
                         pDataViewManager->GetViewcontextManager(),
                         Flags),
      pFeatureSelection_(new FeatureSelection),
      pEventHandler_(NULL),
      pSelectionSource_(NULL), pDataViewManager_(pDataViewManager) {
}

VectorSelectionButton::~VectorSelectionButton() {
   // TODO Auto-generated destructor stub
}

/** Preparacion para el inicio de la selecion */
void VectorSelectionButton::Start() {
   ViewcontextInterface* pviewcontext = pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
   if (!pviewcontext)
      return;
   VectorEditorButtonTool::BlockActivatedDatasources(pviewcontext, pDataViewManager_);

   if (IsActive()) {
      return;
   }

   /** Obtenemos la fuente de datos */
   VectorDatasource* pdatasource =
         dynamic_cast<VectorDatasource*>(VectorEditorButtonTool::GetSelectedDataSource(
               pviewcontext, pDataViewManager_));

   /** Si no logramos obtenerla, salimos */
   if ( !pdatasource ) {
      SHOW_ERROR("Error al intentar obtener el vector desde la fuente de datos.");
      End(); // Nos aseguramos que todo vuelva a 0.
      return;
   }

   pSelectionSource_ = new VectorDatasourceSelectionSource(pdatasource, NULL);
   pFeatureSelection_->Configure(pSelectionSource_);
   pEventHandler_ = new GeometrySelectionEvent(pDataViewManager_->GetViewportManager(),
                                               pDataViewManager_->GetViewcontextManager(),
                                          pFeatureSelection_);
   pViewer_->PushPaintEvent(pEventHandler_);
   pViewer_->PushMouseEvent(pEventHandler_);
}

/** Finalizamos la seleccion */
void VectorSelectionButton::End() {
   if (pEventHandler_ && pViewer_) {

      VectorEditorButtonTool::UnBlockActivatedDatasources(pDataViewManager_);

      pViewer_->PopPaintEvent(pEventHandler_);
      pViewer_->PopMouseEvent(pEventHandler_);

      delete pEventHandler_;
      pEventHandler_ = NULL;
   }
}

/** */
bool VectorSelectionButton::DoSetValue(bool State) {
   if (State)
      Start();
   else if (!State && IsActive())
      End();

   return ToggleButton::DoSetValue(State);
}
}  // namespace suri


