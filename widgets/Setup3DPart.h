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

#ifndef SETUP3DPART_H_
#define SETUP3DPART_H_

#include "suri/Part.h"

namespace suri {

DECLARE_EVENT_OBJECT(Setup3DPartEvent);

/** Clase encargada de administrar la configuracion de las opciones de 3D en la aplicacion **/
class Setup3DPart : public Part {
public:
   /** Declara friend la clase TerrainPropertiesPartEvent */
   FRIEND_EVENT_OBJECT(Setup3DPartEvent);
   /* Ctor. */
   Setup3DPart();
   /* Destructor. */
   virtual ~Setup3DPart();
   /** Crea el part (el interior de la ventana)*/
   virtual bool CreateToolWindow();
   /** Asigna el icono para el part.  */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Actualiza el estado de la parte */
   virtual void Update();
private:
   /** Evento de movimiento de sliders */
   void OnSliderScroll(wxScrollEvent &Event);
   int iquality_;
   int iexageration_;
   int icamera_;
   EVENT_OBJECT_PROPERTY(Setup3DPartEvent); /*! Objeto que conecta */
};

} /* namespace suri */

#endif /* SETUP3DPART_H_ */
