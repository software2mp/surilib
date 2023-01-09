/** \file GuiTreeModel.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUITREEMODEL_H_
#define GUITREEMODEL_H_

// Includes standard
#include <set>
#include <list>

// Includes Suri
#include "suri/NodePath.h"
#include "suri/WorkGroupInterface.h"

namespace suri {

/**
 * Clase con modelo del arbol a mostrar, esto incluye la estructura y la
 * informacion visual. Permite obtener la raiz del arbol, que grupos estan
 * expandidos, que nodos estan inhabilitados y la seleccion.
 */
class GuiTreeModel {
public:
   /** Ctor */
   GuiTreeModel();
   /** Dtor */
   virtual ~GuiTreeModel();

   /** Retorna instancia de WorkGroupInterface que referencia la raiz del arbol. */
   virtual WorkGroupInterface* GetRootWorkGroup() = 0;
   /** Retorna iterador del arbol completo */
   virtual TreeIterator GetIterator() const = 0;
   /** Retorna iterador del arbol debajo de GroupPath */
   virtual TreeIterator GetIterator(const NodePath &GroupPath) const = 0;
   /** Retorna iterador a las hojas del arbol completo */
   virtual TreeIterator GetLeafIterator() const = 0;
   /** Retorna iterador a las hojas del arbol debajo de GroupPath */
   virtual TreeIterator GetLeafIterator(const NodePath &GroupPath) const = 0;
   /** Recorre el arbol buscando el nodo con Id indicado. */
   virtual NodePath SearchNode(const SuriObject::UuidType &Id) const = 0;
   /** Para un nodo, retorna el path relativo al grupo raiz. */
   virtual NodePath GetPathToRoot(TreeNodeInterface* pNode) const = 0;

   /** Mueve un nodo al final del arbol. */
   virtual NodePath MoveNodeToEnd(const NodePath &Node) = 0;
   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index) = 0;

   /** Permite modificar los nodos seleccionados */
   virtual void ChangeSelection(const NodePath &Path, bool Selected) = 0;
   /** Retorna el path de nodos seleccionados */
   virtual std::list<NodePath> GetSelectedNodes() = 0;
   /** Limpia la seleccion de nodos(items y grupos) */
   virtual void ResetSelection() = 0;

   /** Expande/contrae un grupo en interfaz grafica */
   virtual void ExpandGroup(const NodePath &Group, bool Expand);
   /** Indica si un grupo esta expandido */
   virtual bool IsExpanded(const NodePath &Group);
   /** Indica si un nodo es visible en interfaz */
   virtual bool IsVisible(const NodePath &Node);

   /** Habilita/Deshabilita un nodo */
   virtual void Enable(const NodePath &Node, bool Enabled);
   /** Indica si un nodo esta habilitado */
   virtual bool IsEnabled(const NodePath &Node);

protected:
   /** Elimina grupos desplegados que no esten en el modelo */
   void RefreshModel();
   /** Limpia los id invalidos del conjunto */
   void RemoveInvalidItems(std::set<SuriObject::UuidType> &WorkingSet);

   /** Listado con grupos expandidos */
   std::set<SuriObject::UuidType> expandedGroups_;
   /** Listado con grupos inhabilitados */
   std::set<SuriObject::UuidType> disabledGroups_;
};

} /* namespace suri */
#endif /* GUITREEMODEL_H_ */
