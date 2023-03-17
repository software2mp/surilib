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

#ifndef SNAPPART_H_
#define SNAPPART_H_

// Includes Suri
#include "suri/DialogWidget.h"

namespace suri {

// forwards
class SnapTool;

namespace ui {

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(SnapPartEvent);

/**
 * Clase que permite configurar la herramienta de snapping
 */
class SnapPart : public suri::DialogWidget {
public:
   /** constructor */
   SnapPart();
   /** Destructor */
   virtual ~SnapPart();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Setea una referencia a la herramienta de snapping existente */
   void SetTool(SnapTool* Tool);

private:
   /** Inicializa el choice de modos */
   void InitializeModeValues();
   /** Inicializa el choice de unidades */
   void InitializeUnitValues();
   /** Evento click sobre boton aceptar */
   void OnOkButton(wxCommandEvent &Event);
   /** Evento click sobre boton cancelar */
   void OnCancelButton(wxCommandEvent &Event);
   /** Setea el modo en la herramienta de snapping */
   void SetMode();
   /** Setea la unidad en la herramienta de snapping */
   void SetUnit();
   /** Setea la tolerancia en la herramienta de snapping */
   void SetTolerance();
   /** Conecta los eventos a los controles */
   void ConnectEvents();
   /** Herramienta de snapping */
   SnapTool* pSnapTool_;
   EVENT_OBJECT_PROPERTY(SnapPartEvent);
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(SnapPartEvent);
};

} /** namespace ui */
} /** namespace suri */

#endif /* SNAPPART_H_ */
