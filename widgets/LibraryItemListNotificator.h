/*! \file LibraryItemListNotificator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
