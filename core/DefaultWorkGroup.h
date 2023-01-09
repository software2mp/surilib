/** \file DefaultWorkGroup.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SURIOBJECTWORKGROUP_H_
#define SURIOBJECTWORKGROUP_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/MutableWorkGroupInterface.h"
#include "suri/ViewcontextInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

class TreeNodeInterface;
class SuriObjectManagerInterface;

class DefaultWorkGroup : public suri::MutableWorkGroupInterface {
public:
   /** Ctor */
   explicit DefaultWorkGroup(TreeNodeInterface* pRoot);
   /** Dtor */
   virtual ~DefaultWorkGroup();

   /** Retorna iterador del arbol */
   virtual TreeIterator GetIterator(NodeFilterInterface* pFilter = NULL) const;
   /** Retorna iterador del arbol */
   virtual TreeIterator GetIterator(const NodePath &GroupPath,
                                    NodeFilterInterface* pFilter = NULL) const;

   /** Recorre el grupo, subgrupos,... buscando el nodo con contenido igual a 'Id' */
   virtual NodePath SearchContent(const SuriObject::UuidType &Id) const;

   /** Recorre el arbol buscando el nodo con Id indicado. */
   virtual NodePath SearchNode(const SuriObject::UuidType &Id) const;

   /** Configura un NodePath que corresponde al string que recibe. */
   virtual NodePath StringToPath(const std::string &PathString) const;

   /**  Retorna string a partir del cual se puede reconstruir el NodePath. */
   virtual std::string PathToString(const NodePath &Path) const;

   /** Retorna el path al grupo raiz */
   virtual NodePath GetRootPath() const;

   /** Retorna el tamanio del grupo raiz */
   virtual int GetRootSize(ViewcontextInterface::ViewcontextType type = (ViewcontextInterface::ViewcontextType)0) const;

   /** Retorna el grupo raiz asoicado a instancia de WorkGroupInterface */
   virtual TreeNodeInterface* GetRoot();

   /** Para un nodo, retorna el path relativo al grupo raiz. */
   virtual NodePath GetPathToRoot(TreeNodeInterface* pNode) const;

   /** Agrega un grupo dentro de otro. */
   virtual NodePath InsertGroup(const std::string &GroupName,
                              const NodePath &ParentGroup, int Index);
   /** Agrega un grupo dentro de otro. */
   NodePath InsertContextGroup(
   		const SuriObject::UuidType& ViewcontextId, const std::string &GroupName,
   		const NodePath &ParentGroup, int Index, ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);

   /** Agrega item con id recibido dentro de un grupo. */
   virtual NodePath InsertObject(const SuriObject::UuidType &Object,
                              const NodePath &ParentGroup, int Index,
                              ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);

   /** Valida si se puede mover un nodo dentro de un grupo en el arbol */
   virtual bool ValidateMoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Mueve un nodo dentro de un grupo en el arbol */
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index);

   /** Elimina un nodo del arbol */
   virtual bool Remove(const NodePath &Node);

   /** Valida path contra la raiz */
   bool ValidatePath(const NodePath &Node);

private:
   /** Nodo raiz con el que trabaja SuriObjectWorkGroup */
   TreeNodeInterface* pRoot_;
};

} /* namespace suri */
#endif /* SURIOBJECTWORKGROUP_H_ */
