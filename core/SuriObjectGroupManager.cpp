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

#include <string>
#include <vector>

#include "suri/SuriObjectManagerInterface.h"
#include "SuriObjectGroupManager.h"
#include "DefaultWorkGroup.h"
#include "DefaultTreeNode.h"
#include "TreeNodeAuxiliaryFunctions.h"
#include "logmacros.h"
#include "HtmlTreeGroupChangedNotification.h"
#include "GroupsNodeFilter.h"
#include "DatasourceAddtitionNotification.h"
#include "ObjectAditionGroupNotification.h"

namespace suri {

/** Ctor */
SuriObjectGroupManager::SuriObjectGroupManager(
                                 SuriObjectManagerInterface* pObjectManager) {
   pRoot_ = NULL;
   pObjectManager_ = pObjectManager;
   observerId_ = pObjectManager_->Attach(this);
   ignoreNotification_ = false;
   if (observerId_ != Subject::InvalidCount)
      REPORT_DEBUG("D: Error al conectar el GroupManager como viewer");
   Clear();
}

/** Dtor */
SuriObjectGroupManager::~SuriObjectGroupManager() {
   pObjectManager_->Detach(observerId_);
}

/**
 * Elimina el contenido del arbol. Solo queda el nodo raiz.
 */
void SuriObjectGroupManager::Clear() {
   delete pRoot_;
   pRoot_ = new DefaultTreeNode(true);

   HtmlTreeGroupChangedNotification htmltreegroupnotification(this);
   Notify(&htmltreegroupnotification);
}

/**
 * Retorna instancia de WorkGroupInterface que referencia la
 * raiz del arbol.
 * @return WorkGroupInterface configurado con grupo raiz. El puntero
 * retornado es responsabilidad del invocante.
 */
WorkGroupInterface* SuriObjectGroupManager::GetRootWorkGroup() {
   return new DefaultWorkGroup(pRoot_);
}

/**
 * Retorna instancia de MutableWorkGroupInterface que referencia la
 * raiz del arbol.
 * @return MutableWorkGroupInterface configurado con grupo raiz. El puntero
 * retornado es responsabilidad del invocante.
 */
MutableWorkGroupInterface* SuriObjectGroupManager::GetRootMutableWorkGroup() {
   return new DefaultWorkGroup(pRoot_);
}


/**
 * Agrega un grupo dentro de otro.
 * @param[in] ParentGroup NodePath del grupo donde quiero
 * agregar el nuevo grupo
 * @param[in] Index Posicion donde quiero agregar el grupo.
 * Valores posibles: 0 <= Index <= pParent->Size()
 * Si Index == pParent->Size(): Lo agrega al final.
 * Si Index > pParent->Size(): Falla la operacion.
 * @return NodePath del grupo creado
 * @return NodePath invalido si no pudo crear/agregar el grupo
 */
NodePath SuriObjectGroupManager::InsertGroup(const std::string &GroupName,
                                       const NodePath &ParentGroup, int Index) {
   DefaultWorkGroup workgroup(pRoot_);
   NodePath newpath = workgroup.InsertGroup(GroupName, ParentGroup, Index);

   // Notifico el cambio
   if (newpath.IsValid()) {
      HtmlTreeGroupChangedNotification htmltreegroupnotification(this);
      Notify(&htmltreegroupnotification);
   }

   return newpath;
}

/** Agrega un grupo dentro de otro. */
NodePath SuriObjectGroupManager::InsertContextGroup(const SuriObject::UuidType& ViewcontextId,
		   	   	   	   	   	   	   	   const std::string &GroupName,
		   	   	   	   	   	   	   	   const NodePath &ParentGroup, int Index, ViewcontextInterface::ViewcontextType ctype) {
      DefaultWorkGroup workgroup(pRoot_);
	   NodePath newpath = workgroup.InsertContextGroup(ViewcontextId, GroupName,
			ParentGroup, Index, ctype);

	   // Notifico el cambio
	   if (newpath.IsValid()) {
	      HtmlTreeGroupChangedNotification htmltreegroupnotification(this);
	      htmltreegroupnotification.SetViewcontextId(ViewcontextId);
	      Notify(&htmltreegroupnotification);
	   }

	   return newpath;
}
/**
 * Elimina un grupo del arbol
 * @param[in] Node NodePath del grupo a eliminar
 * @return bool que indica si pudo eliminarlo
 */
bool SuriObjectGroupManager::RemoveGroup(const NodePath &Node) {

   // Eliminar todos los subnodos con contenido de manager asociado.
   TreeNodeInterface* pgroup = Node.GetLastPathNode();
   if (!pgroup->AllowsChildren()) {
      REPORT_DEBUG("D: ERROR: Se llamo a RemoveGroup con path de un elemento/item");
      return false;
   }
   // Elimino datasources. Bloqueo update para que no se actualice el arbol.
   ignoreNotification_ = true;
   WorkGroupInterface* pworkgroup  = GetRootWorkGroup();
   TreeIterator it = pworkgroup->GetIterator(Node, new GroupsNodeFilter(NULL));
   bool canremove = true;
   for (; it.IsValid() && canremove; it.NextNode()) {
      canremove = pObjectManager_->CanRemoveObject(it.GetPath().GetLastPathNode()->GetContent());
   }
   if (!canremove)
      return false;
   TreeIterator removeit = pworkgroup->GetIterator(Node, new GroupsNodeFilter(NULL));
   for (; removeit.IsValid(); removeit.NextNode())
      pObjectManager_->RemoveObject(removeit.GetPath().GetLastPathNode()->GetContent());
   delete pworkgroup;
   ignoreNotification_ = false;

   // Elimino grupo del arbol.
   DefaultWorkGroup workgroup(pRoot_);

   // Notifico el cambio
   bool status = workgroup.Remove(Node);
   if (status) {
      HtmlTreeGroupChangedNotification htmltreegroupnotification(this);
      Notify(&htmltreegroupnotification);
   }
   return status;
}

/**
 * Mueve un nodo dentro de un grupo en el arbol
 * @param[in] Node NodePath del nodo a mover
 * @param[in] NewParent NodePath nuevo padre
 * @param[in] NewIndex Posicion donde quiero mover el nodo.
 * Valores posibles: 0 <= NewIndex <= pNewParent->Size()
 * Si NewIndex == pNewParent->Size(): Lo agrega al final.
 * Si NewIndex > pNewParent->Size(): Falla la operacion.
 * @return NodePath del nodo despues de moverse
 * @return NodePath invalido si no pudo mover/agregar el grupo
 */
NodePath SuriObjectGroupManager::MoveNode(const NodePath &Node,
                            const NodePath &NewParent, int Index) {
   DefaultWorkGroup workgroup(pRoot_);
   if ( !workgroup.ValidatePath(Node) || !workgroup.ValidatePath(NewParent)
                             || !NewParent.GetLastPathNode()->AllowsChildren() )
      return NodePath(NULL, NULL);

   /** Valido que no se trate de mover un grupo debajo de si mismo. */
   if (Node.AllowsChildren()) {
      DefaultWorkGroup nodeworkgroup(Node.GetLastPathNode());
      if (nodeworkgroup.SearchNode(NewParent.GetLastPathNode()->GetId()).IsValid())
         return NodePath(NULL, NULL);
   }

   // Busco id destino a donde debo mover todos los objetos en manager
   bool validnode = true;
   TreeNodeInterface* pdestinationnode = NewParent.GetLastPathNode();
   if (Index >= 0 && Index < pdestinationnode->GetSize()) {
      pdestinationnode = pdestinationnode->GetChildAt(Index);
      if (pdestinationnode->AllowsChildren())
         validnode = GetNextContentNode(pRoot_, pdestinationnode, pdestinationnode);
   } else if (Index == pdestinationnode->GetSize()) {
      TreeNodeInterface* lastcontentnode = GetLastContentNode(pdestinationnode);
      lastcontentnode = (lastcontentnode != NULL) ? lastcontentnode : pdestinationnode;
      validnode = GetNextContentNode(pRoot_, lastcontentnode, pdestinationnode);
   }

   if (!validnode)
      return NodePath(NULL, NULL);

   SuriObject::UuidType destinationid = pdestinationnode ?
                                             pdestinationnode->GetContent() :
                                             SuriObject::NullUuid;

   // Muevo el/los SuriObject que corresponda en manager asociado
   if ( Node.GetLastPathNode()->AllowsChildren() ) {
      TreeNodeInterface* pgroup = Node.GetLastPathNode();
      TreeNodeInterface* pcontentnode = NULL;
      while (GetNextContentNode(pgroup, pcontentnode, pcontentnode) && pcontentnode)
         MoveItemContent(pcontentnode, destinationid);
   } else {
       MoveItemContent(Node.GetLastPathNode(), destinationid);
   }

   // Actualizo el arbol.
   NodePath newpath = workgroup.MoveNode(Node, NewParent, Index);

   // Notifico el cambio
   if (newpath.IsValid()) {
      HtmlTreeGroupChangedNotification htmltreegroupnotification(this);
      Notify(&htmltreegroupnotification);
   }

   return newpath;
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void SuriObjectGroupManager::Update(NotificationInterface* pNotification) {
   HtmlTreeGroupChangedNotification htmltreegroupnotification(this);
   if (!ignoreNotification_ && pNotification->ApplyNotification(this)) {

      DatasourceAddtitionNotification* pdsanotif =
            dynamic_cast<DatasourceAddtitionNotification*>(pNotification);

      if (pdsanotif != NULL) {
         htmltreegroupnotification.SetViewcontextId(pdsanotif->GetViewcontextId());
      }

      Notify(&htmltreegroupnotification);
   }
}

/** 
 * Mueve un elemento en el manager asociado
 * @param[in] pItemNode nodo cuyo contenido referencia el objeto que se desea mover
 * @param[in] DestinationId Id del objeto en el manager que esta en la posicion
 * donde deseo mover el item.
 * @return bool que indica el resultado de la operacion.
 */
bool SuriObjectGroupManager::MoveItemContent(TreeNodeInterface* pItemNode,
                                          SuriObject::UuidType DestinationId) {
   ignoreNotification_ = true;
   bool status = false;
   if (DestinationId != SuriObject::NullUuid)
      status = pObjectManager_->MoveObject(pItemNode->GetContent(), DestinationId);
   else
      status = pObjectManager_->MoveObjectToEnd(pItemNode->GetContent());
   ignoreNotification_ = false;
   return status;
}

/**
 * Valida que el orden de los items en arbol sea igual al del manager
 * @return bool que indica si el orden esta sincronizado.
 */
bool SuriObjectGroupManager::ValidateOrder() {
   bool sincronized = true;
   std::vector<SuriObject::UuidType> managerids = pObjectManager_->GetOrderedIds();

   WorkGroupInterface* pworkgroup = GetRootWorkGroup();
   TreeIterator it = pworkgroup->GetIterator(new GroupsNodeFilter());

   size_t i = 0;
   for (; it.IsValid() && sincronized; it.NextNode()) {
      sincronized = (i < managerids.size() &&
                        it.GetPath().GetLastPathNode()->GetContent() == managerids[i]);
      i++;
   }

   delete pworkgroup;

   return sincronized && i == managerids.size();
}

/**
 * Permite recorrer los nodos con contenido secuencialmente.
 * Busca el nodo siguiente a pCurrentContentNode dentro del grupo y lo retorna
 * en pNextContentNode.
 * NOTA: pNextContentNode nunca retorna pGroupNode, siempre es un subnodo o NULL.
 * @param[in] pGroupNode grupo padre donde quiero acotar la busqueda.
 * @param[in] pCurrentContentNode Nodo actual a partir de donde busco el siguiente
 * nodo con contenido. Si es NULL busco el primer nodo con contenido.
 * @param[out] pNextContentNode puntero al proximo nodo con contenido.
 * NULL si pCurrentContentNode es el ultimo nodo o pGroupNode no tiene nodos
 * con contenido.
 * @return true si tuvo exito(Incluso si pCurrentContentNode es el ultimo nodo).
 * @return false si ocurrio un error.
 */
bool SuriObjectGroupManager::GetNextContentNode(TreeNodeInterface* pGroupNode,
                                       TreeNodeInterface* pCurrentContentNode,
                                       TreeNodeInterface* &pNextContentNode) {
   TreeNodeInterface* pcurrentnode = pGroupNode;
   if (pCurrentContentNode != NULL)
      pcurrentnode = pCurrentContentNode;
   TreeIterator it(pGroupNode, pcurrentnode, new GroupsNodeFilter());
   if (pcurrentnode == it.GetPath().GetLastPathNode())
      it.NextNode();
   pNextContentNode = it.IsValid() ? it.GetPath().GetLastPathNode() : NULL;
   return true;
}

/**
 * Retorna el ultimo nodo con contenido dentro del grupo. Puede ser hijo directo
 * del grupo o estar contenido en un subgrupo.
 * @param[in] pGroupNode Grupo padre del que quiero encontrar el ultimo nodo
 * @return puntero al ultimo nodo con contenido.
 * @return NULL si no hay ningun subnodo con contenido.
 */
TreeNodeInterface* SuriObjectGroupManager::GetLastContentNode(
                                          TreeNodeInterface* pGroupNode) {
   TreeNodeInterface* lastcontentnode = NULL;
   TreeIterator it(pGroupNode, new GroupsNodeFilter());
   for (; it.IsValid(); it.NextNode())
      lastcontentnode = it.GetPath().GetLastPathNode();
   return lastcontentnode;
}

#ifdef __DEBUG__
/** Muestra por log el estado del arbol y la lista */
void SuriObjectGroupManager::LogState(const std::string &Header) {
   REPORT_DEBUG("---------------------- START %s ----------------------", Header.c_str());
   REPORT_DEBUG("START MANAGER ORDER");
   std::vector<SuriObject::UuidType> ids = pObjectManager_->GetOrderedIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   while (it != ids.end()) {
      REPORT_DEBUG("id: %s", it->c_str());
      it++;
   }
   REPORT_DEBUG("END MANAGER ORDER");

   REPORT_DEBUG("START TREE ORDER");
   TreeIterator treeit(pRoot_, NULL);
   TreeNodeInterface* pcurrentnode = NULL;
   for (; treeit.IsValid(); treeit.NextNode()) {
      pcurrentnode = treeit.GetPath().GetLastPathNode();
      if (pcurrentnode->AllowsChildren())
         REPORT_DEBUG("GRUPO: %s, PARENT: ", pcurrentnode->GetName().c_str(),
                      pcurrentnode->GetParent() ?
                           pcurrentnode->GetParent()->GetName().c_str() : "NULL");
      else
         REPORT_DEBUG("CONTENT: %s, PARENT: %s",
                      pcurrentnode->GetContent().c_str(),
                      pcurrentnode->GetParent() ?
                           pcurrentnode->GetParent()->GetName().c_str() : "NULL");
   }
   REPORT_DEBUG("END TREE ORDER");
   REPORT_DEBUG("---------------------- FIN %s ----------------------", Header.c_str());
}
#endif

} /* namespace suri */
