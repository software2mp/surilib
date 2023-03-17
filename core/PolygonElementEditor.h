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

#ifndef DRAWPOLYGONBUTTON_H_
#define DRAWPOLYGONBUTTON_H_

// Includes standard

// Includes Suri

// Includes Wx

// Includes App
#include "VectorElementEditor.h"

// Defines

/** namespace suri */
namespace suri {
/** Clase para crear vectores de tipo poligono */
/**
 * Se representa mediante boton (presionable).
 * Una vez activo permite :
 * - agregar un puntos al vector de tipo poligono en curso.
 * - cerrar y salvar el vector en curso.
 * \note Esta clase hereda de VectorElementEditor ya que se necesita que posea
 *       la funcionalidad de ser excluyente con otros botones del mismo grupo
 *       y permita agregar un punto al vector, en este caso de tipo poligono.
 */
class PolygonElementEditor : public VectorElementEditor {
   /** Ctor. de Copia. */
   PolygonElementEditor(const PolygonElementEditor &PolygonElementEditor);

public:
   /** Ctor. default */
   PolygonElementEditor(ViewerWidget *pViewer, int ListGroup,
                        VectorEditor* pVectorEditor,
                        DataViewManager* pDataViewManager = NULL,
                        std::string CategoryName = DRAW_TABLE_NAME);
   /** Dtor. */
   virtual ~PolygonElementEditor();
protected:
   /** configura el poligono a dibujar. */
   virtual void SetupElement();
   /** Agrega un punto al vector. */
   virtual bool AddPointToVector(const Coordinates &Point);
};
}  // namespace suri

#endif /*DRAWPOLYGONBUTTON_H_*/
