/** \file ViewcontextTreeSelectionManager.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
