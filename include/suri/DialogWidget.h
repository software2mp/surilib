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

#ifndef DIALOGWIDGET_H_
#define DIALOGWIDGET_H_

// Include suri
#include "Widget.h"

// Forwards
class wxDialog;

/** namespace suri */
namespace suri {
/** Widget que crea un dialogo en lugar de un frame */
/**
 *  Esta clase permite crear un wxDialog en lugar de un wxFrame para insertar
 * el panel de la clase derivada. De esta manera y exponiendo un metodo extra
 * se puede tener clases derivadas de Widget que sean modales.
 * \note: da la funcionalidad de Widget pero sobreescribe el metodo
 *        CreateContainingWindow para que se genere dialogo en vez de frame
 */
class DialogWidget : public Widget {
   /** Ctor. de Copia. */
   DialogWidget(const DialogWidget &DialogWidget);

public:
   /** ctor */
   DialogWidget(wxWindow *pParent = NULL);
   /** dtor */
   virtual ~DialogWidget();

   /** Crea la herramienta */
   virtual bool CreateTool(wxTopLevelWindow *pParentFrame = NULL, bool Show = true);
   /** Retorna el frame dentro del cual se encuentra la ventana */
   virtual wxTopLevelWindow *GetFrame() const;
   /** Muestra el dialogo en forma modal */
   int ShowModal(bool Show = true);
   /** Termina el dialogo modal */
   void EndModal(int ReturnCode);
protected:
   /** Crea la ventana que contiene a la ventana de la herramienta */
   virtual wxTopLevelWindow* CreateContainingWindow(wxTopLevelWindow *pParentFrame);
private:
   wxDialog *pParentDialog_; /*! Puntero al parent como dialogo */
};
}  // namespace name
#endif /*DIALOGWIDGET_H_*/
