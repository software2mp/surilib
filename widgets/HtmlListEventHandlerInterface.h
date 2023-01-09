/** \file HtmlListEventHandlerInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef HTMLLISTEVENTHANDLERINTERFACE_H_
#define HTMLLISTEVENTHANDLERINTERFACE_H_

// Includes standard
#include <list>

// Includes suri
#include "ItemId.h"

namespace suri {

/**
 * Eventos generados por wxDecoratedHtmlListCtrl a pertir de las acciones del
 * usuario.
 */
class HtmlListEventHandlerInterface {
public:
   /** Dtor */
   virtual ~HtmlListEventHandlerInterface() {}
   /** Se ejecuta cuando se mueve un item despues de otro */
   virtual bool DoOnMoveAfter(const ItemId &Id, const ItemId &DestinationId) = 0;
   /** Se ejecuta cuando se mueve un item antes que otro */
   virtual bool DoOnMoveBefore(const ItemId &Id, const ItemId &DestinationId) = 0;
   /** Se ejecuta cuando se mueve un item despues del ultimo elemento */
   virtual bool DoOnMoveEnd(const ItemId &Id) = 0;
   /** Se ejecuta cuando se modifica la seleccion */
   virtual void DoOnSelectionChanged(std::list<ItemId> &SelectedItems) = 0;
};

}  /* namespace suri */

#endif /* HTMLLISTEVENTHANDLERINTERFACE_H_ */
