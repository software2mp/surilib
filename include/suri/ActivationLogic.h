/*! \file ActivationLogic.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ACTIVATIONLOGIC_H_
#define ACTIVATIONLOGIC_H_

// Include suri
#include "suri/Element.h"
#include "LayerList.h"

/** namespace suri */
namespace suri {
/** Clase abstracta que permite determinar como los elementos de un LayerList se activan */
/**
 * LayerList llama a ActivationLogic::SetActivation cuando se activa un elemento.
 * Usando los parametros de este metodo, los decendientes de ActivationLogic deben
 * decidir el estado de cada elemento en la LayerList
 */
class ActivationLogic {
   /** Ctor. de Copia. */
   ActivationLogic(const ActivationLogic &ActivationLogic);

public:
   /** Ctor. */
   ActivationLogic() {
   }
   /** Dtor. */
   virtual ~ActivationLogic() {
   }

   /** Determina el estado de activacion de los elementos en la LayerList */
   /**
    * @param[in]	pElement Puntero a elemento activado
    * @param[in]	ElementList Lista de elementos en LayerList
    * @param[out]	ElementData Lista de elementos seleccionados
    * @param[in]	Status Si el elemento se activo(true) o desactivo(false)
    */
   virtual void SetActivation(Element *pElement,
                              LayerList::ElementListType &ElementList,
                              LayerList::ElementSelectionType &ElementData,
                              bool Status)=0;
protected:
private:
};
}

#endif /*ACTIVATIONLOGIC_H_*/
