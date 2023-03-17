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

#ifndef DRAWANOTATIONBUTTON_H_
#define DRAWANOTATIONBUTTON_H_

// Includes standard

// Includes Suri
#include "suri/Vector.h"
#include "suri/Coordinates.h"
#include "VectorElementEditor.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Includes OGR
#include "ogrsf_frmts.h"

// Defines
/** Estilo del vector para la anotacion */
#define ANOTATION_STYLE_PREFIX \
   "VECTORSTYLE[1,\"Anotacion_BN\",SYMBOL[\"Suri-symbol\",0,COLOR[0,0,0,0],SIZE[1]],LABEL[\"Suri-label\",1,"
/** Nombre del campo de la anotacion */
#define ANOTATION_STYLE_EXPRESSION "EXPRESSION[\"{" SUR_ANOTATION_FIELD "}\"]"
/** Estilo del tipo de letra para la anotacion */
#define ANOTATION_STYLE_SUFFIX \
   ",COLOR[0,0,0,255],BACKCOLOR[255,255,255,255],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"
/** Estilo de la anotacion, combinacion de los otros estilos */
#define ANOTATION_POINT_1 ANOTATION_STYLE_PREFIX ANOTATION_STYLE_EXPRESSION ANOTATION_STYLE_SUFFIX

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;
class AnotationElement;

/** Clase utilizada para poder ingresar una Anotacion. */
/**
 * Se representa mediante boton (presionable).
 * Una vez activo permite :
 * - agregar un texto de anotacion en una posicion dada.
 * \note Esta clase hereda de ToggleButton ya que se necesita que posea la
 * funcionalidad de ser excluyente con otros botones del mismo grupo.
 */
class AnotationElementEditor : public VectorElementEditor {
   /** Ctor. de Copia. */
   AnotationElementEditor(const AnotationElementEditor &);
public:
   /** Ctor. default. */
   AnotationElementEditor(ViewerWidget *pViewer, int ListGroup,
                      VectorEditor* pVectorEditor,
                      DataViewManager* pDataViewManager = NULL);
   /** Dtor. */
   virtual ~AnotationElementEditor();
   /** Salva el vector. */
   virtual bool CloseVector();
protected:
   /** Configura el punto a dibujar. */
   virtual void SetupElement();
   /** Agrega un punto al vector. */
   virtual bool AddPointToVector(const Coordinates &Point);
   /** Permite configurar el elemento antes de agregarlo a la lista */
   virtual bool ConfigureSavedElement(VectorElement* pVectorElement);
private:
   wxString anotation_; /*! Texto de la anotacion. */
   AnotationElement *pAnotationElement_; /*! Elemento anotacion. */
};
}  // namespace suri

#endif /*DRAWANOTATIONBUTTON_H_*/
