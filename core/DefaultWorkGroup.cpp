/** \file DefaultWorkGroup.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
// Includes Suri
#include "DefaultWorkGroup.h"
#include "TreeNodeAuxiliaryFunctions.h"
#include "logmacros.h"
#include "DefaultTreeNode.h"
#include "GroupsNodeFilter.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

DefaultWorkGroup::DefaultWorkGroup(TreeNodeInterface* pRoot) {
   pRoot_ = pRoot;
}

DefaultWorkGroup::~DefaultWorkGroup() {
}

/**
 * Retorna iterador del arbol
 * @param[in] pFilter Filtro que debe aplicar el iterador
 * @return TreeIterator iterador que apunta a la raiz del arbol
 */
TreeIterator DefaultWorkGroup::GetIterator(NodeFilterInterface* pFilter) const {
   return GetIterator(GetRootPath(), pFilter);
}

/**
 * Retorna iterador del arbol
 * @param[in] pFilter Filtro que debe aplicar el iterador
 * @return TreeIterator iterador que apunta a la raiz del arbol
 */
TreeIterator DefaultWorkGroup::GetIterator(const NodePath &GroupPath,
                                         NodeFilterInterface* pFilter) const {
   if (!GroupPath.IsValid()) {
      REPORT_DEBUG("D: Se llamo a GetIterator con path invalido");
      return TreeIterator(pRoot_, pFilter);
   }
   return TreeIterator(GroupPath.GetLastPathNode(), pFilter);
}

/**
 * Recorre el grupo, subgrupos,... buscando el nodo con contenido igual a 'Id'
 * @param[in] Id SuriObject::UuidType del Objeto que busco
 * @return NodePath path del nodo asociado al objeto
 * @return NodePath invalido si no lo encuentra
 */
NodePath DefaultWorkGroup::SearchContent(const SuriObject::UuidType &Id) const {
   GroupsNodeFilter filter;
   TreeIterator it = GetIterator(&filter);
   NodePath result(NULL, NULL);
   for (; !result.IsValid() && it.IsValid(); it.NextNode()) {
      if (it.GetPath().GetLastPathNode()->GetContent() == Id)
         result = it.GetPath();
   }
   return result;
}

/**
 * Recorre el arbol buscando el nodo con Id indicado. El id corresponde
 * al TreeNode y no a su contenido.
 * @param[in] Id SuriObject::UuidType del TreeNodo que busco
 * @return NodePath path del nodo con id buscado
 * @return NodePath si no lo encuentra
 */
NodePath DefaultWorkGroup::SearchNode(const SuriObject::UuidType &Id) const {
   TreeIterator it = GetIterator();
   NodePath result(NULL, NULL);
   for (; !result.IsValid() && it.IsValid(); it.NextNode()) {
      if (it.GetPath().GetLastPathNode()->GetId() == Id)
         result = it.GetPath();
   }
   return result;
}

/**
 * Configura un NodePath que corresponde al string que recibe.
 * @param[in] PathString string con informacion del path.
 * @return NodePath path del nodo que referencia PathString
 * @return NodePath invalido si el string es invalido o no existe el path.
 */
NodePath DefaultWorkGroup::StringToPath(const std::string &PathString) const {
   return NodePath(NULL, NULL);
}

/**
 * Retorna string a partir del cual se puede reconstruir
 * el NodePath.
 * @param[in] Path NodePath que quiero convertir a string.
 * @return string con informacion de path
 */
std::string DefaultWorkGroup::PathToString(const NodePath &Path) const {
   return "";
}

/**
 * Retorna el path al grupo raiz
 * @return NodePath del grupo raiz.
 */
NodePath DefaultWorkGroup::GetRootPath() const {
   return GetPathToRoot(pRoot_);
}

/**
 * Retorna el tamanio del grupo raiz
 * @return tamanio del grupo raiz.
 */
int DefaultWorkGroup::GetRootSize(
		ViewcontextInterface::ViewcontextType type) const {
	return pRoot_->GetSize();
}

/**
 * Retorna el grupo raiz asoicado a instancia de WorkGroupInterface
 * @return TreeNodeInterface* grupo raiz. El puntero referencia
 * parte del arbol, no eliminar.
 */
TreeNodeInterface* DefaultWorkGroup::GetRoot() {
   return pRoot_;
}

/**
 * Para un nodo, retorna el path relativo al grupo raiz.
 * @param[in] pNode TreeNodeInterface del que quiero el path.
 * @return NodePath a nodo.
 * @return NodePath invalido si no se pudo crear.
 */
