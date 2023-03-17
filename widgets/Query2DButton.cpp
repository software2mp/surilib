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

// Includes Estandar
// Includes Suri
#include "Query2DButton.h"
#include "suri/Viewer2D.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** ctor */
/**
 * @param[in] pToolbar window donde se debe agregar el boton
 * @param[in] pViewer viewer sobre el que se hacen clicks para activar
 * @param[in] pDataViewManager administrador de informacion de la aplicacion
 * hotlink
 */
Query2DButton::Query2DButton(wxWindow *pToolbar, Viewer2D *pViewer,
                             DataViewManager* pDataViewManager) :
      QueryButton(pToolbar, pViewer), pDataViewManager_(pDataViewManager) {
}
/** dtor */
Query2DButton::~Query2DButton() {
}

/** Analiza si el click puede pertenecer a un elemento valido. */
/**
 * @param[in] PosX posicion(X) sobre el que se hizo click
 * @param[in] PosY posicion(Y) sobre el que se hizo click
 */
bool Query2DButton::IsValid(int PosX, int PosY) {
   Coordinates coord;
   pViewer_->GetWorld()->V2WTransform(PosX, PosY, coord.x_, coord.y_);
   Subset world;
   pViewer_->GetWorld()->GetWorld(world);

   return world.IsInside(coord);
}

/** Retorna el box del elemento activo */
/**
 * @param[in] PosX lugar donde se hizo click x
 * @param[in] PosY lugar donde se hizo click y
 * @param[in] ElementSize tamanio del elemento
 * @param[out] QueryBox box con dimencion del elemento centrado en click
 * y en coordenadas de la imange.
 */
bool Query2DButton::GetQueryBox(int PosX, int PosY, int ElementSize, Subset &QueryBox) {
   if (!pViewer_ || !pViewer_->GetWorld()) {
      return false;
   }

   pViewer_->GetWorld()->V2WTransform(PosX - QUERY_TOLERANCE, PosY - QUERY_TOLERANCE,
                                      QueryBox.ul_.x_, QueryBox.ul_.y_);
   pViewer_->GetWorld()->V2WTransform(PosX + QUERY_TOLERANCE, PosY + QUERY_TOLERANCE,
                                      QueryBox.lr_.x_, QueryBox.lr_.y_);

   return true;
}
/** Activa la herramienta de consulta de pixel **/
void Query2DButton::Start() {
   if (!pDataViewManager_)
      return;
   pQuery_ = new Query(pDataViewManager_->GetViewportManager(),
                       pDataViewManager_->GetViewcontextManager(),
                       pDataViewManager_->GetLibraryManager());
   pViewer_->PushPaintEvent(pQuery_);
   pViewer_->PushMouseEvent(pQuery_);
}
/** Desactiva la herramienta de consulta de pixel **/
void Query2DButton::End() {
   if (pQuery_) {
      pViewer_->PopPaintEvent(pQuery_);
      pViewer_->PopMouseEvent(pQuery_);
   }
   delete pQuery_;
   pQuery_ = NULL;
}
#ifdef __PIXEL_INFO__
/** Metodo que se llama cuando se activa/desactiva el togglebutton **/
bool Query2DButton::DoSetValue(bool State) {
   if (State && !pQuery_)
      Start();
   else if (!State)
      End();

   return ToggleButton::DoSetValue(State);
}
#endif
}
