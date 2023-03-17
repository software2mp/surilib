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

#ifndef VIEWORDERCHANGENOTIFICATION_H_
#define VIEWORDERCHANGENOTIFICATION_H_

// Includes Estandar

// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase que notifica a un observador que se ha cambiado el orden en la vista.
 */
class ViewOrderChangeNotification : public suri::NotificationInterface {
public:
   /** Constructor */
   ViewOrderChangeNotification(Element* pElement, Element* pElementDest, const int &Movetype);
   /** Destructor */
   virtual ~ViewOrderChangeNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /** Constructor de Copia */
   ViewOrderChangeNotification(const ViewOrderChangeNotification &);
   /** Tipo de Movimiento: "Mover despues de" */
   static const int MoveAfter = 0;
   /** Tipo de Movimiento: "Mover al final" */
   static const int MoveUnder = 1;
   /** Tipo de Movimiento: "Mover antes de" */
   static const int MoveBefore = 2;
   /** Tipo de movimiento indicado*/
   int moveType_;
   /** Movimiento sobre el cual se mueve */
   Element* pElement_;
   /** Elemento que esta en la posicion donde se desea mover el element */
   Element* pElementDest_;
};

} /** namespace suri */

#endif /* VIEWORDERCHANGENOTIFICATION_H_ */
