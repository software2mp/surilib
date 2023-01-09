/*! \file LibraryItemListNotificator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



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
