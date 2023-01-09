/** \file GeoreferenceEditionEvent.cpp
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "GeoreferenceEditionEvent.h"
#include "suri/GeoreferenceTask.h"
#include "suri/RasterSpatialModel.h"
#include "suri/Configuration.h"

namespace suri {

/**
 * Ctor.
 */
GeoreferenceEditionEvent::GeoreferenceEditionEvent(
      ViewportManagerInterface* pViewportManager,
      ViewcontextManagerInterface* pViewcontextManager,
      GeoreferenceTask* pGeoreferenceTask, bool Editing) :
      ViewportEvent(pViewportManager, pViewcontextManager), validDrag_(false),
      pGeoreferenceTask_(pGeoreferenceTask), dragXHolder_(-1), dragYHolder_(-1),
      editing_(Editing) {
}

/**
 * Dtor.
 */
GeoreferenceEditionEvent::~GeoreferenceEditionEvent() {
}

/**
 * Limpia el estado de la herramienta, aborta edicion.
 */
void GeoreferenceEditionEvent::Reset() {
   validDrag_ = false;
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 */
void GeoreferenceEditionEvent::SetPoint(double Pixel, double Line,
                                        wxMouseEvent& Event) {
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());

   if (!pGeoreferenceTask_->IsActive())
      return;

   if (Event.RightUp()) {
      pGeoreferenceTask_->EndFeatureEdition();
      RefreshViewers(GetViewportManager()->GetViewportIds());
      Configuration::RemoveParameterEx(VETYPE_PARAM_MODE_NAME);
      return;
   }

   if (!editing_ && !pGeoreferenceTask_->IsEditingFeature())
      pGeoreferenceTask_->StartNewFeatureEdition();

   Coordinates imagecoord;
   if (pGeoreferenceTask_->GetGcpCoordinate(imagecoord))
      return;

   // Obtengo cordenadas de pixel y linea de imagen
   Coordinates viewportcoord(Pixel, Line);
   pviewer->GetWorld()->Transform(viewportcoord, imagecoord);
   pGeoreferenceTask_->GetSpatialModel()->InverseTransform(imagecoord);
   pGeoreferenceTask_->SetGcpCoordinate(imagecoord);

   dragXHolder_ = Event.GetX();
   dragYHolder_ = Event.GetY();
   Configuration::SetParameterEx(VETYPE_PARAM_MODE_NAME, VETYPE_PARAM_MODE_EDIT);

   pviewer->GetWindow()->Refresh();
}

/**
 * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse.
 */
void GeoreferenceEditionEvent::UpdatePoint(double Pixel, double Line,
                                           wxMouseEvent& Event) {
}

/**
 * Inicia la edicion de una geometria.
 */
void GeoreferenceEditionEvent::OnLeftDoubleClick(wxMouseEvent& Event) {   
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   
   if (!pGeoreferenceTask_->IsActive())
      return;

   if (!pGeoreferenceTask_->EndFeatureEdition())
      return;

   Coordinates viewportcoord(Event.GetX(), Event.GetY());

   Subset searchsubset = GetWorldSearchBox(viewportcoord,
                                           pGeoreferenceTask_->GetWorld());

   FeatureSelection* pselector = pGeoreferenceTask_->GetFeatureSelector();
   pselector->Select(searchsubset, pGeoreferenceTask_->GetWorld());
   std::vector<FeatureIdType> featureids = pselector->GetSelectedFeatures();
   if (featureids.size() > 0) {
      pselector->ClearSelection();
      pselector->Select(*featureids.begin());
      pGeoreferenceTask_->StartFeatureEdition(*featureids.begin());
      Configuration::SetParameterEx(VETYPE_PARAM_MODE_NAME, VETYPE_PARAM_MODE_EDIT);
   }

   pviewer->GetWindow()->Refresh();
}

/**
 * Al iniciar drag se busca punto en geometrias en edicion.
 */
void GeoreferenceEditionEvent::OnStartDrag(const Coordinates& InitialCoordinate) {
   Coordinates ic = const_cast<Coordinates&>(InitialCoordinate);

   std::string vetype = Configuration::GetParameterEx(VETYPE_PARAM_NAME,
                                                      VETYPE_PARAM_UNDEFINED);

   if (vetype.compare(VETYPE_PARAM_SIMULATED) == 0) {
      ic.x_ = dragXHolder_;
      ic.y_ = dragYHolder_;
      Configuration::RemoveParameterEx(VETYPE_PARAM_NAME);
   } else {
      dragXHolder_ = ic.x_;
      dragYHolder_ = ic.y_;
   }

   validDrag_ = false;
   if (!pGeoreferenceTask_->IsActive() || !pGeoreferenceTask_->IsEditingFeature())
      return;

   Subset searchsubset = GetWorldSearchBox(ic,
                                           pGeoreferenceTask_->GetWorld());

   Coordinates coord;
   if (!pGeoreferenceTask_->GetGcpCoordinate(coord))
      return;
   pGeoreferenceTask_->GetSpatialModel()->Transform(coord);
   if (searchsubset.IsInside(coord))
      validDrag_ = true;
}

/**
 * Al finalizar drag se actualiza geometria en vector.
 */
void GeoreferenceEditionEvent::OnEndDrag(const Coordinates& InitialCoordinate,
                                         const Coordinates& CurrentCoordinate,
                                         wxMouseEvent& Event) {
   validDrag_ = false;

   dragXHolder_ = Event.GetX();
   dragYHolder_ = Event.GetY();

   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   pviewer->GetWindow()->Refresh();
}

/**
 * Al realizar drag se actualiza geoemtria en edicion.
 */
void GeoreferenceEditionEvent::OnDrag(const Coordinates& InitialCoordinate,
                                      const Coordinates& CurrentCoordinate,
                                      wxMouseEvent& Event) {
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   if (!validDrag_ || !IsDragging() || !pGeoreferenceTask_->IsActive() 
      || !pGeoreferenceTask_->IsEditingFeature())
      return;

   // Obtengo cordenadas de pixel y linea de imagen
   Coordinates worldcoord;
   pviewer->GetWorld()->Transform(CurrentCoordinate, worldcoord);
   pGeoreferenceTask_->GetSpatialModel()->InverseTransform(worldcoord);
   // Muevo el punto editado
   pGeoreferenceTask_->SetGcpCoordinate(worldcoord);

   pviewer->GetWindow()->Refresh();
}

/**
 * Solo retorna true para los eventos que ocurren en visualizador de edicion.
 */
bool GeoreferenceEditionEvent::CanProcessEvent(wxEvent& Event) const {   
   return pGeoreferenceTask_->IsActive();
}

}  // namespace suri

