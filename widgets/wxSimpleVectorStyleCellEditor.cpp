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

// Includes Estandar
#include <string>

// Includes Suri
#include "wxSimpleVectorStyleCellEditor.h"
#include "SimpleVectorStylePart.h"
#include "suri/VectorStyle.h"
#include "suri/PartContainerWidget.h"

// Includes Wx
// Defines
// forwards

namespace suri {

wxSimpleVectorStyleCellEditor::wxSimpleVectorStyleCellEditor() :
      pStyleEditor_(NULL) {
}

wxSimpleVectorStyleCellEditor::~wxSimpleVectorStyleCellEditor() {
}

/** Metodo que inicia la edicion del atributo de la celda **/
bool wxSimpleVectorStyleCellEditor::BeginEdition() {
   /** Por ahora no se soporte utilizar la biblioteca.
    *  Necsito generar una instancia aca porque partcontainer la destruye **/
   pStyleEditor_ = new ui::SimpleVectorStylePart(&currentValue_);
   pStyleEditor_->SetVectorStyleWkt(currentValue_);
   PartContainerWidget* pcontainer = new PartContainerWidget(
         pStyleEditor_, _(pStyleEditor_->GetWindowTitle()));
   if (pcontainer->ShowModal(true) == wxID_OK) {
      return true;
   }
   return false;
}

/** Obtiene el valor resultante de la edicion de la grilla */
std::string wxSimpleVectorStyleCellEditor::GetValue() {
   return currentValue_;
}

/** Configura el valor a editar **/
void wxSimpleVectorStyleCellEditor::SetValue(const std::string& Value) {
   std::string wkt = Value;
   if (Value.empty()) {
      wkt = "VECTORSTYLE[3,\"Poligono rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]]]";
   }
   currentValue_ = Value;
}
} /** namespace suri */
