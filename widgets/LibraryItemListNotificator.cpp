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

// Includes Suri
#include "LibraryItemListNotificator.h"
#include "suri/FullFledgeLibraryEditorWidget.h"
#include "LibraryItemListViewWidget.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param pFullFledge instancia correspondiente al editor full fledge
 * @param pListView instancia de la lista que contiene el editor full fledge
 */
LibraryItemListNotificator::LibraryItemListNotificator(
      FullFledgeLibraryEditorWidget* pFullFledge,
      LibraryItemListViewWidget* pListView): pFullFledge_(pFullFledge), pListView_(pListView) {
}

/** Destructor **/
LibraryItemListNotificator::~LibraryItemListNotificator() {
}

/**
 * Notifica que se ha cambiado la seleccion en la lista
 * @param pItem item que se ha seleccionado
 */
void LibraryItemListNotificator::NotifyLibraryItemSelectionChanged(
      const LibraryItem* pItem) {
   if (pFullFledge_)
      pFullFledge_->UpdateEditor(pItem);
}

/** Configura la lista sobre la cual se notificara la seleccion **/
void LibraryItemListNotificator::SetLibraryItemList(
      LibraryItemListViewWidget* pListView) {
   pListView_ = pListView;
}

/**
 *
 * @param pListView
 * @return true en caso de configurar correctamente el listview
 * @return false  en caso contrario
 */
bool LibraryItemListNotificator::SetListViewWidget(LibraryItemListViewWidget* pListView) {
   if (pListView_ != NULL)
      return false;
   pListView_ = pListView;
   return true;
}

/** Indica que se ha agrega un nuevo item y esto debe notificarse a la lista para que
 *  actualice su contenido
 *  @param OriginId id del origen sobre el cual se agrega el item
 *  @param pItem item agregado
 */
void LibraryItemListNotificator::NotifyItemAdded(const SuriObject::UuidType& OriginId,
                                                 const LibraryItem* pItem) {
   pListView_->AddItem(OriginId, pItem);
}

/** Indica que se ha eliminado un item y esto debe notificarse a la lista para que
 *  actualice su contenido
 */
void LibraryItemListNotificator::NotifyActiveItemRemoved() {
   pListView_->RemoveActiveItem();
}

} /** namespace suri */
