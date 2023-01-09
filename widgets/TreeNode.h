/*! \file TreeNode.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TREENODE_H_
#define TREENODE_H_

// Includes standard
#include <vector>

// Includes Suri
#include "TreeNodeId.h"
#include "DecoratedHtmlItem.h"
#include "HtmlConfigurationData.h"

// Includes Wx

// Includes App

// Defines

#define SimpleNode 0
#define CheckableNode 1

namespace suri {
class TreeNode {
public:
   /** Ctor */
   TreeNode(HtmlConfigurationData* pHtmlData = NULL, long VisualizationFlags =
         SimpleNode);
   /** Dtor */
   virtual ~TreeNode();

// --------------- Metodos de interaccion con lista ---------------

   /** Retorna la cantidad de nodos visibles debajo del nodo. */
   virtual int GetVisibleNodeCount()=0;
   /** Actualiza el html de todos los items visibles; */
   virtual void UpdateHtmlVectorWithVisibleItems(
         int WindowWidth, int &VectorIndex,
         std::vector<DecoratedHtmlItem> &HtmlVector)=0;
   /** Actualiza el estado de activacion de nodos internos con el valor de las hojas */
   virtual HtmlConfigurationData::State UpdateStateUsingLeafs()=0;
   /** Actualiza el estado de activacion del nodo y de todos sus hijos */
   virtual void UpdateState(const ItemId &NodeId,
                            HtmlConfigurationData::State NewState)=0;
   /** Configura id del nodo y actualiza el de todos los subnodos */
   virtual void UpdateNodeId(const TreeNodeId &Id)=0;

// --------------- Metodos de busqueda/edicion por id ---------------

   /** Busca el nodo con id 'NodeId' y retorna el primer hijo. */
   virtual ItemId GetFirstChild(const ItemId &NodeId)=0;
   /** Busca el nodo con id 'NodeId' y retorna el siguiente hijo del mismo padre. */
   virtual ItemId GetNextSibling(const ItemId &NodeId)=0;
   /** Busca el nodo con id 'NodeId' y retorna el padre. */
   virtual ItemId GetParent(const ItemId &NodeId)=0;
   /** Busca el nodo con id 'NodeId' y agrega 'pNode' a continuacion. */
   virtual ItemId AddNodeAfter(const ItemId &NodeId, TreeNode* pNode)=0;
   /** Busca el nodo con id 'NodeId' y agrega 'pNode' antes del nodo encontrado. */
   virtual ItemId AddNodeBefore(const ItemId &NodeId, TreeNode* pHtmlNode)=0;
   /** Busca el nodo con id 'NodeId' y agrega 'pNode' antes o despues. */
   virtual ItemId AddNode(const ItemId &NodeId, TreeNode* pNode, bool After)=0;
   /** Busca el nodo con id 'NodeId'. Agrega 'pNode' como su ultimo hijo. */
   virtual ItemId AppendNode(const ItemId &NodeId, TreeNode* pHtmlNode)=0;
   /** Busca el nodo con id 'NodeId' y lo saca del arbol. */
   virtual TreeNode* RemoveNode(const ItemId &NodeId)=0;
   /** Busca el nodo con id 'NodeId', si es nodo interno lo expande/contrae. */
   virtual void Expand(const ItemId &NodeId, bool Expand)=0;
   /** Busca el nodo con id 'NodeId', informa si esta expandido */
   virtual bool IsExpanded(const ItemId &NodeId)=0;
   /** Informa si el nodo es visible en la lista */
   virtual bool IsVisible(const ItemId &NodeId)=0;
   /** Retorna HtmlConfigurationData de nodo con identificador NodeId. */
   virtual HtmlConfigurationData* GetHtmlConfigurationData(const ItemId &NodeId)=0;

// --------------- Metodos para configurar nodo ---------------
   /** Retorna id del nodo */
   TreeNodeId GetNodeId();
   /** Configura id del nodo */
   void SetNodeId(const TreeNodeId &Id);
   /** Retorna clase que se usa para configurar html del item */
   HtmlConfigurationData* GetHtmlConfigurationData();
   /** Configura clase que se usa para configurar html del item */
   void SetHtmlConfigurationData(HtmlConfigurationData* pHtmlData);

protected:
   /** Configura o agrega un DecoratedHtmlItem en vector con Html */
   bool SetHtmlItemToVector(int VectorIndex, const DecoratedHtmlItem &HtmlItem,
                            std::vector<DecoratedHtmlItem> &HtmlVector);
   /** Modifica Text para que entre en ventana de longitud WindowWidth */
   std::string GetSizedText(std::string Text, int WindowWidth);

   TreeNodeId nodeId_; /*! Id del nodo asociado al arbol. */
   HtmlConfigurationData* pHtmlConfigurationData_;
   /*! Clase que se usa para generar html de un item */
   long visualizationFlags_; /*! Se usa para configurar html */
};
}

#endif /* TREENODE_H_ */
