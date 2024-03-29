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

#include "PixelVectorButton.h"

// Includes suri
#include "VectorElementEditor.h"
#include "resources.h"
#include "suri/messages.h"
#include "suri/ViewerWidget.h"
#include "logmacros.h"

// Includes app

// Includes OGR
/** Defines */
/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in]	pToolbar: ventana en la cual se inserta.
 * @param[in]	pViewer:	Viewer sobre el cual actua.
 * @param[in]  Bitmap: ruta del bitmap del boton
 * @param[in]	ButtonTooltip: texto que muestra boton.
 * @param[in]	pVectorElementEditor: VectorElementEditor que se usa para agregar puntos.
 * @param[in]	Flags: flags para indicar los eventos que procesa.
 */
PixelVectorButton::PixelVectorButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                     const wxString &Bitmap,
                                     VectorElementEditor* pVectorElementEditor,
                                     wxString ButtonTooltip, int Flags) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, pVectorElementEditor, ButtonTooltip,
                         Flags) {
}

/** Ctor. default */
PixelVectorButton::PixelVectorButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                     const wxString &Bitmap,
                                     GuiGeometryCreator::GeometryType GeoType,
                                     wxString ButtonTooltip,
                                     ViewportManagerInterface* pViewportManager,
                                     ViewcontextManagerInterface* pViewcontextManager,
                                     int Flags) :
      VectorEditorButton(pToolbar, pViewer, Bitmap, GeoType, ButtonTooltip,
                         pViewportManager, pViewcontextManager, Flags) {
}

/**
 * Setea el estado del boton.
 * \post Si el boton se esta activando y hay elementos en edicion
 * informa de la situacion
 * @param[in]	State: estado del boton.
 * @return Siempre cambia de estado con exito
 * \todo cerrar el vector en vez de dar error
 */
bool PixelVectorButton::DoSetValue(bool State) {
   if (State && pGeoEditor_) {
      if (pGeoEditor_->IsEditing()) {
         REPORT_ERROR(message_ELEMENT_IN_EDITION);
         return false;
      }
   } else if (State && (pList_->GetEditedCount() > 0)) {
      bool isinlist = pList_->GetElement(
            pVectorElementEditor_->GetCurrentElement())!=NULL;
      if (!(isinlist && pList_->GetEditedCount() == 1
            && pVectorElementEditor_->GetCurrentElement()->IsEdited())) {
         // Remplazar para que cierre el vector en vez de dar error
         REPORT_ERROR(message_ELEMENT_IN_EDITION);
         state_ = false;
         ToggleButton::DoSetValue(false);
         return false;
      }
   }
   return ToggleButton::DoSetValue(State);
}

/**
 * Metodo ejecutado al levantar/soltar el boton izquierdo del mouse.
 * @param[in] Event evento
 */
void PixelVectorButton::DoOnLeftUp(wxMouseEvent &Event) {
   // Si el boton no esta habilitado retorno sin procesar el evento
   if (!IsEnabled()) {
      return;
   }
   double wx, wy;
   pViewer_->GetWorld()->V2WTransform(Event.GetX(), Event.GetY(), wx, wy);
   Coordinates point(wx, wy);

   // verifico que el punto entre en el mundo
   Subset world;
   pViewer_->GetWorld()->GetWorld(world);
   if (!world.IsInside(point)) {
      SHOW_STATUS(message_POINT_OUTSIDE_ALLOWED_AREA_ERROR);
      // No se traduce. Lo hace SHOW_
      return;
   }
   if (pGeoEditor_ && !pGeoEditor_->IsActive()) {
         pGeoEditor_->Start();
   } else if (!pGeoEditor_ && pVectorElementEditor_) {
      pVectorElementEditor_->AddPoint(point);
   }

}

/**
 * Este metodo se ejecuta al recibir el evento OnRightUp del mouse.
 * Cierra el feature, o cierra el vector si el elemento en edicion ya no existe
 * en la lista (porque fue eliminado externamente)
 * \pre El vector debe estar abierto.
 * @param[in]	Event: evento
 */
void PixelVectorButton::DoOnRightUp(wxMouseEvent &Event) {
   // Si el boton no esta habilitado retorno sin procesar el evento
   if (!IsEnabled()) {
      return;
   }
   if (pGeoEditor_)
      return;

   bool isinlist = pList_ != NULL
         && pList_->GetElement(pVectorElementEditor_->GetCurrentElement()) != NULL;
   // Si el elemento esta en la lista, cierra el feature
   if (isinlist) {
      pVectorElementEditor_->CloseFeature();
   }
   // Si el elemento fue eliminado de la lista cierra el vector
   else {
      pVectorElementEditor_->CloseVector();
   }
}

/**
 * Metodo que se ejecuta al recibir el evento InMiddleUp del mouse.
 * Elimina el ultimo punto agregado al feature.
 * @param[in] Event evento
 */
void PixelVectorButton::DoOnMiddleUp(wxMouseEvent &Event) {
   // Si el boton no esta habilitado retorno sin procesar el evento
   if (!IsEnabled()) {
      return;
   }
   if (pGeoEditor_) {
      // Analogo a la logica de vectorelementeditor
      return;
   }
   int lastpoint = pVectorElementEditor_->GetPointCount() - 1;
   if (lastpoint >= 0) {
      pVectorElementEditor_->DeletePoint(lastpoint);
   }
}

/** Indica si se encuentra activo el editor **/
bool PixelVectorButton::IsActive() const {
   if (pGeoEditor_) {
      return pGeoEditor_->IsActive();
   } else {
      return pVectorElementEditor_->IsVectorOpen();
   }
}
}  // namespace suri

