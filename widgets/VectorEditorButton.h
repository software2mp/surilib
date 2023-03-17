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

#ifndef VECTOREDITORBUTTON_H_
#define VECTOREDITORBUTTON_H_

// Includes suri
#include "suri/ToggleButton.h"
#include "suri/Subject.h"
#include "VectorEditionComponentInterface.h"

// Includes app
#include "VectorElementEditor.h"
#include "GeometryElementEditor.h"

/** namespace suri */
namespace suri {

// forwards
class ViewportManagerInterface;
class ViewcontextManagerInterface;

/** Clase que heredan los botones que desean editar vectores. */
/**
 * Clase que se crea para unir la funcionalidad de PixelVectorButton y
 * GPSVectorButton. Sirve para que la clase DrawTool (de donde heredan todas las
 * herramientas de edicion de vectores) use botones genericos.
 * \todo Esta clase tiene que incluir la funcionalidad para agregar puntos,
 *       eliminar puntos y cerrar geometrias, y las clases que heredan definen
 *       como llaman a estos metodos (PixelVectorButton los llama ante eventos
 *       de mouse, GPSVectorButton los llama ante eventos en el widget de
 *       ingreso de coordenadas).
 */
class VectorEditorButton : public ToggleButton,
                              public VectorEditionComponentInterface,
                              public Subject {
public:
   /** Ctor. */
   VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                      VectorElementEditor* pVectorElementEditor, wxString ButtonTooltip,
                      int Flags = BUTTON_NULL);
   /** Ctor. default */
   VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer, const wxString &Bitmap,
                      GuiGeometryCreator::GeometryType GeoType, wxString ButtonTooltip,
                      ViewportManagerInterface* pViewportManager = NULL,
                      ViewcontextManagerInterface* pViewcontextManager = NULL,
                      int Flags = BUTTON_NULL);

   /** Dtor. */
   virtual ~VectorEditorButton();
   /** Retorna el VectorElementEditor que usa el boton */
   virtual VectorElementEditor *GetVectorElementEditor();
   /** Retorna el VectorElementEditor que usa el boton */
   virtual GeometryElementEditor* GetGeometryElementEditor();

   /** Indica si se encuentra activo el editor **/
   virtual bool IsActive() const { return false;}
   /** Metodo que se llama cuando se inicializa la edicion vectorial. **/
   virtual void OnVectorEditionStarted() {
   }
   /**  Metodo que se llama cuando finaliza la edicion vectorial.*/
   virtual void OnVectorEditionFinished() {
   }

protected:
   ViewerWidget* pViewer_; /*! View de donde se obtiene el NavigationWidget */
   /*! VectorElementEditor que se utilza para agregar puntos a los vectores */
   VectorElementEditor* pVectorElementEditor_;
   GeometryElementEditor* pGeoEditor_;

};
}  // namespace suri

#endif /* VECTOREDITORBUTTON_H_ */
