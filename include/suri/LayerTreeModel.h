/** \file LayerTreeModel.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERTREEMODEL_H_
#define LAYERTREEMODEL_H_

// Includes standard
#include <list>

// Includes suri
#include "suri/GuiTreeModel.h"
#include "suri/TreeSelectionManagerInterface.h"
#include "suri/GroupManagerInterface.h"

namespace suri {

/**
 * Adapta GuiTreeModel para que trabaje con capas y grupos de proyecto.
 */
class LayerTreeModel : public GuiTreeModel {
public:
   /** Ctor */
   LayerTreeModel(GroupManagerInterface* pGroupManager,
                        TreeSelectionManagerInterface* pTreeSelectionManager);
   /** Dtor */
   virtual ~LayerTreeModel();

   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual WorkGroupInterface* GetRootWorkGroup();
   /** Retorna iterador del arbol completo */
   virtual TreeIterator GetIterator() const;
   /** Retorna iterador del arbol debajo de GroupPath */
   virtual TreeIterator GetIterator(const NodePath &GroupPath) const;
   /** Retorna iterador a las hojas del arbol completo */
   virtual TreeIterator GetLeafIterator() const;
   /** Retorna iterador a las hojas del arbol debajo de GroupPath */
   virtual TreeIterator GetLeafIterator(const NodePath &GroupPath) const;
   /** Recorre el arbol buscando el nodo con Id indicado. */
   virtual NodePath SearchNode(const SuriObject::UuidType &Id) const;
   /** Para un nodo, retorna el path relativo al grupo raiz. */
   virtual NodePath GetPathToRoot(TreeNodeInterface* pNode) const;

   /** Mueve un nodo al final del arbol. */
   virtual NodePath MoveNodeToEnd(const NodePath &Node);
   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Permite modificar los nodos seleccionados */
   virtual void ChangeSelection(const NodePath &Path, bool Selected);
   /** Retorna el path de nodos seleccionados */
   virtual std::list<NodePath> GetSelectedNodes();
   /** Limpia la seleccion de nodos(items y grupos) */
   virtual void ResetSelection();

   /** Configura un filtro para las capas del arbol */
   void SetFilter(NodeFilterInterface* pFilter);

protected:
   /** Manager de grupos. */
   GroupManagerInterface* pGroupManager_;
   /** Interfaz que permite interactuar con items */
   TreeSelectionManagerInterface* pTreeSelectionManager_;

   /** Filtro que se le debe aplicar al arbol */
   NodeFilterInterface* pFilter_;
};

} /* namespace suri */
#endif /* LAYERTREEMODEL_H_ */
