/*! \file wxHtmlTreeEventHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXHTMLTREEEVENTHANDLER_H_
#define WXHTMLTREEEVENTHANDLER_H_

// Includes standard

// Includes Suri
#include "ItemId.h"
#include "HtmlConfigurationData.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

/**
 * Interfaz con los eventos que genera wxSimpleHtmlTreeCtrl.
 * Crear clase que implemente la interfaz con el comportamiento
 * deseado y configurarla en la lista con
 * wxSimpleHtmlTreeCtrl::SetTreeEventHandler().
 *
 * Los eventos soportados son:
 * - DoOnSelectionChanged: se ejecuta cuando se modifica seleccion de items.
 * - DoOnStateChanged: Se encarga de modificar el estado de un elemento.
 * - ValidateNodeNewParent: Permite agregar restricciones al movimiento de nodos
 * - CreateContextMenu y DoOnContextMenu: Permiten configurar el menu
 * que se despliega cuando se hace click derecho sobre un item del arbol
 *
 * Si el arbol se usa para representar otra estructura de datos, existen metodos
 * virtuales que se llaman cuando se mueve un nodo:
 * - DoOnMoveNodeAfter: Mueve un nodo despues de otro.
 * - DoOnMoveNodeBefore: Mueve un nodo antes de otro.
 * - DoOnMoveNodeUnder: Mueve un nodo dentro(al final) de otro.
 * La clase debera actualizar la estructura asociada.
 *
 * Nota sobre funcionamiento
 *
 * Originalmente, se llamaba a TreeEventHandler::OnMoveAfter/Before/Under
 * y si la operacion tenia exito se se modificaba la
 * estructura del arbol desde wxSimpleHtmlTreeCtrl.
 * Esta solucion no contempla que el arbol del modelo se puede
 * modificar desde otros lugares ademas de TreeEventHandler lo
 * que tambien actualizaria el arbol. Esto significa que con el
 * disenio anterior al modificar el arbol del modelo
 * se tenia que bloquear este proceso de actualizacion.
 *
 * Nota: Esto rompe con la interfaz similar a la de wxTreeCtrl.
 */
namespace suri {

class wxHtmlTreeEventHandler {
public:
   wxHtmlTreeEventHandler() {
   }
   virtual ~wxHtmlTreeEventHandler() {
   }
   /** Se ejecuta cuando se modifican los elementos seleccionados */
   virtual void DoOnSelectionChanged()=0;
   /** Modifica el estado del nodo y el de sus hijos. */
   virtual void DoOnStateChanged(const ItemId &Id,
                                 HtmlConfigurationData::State NewState)=0;
   /** Valida si se puede mover nodo Id debajo de ParentId */
   virtual bool ValidateNodeNewParent(const ItemId &Id, const ItemId &ParentId)=0;
   /** Mueve un nodo despues de otro. */
   virtual void DoOnMoveNodeAfter(const ItemId &Id, const ItemId &DestinationId)=0;
   /** Mueve un nodo antes de otro. */
   virtual void DoOnMoveNodeBefore(const ItemId &Id, const ItemId &DestinationId)=0;
   /** Mueve un nodo debajo de otro. */
   virtual void DoOnMoveNodeUnder(const ItemId &Id, const ItemId &DestinationId)=0;
   /** Genera menu que se debera mostrar sobre la lista */
   virtual wxMenu* CreateContextMenu(const ItemId &Id)=0;
   /** Se llama cuando se selecciona item del menu contextual */
   virtual void DoOnContextMenu(wxMenuEvent &Event)=0;
};

}  //namespace

#endif /* WXHTMLTREEEVENTHANDLER_H_ */
