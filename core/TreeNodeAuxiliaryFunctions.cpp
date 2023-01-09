/** \file TreeNodeAuxiliaryFunctions.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <list>
// Includes Suri
#include "TreeNodeAuxiliaryFunctions.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/**
 * Para un nodo, busca su posicion en el grupo que lo contiene
 * @param[in] pParent TreeNodeInterfacedonde donde quiero buscar.
 * @param[in] pChild TreeNodeInterfacedonde del que me interesa la
 * posicion
 * @return Posicion de pChild en pParent.
 * @return -1 si no lo encuentra
 */
int GetIndexOfChild(TreeNodeInterface* pGroupNode, TreeNodeInterface* pChild) {
   int index = -1;
   int childcount = pGroupNode->GetSize();
   for (int i = 0; i < childcount && index < 0; i++)
      if (pGroupNode->GetChildAt(i) == pChild)
         index = i;
   return index;
}

}  /* namespace suri */



