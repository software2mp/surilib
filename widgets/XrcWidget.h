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

#ifndef XRCWIDGET_H_
#define XRCWIDGET_H_

// Includes standard

// Includes Suri
#include "suri/Widget.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
class Part;

/** Widget que se configura con XRC. Da una interfaz generica para acceder a controles */
/**
 * Configura la interfaz del widget a partir de un XRC. Da una interfaz
 * para el manejo de los valores en los controles que contiene(permite abstraerse de la
 * interfaz de wx).
 * La pantalla que despliegue XrcWidget cambiara en cada instancia,
 * dependiendo del XRC que se le pase.
 */
class XrcWidget : public Widget {
public:
   /** Ctor */
   XrcWidget(Part *pParentPart, const std::string &XrcId,
             const std::string &WidgetTitle);
   /** Dtor */
   virtual ~XrcWidget();

   /** Crea la ventana usando el xrc */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
protected:
private:
   Part *pParentPart_; /*! Part que se usa para configurar el Widget despues de crearlo */
   std::string xrcId_; /*! Nombre del part que quiero crear. */
};

/** Retorna el contenido del control como string */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, std::string &Value);
/** Retorna el contenido del control como entero */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, int &Value);
/** Retorna el contenido del control como double */
bool GetCtrlValue(Widget* pWidget, const std::string &ControlName, double &Value);

/** Modifica el contenido del control con string */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName,
                  const std::string &NewValue);
/** Modifica el contenido del control con entero */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName, int NewValue);
/** Modifica el contenido del control con double */
bool SetCtrlValue(Widget* pWidget, const std::string &ControlName, double NewValue);

}  // namespace suri

#endif /* XRCWIDGET_H_ */
