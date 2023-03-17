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

#ifndef LIBRARYITEMLISTNOTIFICATOR_H_
#define LIBRARYITEMLISTNOTIFICATOR_H_

// Includes Estandar
// Includes Suri
#include "suri/LibraryItem.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class FullFledgeLibraryEditorWidget;
class LibraryItemListViewWidget;

/*!
 * Clase encargada de notificar cambios y eventos ocurridos en el editor fullfledge
 * y la lista que lo contiene. Esta clase se encarga de encapsular la dependencia bidireccional
 * entre el editor y la lista.
 */
class LibraryItemListNotificator {
   /** ctor copia **/
   LibraryItemListNotificator(const LibraryItemListNotificator&);

public:
   /** Constructor **/
   LibraryItemListNotificator(FullFledgeLibraryEditorWidget* pFullFledge,
                              LibraryItemListViewWidget* pListView = NULL);
   /** Destructor **/
   virtual ~LibraryItemListNotificator();
   /**
    * Notifica que se ha cambiado la seleccion en la lista
    * @param pItem item que se ha seleccionado
    */
   void NotifyLibraryItemSelectionChanged(const LibraryItem* pItem);
   /** Configura la lista sobre la cual se notificara la seleccion **/
   void SetLibraryItemList(LibraryItemListViewWidget* pListView);

   /** Indica cual es el list view sobre el cual capturar las notificaciones.
    *  Solo se configura una vez (si != null no se tiene en cuenta) **/
   bool SetListViewWidget(LibraryItemListViewWidget* pListView);
   /** Indica que se ha agrega un nuevo item y esto debe notificarse a la lista para que
    *  actualice su contenido
    * @param pItem item agregado
    */
   void NotifyItemAdded(const SuriObject::UuidType& OriginId, const LibraryItem* pItem);
   /** Indica que se ha eliminado un item y esto debe notificarse a la lista para que
    *  actualice su contenido
    */
   void NotifyActiveItemRemoved();

private:
   FullFledgeLibraryEditorWidget* pFullFledge_;
   LibraryItemListViewWidget* pListView_;
};

} /** namespace suri */

#endif /* LIBRARYITEMLISTNOTIFICATOR_H_ */
