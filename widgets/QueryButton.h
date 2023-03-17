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

#ifndef QUERYBUTTON_H_
#define QUERYBUTTON_H_

// Includes standard
// Includes Suri
#include "suri/ToggleButton.h"
#include "suri/messages.h"
#include "resources.h"
#include "Query.h"

// Includes Wx

// Defines

/** namespace suri */
namespace suri {
class ViewerWidget;
struct HotlinkData;

/** Clase con comportamiento comun a herramientas que abren un hotlink. */
/**
 * Boton que abre archivo de hotlink. Usa metodos virtuales para comportamiento
 * particular de contextos 2D y 3D
 * \note hereda de ToggleButton para que se muestre que la opcion activa es la
 *       de Query
 */
class QueryButton : public ToggleButton {
   /** Ctor. de Copia. */
   QueryButton(const QueryButton &QueryButton);

public:
   /** Ctor */
   QueryButton(wxWindow *pToolbar, ViewerWidget *pViewer);
   /** Dtor */
   virtual ~QueryButton() {
   }

   /** Cambia cursor a flecha con signo de interrogacion */
   virtual void DoSetCursor(bool State);
   /** Busca hotlinks y los despliega */
   virtual void DoOnLeftUp(wxMouseEvent &Event);
   /** Retorna el box del elemento activo */
   virtual bool GetQueryBox(int PosX, int PosY, int VectorSize, Subset &QueryBox)=0;
   /** Analiza si el click puede pertenecer a un elemento valido. */
   virtual bool IsValid(int PosX, int PosY)=0;
   /** Abre el hotlink usando el sistema operativo. */
   virtual void DisplayHotlink(const std::vector<HotlinkData> &Hotlinks);

protected:
   ViewerWidget* pViewer_; /*! Viewer */
   Query* pQuery_;
};
}

#endif /* QUERYBUTTON_H_ */
