/*! \file SingleElementActivationLogic.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SingleElementActivationLogic.h"

/** namespace suri */
namespace suri {

/* Desactiva todos los elemetnos y despues activa el que le solicitaron
 * @param[in] pElement elemento a activar
 * @param[in] Status estado de activacion del elemento.
 * @param[in] ElementList Lista de Elementos
 * @param[in] ElementData sin uso
 */
void SingleElementActivationLogic::SetActivation(
      Element *pElement, LayerList::ElementListType &ElementList,
      LayerList::ElementSelectionType &ElementData, bool Status) {
   LayerList::ElementListType::iterator it = ElementList.begin();
   while (it != ElementList.end()) {
      Element *ptemp = *it;
      if (ptemp && ptemp->IsActive() && Status) {
         ptemp->PreventNextViewerUpdate();
         ptemp->Activate(false);
      }
      it++;
   }
   pElement->Activate(Status);
}
}
