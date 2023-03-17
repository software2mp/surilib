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

#include "TreeInnerNode.h"

// Includes standard
#include <sstream>

// Includes Suri
#include "resources.h"

// Includes Wx

// Includes App

// Defines
#define SUBLEVEL \
   "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\
		<tr>\
			<td>\
				<table width=\"16px\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"></table>\
			</td>\
			<td width=\"100%\">__SUBNODE__\
			</td>\
	</table>"

#define SUBNODE_FLAG "__SUBNODE__"

#define INNERNODE \
   "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">	\
	<tr>\
		<td>\
			<table border=\"0\"  cellspacing=\"0\" cellpadding=\"0\">   \
				<tr>\
					<td>\
                       <a href=\"expand\">\
                          <img src=\"__EXPANDED__\">\
                       </a>\
					</td>\
				</tr>\
			</table>\
		</td>\
		<td width=\"100%\">\
			<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">	\
				<tr>\
					<td>\
					  &nbsp;&nbsp;__NAME__\
					</td>\
				</tr>\
			</table>\
		</td>\
		__CHECKBOXHTML__\
	</tr>\
</table>\
"

#define CHECKBOXHTML \
   "\
		<td>\
			<table align= \"left\" width=\"16px\">\
				<tr>\
					<td align=\"right\">\
						<a href=\"checkbox\">\
							<img src=\"__CHOICE__\">\
						</a>\
					</td>\
				</tr>\
			</table>\
		</td>"

#define NAME_FLAG "__NAME__"
#define EXPANDED_FLAG "__EXPANDED__"
#define CHECKBOXHTML_FLAG "__CHECKBOXHTML__"
#define CHOICE_FLAG "__CHOICE__"

#define LEVELOFFSET 16
#if defined(__LINUX__)
#define INNERNODEOFFSET 32
#else
#define INNERNODEOFFSET 28
#endif
#define CHECKOFFSET 20

