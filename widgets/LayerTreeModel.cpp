/** \file LayerTreeModel.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <list>

// Includes suri
#include "suri/LayerTreeModel.h"
#include "logmacros.h"
#include "GroupsNodeFilter.h"

namespace suri {

/** Ctor */
LayerTreeModel::LayerTreeModel(GroupManagerInterface* pGroupManager,
                     TreeSelectionManagerInterface* pTreeSelectionManager) {
   pGroupManager_ = pGroupManager;
   pTreeSelectionManager_ = pTreeSelectionManager;
   pFilter_ = NULL;
}

/** Dtor */
LayerTreeModel::~LayerTreeModel() {
}

/**
 * Retorna instancia de WorkGroupInterface que referencia la raiz del arbol.
 * @return WorkGroupInterface configurado con grupo raiz.
 * \attention El puntero retornado es responsabilidad del invocante.
 */
WorkGroupInterface* LayerTreeModel::GetRootWorkGroup() {
   return pGroupManager_->GetRootWorkGroup();
}

/** Retorna iterador del arbol completo */
TreeIterator LayerTreeModel::GetIterator() const {
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   TreeIterator it = pworkgroup->GetIterator(pFilter_);
   delete pworkgroup;
   return it;
}

/**
 * Retorna iterador del arbol
 * @param[in] pFilter Filtro que debe aplicar el iterador
 * @return TreeIterator iterador que apunta a la raiz del arbol
 */
TreeIterator LayerTreeModel::GetIterator(const NodePath &GroupPath) const {
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   TreeIterator it = pworkgroup->GetIterator(GroupPath, pFilter_);
   delete pworkgroup;
   return it;
}

/** Retorna iterador a las hojas del arbol completo */
TreeIterator LayerTreeModel::GetLeafIterator() const {
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   TreeIterator it = pworkgroup->GetIterator(new GroupsNodeFilter(pFilter_));
   delete pworkgroup;
   return it;
}

/**
 * Retorna iterador del arbol
 * @param[in] pFilter Filtro que debe aplicar el iterador
 * @return TreeIterator iterador que apunta a la raiz del arbol
 */
TreeIterator LayerTreeModel::GetLeafIterator(const NodePath &GroupPath) const {
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   TreeIterator it = pworkgroup->GetIterator(GroupPath,
                                        new GroupsNodeFilter(pFilter_));
   delete pworkgroup;
   return it;
}

/**
 * Recorre el arbol buscando el nodo con Id indicado. El id corresponde
 * al TreeNode y no a su contenido.
 * @param[in] Id SuriObject::UuidType del TreeNodo que busco
 * @return NodePath path del nodo con id buscado
 * @return NodePath invalido si no lo encuentra
 */
NodePath LayerTreeModel::SearchNode(const SuriObject::UuidType &Id) const {
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   NodePath path = pworkgroup->SearchNode(Id);
   delete pworkgroup;
   return path;
}

/**
 * Para un nodo, retorna el path relativo al grupo raiz.
 * @param[in] pNode TreeNodeInterface del que quiero el path.
 * @return NodePath a nodo.
 * @return NodePath invalido si no se pudo crear.
 */
NodePath LayerTreeModel::GetPathToRoot(TreeNodeInterface* pNode) const {
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   NodePath path = pworkgroup->GetPathToRoot(pNode);
   delete pworkgroup;
   return path;
}

/**
 * Mueve un nodo al final del arbol.
 * @param[in] Node NodePath del nodo a mover
 * @return NodePath del nodo despues de moverse
 * @return NodePath invalido si no lo pudo mover
 */
NodePath LayerTreeModel::MoveNodeToEnd(const NodePath &Node) {
   if (pFilter_) {
      REPORT_DEBUG("D: Se intento mover un nodo en un arbol filtrado");
      return NodePath(NULL, NULL);
   }

   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   NodePath rootpath = pworkgroup->GetRootPath();
   int index = pworkgroup->GetRootSize();
   NodePath path = pGroupManager_->MoveNode(Node, rootpath, index);
   delete pworkgroup;
   return path;
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
NodePath LayerTreeModel::MoveNode(const NodePath &Node,
                            const NodePath &NewParent, int Index) {
   /**
    * TODO(Gabriel - TCK #5727): Ver porque no se podía mover cuando esta
    * filtrado el arbol. Por ahora se comenta el filtro para que funcione
   if (pFilter_) {
      REPORT_DEBUG("D: Se intento mover un nodo en un arbol filtrado");
      return NodePath(NULL, NULL);
   }
   **/
   return pGroupManager_->MoveNode(Node, NewParent, Index);
}

/**
 * Permite modificar los nodos seleccionados
 * @param[in] Path path del nodo
 * @param[in] Selected flag que indica si el nodo debe estar seleccionado o no.
 */
void LayerTreeModel::ChangeSelection(const NodePath &Path, bool Selected) {
   pTreeSelectionManager_->ChangeSelection(Path, Selected);
}

/**
 * Retorna el path de nodos seleccionados
 * @return listado con nodos seleccionados
 */
std::list<NodePath> LayerTreeModel::GetSelectedNodes() {
   if (pTreeSelectionManager_)
      return pTreeSelectionManager_->GetSelectedNodes();
   std::list<NodePath> selectlist;
   return selectlist;
}

/**
 * Limpia la seleccion de nodos(items y grupos)
 */
void LayerTreeModel::ResetSelection() {
   pTreeSelectionManager_->ResetSelection();
}

/**
 * Configura un filtro para las capas del arbol
 * @param[in] pFilter filtro
 */
void LayerTreeModel::SetFilter(NodeFilterInterface* pFilter) {
   pFilter_ = pFilter;
}

} /* namespace suri */