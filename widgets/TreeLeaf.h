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

#ifndef TREELEAF_H_
#define TREELEAF_H_

// Includes standard

// Includes Suri
#include "TreeNode.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
class TreeLeaf : public TreeNode {
public:
   /** Ctor */
   TreeLeaf(HtmlConfigurationData* pHtmlData, long VisualizationFlags = SimpleNode);
   /** Dtor */
   ~TreeLeaf();

// --------------- Metodos de interaccion con lista ---------------

   /** Retorna la cantidad de nodos visibles debajo del nodo. */
   virtual int GetVisibleNodeCount();
   /** Actualiza el html de todos los items visibles; */
   virtual void UpdateHtmlVectorWithVisibleItems(
         int WindowWidth, int &VectorIndex, std::vector<DecoratedHtmlItem> &HtmlVector);
   /** Actualiza el estado de activacion de nodos internos con el valor de las hojas */
   virtual HtmlConfigurationData::State UpdateStateUsingLeafs();
   /** Actualiza el estado de activacion del nodo y de todos sus hijos */
   virtual void UpdateState(const ItemId &NodeId,
                            HtmlConfigurationData::State NewState);
   /** Configura id del nodo y actualiza el de todos los subnodos */
   virtual void UpdateNodeId(const TreeNodeId &Id);

// --------------- Metodos de busqueda/edicion por id ---------------

   /** Busca el nodo con id 'NodeId' y retorna el primer hijo. */
   virtual ItemId GetFirstChild(const ItemId &NodeId);
   /** Busca el nodo con id 'NodeId' y retorna el siguiente hijo del mismo padre. */
   virtual ItemId GetNextSibling(const ItemId &NodeId);
   /** Busca el nodo con id 'NodeId' y retorna el padre. */
   virtual ItemId GetParent(const ItemId &NodeId);
   /** Busca el nodo con id 'NodeId' y agrega 'pNode' a continuacion. */
   virtual ItemId AddNodeAfter(const ItemId &NodeId, TreeNode* pNode);
   /** Busca el nodo con id 'NodeId' y agrega 'pNode' antes del nodo encontrado. */
   virtual ItemId AddNodeBefore(const ItemId &NodeId, TreeNode* pHtmlNode);
   /** Busca el nodo con id 'NodeId' y agrega 'pNode' antes o despues. */
   virtual ItemId AddNode(const ItemId &NodeId, TreeNode* pNode, bool After);
   /** Busca el nodo con id 'NodeId'. Agrega 'pNode' como su ultimo hijo. */
   virtual ItemId AppendNode(const ItemId &NodeId, TreeNode* pHtmlNode);
   /** Busca el nodo con id 'NodeId' y lo saca del arbol. */
   virtual TreeNode* RemoveNode(const ItemId &NodeId);
   /** Busca el nodo con id 'NodeId', si es nodo interno lo expande/contrae. */
   virtual void Expand(const ItemId &NodeId, bool Expand);
   /** Busca el nodo con id 'NodeId', informa si esta expandido */
   virtual bool IsExpanded(const ItemId &NodeId);
   /** Informa si el nodo es visible en la lista */
   virtual bool IsVisible(const ItemId &NodeId);
   /** Retorna HtmlConfigurationData de nodo con identificador NodeId. */
   virtual HtmlConfigurationData* GetHtmlConfigurationData(const ItemId &NodeId);

protected:
   /** Retorna html de nodo interno que encabeza nivel en arbol */
   std::string GetLeafHtml(int WindowWidth);
};
}  // namespace

#endif /* TREELEAF_H_ */
