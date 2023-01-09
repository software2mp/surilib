/*! \file wxHtmlTreeEventHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "wxNoModelTreeEventHandler.h"
#include "wxSimpleHtmlTreeCtrl.h"

// Includes standard

// Includes Suri
#include "logmacros.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

//! Ctor
wxNoModelTreeEventHandler::wxNoModelTreeEventHandler(
      wxSimpleHtmlTreeCtrl* pHtmlTreeCtrl) {
   pHtmlTreeCtrl_ = pHtmlTreeCtrl;
}

//! Dtor
wxNoModelTreeEventHandler::~wxNoModelTreeEventHandler() {

}

/**
 * Se ejecuta cuando se modifica la seleccion de elementos
 */
void wxNoModelTreeEventHandler::DoOnSelectionChanged() {
}

/**
 * Se encarga de modificar el estado de un item y el de sus hijos
 * @param[in] Id del item que quiero modificar
 * @param[in] NewState nuevo estado del item que quiero modificar
 */
void wxNoModelTreeEventHandler::DoOnStateChanged(
      const ItemId &Id, HtmlConfigurationData::State NewState) {
   pHtmlTreeCtrl_->root_->UpdateState(Id, NewState);
}

/**
 * Valida si se puede mover nodo Id debajo de DestinationId.
 * Se puede redefinir para hacer mas restrictivo el movimiento de nodos dentro
 * del arbol.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] ParentId id del nuevo nodo padre
 * @return resultado de la validacion
 */
bool wxNoModelTreeEventHandler::ValidateNodeNewParent(const ItemId &Id,
                                                      const ItemId &ParentId) {
   return true;
}

/**
 * Mueve un nodo despues de otro.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del destino
 */
void wxNoModelTreeEventHandler::DoOnMoveNodeAfter(const ItemId &Id,
                                                  const ItemId &DestinationId) {
   TreeNode* pnode = pHtmlTreeCtrl_->root_->RemoveNode(Id);
   pHtmlTreeCtrl_->root_->AddNodeAfter(DestinationId, pnode);
   pHtmlTreeCtrl_->dirty_ = true;
}

/**
 * Mueve un nodo antes de otro.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del destino
 */
void wxNoModelTreeEventHandler::DoOnMoveNodeBefore(const ItemId &Id,
                                                   const ItemId &DestinationId) {
   TreeNode* pnode = pHtmlTreeCtrl_->root_->RemoveNode(Id);
   pHtmlTreeCtrl_->root_->AddNodeBefore(DestinationId, pnode);
   pHtmlTreeCtrl_->dirty_ = true;
}

/**
 * Mueve un nodo debajo de otro.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del destino
 */
void wxNoModelTreeEventHandler::DoOnMoveNodeUnder(const ItemId &Id,
                                                  const ItemId &DestinationId) {
   TreeNode* pnode = pHtmlTreeCtrl_->root_->RemoveNode(Id);
   pHtmlTreeCtrl_->root_->AppendNode(DestinationId, pnode);
   pHtmlTreeCtrl_->dirty_ = true;
}

/**
 * Retorna el menu que se debe mostrar sobre la lista
 * \attention el menu debe ser eliminado por la clase que invoco el metodo
 * @param[in] ItemId id del item seleccionado
 * @return menu creado
 */
wxMenu* wxNoModelTreeEventHandler::CreateContextMenu(const ItemId &Id) {
   return NULL;
}

/**
 * Se llama cuando se selecciona item del menu contextual
 * @param[in] Event evento del menu
 */
void wxNoModelTreeEventHandler::DoOnContextMenu(wxMenuEvent &Event) {
   REPORT_DEBUG("Debug: Logica de menu contextual no implementada");
}

}
