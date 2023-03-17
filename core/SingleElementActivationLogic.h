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
