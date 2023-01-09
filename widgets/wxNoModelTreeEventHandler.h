/*! \file wxNoModelTreeEventHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXNOMODELTREEEVENTHANDLER_H_
#define WXNOMODELTREEEVENTHANDLER_H_

// Includes standard

// Includes Suri
#include "wxHtmlTreeEventHandler.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

//Includes suri
class wxSimpleHtmlTreeCtrl;

/**
 * Comportamiento default de wxHtmlTreeEventHandler.
 */
class wxNoModelTreeEventHandler : public wxHtmlTreeEventHandler {
public:
   //! Ctor
   wxNoModelTreeEventHandler(wxSimpleHtmlTreeCtrl* pHtmlTreeCtrl);
   //! Dtor
   ~wxNoModelTreeEventHandler();

   //! Se ejecuta cuando se modifican los elementos seleccionados
   virtual void DoOnSelectionChanged();
   //! Modifica el estado del nodo y el de sus hijos.
   virtual void DoOnStateChanged(const ItemId &Id,
                                 HtmlConfigurationData::State NewState);

   //! Valida si se puede mover nodo Id debajo de ParentId
   virtual bool ValidateNodeNewParent(const ItemId &Id, const ItemId &ParentId);
   //! Mueve un nodo despues de otro.
   virtual void DoOnMoveNodeAfter(const ItemId &Id, const ItemId &DestinationId);
   //! Mueve un nodo antes de otro.
   virtual void DoOnMoveNodeBefore(const ItemId &Id, const ItemId &DestinationId);
   //! Mueve un nodo debajo de otro.
   virtual void DoOnMoveNodeUnder(const ItemId &Id, const ItemId &DestinationId);
   //! Genera menu que se debera mostrar sobre la lista
   virtual wxMenu* CreateContextMenu(const ItemId &Id);
   //! Se llama cuando se selecciona item del menu contextual
   virtual void DoOnContextMenu(wxMenuEvent &Event);

protected:
   wxSimpleHtmlTreeCtrl* pHtmlTreeCtrl_;
};

}  //namespace

#endif /* WXNOMODELTREEEVENTHANDLER_H_ */
