/** \file WorkingGroupInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WORKINGGROUPINTERFACE_H_
#define WORKINGGROUPINTERFACE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/TreeNodeInterface.h"
#include "suri/NodePath.h"
#include "suri/TreeIterator.h"

namespace suri {

/**
 * Interfaz que simplifica la interaccion con el arbol.
 * Cualquier grupo puede ser la raiz de un WorkGroupInterface
 */
class WorkGroupInterface {
public:
   /** Dtor */
   virtual ~WorkGroupInterface() {}

   /**
    * Retorna iterador del arbol
    * @param[in] pFilter Filtro que debe aplicar el iterador
    * @return TreeIterator iterador que apunta a la raiz del arbol
    */
   virtual TreeIterator GetIterator(NodeFilterInterface* pFilter = NULL) const = 0;
   /**
    * Retorna iterador del arbol
    * @param[in] GroupPath Subgrupo que debe recorrer el iterador
    * @param[in] pFilter Filtro que debe aplicar el iterador
    * @return TreeIterator iterador que apunta a la raiz del arbol
    */
   virtual TreeIterator GetIterator(const NodePath &GroupPath,
                              NodeFilterInterface* pFilter = NULL) const = 0;

   /**
    * Recorre el grupo, subgrupos,... buscando el nodo con contenido igual a 'Id'
    * @param[in] Id SuriObject::UuidType del Objeto que busco
    * @return NodePath path del nodo asociado al objeto
    * @return NodePath invalido si no lo encuentra
    */
   virtual NodePath SearchContent(const SuriObject::UuidType &Id) const = 0;

   /**
    * Recorre el arbol buscando el nodo con Id indicado. El id corresponde
    * al TreeNode y no a su contenido.
    * @param[in] Id SuriObject::UuidType del TreeNodo que busco
    * @return NodePath path del nodo con id buscado
    * @return NodePath si no lo encuentra
    */
   virtual NodePath SearchNode(const SuriObject::UuidType &Id) const = 0;

   /**
    * Configura un NodePath que corresponde al string que recibe.
    * @param[in] PathString string con informacion del path.
    * @return NodePath path del nodo que referencia PathString
    * @return NodePath invalido si el string es invalido o no existe el path.
    */
   virtual NodePath StringToPath(const std::string &PathString) const = 0;

   /**
    * Retorna string a partir del cual se puede reconstruir
    * el NodePath.
    * @param[in] Path NodePath que quiero convertir a string.
    * @return string con informacion de path
    */
   virtual std::string PathToString(const NodePath &Path) const = 0;

   /**
    * Retorna el grupo raiz asoicado a instancia de WorkGroupInterface
    * @return TreeNodeInterface* grupo raiz. El puntero referencia
    * parte del arbol, no eliminar.
    */
   virtual TreeNodeInterface* GetRoot() = 0;

   /**
    * Para un nodo, retorna el path relativo al grupo raiz.
    * @param[in] pNode TreeNodeInterface del que quiero el path.
    * @return NodePath a nodo.
    * @return NodePath invalido si no se pudo crear.
    */
   virtual NodePath GetPathToRoot(TreeNodeInterface* pNode) const = 0;

   /**
    * Retorna el path al grupo raiz
    * @return NodePath del grupo raiz.
    */
   virtual NodePath GetRootPath() const = 0;

   /**
    * Retorna el tamanio del grupo raiz
    * @return tamanio del grupo raiz.
    */
   virtual int GetRootSize(ViewcontextInterface::ViewcontextType type = (ViewcontextInterface::ViewcontextType)0) const = 0;
};

} /* namespace suri */
#endif /* WORKINGGROUPINTERFACE_H_ */
