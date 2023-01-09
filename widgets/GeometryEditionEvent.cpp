/** \file VectorLayerEditionEvent.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
