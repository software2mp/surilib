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
