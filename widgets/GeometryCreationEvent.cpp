/** \file GeometryCreationEvent.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>

// Includes suri
#include "GeometryCreationEvent.h"
#include "GeometryRenderer.h"
#include "suri/VectorEditionTask.h"
#include "suri/SnapTool.h"
#include "suri/wxSuriMouseEvent.h"

namespace suri {

/** Ctor */
GeometryCreationEvent::GeometryCreationEvent(
      ViewportManagerInterface *pViewportManager,
      ViewcontextManagerInterface *pViewcontextManager,
      VectorEditionTask* pVectorEditionTask, bool SimpleFeatureCreation) :
      ViewportEvent(pViewportManager, pViewcontextManager),
      pVectorEditionTask_(pVectorEditionTask),
      simpleFeatureCreation_(SimpleFeatureCreation), featureCreated_(false) {
}

/** Dtor */
GeometryCreationEvent::~GeometryCreationEvent() {
}

/** Limpia el estado de la herramienta, aborta edicion */
void GeometryCreationEvent::Reset() {
   if (pVectorEditionTask_->GetFeatureCreator()) {
      pVectorEditionTask_->GetFeatureCreator()->HidePhantomPoint();
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      pviewer->GetWindow()->Refresh();
   }
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 * @param[in] Pixel pixel donde ocurrio el evento de mouse
 * @param[in] Line linea donde ocurrio el evento de mouse
 * @param[in] Event evento wx de mouse
 */
void GeometryCreationEvent::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
   if (Event.RightUp()) {
      pVectorEditionTask_->EndFeatureEdition();
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      pviewer->GetWindow()->Refresh();
      return;
   }
   if (!pVectorEditionTask_->IsEditingFeature()
         && (!simpleFeatureCreation_ || (simpleFeatureCreation_ && !featureCreated_))) {
      pVectorEditionTask_->StartNewFeatureEdition();
      featureCreated_ = true;
   } 
	if (pVectorEditionTask_->IsEditingFeature()) {
	   wxSuriMouseEvent* evnt = NULL;
	   evnt = dynamic_cast<wxSuriMouseEvent*>(&Event);
	   if (evnt) {
	      Pixel = evnt->x_;
	      Line = evnt->y_;
	   }
      Coordinates viewportposition(Pixel, Line);
      if (pVectorEditionTask_->GetSnapTool())
         pVectorEditionTask_->GetSnapTool()->SnapPoint(viewportposition);
      pVectorEditionTask_->GetFeatureCreator()->AppendPoint(viewportposition);
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      pviewer->GetWindow()->Refresh();
   }
}

/**
 * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse
 * @param[in] Pixel nro de pixel en pantalla donde se hizo click
 * @param[in] Line nro de linea en pantalla donde se hizo click
 * @param[in] Event evento de mouse
 */
void GeometryCreationEvent::UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {
   if (!pVectorEditionTask_->IsEditingFeature())
      return;
   Coordinates viewportposition(Pixel, Line);
   pVectorEditionTask_->GetFeatureCreator()->MovePhantomPoint(viewportposition);
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   pviewer->GetWindow()->Refresh();
}

/**
 * Al finalizar drag se actualiza geometria en vector
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void GeometryCreationEvent::OnEndDrag(const Coordinates &InitialCoordinate,
                       const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {

	if (!pVectorEditionTask_->IsEditingFeature()) {
		pVectorEditionTask_->StartNewFeatureEdition();
	}
	if ( Event.LeftUp()) {
			pVectorEditionTask_->GetFeatureCreator()->AppendPoint(CurrentCoordinate);
			ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
			pviewer->GetWindow()->Refresh();
   }
}

/**
 * Solo retorna true para los eventos que ocurren en visualizador de creacion
 * @param[in] Evento a analizar
 * @return bool que indica si se puede procesar el evento.
 */
bool GeometryCreationEvent::CanProcessEvent(wxEvent &Event) const {
   ViewerWidget* pviewer = GetEventViewer(Event);
   return pVectorEditionTask_->IsActive() &&
               pviewer->GetWorld() == pVectorEditionTask_->GetWorld();
}

} /* namespace suri */
