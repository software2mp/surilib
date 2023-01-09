/*! \file MultipleRasterElement3DActivationLogic.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MULTIPLERASTERELEMENT3DACTIVATIONLOGIC_H_
#define MULTIPLERASTERELEMENT3DACTIVATIONLOGIC_H_

#include <string>

#include "suri/ActivationLogic.h"
#include "suri/Element.h"
#include "suri/LayerList.h"

namespace suri {

/**
 * Logica de activacion para modo 3D (simur)
 */
class MultipleRasterElement3DActivationLogic : public ActivationLogic {
public:
   /**
    * Ctor.
    */
   MultipleRasterElement3DActivationLogic();

   /**
    * Dtor.
    */
   virtual ~MultipleRasterElement3DActivationLogic();

   /**
    * Determina el estado de activacion de los elementos en la LayerList.
    */
   virtual void SetActivation(Element *pElement, LayerList::ElementListType &ElementList,
                              LayerList::ElementSelectionType &ElementData, bool Status);
};

}  // namespace suri

#endif  // MULTIPLERASTERELEMENT3DACTIVATIONLOGIC_H_

