/** \file GeometryDeleteEvent.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "GeometryDeleteEvent.h"
#include "suri/VectorEditionTask.h"

namespace suri {

/** Ctor */
GeometryDeleteEvent::GeometryDeleteEvent(ViewportManagerInterface *pViewportManager,
                                 ViewcontextManagerInterface *pViewcontextManager,
                                 VectorEditionTask* pVectorEditionTask) :
                                 ViewportEvent(pViewportManager, pViewcontextManager) {
   pVectorEditionTask_ = pVectorEditionTask;
}
/** Dtor */
GeometryDeleteEvent::~GeometryDeleteEvent() {
}

/** Limpia el estado de la herramienta, aborta edicion */
void GeometryDeleteEvent::Reset() {
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 * @param[in] Pixel nro de pixel sobre el que se hizo click
 * @param[in] Line nro de linea sobre el que se hizo click
 * @param[in] Event evento wx
 */
void GeometryDeleteEvent::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
   Coordinates viewportposition(Pixel, Line);
   Subset worldsubset = GetWorldSearchBox(viewportposition,
                                   pVectorEditionTask_->GetWorld());

   FeatureSelection* pselector = pVectorEditionTask_->GetFeatureSelector();
   pselector->Select(worldsubset, pVectorEditionTask_->GetWorld());

   std::vector<FeatureIdType> featureids = pselector->GetSelectedFeatures();
   std::vector<FeatureIdType>::iterator it = featureids.begin();
   for (; it != featureids.end(); ++it)
      pVectorEditionTask_->DeleteFeature(*it);
      
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   pviewer->GetWindow()->Refresh();
}

/**
 * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse
 * @param[in] Pixel nro de pixel sobre el que se hizo click
 * @param[in] Line nro de linea sobre el que se hizo click
 * @param[in] Event evento wx
 */
void GeometryDeleteEvent::UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {
}

/**
 * Solo retorna true para los eventos que ocurren en visualizador de edicion
 * @param[in] Event evento wx
 */
bool GeometryDeleteEvent::CanProcessEvent(wxEvent &Event) const {
   ViewerWidget* pviewer = GetEventViewer(Event);
   return pVectorEditionTask_->IsActive() &&
               pviewer->GetWorld() == pVectorEditionTask_->GetWorld();
}


} /* namespace suri */
