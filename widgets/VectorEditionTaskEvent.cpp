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
#include "VectorEditionTaskEvent.h"
#include "GeometryRenderer.h"

namespace suri {

/** Ctor */
VectorEditionTaskEvent::VectorEditionTaskEvent (
                              ViewportManagerInterface *pViewportManager,
                              ViewcontextManagerInterface *pViewcontextManager,
                              VectorEditionTask* pVectorEditionTask) :
                  ViewportEvent(pViewportManager, pViewcontextManager) {
   pVectorEditionTask_ = pVectorEditionTask;
   closedBeforeSetAPoint_ = true;
}

/** Dtor */
VectorEditionTaskEvent::~VectorEditionTaskEvent () {
}

/** Limpia el estado de la herramienta, aborta edicion */
void VectorEditionTaskEvent::Reset() {
}

/** Se ejecuta cuando el usuario hace click sobre el visualizador. */
void VectorEditionTaskEvent::SetPoint(double Pixel, double Line,
                                                        wxMouseEvent &Event) {
}

/** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
void VectorEditionTaskEvent::UpdatePoint(double Pixel, double Line,
                                                        wxMouseEvent &Event) {
}

/**
 * Al pintar se dibuja geometria en edicion
 * Centraliza el renderizado de geometrias sobre el visualizador para asegurar
 * que se renderizen en el orden correcto:
 *  i. Geometrias modificadas/creadas
 *  i. Geometrias seleccionadas
 *  i. Geometria que se esta modificando/creando
 */
void VectorEditionTaskEvent::OnPaint(wxPaintEvent&Event, wxDC &Dc) {
   if (!pVectorEditionTask_->IsActive())
      return;
   SetClosedWithoutEdition(false);

   ViewerWidget* pviewer = GetEventViewer(Event);
   GeometryRenderer renderer(&Dc, pviewer->GetWorld());

   // Renderizo geometrias borradas
   GeometryCollection* pgeometries = pVectorEditionTask_->GetDeletedGeometries();
   renderer.Render(pgeometries, NULL);

   // Renderizo capas modificadas
   pgeometries = pVectorEditionTask_->GetModifiedGeometries();
   renderer.Render(pgeometries, NULL);

   // Renderizo capas seleccionadas
   pgeometries = pVectorEditionTask_->GetFeatureSelector()->
                           GetSelectedGeometries(pviewer->GetWorld());
   renderer.Render(pgeometries, NULL);

   // Renderizo geometria en creacion.
   if (pVectorEditionTask_->IsEditingFeature()) {
      Geometry* pgeom = pVectorEditionTask_->GetEditedGeometry(
                                 VectorEditionTask::GEOMETRY_COLUMN_NAME);
      if (pgeom) {
         std::vector<Geometry*> vector;
         vector.push_back(pgeom);
         renderer.Render(vector, NULL);
      }
   }
}

/** Indica que quiere recibir evento OnPaint */
PaintEventInterface::PaintEventFlags VectorEditionTaskEvent::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/** Indica que no se quieren recibir eventos de mouse */
MouseEventInterface::MouseEventFlags VectorEditionTaskEvent::GetMouseFlags() const {
   return MouseEventInterface::NoneFlag;
}

/** Solo retorna true para los eventos que ocurren en visualizador de edicion */
bool VectorEditionTaskEvent::CanProcessEvent(wxEvent &Event) const {
   ViewerWidget* pviewer = GetEventViewer(Event);
   return pVectorEditionTask_->IsActive() &&
               pviewer->GetWorld() == pVectorEditionTask_->GetWorld();
}

/**
 * Metodo que es llamado cuando se actualiza la seleccion.
 * @param[in] FeaturesSelected id de los features seleccionados
 */
void VectorEditionTaskEvent::SelectionUpdated() {
   if (pVectorEditionTask_->IsActive())
      RefreshViewers(GetViewportManager()->GetViewportIds());
}
/** Metodo que se utiliza para saber cuando alguien hace click en
 * editar y antes de hacer cualquier edición */
void VectorEditionTaskEvent::SetClosedWithoutEdition(bool value){
   closedBeforeSetAPoint_ = value;
}
bool VectorEditionTaskEvent::GetClosedWithoutEdition() {
   return closedBeforeSetAPoint_;
}


} /* namespace suri */
