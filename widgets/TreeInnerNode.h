/*! \file TreeInnerNode.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TREEINNERNODE_H_
#define TREEINNERNODE_H_

// Includes standard
#include <list>

// Includes Suri
#include "TreeNode.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/**
 * Representa los nodo interno de arbol, por lo que contendra 0,1 o mas subnodos.
 * Esta clase es la responsable de generar los TreeNodeId usando la posicion
 * de los nodos en el arbol. Cada vez que agrega un nodo
 * le asigna un id formado con: 'su id'+'/'+'id unico dentro de tree node'
 */
class TreeInnerNode : public TreeNode {
public:
   /** Ctor */
   TreeInnerNode(HtmlConfigurationData* pHtmlData, bool IsRoot,
                 long VisualizationFlags = SimpleNode);
   /** Dtor */
   virtual ~TreeInnerNode();

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
   /** Actualiza pTreeNode con el nodo en la posicion visible indicada. */
   int FindSubnode(int Position, TreeNode* &pTreeNode);
   /** Busca en pTreeNode el nodo con id indicado. Retorna iterador */
   std::list<TreeNode*>::iterator FindSubnode(const ItemId &NodeId);
   /** Busca NodeId y actualiza pTreeNode con el nodo de id indicado. */
   bool FindSubnode(const ItemId &NodeId, TreeNode* &pTreeNode);
   /** Agrega nivel a nodo que recibe por parametro */
   std::string DecorateHtml(const std::string &ItemHtml);
   /** Retorna html de nodo interno que encabeza nivel en arbol */
   std::string GetInnerNodeHtml(int WindowWidth);
   /** Genera id unico y lo configura en pNode */
   void ConfigureUniqueId(TreeNode* pNode);

   bool isRoot_; /*! Indica si el nodo es la raiz del arbol. */
   /*! En caso de ser true el nodo no tiene item asociado */
   /*! en la lista y no se puede colapsar */
   bool expanded_; /*! Indica si el nodo esta expandido o */
   /*! contraido en la interfaz grafica. */
   std::list<TreeNode*> nodeList_; /*! Lista ordenada con nodos contenidos en nodo interno */
   int idGenerator_; /*! Se incrementa cada vez que agrego elemento. Se usa para generar id unico */
};
}  // namespace

#endif /* TREEINNERNODE_H_ */
