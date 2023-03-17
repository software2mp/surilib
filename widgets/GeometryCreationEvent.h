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

#ifndef GEOMETRYCREATIONEVENT_H_
#define GEOMETRYCREATIONEVENT_H_

// Includes suri
#include "suri/ViewportEvent.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "GuiGeometryCreator.h"

namespace suri {

// fordwards
class VectorEditionTask;
class SnapTool;

/**
 * Se encarga de administrar los eventos de ususario al crear geometria.
 */
class GeometryCreationEvent : public ViewportEvent {
public:
   /** Ctor */
   GeometryCreationEvent(ViewportManagerInterface *pViewportManager,
                           ViewcontextManagerInterface *pViewcontextManager,
                           VectorEditionTask* pVectorEditionTask,
                           bool SimpleFeatureCreation = false);
   /** Dtor */
   virtual ~GeometryCreationEvent();

   /** Limpia el estado de la herramienta, aborta edicion */
   virtual void Reset();
   /** Se ejecuta cuando el usuario hace click sobre el visualizador. */
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario realiza un desplazamiento del mouse */
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);
   /** Al finalizar drag se actualiza geometria en vector */
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);

private:
   /** Solo retorna true para los eventos que ocurren en visualizador de creacion */
   virtual bool CanProcessEvent(wxEvent &Event) const;
   void SnapPoint(SnapTool* pSnapTool, Coordinates &ViewportPosition);
   /** Task encargado de administrar la creacion. */
   VectorEditionTask* pVectorEditionTask_;
   bool simpleFeatureCreation_;
   bool featureCreated_;
};

} /* namespace suri */
#endif /* GEOMETRYCREATIONEVENT_H_ */
