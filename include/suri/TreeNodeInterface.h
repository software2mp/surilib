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

#ifndef GROUPINTERFACE_H_
#define GROUPINTERFACE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/DatasourceManagerInterface.h"
#include "suri/SuriObject.h"
#include "suri/ViewcontextInterface.h"

namespace suri {

/**
 * Representa un nodo dentro del arbol.
 * Hereda de SuriObject para tener su propio Id.
 * Importante: existe una distincion entre el Id del nodo y el id del
 * contenido, este ultimo esta asociado a otro objeto(por ejemplo un
 * DatasourceInterface).
 */
class TreeNodeInterface : public SuriObject {
public:
   /** Dtor */
   virtual ~TreeNodeInterface() { }

   /**
    * Agrega un nodo en la posicion indicada
    * @param[in] pNewNode nodo que quiero agregar. El nodo
    * recibido pasa a ser responsabilidad de esta clase, no eliminar.
    * @param[in] Index Posicion donde quiero agregar el nodo.
    * @return bool que indica si pudo agregar el nodo.
    */
   virtual bool Insert(TreeNodeInterface* pNewNode, int Index) = 0;

   /**
    * Mueve el nodo desde el padre actual al nodo indicado
    * @param[in] pNewParent TreeNodeInterface donde quiero mover el nodo.
    * @param[in] Index Posicion donde quiero agregar el nodo.
    * @return bool que indica si pudo mover el nodo.
    */
   virtual bool Move(TreeNodeInterface* pNewParent, int Index) = 0;

   /**
    * Saca un nodo hijo del arbol. El puntero pasa a ser responsabilidad
    * del invocante.
    * @param[in] pNode TreeNodeInterface que quiero remover
    * @return bool que indica si pudo remover el nodo
    */
   virtual bool Remove(TreeNodeInterface* pNode) = 0;

   /**
    * Elimina el nodo del grupo en la posicion indicada
    * @param[in] Index posicion del nodo a eliminar
    * @param[out] pNode TreeNodeInterface removido. El puntero
    * pasa a ser responsabilidad del invocante.
    * @return bool que indica si pudo remover el nodo
    */
   virtual bool Remove(int Index, TreeNodeInterface* &pNode) = 0;

   /**
    * Retorna el padre del nodo
    * @return TreeNodeInterface* nodo padre. El nodo retornado
    * es parte del arbol, no eliminar.
    * @return NULL si es nodo raiz.
    */
   virtual TreeNodeInterface* GetParent() = 0;

   /**
    * Configura el nodo padre
    * @param[in] pNewParent nuevo padre del nodo.
    */
   virtual void SetParent(TreeNodeInterface* pNewParent) = 0;

   /**
    * Retorna el nodo hijo en la posicion indicada
    * @param[in] Index posicion del nodo buscado
    * @return TreeNodeInterface nodo buscado. El nodo retornado
    * es responsabilidad de esta clase, no eliminar.
    */
   virtual TreeNodeInterface* GetChildAt(int Index) = 0;

   /**
    * Indica si el nodo puede contener nodos hijo.
    * @return bool que indica si puede tener nodos hijo
    */
   virtual bool AllowsChildren() const = 0;

   /**
    * Configura el id de SuriObject asociado al nodo.
    * @param[in] NewContent Nuevo id asociado al nodo
    * @return bool que informa si tuvo exito
    */
   virtual bool SetContent(SuriObject::UuidType NewContent) = 0;

   /**
    * Retorna el id de SuriObject asociado al nodo.
    * @return SuriObject::UuidType id de SuriObject asociado al nodo.
    */
   virtual SuriObject::UuidType GetContent() const = 0;

   /**
    * Retorna la cantidad de subnodos que contiene el nodo.
    * @return int cantidad de subnodos
    */
   virtual int GetSize() const= 0;

   /**
    * Retorna el nombre del nodo.
    * @return std::string con nombre del nodo
    */
   virtual std::string GetName() const = 0;

   /**
    * Configura el nombre del nodo.
    * @param[in] Name string con el nuevo nombre del nodo
    */
   virtual void SetName(const std::string &Name) = 0;


   /** Devuelve el tipo de contexto */
   virtual ViewcontextInterface::ViewcontextType GetContextType() = 0;

   /** Setea el tipo de contexto */
   virtual void SetContextType(ViewcontextInterface::ViewcontextType ctype) = 0;

   /** Devuelve la cantidad de subnodos que contiene el nodo,
    * pero filtrado por contextos */
   virtual int GetContextNodesSize(ViewcontextInterface::ViewcontextType ctype,
			std::map<int, std::vector<std::string> > elementsAndContexts, bool boutside) = 0;

};

} /* namespace suri */
#endif /* GROUPINTERFACE_H_ */
