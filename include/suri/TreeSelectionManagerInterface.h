/** \file TreeSelectionManagerInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef TREESELECTIONMANAGERINTERFACE_H_
#define TREESELECTIONMANAGERINTERFACE_H_

// Includes standard
#include <list>
// Includes suri
#include "NodePath.h"

namespace suri {

/**
 * Administra la seleccion de capas y grupos
 */
class TreeSelectionManagerInterface {
public:
   /** Dtor */
   virtual ~TreeSelectionManagerInterface() {}

   /**
    * Limpia la seleccion de grupos e items
    * @param[in] ViewcontextId Identificador del viewcontext
    */
   virtual void ResetSelection() = 0;

   /**
    * Permite modificar los nodos seleccionados
    * @param[in] Path NodePath del nodo
    * @param[in] Selected flag que indica si el nodo debe estar seleccionado
    * o no.
    */
   virtual void ChangeSelection(const NodePath &Path, bool Selected) = 0;

   /**
    * Retorna el path de nodos seleccionados
    * @return listado con el path de los nodos seleccionados
    */
   virtual std::list<NodePath> GetSelectedNodes() = 0;
};
} /* namespace suri */

#endif /* TREESELECTIONMANAGERINTERFACE_H_ */
