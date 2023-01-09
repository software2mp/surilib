/** \file GroupManagerInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef GROUPMANAGERINTERFACE_H_
#define GROUPMANAGERINTERFACE_H_

// Includes standard
#include <string>
#include <list>
// Includes Suri
#include "suri/WorkGroupInterface.h"
#include "suri/NodePath.h"
#include "suri/ObserverInterface.h"
#include "suri/Subject.h"
#include "suri/ViewcontextInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/**
 * Es el punto de entrada a la estructura de grupos.
 */
class GroupManagerInterface : public Subject, public ObserverInterface {
public:
   /** Dtor */
   virtual ~GroupManagerInterface() {}

   /**
    * Elimina el contenido del arbol. Solo queda el nodo raiz.
    */
   virtual void Clear() = 0;

   /**
    * Retorna instancia de WorkGroupInterface que referencia la
    * raiz del arbol.
    * @return WorkGroupInterface configurado con grupo raiz
    */
   virtual WorkGroupInterface* GetRootWorkGroup() = 0;

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
   /** Agrega un grupo dentro de otro. */
   virtual NodePath InsertContextGroup(const SuriObject::UuidType& ViewcontextId,
		   	   	   	   	   	   	   	   const std::string &GroupName,
		   	   	   	   	   	   	   	   const NodePath &ParentGroup, int Index, ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D)=0;
   /**
    * Elimina un grupo del arbol
    * @param[in] Node NodePath del grupo a eliminar
    * @return bool que indica si pudo eliminarlo
    */
   virtual bool RemoveGroup(const NodePath &Node) = 0;

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
};

} /* namespace suri */

#endif /* GROUPMANAGERINTERFACE_H_ */
