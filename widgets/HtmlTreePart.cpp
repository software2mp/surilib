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

#include "HtmlTreePart.h"

namespace suri {
const std::string HtmlTreePart::HTML_TREE_XRC_NAME = wxT("ID_HTML_TREE_MAIN_PANEL");

HtmlTreePart::HtmlTreePart(LayerList* pLayerList) :
      Part(HTML_TREE_XRC_NAME) {
   pLayerList_ = pLayerList;
}

HtmlTreePart::~HtmlTreePart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool HtmlTreePart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool HtmlTreePart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool HtmlTreePart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void HtmlTreePart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void HtmlTreePart::Update() {
}

/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool HtmlTreePart::ConfigureWidget() {
   Widget *ptree = new FiltredElementListWidget(GetWidget()->GetWindow(), pLayerList_);

   GetWidget()->AddControl(ptree, "ID_HTML_TREE_PANEL");

   return true;
}
}
