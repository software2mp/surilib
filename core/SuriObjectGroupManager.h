/** \file SuriObjectGroupManager.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SURIOBJECTGROUPMANAGER_H_
#define SURIOBJECTGROUPMANAGER_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/GroupManagerInterface.h"
#include "suri/MutableWorkGroupInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

// fordward
class DefaultTreeNode;
class SuriObjectManagerInterface;

class SuriObjectGroupManager : public GroupManagerInterface {
public:
   /** Ctor */
   explicit SuriObjectGroupManager(SuriObjectManagerInterface* pObjectManager);
   /** Dtor */
   virtual ~SuriObjectGroupManager();

   /** Elimina el contenido del arbol. Solo queda el nodo raiz. */
   virtual void Clear();
   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual WorkGroupInterface* GetRootWorkGroup();
   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual MutableWorkGroupInterface* GetRootMutableWorkGroup();
   /** Agrega un grupo dentro de otro. */
   virtual NodePath InsertGroup(const std::string &GroupName,
                                    const NodePath &ParentGroup, int Index);
   /** Agrega un grupo dentro de otro. */
   virtual NodePath InsertContextGroup(const SuriObject::UuidType& ViewcontextId,
		   	   	   	   	   	   	   	   const std::string &GroupName,
		   	   	   	   	   	   	   	   const NodePath &ParentGroup, int Index, ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);
   /** Elimina un grupo del arbol */
   virtual bool RemoveGroup(const NodePath &Node);
   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

#ifdef __DEBUG__
   /** Muestra por log el estado del arbol y la lista */
   void LogState(const std::string &Header);
#endif

private:
   /** Mueve un elemento en el manager asociado */
   bool MoveItemContent(TreeNodeInterface* pItemNode, SuriObject::UuidType DestinationId);

   /** Valida que el orden de los items en arbol sea igual al del manager */
   bool ValidateOrder();
   /** Permite recorrer los nodos con contenido secuencialmente. */
   bool GetNextContentNode(TreeNodeInterface* pGroupNode,
                                    TreeNodeInterface* pCurrentContentNode,
                                    TreeNodeInterface* &pNextContentNode);
   /** Retorna el ultimo nodo con contenido dentro del grupo. */
   TreeNodeInterface* GetLastContentNode(TreeNodeInterface* pGroupNode);

   /** Nodo raiz del arbol */
   DefaultTreeNode* pRoot_;
   /** Manager asociado al arbol */
   SuriObjectManagerInterface* pObjectManager_;
   /** Id de observer en pObjectManager_ */
   int observerId_;
   /** Flag que se usa para ignorar las notificaciones. */
   bool ignoreNotification_;
};

} /* namespace suri */

#endif /* SURIOBJECTGROUPMANAGER_H_ */
