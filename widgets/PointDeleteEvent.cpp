/** \file PointDeleteEvent.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "PointDeleteEvent.h"
#include "suri/VectorEditionTask.h"

namespace suri {

/** Ctor */
PointDeleteEvent::PointDeleteEvent (ViewportManagerInterface *pViewportManager,
                              ViewcontextManagerInterface *pViewcontextManager,
                              VectorEditionTask* pVectorEditionTask) :
                              GeometryEditionEvent(pViewportManager,
                                    pViewcontextManager, pVectorEditionTask) {
}

/** Dtor */
PointDeleteEvent::~PointDeleteEvent () {
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 * @param[in] Pixel pixel donde ocurrio el evento de mouse
 * @param[in] Line linea donde ocurrio el evento de mouse
 * @param[in] Event evento wx de mouse
 */
void PointDeleteEvent::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
   GuiGeometryEditorInterface* pgeomeditor = pVectorEditionTask_->GetFeatureEditor();
   Coordinates viewportposition(Pixel, Line);

   std::set<SuriObject::UuidType> points = FindPointsNearCoordinate(
                                                         viewportposition);
   if (!points.empty() && pgeomeditor) {
      pgeomeditor->SelectPoint(*points.begin());
      pgeomeditor->DeleteSelectedPoint();
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      pviewer->GetWindow()->Refresh();
   }
}

/** 
 * Al iniciar drag se busca punto en geometrias en edicion 
 * @param[in] InitialCoordinate coordenada donde se inicio drag.
 */
void PointDeleteEvent::OnStartDrag(const Coordinates &InitialCoordinate) {
   validDrag_ = false;
}

} /* namespace suri */
