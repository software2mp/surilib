/*! \file LibraryItemEditorNotificator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


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
