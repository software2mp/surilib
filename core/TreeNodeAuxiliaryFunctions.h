/** \file TreeNodeAuxiliaryFunctions.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef TREENODEAUXILIARYFUNCTIONS_H_
#define TREENODEAUXILIARYFUNCTIONS_H_

// Includes standard
// Includes Suri
#include "suri/TreeNodeInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/** Para un nodo, busca su posicion en el grupo que lo contiene */
int GetIndexOfChild(TreeNodeInterface* pGroupNode,
                                 TreeNodeInterface* pChild);


} /* namespace suri */


#endif /* TREENODEAUXILIARYFUNCTIONS_H_ */
