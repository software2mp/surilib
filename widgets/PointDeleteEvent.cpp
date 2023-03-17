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
