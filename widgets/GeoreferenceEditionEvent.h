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

#include "suri/ViewportEvent.h"
#include "suri/GeoreferenceTask.h"

#ifndef GEOREFERENCEEDITIONEVENT_H_
#define GEOREFERENCEEDITIONEVENT_H_

namespace suri {

class ViewportManagerInterface;
class ViewcontextManagerInterface;
class GeoreferenceTask;

/**
 * Clase para manejar los eventos para la edicion y toma de puntos.
 */
class GeoreferenceEditionEvent : public ViewportEvent {
public:
   /**
    * Ctor.
    */
   GeoreferenceEditionEvent(ViewportManagerInterface* pViewportManager,
                            ViewcontextManagerInterface* pViewcontextManager,
                            GeoreferenceTask* pGeoreferenceTask, bool Editing = false);

   /**
    * Dtor.
    */
   virtual ~GeoreferenceEditionEvent();

   /**
    * Limpia el estado de la herramienta, aborta edicion.
    */
   virtual void Reset();

   /**
    * Se ejecuta cuando el usuario hace click sobre el visualizador.
    */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse.
    */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent& Event);

   /**
    * Inicia la edicion de una geometria.
    */
   virtual void OnLeftDoubleClick(wxMouseEvent& Event);

   /**
    * Al iniciar drag se busca punto en geometrias en edicion.
    */
   virtual void OnStartDrag(const Coordinates& InitialCoordinate);

   /**
    * Al finalizar drag se actualiza geometria en vector.
    */
   virtual void OnEndDrag(const Coordinates& InitialCoordinate,
                          const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

   /**
    * Al realizar drag se actualiza geoemtria en edicion.
    */
   virtual void OnDrag(const Coordinates& InitialCoordinate,
                       const Coordinates& CurrentCoordinate, wxMouseEvent& Event);

protected:
   bool validDrag_;  // Indica si el drag actual comenzo en el punto seleccionado.
   GeoreferenceTask* pGeoreferenceTask_;  // Task encargado de administrar la edicion.

   /**
    * Solo retorna true para los eventos que ocurren en visualizador de edicion.
    */
   virtual bool CanProcessEvent(wxEvent& Event) const;

private:
   double dragXHolder_;
   double dragYHolder_;
   bool editing_;
};

}  // namespace suri

#endif  // GEOREFERENCEEDITIONEVENT_H_
