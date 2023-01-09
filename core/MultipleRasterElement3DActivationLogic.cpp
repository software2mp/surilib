/*! \file MultipleRasterElement3DActivationLogic.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MultipleRasterElement3DActivationLogic.h"
#include "suri/TerrainElement.h"

namespace suri {

/**
 * Ctor.
 */
MultipleRasterElement3DActivationLogic::MultipleRasterElement3DActivationLogic() {

}

/**
 * Dtor.
 */
MultipleRasterElement3DActivationLogic::~MultipleRasterElement3DActivationLogic() {

}

/**
 * Determina el estado de activacion de los elementos en la LayerList.
 */
void MultipleRasterElement3DActivationLogic::SetActivation(
      Element *pElement, LayerList::ElementListType &ElementList,
      LayerList::ElementSelectionType &ElementData, bool Status) {
   TerrainElement* pterrain = dynamic_cast<TerrainElement*>(pElement);

   if (pterrain == NULL) {
      pElement->BlockViewerUpdate();
      pElement->Activate(Status);

      LayerList::ElementListType::iterator beginit = ElementList.begin();
      LayerList::ElementListType::iterator endit = ElementList.end();
      for (; beginit != endit; ++beginit) {
         TerrainElement* pterrain = dynamic_cast<TerrainElement*>(*beginit);
         if (pterrain != NULL && pterrain->IsActive()) {
            pterrain->UnblockViewerUpdate();
            pterrain->ForceUpdate();
            pterrain->SetChanged();
            pterrain->SendViewerUpdate();
         }
      }
   } else {
      pElement->Activate(Status);
   }
}

}  // namespace suri