NodePath DefaultWorkGroup::GetPathToRoot(TreeNodeInterface* pNode) const {
   return NodePath(pNode, pRoot_);
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
NodePath DefaultWorkGroup::InsertGroup(const std::string &GroupName,
                                    const NodePath &ParentGroup, int Index) {
   TreeNodeInterface* pparent = ParentGroup.GetLastPathNode();
   DefaultTreeNode* pnewgroup = new DefaultTreeNode(true);
   pnewgroup->SetName(GroupName);
   pparent->Insert(pnewgroup, Index);
   return NodePath(pnewgroup, pRoot_);
}

/** Agrega un grupo dentro de otro. */
NodePath DefaultWorkGroup::InsertContextGroup(
		const SuriObject::UuidType& ViewcontextId, const std::string &GroupName,
		const NodePath &ParentGroup, int Index, ViewcontextInterface::ViewcontextType ctype) {
	   TreeNodeInterface* pparent = ParentGroup.GetLastPathNode();
	   DefaultTreeNode* pnewgroup = new DefaultTreeNode(true);
	   pnewgroup->SetName(GroupName);
	   pnewgroup->SetContextType(ctype);
	   pnewgroup->AddAssociatedId(ViewcontextId);
	   pparent->Insert(pnewgroup, Index);
	   return NodePath(pnewgroup, pRoot_);
}

/**
 * Agrega item que contiene id recibido dentro de un grupo. El nodo
 * no permitira hijos(AllowsChildren == false)
 * @param[in] Object Id del objeto que quiero referenciar.
 * @param[in] ParentGroup NodePath del grupo donde quiero
 * agregar el item
 * @param[in] Index Posicion donde quiero agregar el item.
 * Valores posibles: 0 <= Index <= pParent->Size()
 * Si Index == pParent->Size(): Lo agrega al final.
 * Si Index > pParent->Size(): Falla la operacion.
 * @return NodePath del grupo creado
 * @return NodePath invalido si no pudo crear/agregar el item
 */
NodePath DefaultWorkGroup::InsertObject(const SuriObject::UuidType &Object,
                                   const NodePath &ParentGroup, int Index,
                                   ViewcontextInterface::ViewcontextType ctype) {
   TreeNodeInterface* pparent = ParentGroup.GetLastPathNode();
   DefaultTreeNode* pnewgroup = new DefaultTreeNode(false);
   pnewgroup->SetContent(Object);
   pnewgroup->SetContextType(ctype);
   pparent->Insert(pnewgroup, Index);
   return NodePath(pnewgroup, pRoot_);
}

/** Valida si se puede mover un nodo dentro de un grupo en el arbol */
bool DefaultWorkGroup::ValidateMoveNode(const NodePath &Node,
                            const NodePath &NewParent, int Index) {
   TreeNodeInterface* pnode = Node.GetLastPathNode();
   if (!pnode)
      return false;
   TreeNodeInterface* pparent = pnode->GetParent();
   TreeNodeInterface* pnewparent = NewParent.GetLastPathNode();
   if (!pparent || !pnewparent || !ValidatePath(Node) || Index < 0 ||
            !ValidatePath(NewParent) || pnewparent->GetSize() < Index)
      return false;

   int count = NewParent.GetPathCount();
   for (int i = 0; i < count; i++) {
      if (NewParent.GetPathNode(i) == pnode)
         return false;
   }
   return true;
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
NodePath DefaultWorkGroup::MoveNode(const NodePath &Node,
                            const NodePath &NewParent, int Index) {
   if (!ValidateMoveNode(Node, NewParent, Index))
      return NodePath(NULL, NULL);

   TreeNodeInterface* pnode = Node.GetLastPathNode();
   TreeNodeInterface* pparent = pnode->GetParent();
   TreeNodeInterface* pnewparent = NewParent.GetLastPathNode();

   // Si lo muevo al mismo padre recalculo index donde agregarlo
   int index = Index;
   if (pparent == pnewparent  && GetIndexOfChild(pparent, pnode) < Index)
      index--;

   if (!pparent->Remove(pnode) || !pnewparent->Insert(pnode, index)) {
      REPORT_DEBUG("D: El arbol esta en un estado invalido");
      return NodePath(NULL, NULL);
   }

   return NodePath(pnode, pRoot_);
}

/**
 * Elimina un nodo del arbol
 * @param[in] Node NodePath del nodo a eliminar
 * @return bool que indica si pudo eliminarlo
 */
bool DefaultWorkGroup::Remove(const NodePath &Node) {
   TreeNodeInterface* pnode = Node.GetLastPathNode();
   if (!ValidatePath(Node) || !pnode || !pnode->GetParent())
      return false;

   if (!pnode->GetParent()->Remove(pnode)) {
      REPORT_DEBUG("D: El arbol esta en un estado invalido");
      return false;
   }

   delete pnode;
   return true;
}

/** 
 * Valida path contra la raiz 
 * @param[in] Node path a validar
 * @return bool con resultado de la validacion
 */
bool DefaultWorkGroup::ValidatePath(const NodePath &Node) {
   bool validpath = Node.IsValid();

   NodePath testpath(Node.GetLastPathNode(), pRoot_);
   validpath = (Node.GetPathCount() == testpath.GetPathCount());
   for (int i = 0; validpath && i < Node.GetPathCount(); i++)
      validpath = (Node.GetPathNode(i) == testpath.GetPathNode(i));
   return validpath;
}


} /* namespace suri */
