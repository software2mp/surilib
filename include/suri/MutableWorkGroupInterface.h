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

#ifndef MUTABLEWORKGROUPINTERFACE_H_
#define MUTABLEWORKGROUPINTERFACE_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/WorkGroupInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/**
 * Extiende WorkGroupInterface para que se pueda modificar la
 * estructura del arbol.
 */
class MutableWorkGroupInterface : public WorkGroupInterface {
public:
   /** Dtor */
   virtual ~MutableWorkGroupInterface() {}

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
   virtual NodePath InsertGroup(const std::string &GroupName,
                              const NodePath &ParentGroup, int Index) = 0;

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
   virtual NodePath InsertObject(const SuriObject::UuidType &Object,
                              const NodePath &ParentGroup, int Index,
                              ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D) = 0;

   /**
    * Valida si se puede mover un nodo dentro de un grupo en el arbol
    * @param[in] Node NodePath del nodo a mover
    * @param[in] NewParent NodePath nuevo padre
    * @param[in] NewIndex Posicion donde quiero mover el nodo.
    * Si NewIndex == pNewParent->Size(): Lo agrega al final.
    * Si NewIndex > pNewParent->Size(): Falla la operacion.
    * @return bool indica si es un movimiento valido
    */
   virtual bool ValidateMoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index) = 0;

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
   virtual NodePath MoveNode(const NodePath &Node,
                               const NodePath &NewParent, int Index) = 0;

   /**
    * Elimina un nodo del arbol
    * @param[in] Node NodePath del nodo a eliminar
    * @return bool que indica si pudo eliminarlo
    */
   virtual bool Remove(const NodePath &Node) = 0;
};

} /* namespace suri */

#endif /* MUTABLEWORKGROUPINTERFACE_H_ */
