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
#include <vector>
#include <string>

// Includes Suri
#include "LibraryItemEditorNotificator.h"

// Includes Wx
// Defines
// forwards

namespace suri {

LibraryItemEditorNotificator::LibraryItemEditorNotificator(
      LibraryItemEditorPart* pEditor, UniversalGraphicalComponentPartInterface* pCgu):
      pEditor_(pEditor), pCgu_(pCgu) {
}

LibraryItemEditorNotificator::~LibraryItemEditorNotificator() {
}

/** Notifica que se selecciono un nuevo item en el CGU
 *  @param pItem item seleccionado. Si el item pasado por parametro es NULL
 *  significa que no se encuentra seleccionado ningun item
 *  @param FirstSelection booleano que indica si se llama (indica que es la
 *  seleccion inicial y no se debe marcar el part como modificado)
 *  @param Modifiable boolean que indica si el item es modificable
 *  **/
void LibraryItemEditorNotificator::NotifyItemSelectionFromCgu(
      const LibraryItem* pItem, bool FirstSelection, bool Modifiable) {
   if (pEditor_) {
      pEditor_->SetActiveItem(pItem, FirstSelection, Modifiable);
   }
}

/** Notifica al cgu que el editor ha cambiado informacion del item,
 *  notificando ademas la validez de la informacion*/
void LibraryItemEditorNotificator::NotifyItemInformationChangedFromEditor(
      bool IsValidData) {
   if (pCgu_) {
      pCgu_->ItemInformationChanged(IsValidData);
   }
}

/** Indica la referencia al editor in situ*/
void LibraryItemEditorNotificator::SetLibraryItemEditorDebugPart(
      LibraryItemEditorPart* pEditor) {
   if (!pEditor_)
      pEditor_ = pEditor;
}

/** Indica la referencia al cgu*/
void LibraryItemEditorNotificator::SetUniversalGraphicalComponentPart(
      UniversalGraphicalComponentPartInterface* pCgu) {
   if (!pCgu_)
      pCgu_ = pCgu;
}

/** Obtiene los atributos modificados desde el editor
 *  @return vector con atributos
 *  @return NULL en caso de que no se haya modificado ningun atributo */
std::vector<LibraryItemAttribute*>*
LibraryItemEditorNotificator::GetAttributesModifiedFromEditor() {
   return (pEditor_)? pEditor_->CreateAttributesFromFields() : NULL;
}

/** Obtiene los atributos eliminados. */
std::vector<LibraryItemAttribute*>*
LibraryItemEditorNotificator::GetAttributesRemovedFromEditor() {
   return (pEditor_ != NULL) ? pEditor_->GetAttributesToRemove() : NULL;
}

/** Notifica que se selecciono un item desde el editor, indicandole el
 *  atributo principal. En caso de que no exista el item en el CGU
 *  Es resposabilidad del CGU tratarlo**/
void LibraryItemEditorNotificator::NotifyItemSelectionFromEditor(
      const std::string& PrincipalAttribute) {
   if (pCgu_)
      pCgu_->SelectItemByPrincipal(PrincipalAttribute);
}

/** Metodo que consulta la validez de informacion del part del editor. **/
bool LibraryItemEditorNotificator::AskEditorForValidData() const {
   return (pEditor_)? pEditor_->HasValidData() : false;
}

/** Metodo que consulta el estado del check del cgu **/
bool LibraryItemEditorNotificator::AskCguForCheckState() const {
   return (pCgu_)? pCgu_->GetLastCheckState() : false;
}
} /** namespace suri */
