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

#include "VectorEditorButton.h"
#include "GeometryElementEditor.h"

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in] pToolbar lugar donde se agrega el boton
 * @param[in] pViewer viewer donde se editan los botones
 * @param[in] Bitmap icono de la herramienta
 * @param[in] pVectorElementEditor clase que se usa para editar el vector
 * @param[in] ButtonTooltip tooltip del boton
 * @param[in] Flags Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * @return instancia de la calse VectorEditorButton
 */
VectorEditorButton::VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                       const wxString &Bitmap,
                                       VectorElementEditor* pVectorElementEditor,
                                       wxString ButtonTooltip, int Flags) :
      ToggleButton(pToolbar, Bitmap, ButtonTooltip, Flags), pViewer_(pViewer),
      pVectorElementEditor_(pVectorElementEditor), pGeoEditor_(NULL) {
   windowTitle_ = ButtonTooltip;
}

/**
 * Constructor.
 * @param[in] pToolbar lugar donde se agrega el boton
 * @param[in] pViewer viewer donde se editan los botones
 * @param[in] Bitmap icono de la herramienta
 * @param[in] GeoType tipo de geometria que edita el boton
 * @param[in] ButtonTooltip tooltip del boton
 * @param[in] pViewportManager
 * @param[in] pViewcontextManager
 * @param[in] Flags Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * @return instancia de la calse VectorEditorButton
 */
VectorEditorButton::VectorEditorButton(wxWindow *pToolbar, ViewerWidget *pViewer,
                                       const wxString &Bitmap,
                                       GuiGeometryCreator::GeometryType GeoType,
                                       wxString ButtonTooltip,
                                       ViewportManagerInterface* pViewportManager,
                                       ViewcontextManagerInterface* pViewcontextManager,
                                       int Flags) :
      ToggleButton(pToolbar, Bitmap, ButtonTooltip, Flags), pViewer_(pViewer),
      pVectorElementEditor_(NULL),
      pGeoEditor_(
            new GeometryElementEditor(GeoType, pViewer, pViewportManager,
                                      pViewcontextManager)) {
   windowTitle_ = ButtonTooltip;
}

/** Destructor */
VectorEditorButton::~VectorEditorButton() {
   delete pGeoEditor_;
}

/**
 * Devuelve el VectorElementEditor del boton.
 * @return pVectorElementEditor
 */
VectorElementEditor *VectorEditorButton::GetVectorElementEditor() {
   return pVectorElementEditor_;
}

/** Retorna el VectorElementEditor que usa el boton */
GeometryElementEditor* VectorEditorButton::GetGeometryElementEditor() {
   return pGeoEditor_;
}
}  // namespace suri

