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

#include "GcpEditionEvent.h"
#include "suri/Configuration.h"

namespace suri {

/**
 * Ctor.
 */
GcpEditionEvent::GcpEditionEvent(ViewportManagerInterface *pViewportManager,
                                 ViewcontextManagerInterface *pViewcontextManager,
                                 CoregisterTask* pCoregisterTask, bool Editing) :
      ViewportEvent(pViewportManager, pViewcontextManager), validDrag_(false),
      pCoregisterTask_(pCoregisterTask), dragXHolder_(-1), dragYHolder_(-1), editing_(Editing) {
}

/**
 * Dtor.
 */
GcpEditionEvent::~GcpEditionEvent () {
}

/** Limpia el estado de la herramienta, aborta edicion */
void GcpEditionEvent::Reset() {
   validDrag_ = false;
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 * @param[in] Pixel nro de pixel sobre el que se hizo click
 * @param[in] Line nro de linea sobre el que se hizo click
 * @param[in] Event evento wx
 */
void GcpEditionEvent::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
   CoregisterTask::WorkingImageType imagetype;
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());

   if (!pCoregisterTask_->IsActive()
         || !pCoregisterTask_->GetImageType(pviewer->GetWorld(), imagetype)) return;

   if (Event.RightUp()) {
      pCoregisterTask_->EndFeatureEdition();
      RefreshViewers(GetViewportManager()->GetViewportIds());
      Configuration::RemoveParameterEx(VETYPE_PARAM_MODE_NAME);
      return;
   }

   if (!editing_ && !pCoregisterTask_->IsEditingFeature())
      pCoregisterTask_->StartNewFeatureEdition();

   Coordinates imagecoord;
   if (pCoregisterTask_->GetGcpCoordinate(imagetype, imagecoord))
      return;

   // Obtengo cordenadas de pixel y linea de imagen
   Coordinates viewportcoord(Pixel, Line);
   pviewer->GetWorld()->Transform(viewportcoord, imagecoord);
   pCoregisterTask_->GetSpatialModel(imagetype)->InverseTransform(imagecoord);
   pCoregisterTask_->SetGcpCoordinate(imagetype, imagecoord);

   dragXHolder_ = Event.GetX();
   dragYHolder_ = Event.GetY();
   Configuration::SetParameterEx(VETYPE_PARAM_MODE_NAME, VETYPE_PARAM_MODE_EDIT);

   pviewer->GetWindow()->Refresh();
}

/**
 * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse sobre
 * el visualizador.
 * @param[in] Pixel nro de pixel sobre el que esta el mouse
 * @param[in] Line nro de linea sobre el que esta el mouse
 * @param[in] Event evento wx
 */
void GcpEditionEvent::UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {

}

/**
 * Inicia la edicion de un gcp
 * @param[in] Event evento de mouse wx
 */
void GcpEditionEvent::OnLeftDoubleClick(wxMouseEvent& Event) {
   CoregisterTask::WorkingImageType imagetype;
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   if (!pCoregisterTask_->IsActive() ||
               !pCoregisterTask_->GetImageType(pviewer->GetWorld(), imagetype))
      return;

   if (!pCoregisterTask_->EndFeatureEdition())
      return;

   Coordinates viewportcoord(Event.GetX(), Event.GetY());
   Subset searchsubset = GetWorldSearchBox(viewportcoord,
                                   pCoregisterTask_->GetWorld(imagetype));

   FeatureSelection* pselector = pCoregisterTask_->GetFeatureSelector();
   pselector->Select(searchsubset, pCoregisterTask_->GetWorld(imagetype));
   std::vector<FeatureIdType> featureids = pselector->GetSelectedFeatures();
   if (featureids.size() > 0) {
      pselector->ClearSelection();
      pselector->Select(*featureids.begin());
      pCoregisterTask_->StartFeatureEdition(*featureids.begin());
      Configuration::SetParameterEx(VETYPE_PARAM_MODE_NAME,
                                                   VETYPE_PARAM_MODE_EDIT);
   }

   pviewer->GetWindow()->Refresh();
}

/**
 * Se ejecuta cuando el usuario realiza drag sobre un visualizador
 * Si corresponde, inicia desplazamiento de un gcp.
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag. Esta
 * en pixel-linea.
 */
void GcpEditionEvent::OnStartDrag(const Coordinates &InitialCoordinate) {
   Coordinates ic = const_cast<Coordinates&>(InitialCoordinate);

   std::string vetype = Configuration::GetParameterEx(
         VETYPE_PARAM_NAME, VETYPE_PARAM_UNDEFINED);

   if (vetype.compare(VETYPE_PARAM_SIMULATED) == 0) {
      ic.x_ = dragXHolder_;
      ic.y_ = dragYHolder_;
      Configuration::RemoveParameterEx(VETYPE_PARAM_NAME);
   } else {
      dragXHolder_ = ic.x_;
      dragYHolder_ = ic.y_;
   }

   validDrag_ = false;
   CoregisterTask::WorkingImageType imagetype;
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());

   if (!pCoregisterTask_->IsActive() || !pCoregisterTask_->IsEditingFeature()
         || !pCoregisterTask_->GetImageType(pviewer->GetWorld(), imagetype)) return;

   Subset searchsubset = GetWorldSearchBox(ic, pCoregisterTask_->GetWorld(imagetype));

   Coordinates coord;
   if (!pCoregisterTask_->GetGcpCoordinate(imagetype, coord)) return;

   pCoregisterTask_->GetSpatialModel(imagetype)->Transform(coord);
   if (searchsubset.IsInside(coord)) validDrag_ = true;
}

/**
 * Se ejecuta al finalizar drag.
 * Finaliza el drag y refresca la pantalla
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void GcpEditionEvent::OnEndDrag(const Coordinates &InitialCoordinate,
                       const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
   validDrag_ = false;

   dragXHolder_ = Event.GetX();
   dragYHolder_ = Event.GetY();

   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   pviewer->GetWindow()->Refresh();
}

/**
 * Si se esta desplazando un punto, modifica su coordenada a las del mouse.
 * Debe transformar las coordenadas a pixel linea de imagen.
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void GcpEditionEvent::OnDrag(const Coordinates &InitialCoordinate,
                                      const Coordinates &CurrentCoordinate,
                                      wxMouseEvent &Event) {
   CoregisterTask::WorkingImageType imagetype;
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   if (!validDrag_ || !IsDragging() || !pCoregisterTask_->IsActive()
                   || !pCoregisterTask_->IsEditingFeature()
                   || !pCoregisterTask_->GetImageType(pviewer->GetWorld(), imagetype))
      return;

   // Obtengo cordenadas de pixel y linea de imagen
   Coordinates worldcoord;
   pviewer->GetWorld()->Transform(CurrentCoordinate, worldcoord);
   pCoregisterTask_->GetSpatialModel(imagetype)->InverseTransform(worldcoord);

   // Muevo el punto editado
   pCoregisterTask_->SetGcpCoordinate(imagetype, worldcoord);

   pviewer->GetWindow()->Refresh();
}

/**
 * Solo retorna true para los eventos que ocurren en visualizador de edicion
 * @param[in] Event evento que quiero analizar
 * @return true si el evento ocurrio sobre el visualizador de edicion.
 * @return false para otro caso
 */
bool GcpEditionEvent::CanProcessEvent(wxEvent &Event) const {
   ViewerWidget* pviewer = GetEventViewer(Event);
   CoregisterTask::WorkingImageType pimagetype;
   return pCoregisterTask_->IsActive() &&
            pCoregisterTask_->GetImageType(pviewer->GetWorld(), pimagetype);
}

} /* namespace suri */
