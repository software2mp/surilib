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

#ifndef LABELTEXT_H_
#define LABELTEXT_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "Widget.h"

// Includes Wx
// Defines
// forwards
class wxStaticText;

namespace suri {

/*!
 * Clase que representa un control grafico que contiene
 * un texto estatico
 */
class LabelText : public suri::Widget {
   /** Constructor copia **/
   LabelText(const LabelText&);

public:
   /** Constructor**/
   LabelText(wxWindow *pViewerToolbar, const std::string& Text);
   /** Destructor **/
   virtual ~LabelText();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Obtiene el texto del control grafico. */
   std::string GetValue() const;
   /** Indica el texto a mostrar por el control grafico. */
   void SetValue(std::string& Value);
   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled() const;

private:
   std::string text_;
   bool enabled_;
   wxStaticText* pTextCtrl_;
};

} /** namespace suri */

#endif /* LABELTEXT_H_ */
