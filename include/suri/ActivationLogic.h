/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
