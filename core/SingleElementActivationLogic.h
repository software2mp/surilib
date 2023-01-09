/*! \file SingleElementActivationLogic.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SINGLEELEMENTACTIVATIONLOGIC_H_
#define SINGLEELEMENTACTIVATIONLOGIC_H_

// Includes suri
#include "suri/ActivationLogic.h"

/** namespace suri */
namespace suri {
/** ActivationLogic que permite un unico elemento activo en el ListViewer */
/**
 * Desactiva todos los elementos y despues activa el que le solicitaron
 * \note Da funcionalidad a ActivationLogic
 */
class SingleElementActivationLogic : public ActivationLogic {
   /** Ctor. de Copia. */
   SingleElementActivationLogic(
         const SingleElementActivationLogic &SingleElementActivationLogic);

public:
   /** Ctor */
   SingleElementActivationLogic() {
   }
   /** Dtor */
   virtual ~SingleElementActivationLogic() {
   }
   /** solo permite que halla un elemento activo en LayerList */
   virtual void SetActivation(Element *pElement,
                              LayerList::ElementListType &ElementList,
                              LayerList::ElementSelectionType &ElementData,
                              bool Status);
protected:
private:
};
}

#endif /*SINGLEELEMENTACTIVATIONLOGIC_H_*/
