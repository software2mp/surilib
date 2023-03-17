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

#ifndef DRAWLINEBUTTON_H_
#define DRAWLINEBUTTON_H_

// Includes standard

// Includes Suri
#include "VectorElementEditor.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Clase para crear vectores de tipo linea. */
/**
 * Se representa mediante boton (presionable).
 * Una vez activo permite :
 * - agregar un puntos al vector de tipo linea en curso.
 * - cerrar y salvar el vector en curso.
 * \note Esta clase hereda de VectorElementEditor ya que se necesita que posea
 * la funcionalidad de ser excluyente con otros botones del mismo grupo y
 * permita agregar un punto al vector, en este caso de tipo linea.
 */
class LineElementEditor : public VectorElementEditor {
   /** Ctor. de Copia. */
   LineElementEditor(const LineElementEditor &LineElementEditor);

public:
   /** Ctor. default. */
   LineElementEditor(ViewerWidget *pViewer, int ListGroup, VectorEditor* pVectorEditor,
         DataViewManager* pDataViewManager = NULL,
         std::string CategoryName = DRAW_TABLE_NAME);
   /** Dtor. */
   virtual ~LineElementEditor();
protected:
   /** configura la linea a dibujar. */
   virtual void SetupElement();
   /** Agrega un punto a la linea */
   virtual bool AddPointToVector(const Coordinates &Point);
};
}  // namespace suri

#endif /*DRAWLINEBUTTON_H_*/
