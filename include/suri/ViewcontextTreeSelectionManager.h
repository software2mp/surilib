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

#ifndef VIEWCONTEXTTREESELECTIONMANAGER_H_
#define VIEWCONTEXTTREESELECTIONMANAGER_H_

// Includes standar
#include <list>
// Includes suri
#include "suri/TreeSelectionManagerInterface.h"
#include "suri/NodePath.h"

namespace suri {

class GroupManagerInterface;
class ViewcontextManagerInterface;

/**
 * Implementa TreeSelectionManagerInterface. 
 * Guarda informacion de seleccion de grupos y propaga seleccion 
 * de capas a listado de elementos.
 */
class ViewcontextTreeSelectionManager : public TreeSelectionManagerInterface {
public:
   /** Ctor */
   ViewcontextTreeSelectionManager(GroupManagerInterface* pGroupManager,
                              ViewcontextManagerInterface* pViewcontextManager);
   /** Dtor */
   virtual ~ViewcontextTreeSelectionManager();

   /** Limpia la seleccion de nodos(items y grupos) */
   virtual void ResetSelection();

   /**
    * Permite modificar los nodos seleccionados
    * @param[in] Path NodePath del nodo
    * @param[in] Selected flag que indica si el nodo debe estar seleccionado
    * o no.
    */
   virtual void ChangeSelection(const NodePath &Path, bool Selected);

   /**
    * Retorna el path de nodos seleccionados
    * @return listado con el path de los nodos seleccionados
    */
   virtual std::list<NodePath> GetSelectedNodes();

private:
   /** Cambia estado de seleccion de un grupo */
   void SelectGroup(const SuriObject::UuidType &GroupId, bool State);
   /** Busca los grupos seleccionados en modelo y elimina los que no encuentra */
   void UpdateSelectedGroupsWithModel();

   /** Manager de capas. */
   ViewcontextManagerInterface* pViewcontextManager_;
   /** Manager de grupos. */
   GroupManagerInterface* pGroupManager_;
   /** Def tipo de dato de lista de capas */
   typedef std::list<SuriObject::UuidType> GroupUuidList;
   /** Lista de grupos seleccionados */
   GroupUuidList selectedGroupsList_;
};

} /* namespace suri */
#endif /* VIEWCONTEXTTREESELECTIONMANAGER_H_ */
