/** \file HtmlTreeEventHandler.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLTREEEVENTHANDLER_H_
#define HTMLTREEEVENTHANDLER_H_

// Includes Standard
#include <list>

// Includes suri
#include "HtmlListEventHandlerInterface.h"

namespace suri {

// fordwards
class GuiTreeModel;

/**
 * Aplica los movimientos realizados por el usuario en control grafico, al
 * arbol en modelo
 */
class HtmlTreeEventHandler : public HtmlListEventHandlerInterface {
public:
   /** Ctor */
   explicit HtmlTreeEventHandler(GuiTreeModel* pGuiTreeModel);
   /** Dtor */
   virtual ~HtmlTreeEventHandler();

   /** Se ejecuta cuando se mueve un item despues de otro */
   virtual bool DoOnMoveAfter(const ItemId &Id, const ItemId &DestinationId);
   /** Se ejecuta cuando se mueve un item antes que otro */
   virtual bool DoOnMoveBefore(const ItemId &Id, const ItemId &DestinationId);
   /** Se ejecuta cuando se mueve un item despues del ultimo elemento */
   virtual bool DoOnMoveEnd(const ItemId &Id);
   /** Se ejecuta cuando se modifica la seleccion */
   virtual void DoOnSelectionChanged(std::list<ItemId> &SelectedItems);

private:
   /** Estructura que representa modelo de arbol */
   GuiTreeModel* pGuiTreeModel_;
};

} /* namespace suri */

#endif /* HTMLTREEEVENTHANDLER_H_ */
