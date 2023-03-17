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

#include "GeometryEditionEvent.h"
#include "GeometryRenderer.h"
#include "GeometryCollection.h"
#include "suri/VectorStyle.h"
#include "suri/Viewer2D.h"
#include "SimpleGeometryEditor.h"
#include "suri/VectorEditionTask.h"
#include "suri/Configuration.h"
#include "VectorEditionButton.h"

namespace suri {

/** Ctor */
GeometryEditionEvent::GeometryEditionEvent(
      ViewportManagerInterface *pViewportManager,
      ViewcontextManagerInterface *pViewcontextManager,
      VectorEditionTask* pVectorEditionTask, VectorEditionButton* pEditionButton) :
      ViewportEvent(pViewportManager, pViewcontextManager) {
   validDrag_ = false;
   pVectorEditionTask_ = pVectorEditionTask;
   dragXHolder_ = -1;
   dragYHolder_ = -1;
   pEditionButton_ = pEditionButton;
}

/** Dtor */
GeometryEditionEvent::~GeometryEditionEvent() {
}

/** Limpia el estado de la herramienta, aborta edicion */
void GeometryEditionEvent::Reset() {
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 * @param[in] Pixel nro de pixel sobre el que se hizo click
 * @param[in] Line nro de linea sobre el que se hizo click
 * @param[in] Event evento wx
 */
void GeometryEditionEvent::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
   if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
      if (Event.RightUp()) {
         pEditionButton_->taskended_ = true;
         pVectorEditionTask_->EndFeatureEdition();
         ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
         pviewer->GetWindow()->Refresh();
         return;
      }
   }

   GuiGeometryEditorInterface* pgeomeditor = pVectorEditionTask_->GetFeatureEditor();
   Coordinates viewportposition(Pixel, Line);
   std::set<SuriObject::UuidType> points = FindPointsNearCoordinate(viewportposition);
   if (!points.empty() && pgeomeditor) {
      pgeomeditor->SelectPoint(*points.begin());
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      pviewer->GetWindow()->Refresh();
   }
}

/**
 * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse sobre
 * el visualizador.
 * @param[in] Pixel nro de pixel sobre el que esta el mouse
 * @param[in] Line nro de linea sobre el que esta el mouse
 * @param[in] Event evento wx
 */
void GeometryEditionEvent::UpdatePoint(double Pixel, double Line,
		wxMouseEvent &Event) {

}

/** 
 * Inicia la edicion de una geometria 
 * @param[in] Event evento de mouse wx
 */
void GeometryEditionEvent::OnLeftDoubleClick(wxMouseEvent& Event) {
	if (!pVectorEditionTask_->IsActive())
		return;

	pVectorEditionTask_->EndFeatureEdition();

	Coordinates viewportcoord(Event.GetX(), Event.GetY());
	Subset searchsubset = GetWorldSearchBox(viewportcoord,
			pVectorEditionTask_->GetWorld());

	FeatureSelection* pselector = pVectorEditionTask_->GetFeatureSelector();
	pselector->Select(searchsubset, pVectorEditionTask_->GetWorld());
	std::vector<FeatureIdType> featureids = pselector->GetSelectedFeatures();
	if (featureids.size() > 0) {
		pselector->ClearSelection();
		pselector->Select(*featureids.begin());
		pVectorEditionTask_->StartFeatureEdition(*featureids.begin());
	}

	ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
	pviewer->GetWindow()->Refresh();
}

/**
 * Se ejecuta cuando el usuario realiza drag sobre un visualizador
 * Si corresponde, inicia desplazamiento de un punto en vector.
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag. Esta
 * en pixel-linea.
 */
void GeometryEditionEvent::OnStartDrag(const Coordinates &InitialCoordinate) {
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

	std::set<SuriObject::UuidType> points = FindPointsNearCoordinate(ic);

	validDrag_ = false;
	if (!points.empty() && pVectorEditionTask_->IsEditingFeature()) {
		pVectorEditionTask_->GetFeatureEditor()->SelectPoint(*points.begin());
		validDrag_ = true;
	}
}

/**
 * Se ejecuta al finalizar drag.
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void GeometryEditionEvent::OnEndDrag(const Coordinates &InitialCoordinate,
		const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
	validDrag_ = false;
	dragXHolder_ = Event.GetX();
	dragYHolder_ = Event.GetY();
}

/**
 * Si se esta desplazando un punto, modifica su coordenada a las del mouse.
 * Debe transformar las coordenadas al srs de la geometria.
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void GeometryEditionEvent::OnDrag(const Coordinates &InitialCoordinate,
		const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
	// Muevo el primer punto del overlay
	if (pVectorEditionTask_->IsEditingFeature() && IsDragging() && validDrag_)
		pVectorEditionTask_->GetFeatureEditor()->MoveSelectedPoint(
				CurrentCoordinate);
}

/**
 * Busca los puntos de la geometria editada, en subset cercano a la
 * coordenada recibida
 * @param[in] Coord coordenada central del subset.
 * @return conjunto de puntos en subset.
 */
std::set<SuriObject::UuidType> GeometryEditionEvent::FindPointsNearCoordinate(
		const Coordinates &Coord) {
	Subset viewportsubset = Subset(Coord, Coord) + SEARCH_PIXEL_BUFFER;

	if (pVectorEditionTask_->IsEditingFeature()) {
		GuiGeometryEditorInterface* pfeaturededitor =
				pVectorEditionTask_->GetFeatureEditor();
		if (pfeaturededitor)
			return pfeaturededitor->FindPoints(viewportsubset);
	}

	return std::set<SuriObject::UuidType>();
}

/**
 * Solo retorna true para los eventos que ocurren en visualizador de edicion
 * @param[in] Event evento que quiero analizar
 * @return true si el evento ocurrio sobre el visualizador de edicion.
 * @return false para otro caso
 */
bool GeometryEditionEvent::CanProcessEvent(wxEvent &Event) const {
	ViewerWidget* pviewer = GetEventViewer(Event);
	return pVectorEditionTask_->IsActive()
			&& pviewer->GetWorld() == pVectorEditionTask_->GetWorld();
}

} /* namespace suri */
