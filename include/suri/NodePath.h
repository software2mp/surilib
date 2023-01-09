/** \file GroupPath.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GROUPPATH_H_
#define GROUPPATH_H_

// Includes standard
#include <vector>

// Includes suri
#include "suri/TreeNodeInterface.h"

namespace suri {

/**
 * Permite recorrer los grupos que contienen a un nodo. El path puede ser
 * relativo a cualquiera de los grupos.
 */
class NodePath {
public:
   /** Ctor de Copia */
   NodePath(const NodePath &Path);
   /** Ctor */
   NodePath(TreeNodeInterface* pLastNode, TreeNodeInterface* pRoot);
   /** Dtor */
   ~NodePath();

   /** Retorna path al nodo en posicion indicada. */
   NodePath GetPathToParentNode(int Index) const;
   /** Retorna el nodo en posicion indicada de path. */
   TreeNodeInterface* GetPathNode(int Index) const;
   /** Retorna el nodo al que dirige el path. */
   TreeNodeInterface* GetLastPathNode() const;
   /** Retorna la cantidad de nodos en path. */
   int GetPathCount() const;
   /** Informa si el path es valido */
   bool IsValid() const;
   /** Informa si el nodo al que referencia el path puede tener hijos */
   bool AllowsChildren() const;

   /** Compara dos paths */
   bool Compare(const NodePath &OtherPath) const;

   /** Compara dos paths. */
   bool operator<(const NodePath &OtherPath) const;

private:
   /** Vector con nodos en path */
   std::vector<TreeNodeInterface*> pathContent_;
};

} /* namespace suri */

#endif /* GROUPPATH_H_ */
