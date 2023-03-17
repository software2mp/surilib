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

#ifndef GPSTOGGLEBUTTON_H_
#define GPSTOGGLEBUTTON_H_

// Includes suri
#include "suri/Vector.h"

// Includes App
#include "VectorEditorButton.h"

// Defines
/** Flag para indicar que hay que agregar un boton 'marcar' (punto) */
#define BUTTON_MARK                 0x00000001
/** Flag para indicar que hay que agregar un boton 'eliminar' (punto) */
#define BUTTON_DELETE               0x00000002
/** Flag para indicar que hay que agregar un boton 'cerrar' (feature) */
#define BUTTON_CLOSE                0x00000004

// Forwards
class VectorElementEditor;

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;
class MarkButton;
class DeleteButton;
class CloseButton;
class DataViewManager;

/** Clase para botones de la herramienta GPS. */
/**
 * Cuando se activa/desactiva el boton carga/saca los botones en NavigationWidget
 * \todo pasar toda la funcionalidad a VectorEditorButton.
 */
class GpsVectorButton : public VectorEditorButton {
public:
   /** ctor */
   GpsVectorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                   VectorElementEditor* pVectorElementEditor,
                   DataViewManager* pDataViewManager = NULL,
                   wxString ButtonTooltip =
                         wxT(tooltip_BUTTON),
                   int Flag = BUTTON_MARK | BUTTON_DELETE | BUTTON_CLOSE);
   /** dtor */
   virtual ~GpsVectorButton();
   /** Setea el estado del boton. Ingresa/Saca boton de NavigationWidget. */
   virtual bool DoSetValue(bool State);

protected:
   /** Elimina los botones del NavigationWidget */
   virtual void RemoveButtonsFromNavigationWidget();

private:
   MarkButton* pMarkButton_; /*! Boton que permite agregar puntos */
   DeleteButton* pDeleteButton_; /*! Boton que elimina el ultimo punto agregado */
   CloseButton* pCloseButton_; /*! Boton que cierra vector */
   /*! informa que botones deberan mostarse en NavigationWidget cuando se active boton */
   int flag_;
   /** administrador de info de la aplicacion **/
   DataViewManager* pDataViewManager_;
};
}  // namespace suri

#endif /*GPSTOGGLEBUTTON_H_*/
