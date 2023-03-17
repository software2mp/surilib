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

#ifndef NAVIGATIONINPUTWIDGET_H_
#define NAVIGATIONINPUTWIDGET_H_

// Includes suri
#include "suri/Widget.h"
#include "suri/Coordinates.h"
#include "suri/CoordinatesNavigationInputInterface.h"

/** namespace suri */
namespace suri {
class ViewerWidget;

/** Interfaz que deben cumplir las herramientas que ingresan coordenadas */
class NavigationInputWidget : public Widget, public CoordinatesNavigationInputInterface {
public:
   /** Ctor */
   NavigationInputWidget() {
   }
   /** Dtor */
   virtual ~NavigationInputWidget() {
   }

   /** Creacion de la ventana */
   virtual bool CreateToolWindow()=0;
   /** Titulo de la ventana (en caso de pParent!=NULL */
   virtual const wxString GetWindowTitle() const=0;
   /** Retorna la ventana asociada a la herramienta */
   virtual wxWindow *GetToolWindow() const {
      return pToolWindow_;
   }
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &Coordinate)=0;
   /** Actualiza el parent */
   virtual void SetParent(wxWindow *pParent) {
      pParentWindow_ = pParent;
   }
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool SetCoordinate(Coordinates Coordinate)=0;
   virtual Widget* GetInputWidget() {
      return this;
   }
};
}

#endif /* NAVIGATIONINPUTWIDGET_H_ */
