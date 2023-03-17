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
