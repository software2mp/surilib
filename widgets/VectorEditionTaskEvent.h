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

#ifndef VECTOREDITIONTASKEVENT_H_
#define VECTOREDITIONTASKEVENT_H_

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/VectorEditionTask.h"
#include "FeatureSelectionObserver.h"

namespace suri {

class VectorEditionTaskEvent : public ViewportEvent,
                         public suri::FeatureSelectionObserver {
public:
   /** Ctor */
   VectorEditionTaskEvent(ViewportManagerInterface *pViewportManager,
                              ViewcontextManagerInterface *pViewcontextManager,
                              VectorEditionTask* pVectorEditionTask);
   /** Dtor */
   virtual ~VectorEditionTaskEvent();

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset();

   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Al pintar se dibuja geometria en edicion */
   virtual void OnPaint(wxPaintEvent&Event, wxDC &Dc);
   /** Indica que quiere recibir evento OnPaint */
   virtual PaintEventFlags GetPaintFlags() const;
   /** Indica que no se quieren recibir eventos de mouse */
   virtual MouseEventFlags GetMouseFlags() const;

   /** Solo retorna true para los eventos que ocurren en visualizador de edicion */
   virtual bool CanProcessEvent(wxEvent &Event) const;

   /** Metodo que es llamado cuando se actualiza la seleccion. */
   virtual void SelectionUpdated();

   /** Para saber si al ingresar a la edición, se agregó algun punto o se arrepintió. */
   virtual void SetClosedWithoutEdition(bool value);
   virtual bool GetClosedWithoutEdition();
private:
   /** Task encargado de administrar la edicion. */
   VectorEditionTask* pVectorEditionTask_;
   bool closedBeforeSetAPoint_;
};

} /* namespace suri */
#endif /* VECTOREDITIONTASKEVENT_H_ */
