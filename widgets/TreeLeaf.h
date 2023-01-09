/*! \file TreeLeaf.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