namespace suri {
/**
 * Constructor de nodo
 * @param[in] pHtmlData
 * @param[in] IsRoot bool que indica si este es el nodo raiz
 * @param[in] VisualizationFlags
 * del arbol.
 */
TreeInnerNode::TreeInnerNode(HtmlConfigurationData* pHtmlData, bool IsRoot,
                             long VisualizationFlags) :
      TreeNode(pHtmlData, VisualizationFlags), isRoot_(IsRoot), expanded_(false),
      idGenerator_(0) {
   if (isRoot_) {
      expanded_ = true;
   }
}

/** Destructor **/
TreeInnerNode::~TreeInnerNode() {
   std::list<TreeNode*>::iterator it;
   for (it = nodeList_.begin(); it != nodeList_.end(); it++)
      delete (*it);
   nodeList_.clear();
}

/**
 * Retorna la cantidad de nodos visibles debajo del nodo.
 * @return int cantidad de nodos visibles debajo del nodo
 */
int TreeInnerNode::GetVisibleNodeCount() {
   int visibleitemcount = 0;

   // Si no es raiz sumo el item que corresponde al nodo interno
   if (!isRoot_) {
      visibleitemcount++;
   }

   // Si esta expandido sumo los items internos
   if (expanded_) {
      std::list<TreeNode*>::iterator it;
      for (it = nodeList_.begin(); it != nodeList_.end(); it++)
         visibleitemcount += (*it)->GetVisibleNodeCount();
   }
   return visibleitemcount;
}

/**
 * Actualiza el html de todos los items visibles. El html lo guarda en el vector
 * que recibe por parametro, si faltan elementos los agrega, si hay elementos
 * de mas los elimina.
 * @param[in] WindowWidth Ancho en pixels de la ventana donde quiero graficarlo
 * @param[in] VectorIndex posicion del proximo item a modificar del vector.
 * @param[in] HtmlVector vector en blanco o no cuyo contenido se quiere actualizar.
 * El contenido previo del vector se perdera.
 * @param[out] HtmlVector vector configurado con el html de todos los elementos
 * visibles. Si se configuran los items en el orden del vector la lista parecera un
 * arbol.
 */
void TreeInnerNode::UpdateHtmlVectorWithVisibleItems(
      int WindowWidth, int &VectorIndex, std::vector<DecoratedHtmlItem> &HtmlVector) {
   // Agrego html de nodo interno
   if (!isRoot_) {
      DecoratedHtmlItem decoratedhtml(GetNodeId(), GetInnerNodeHtml(WindowWidth));
      SetHtmlItemToVector(VectorIndex, decoratedhtml, HtmlVector);
      VectorIndex++;
   }
   if (expanded_) {
      int width = std::max((isRoot_ == true) ? WindowWidth : WindowWidth - LEVELOFFSET,
                           0);

      // Agrego html de nodos internos
      int vectorindex = VectorIndex;
      std::list<TreeNode*>::iterator it;
      for (it = nodeList_.begin(); it != nodeList_.end(); it++)
         (*it)->UpdateHtmlVectorWithVisibleItems(width, VectorIndex, HtmlVector);

      // Agrego decoracion a los nodos cargados
      for (; vectorindex < VectorIndex; vectorindex++) {
         std::string decoratedhtml = DecorateHtml(HtmlVector[vectorindex].GetHtml());
         HtmlVector[vectorindex].SetHtml(decoratedhtml);
      }
   }
}

/**
 * Actualiza el estado de activacion de los nodos internos usando el valor
 * de sus hijos.
 * @return retorna el nuevo estado de activacion del nodo interno.
 */
HtmlConfigurationData::State TreeInnerNode::UpdateStateUsingLeafs() {
   HtmlConfigurationData::State newstate = HtmlConfigurationData::OFF;

   std::list<TreeNode*>::iterator it = nodeList_.begin();
   if (it != nodeList_.end()) {
      newstate = (*it)->UpdateStateUsingLeafs();
   }

   for (; it != nodeList_.end(); it++) {
      HtmlConfigurationData::State newsubstate = (*it)->UpdateStateUsingLeafs();
      if ((newsubstate == HtmlConfigurationData::BOTH || newstate != newsubstate)
            && newstate != HtmlConfigurationData::BOTH) {
         newstate = HtmlConfigurationData::BOTH;
      }
   }

   if (!isRoot_) {
      pHtmlConfigurationData_->SetState(newstate);
   }

   return newstate;
}

/**
 * Actualiza el estado de activacion del nodo y de todos sus hijos
 * @param[in] NodeId id del nodo que quiero actualizar
 * @param[in] NewState nuevo estado del nodo
 */
void TreeInnerNode::UpdateState(const ItemId &NodeId,
                                HtmlConfigurationData::State NewState) {
   if (nodeId_.Compare(NodeId) == 0) {
      pHtmlConfigurationData_->SetState(NewState);
      std::list<TreeNode*>::iterator it = nodeList_.begin();
      while (it != nodeList_.end()) {
         (*it)->UpdateState((*it)->GetNodeId(), NewState);
         it++;
      }
      return;
   }

   TreeNode* pnode = NULL;
   if (FindSubnode(NodeId, pnode)) {
      pnode->UpdateState(NodeId, NewState);
   }
}

/**
 * Configura id del nodo y actualiza el de todos los subnodos
 * @param[in] Id nuevo id del nodo.
 */
void TreeInnerNode::UpdateNodeId(const TreeNodeId &Id) {
   SetNodeId(Id);
   std::list<TreeNode*>::iterator it = nodeList_.begin();
   while (it != nodeList_.end()) {
      ConfigureUniqueId(*it);
      it++;
   }
}

/**
 * Busca debajo del nodo otro con id 'NodeId', si es nodo interno lo expande/contrae
 * deacuerdo a contenido de 'Expand'.
 * @param[in] NodeId id de nodo interno que se quire expander/contraer
 * @param[in] Expand indica si el nodo deberia estar expandido o no
 */
void TreeInnerNode::Expand(const ItemId &NodeId, bool Expand) {
   if (nodeId_.Compare(NodeId) == 0) {
      if (isRoot_) {
         return;
      }
      expanded_ = Expand;
   } else {
      TreeNode* pnode = NULL;
      if (FindSubnode(NodeId, pnode)) {
         pnode->Expand(NodeId, Expand);
      }
   }
}

/**
 * Busca el nodo con id 'NodeId', informa si esta expandido
 * @param[in] NodeId id de nodo interno que se quire expander/contraer
 * @return bool que indica si el nodo esta expandido
 */
bool TreeInnerNode::IsExpanded(const ItemId &NodeId) {
   if (nodeId_.Compare(NodeId) == 0) {
      return expanded_;
   }

   TreeNode* pnode = NULL;
   if (FindSubnode(NodeId, pnode)) {
      return pnode->IsExpanded(NodeId);
   }

   return false;
}

/**
 * Informa si el nodo es visible o esta oculto dentro de
 * un nodo colapsado.
 * @param[in] NodeId id de nodo
 */
bool TreeInnerNode::IsVisible(const ItemId &NodeId) {
   bool returnvalue = false;

   if (nodeId_.Compare(NodeId) == 0) {
      returnvalue = true;
   }

   if (!returnvalue && expanded_) {
      TreeNode* pnode = NULL;
      if (FindSubnode(NodeId, pnode)) {
         returnvalue = pnode->IsVisible(NodeId);
      }
   }

   return returnvalue;
}

/**
 * Retorna HtmlConfigurationData de nodo con identificador NodeId.
 * @param[in] NodeId id de nodo
 * @return HtmlConfigurationData* buscado si encuentra NodeId
 * @return NULL si no encuentra NodeId
 */
HtmlConfigurationData* TreeInnerNode::GetHtmlConfigurationData(const ItemId &NodeId) {
   if (nodeId_.Compare(NodeId) == 0) {
      return pHtmlConfigurationData_;
   }

   TreeNode* pnode = NULL;
   if (FindSubnode(NodeId, pnode)) {
      return pnode->GetHtmlConfigurationData(NodeId);
   }

   return NULL;
}

/**
 * Busca en pTreeNode el nodo en la posicion visible indicada. Si el nodo en
 * la posicion visible esta dentro de un subnodo retorna el puntero a ese subnodo.
 * @param[in] Position posicion del nodo(solo considera nodos visibles) que busco
 * @param[out] pTreeNode nodo buscado. Si no lo encontro retorna NULL.
 * \attention pNode pTreeNode es responsabilidad de este nodo. No eliminar
 * @return int posicion del nodo(solo considera nodos visibles) del
 * nodo retornado. Empieza desde 0.
 * @return <0 si no encontro el nodo.
 */
int TreeInnerNode::FindSubnode(int Position, TreeNode* &pTreeNode) {
   if (Position < 0) {
      return -1;
   }

   int visibleitemindex = -1;
   int visibleitemcount = 0;
   std::list<TreeNode*>::iterator it = nodeList_.begin();
   while (it != nodeList_.end() && visibleitemindex < Position) {
      visibleitemcount = visibleitemindex + 1;
      visibleitemindex += (*it)->GetVisibleNodeCount();
      if (visibleitemindex < Position) {
         it++;
      }
   }

   if (it == nodeList_.end()) {
      return -1;
   }

   pTreeNode = *it;
   return Position - visibleitemcount;
}

/**
 * Busca en pTreeNode el nodo con id indicado. Retorna iterador que apunta
 * a el nodo correspondiente.
 * @param[in] NodeId posicion del nodo que busco
 * @return iterador que apunta al nodo. Si no encontro el nodo retorna
 * nodeList_.end().
 */
std::list<TreeNode*>::iterator TreeInnerNode::FindSubnode(const ItemId &NodeId) {
   int compare = -1;
   std::list<TreeNode*>::iterator it = nodeList_.begin();
   while (it != nodeList_.end() && compare < 0) {
      compare = (*it)->GetNodeId().Compare(NodeId);
      if (compare < 0) {
         it++;
      }
   }
   return it;
}

/**
 * Busca en pTreeNode el nodo con id indicado. Si el nodo
 * esta dentro de un subnodo, retorna el puntero a ese subnodo.
 * @param[in] NodeId posicion del nodo que busco
 * @param[out] pTreeNode nodo buscado. Si no lo encontro retorna NULL.
 * \attention pNode pTreeNode es responsabilidad de este nodo. No eliminar
 * @return bool que indica si encontro el nodo
 */
bool TreeInnerNode::FindSubnode(const ItemId &NodeId, TreeNode* &pTreeNode) {
   std::list<TreeNode*>::iterator it = FindSubnode(NodeId);
   if (it == nodeList_.end()) {
      return false;
   }

   pTreeNode = *it;
   return true;
}

/**
 * Encapsula html que recibe por parametro para que parezca dentro de un
 * arbol
 * @param[in] ItemHtml html correspondiente a nodo dentro de nodo interno
 * @return html con ItemHtml encapsulado
 */
std::string TreeInnerNode::DecorateHtml(const std::string &ItemHtml) {
   if (isRoot_) {
      return ItemHtml;
   }
   std::string decoratedhtml = SUBLEVEL;
   size_t position = decoratedhtml.find(SUBNODE_FLAG);
   return decoratedhtml.replace(position, sizeof(SUBNODE_FLAG), ItemHtml);
}

/**
 * Retorna html con datos de nodo interno. El html encabeza el nivel dentro del
 * arbol
 * @return html de nodo interno
 */
std::string TreeInnerNode::GetInnerNodeHtml(int WindowWidth) {
   std::string innernode = INNERNODE;

   // Configuro nombre del nodo
   int offset =
         ((visualizationFlags_ & CheckableNode) != 0) ? INNERNODEOFFSET + CHECKOFFSET :
                                                        INNERNODEOFFSET;
   int width = std::max(WindowWidth - offset, 0);
   size_t position = innernode.find(NAME_FLAG);
   innernode.replace(position, sizeof(NAME_FLAG) - 1,
                     GetSizedText(pHtmlConfigurationData_->GetName(), width));

   // Configuro icono para expandir/contraer nodo
   std::string src = button_COLLAPSEDNODE;
   if (expanded_) {
      src = button_EXPANDEDNODE;
   }
   position = innernode.find(EXPANDED_FLAG);
   innernode.replace(position, sizeof(EXPANDED_FLAG) - 1, src);

   // Configuro check
   std::string checkhtml = "";
   if ((visualizationFlags_ & CheckableNode) != 0) {
      checkhtml = CHECKBOXHTML;
      src = button_CHECK_UNCHECKED;
      if (pHtmlConfigurationData_->GetState() == HtmlConfigurationData::ON) {
         src = button_CHECK_CHECKED;
      }
      if (pHtmlConfigurationData_->GetState() == HtmlConfigurationData::BOTH) {
         src = button_CHECK_BOTH;
      }
      position = checkhtml.find(CHOICE_FLAG);
      checkhtml.replace(position, sizeof(CHOICE_FLAG) - 1, src);
   }
   position = innernode.find(CHECKBOXHTML_FLAG);
   innernode.replace(position, sizeof(CHECKBOXHTML_FLAG) - 1, checkhtml);

   return innernode;
}

/**
 * Busca el nodo con id 'NodeId' y retorna el id del primer hijo.
 * @param[in] NodeId id de nodo
 * @return ItemId id del primer hijo del nodo con id 'NodeId'
 * @return INVALIDTREEID si 'NodeId' es nodo hoja o no encontro NodeId
 */
ItemId TreeInnerNode::GetFirstChild(const ItemId &NodeId) {
   ItemId returnvalue = INVALIDTREEID;

   if (nodeId_.Compare(NodeId) == 0) {
      if (nodeList_.size() > 0) {
         returnvalue = (*nodeList_.begin())->GetNodeId();
      }
   } else {
      TreeNode* pnode = NULL;
      if (FindSubnode(NodeId, pnode)) {
         returnvalue = pnode->GetFirstChild(NodeId);
      }
   }

   return returnvalue;
}

/**
 * Busca el nodo con id 'NodeId' y retorna el siguiente hijo del mismo padre.
 * @param[in] NodeId id de nodo
 * @return ItemId id del proximo nodo del mismo padre
 * @return INVALIDTREEID si no tiene proximo hermano o no encontro !NodeId
 */
ItemId TreeInnerNode::GetNextSibling(const ItemId &NodeId) {
   std::list<TreeNode*>::iterator it = FindSubnode(NodeId);
   if (it != nodeList_.end()) {
      if ((*it)->GetNodeId().Compare(NodeId) > 0) {
         return (*it)->GetNextSibling(NodeId);
      }

      it++;  // Obtengo nodo siguiente
      return (it != nodeList_.end()) ? (*it)->GetNodeId() : INVALIDTREEID;
   }
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y retorna el padre.
 * @param[in] NodeId id de nodo existente en arbol
 * @return ItemId id del padre
 * @return INVALIDID si NodeId es root o no existe.
 */
ItemId TreeInnerNode::GetParent(const ItemId &NodeId) {
   std::list<TreeNode*>::iterator it = FindSubnode(NodeId);
   if (it != nodeList_.end()) {
      if ((*it)->GetNodeId().Compare(NodeId) > 0) {
         return (*it)->GetParent(NodeId);
      }

      return this->nodeId_;
   }
   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y agrega Node al mismo nivel, despues del
 * nodo encontrado.
 * @param[in] NodeId id de nodo
 * @param[in] pNode pNode nodo que quiero agregar
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase
 * @return ItemId id del nodo que se agrego
 * @return INVALIDTREEID si no encontro NodeId en el arbol.
 */
ItemId TreeInnerNode::AddNodeAfter(const ItemId &NodeId, TreeNode* pNode) {
   return AddNode(NodeId, pNode, true);
}

/**
 * Busca el nodo con id 'NodeId' y agrega 'Node' al mismo nivel, antes del
 * nodo encontrado.
 * @param[in] NodeId id de nodo
 * @param[in] pNode pNode nodo que quiero agregar
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase
 * @return ItemId id del nodo que se agrego
 * @return INVALIDTREEID si no encontro NodeId en el arbol.
 */
ItemId TreeInnerNode::AddNodeBefore(const ItemId &NodeId, TreeNode* pNode) {
   return AddNode(NodeId, pNode, false);
}

/**
 * Busca el nodo con id 'NodeId'. Agrega 'pNode' como ultimo nodo debajo
 * de nodo encontrado.
 * @param[in] NodeId id de nodo interno
 * @param[in] pNode pNode nodo que quiero agregar al arbol
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase.
 * @return ItemId id del nodo que se agrego
 * @return INVALIDTREEID si no encontro NodeId en el arbol.
 */
ItemId TreeInnerNode::AppendNode(const ItemId &NodeId, TreeNode* pNode) {
   if (GetNodeId().Compare(NodeId) == 0) {
      ConfigureUniqueId(pNode);
      nodeList_.push_back(pNode);
      ItemId newid = pNode->GetNodeId();
      return newid;
   }

   TreeNode* pnode = NULL;
   if (FindSubnode(NodeId, pnode)) {
      return pnode->AppendNode(NodeId, pNode);
   }

   return INVALIDTREEID;
}

/**
 * Busca el nodo con id 'NodeId' y lo saca del nodo. Si es un nodo
 * interno, solo se puede remover si esta vacio.
 * @param[in] NodeId id de nodo
 * @return Node nodo que se saco del arbol
 * \attention pNode deja de ser responsabilidad de este nodo.
 * Eliminar puntero en codigo cliente. La instancia es la misma que se
 * agrego con AddNodeBefore, AddNodeAfter o AppendNode.
 * @return NULL si no encontro NodeId en el arbol.
 */
TreeNode* TreeInnerNode::RemoveNode(const ItemId &NodeId) {
   TreeNode* pnode = NULL;
   std::list<TreeNode*>::iterator it = FindSubnode(NodeId);
   if (it != nodeList_.end()) {
      if ((*it)->GetNodeId().Compare(NodeId) > 0) {
         return (*it)->RemoveNode(NodeId);
      }

      pnode = *it;
      nodeList_.erase(it);
   }
   return pnode;
}

/**
 * Busca el nodo con id 'NodeId' y agrega Node al mismo nivel, antes o despues
 * del nodo encontrado dependiendo del valor de After.
 * @param[in] NodeId id de nodo
 * @param[in] pNode pNode nodo que quiero agregar
 * \attention pNode pasa a ser responsabilidad de este nodo. No
 * eliminar fuera de la clase
 * @param[in] After Si es true agrega nodo despues de pNode, si es false se
 * agrega nodo antes de pNode.
 * @return ItemId id del nodo que se agrego
 * @return INVALIDTREEID si no encontro NodeId en el arbol.
 */
ItemId TreeInnerNode::AddNode(const ItemId &NodeId, TreeNode* pNode, bool After) {
   std::list<TreeNode*>::iterator it = FindSubnode(NodeId);
   if (it != nodeList_.end()) {
      if ((*it)->GetNodeId().Compare(NodeId) > 0) {
         return (*it)->AddNode(NodeId, pNode, After);
      }

      ConfigureUniqueId(pNode);
      if (After) {
         it++;
      }
      nodeList_.insert(it, pNode);
      return pNode->GetNodeId();
   }
   return INVALIDTREEID;
}

/**
 * Genera id unico y lo configura en pNode
 * @param[in] pNode nodo que quiero configurar
 * @param[out] pNode nodo configurado con id unico dentro de nodo interno
 */
void TreeInnerNode::ConfigureUniqueId(TreeNode* pNode) {
   idGenerator_++;
   std::stringstream ss;
   ss << idGenerator_;

   TreeNodeId newid = nodeId_;
   newid.AppendStringToId(ss.str());
   pNode->UpdateNodeId(newid);
}
}  // namespace

