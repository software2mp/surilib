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
