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

#ifndef SURIOBJECTTREENODE_H_
#define SURIOBJECTTREENODE_H_

// Includes standard
#include <string>
#include <list>
// Includes Suri
#include "suri/TreeNodeInterface.h"
#include "suri/ViewcontextInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/**
 * Implementa TreeNodeInterface.
 */
class DefaultTreeNode : public TreeNodeInterface {
public:

	typedef enum {
		ContextGenericType = 0, Context2DType, Context3DType, ContextPhotosType, ContextHtml,
		ContextMapType, ContextDocumentsType, ContextActivitiesType, ContextErrorType
    } ContextType;

	/** Ctor */
   explicit DefaultTreeNode(bool AllowsChildren);
   /** Dtor */
   virtual ~DefaultTreeNode();

   //  --------  METODOS DE TREENODEINTERFACE --------
   /** Agrega un nodo en la posicion indicada */
   virtual bool Insert(TreeNodeInterface* pNewNode, int Index);
   /** Mueve el nodo desde el padre actual al nodo indicado */
   virtual bool Move(TreeNodeInterface* pNewParent, int Index);
   /** Saca un nodo hijo del arbol. */
   virtual bool Remove(TreeNodeInterface* pNode);
   /** Elimina el nodo del grupo en la posicion indicada */
   virtual bool Remove(int Index, TreeNodeInterface* &pNode);
   /** Retorna el padre del nodo */
   virtual TreeNodeInterface* GetParent();
   /** Configura el nodo padre */
   virtual void SetParent(TreeNodeInterface* pNewParent);
   /** Retorna el nodo hijo en la posicion indicada */
   virtual TreeNodeInterface* GetChildAt(int Index);
   /** Indica si el nodo puede contener nodos hijo. */
   virtual bool AllowsChildren() const;
   /** Configura el id de SuriObject asociado al nodo. */
   virtual bool SetContent(SuriObject::UuidType NewContent);
   /** Retorna el id de SuriObject asociado al nodo. */
   virtual SuriObject::UuidType GetContent() const;
   /** Retorna la cantidad de subnodos que contiene el nodo. */
   virtual int GetSize() const;
   /** Retorna el nombre del nodo. */
   virtual std::string GetName() const;
   /** Configura el nombre del nodo. */
   virtual void SetName(const std::string &Name);
   /** Retorna el id de SuriObject asociado al nodo. */
   virtual std::vector<SuriObject::UuidType> GetAssociatedIds() const;
   /** Retorna el id de SuriObject asociado al nodo. */
   virtual void AddAssociatedId(const SuriObject::UuidType& Id);
   /** Devuelve el tipo de contexto */
   virtual ViewcontextInterface::ViewcontextType GetContextType();

   /** Setea el tipo de contexto */
   virtual void SetContextType(ViewcontextInterface::ViewcontextType ctype);

   /** Devuelve la cantidad de subnodos que contiene el nodo,
    * pero filtrado por contextos */
   virtual int GetContextNodesSize(ViewcontextInterface::ViewcontextType ctype,
			std::map<int, std::vector<std::string> > elementsAndContexts, bool boutside);

private:
   /** Listado con los hijos de este nodo */
   std::list<TreeNodeInterface*> childNodes_;
   /** Nodo padre. Si es NULL es el nodo raiz. */
   TreeNodeInterface* pParent_;
   /** Flag que indica si el nodo permite hijos */
   bool allowsChildren_;

   /**
    * Contenido del nodo. Id del SuriObject al que hace referencia. Solo es
    * valido si es nodo hoja.
    */
   SuriObject::UuidType nodeContent_;

   /**
    * Contenido del nodo. Id del SuriObject al que hace referencia. Solo es
    * valido si es nodo hoja.
    */
   std::vector<SuriObject::UuidType> associatedIds_;

   ViewcontextInterface::ViewcontextType ctype_;

   /** Nombre asignado al nodo. */
   std::string name_;
};

} /* namespace suri */

#endif /* SURIOBJECTTREENODE_H_ */
