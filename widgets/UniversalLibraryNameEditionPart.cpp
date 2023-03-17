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

#include "UniversalLibraryNameEditionPart.h"

namespace suri {
const std::string UniversalLibraryNameEditionPart::LIBRARY_NAME_EDITION_XRC_NAME =
      wxT("ID_LIBRARY_NAME_EDITION_PANEL");

UniversalLibraryNameEditionPart::UniversalLibraryNameEditionPart(int flags) :
      Part(LIBRARY_NAME_EDITION_XRC_NAME) {
   representationflags_ = flags;
}

UniversalLibraryNameEditionPart::~UniversalLibraryNameEditionPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool UniversalLibraryNameEditionPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool UniversalLibraryNameEditionPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool UniversalLibraryNameEditionPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void UniversalLibraryNameEditionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void UniversalLibraryNameEditionPart::Update() {
}

/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool UniversalLibraryNameEditionPart::ConfigureWidget() {
   if ((representationflags_ & SHOW_NAME) == 0) {
      pWidget_->Hide("ID_NAME_EDITION_TAG");
   }

   if ((representationflags_ & SHOW_DELETE_BUTTON) == 0) {
      pWidget_->Hide("ID_DELETE_ITEM_BUTTON");
   }

   if ((representationflags_ & SHOW_FAVOURITES) == 0) {
      pWidget_->Hide("ID_SHOW_FAVORITES_CHECK");
   }

   if ((representationflags_ & READ_ONLY) != 0) {
      pWidget_->Disable("ID_SHOW_FAVORITES_CHECK");
      pWidget_->Disable("ID_NAME_EDITION_COMBO");
   }

   return true;
}
}
