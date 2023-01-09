/** \file IndependentTreeSelectionManager.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INDEPENDENTTREESELECTIONMANAGER_H_
#define INDEPENDENTTREESELECTIONMANAGER_H_

// Includes standard
#include <set>
#include <list>
// Includes suri
#include "suri/TreeSelectionManagerInterface.h"
#include "suri/GroupManagerInterface.h"

namespace suri {

/**
 * Implementa TreeSelectionManagerInterface.
 * Guarda informacion de seleccion.
 */
class IndependentTreeSelectionManager : public TreeSelectionManagerInterface {
public:
   /** Ctor */
   explicit IndependentTreeSelectionManager(GroupManagerInterface* pGroupManager);
   /** Dtor */
   virtual ~IndependentTreeSelectionManager();

   /** Limpia la seleccion de grupos e items */
   virtual void ResetSelection();
   /** Permite modificar los nodos seleccionados */
   virtual void ChangeSelection(const NodePath &Path, bool Selected);
   /** Retorna el path de nodos seleccionados */
   virtual std::list<NodePath> GetSelectedNodes();

protected:
   /** Manager de grupos. */
   GroupManagerInterface* pGroupManager_;
   /** Def tipo de dato de lista de capas */
   typedef std::set<SuriObject::UuidType> NodeUuidSet;
   /** Lista de grupos seleccionados */
   NodeUuidSet selectedNodes_;
};

} /* namespace suri */
#endif /* INDEPENDENTTREESELECTIONMANAGER_H_ */
