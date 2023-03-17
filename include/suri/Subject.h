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

#ifndef SUBJECT_H_
#define SUBJECT_H_

// Includes Estandar
#include <map>
#include <utility>

// Includes Suri
#include "ObserverInterface.h"
#include "NotificationInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Un sujeto que puede ser observado por observadores para que estos puedan recibir notificaciones
 */
class Subject {
public:
   /** Destructor */
   virtual ~Subject();
   /** Registra un Observer para que pueda recibir notificaciones */
   int Attach(ObserverInterface* pObserver);
   /** Quita un Observer para que pueda recibir notificaciones */
   bool Detach(int ObserverId);
   /** Notifica a todos los observadores sobre una modificacion del sujeto */
   void Notify(NotificationInterface* pNotification);

   static const int InvalidCount = -1; /*! Valor si hay errores en Attach/Detach de Observers */

protected:
   /** Constructor */
   Subject();

private:
   typedef std::map<int, ObserverInterface*> SubjectHashmapType; /*! Typedef para el mapa */
   typedef std::pair<int, ObserverInterface*> SubjectPairType; /*! Typedef para el par (del mapa) */
   typedef std::pair<SubjectHashmapType::iterator, bool> SubjectAddResultPairType; /*! Resultado */
   SubjectHashmapType observers_; /*! Mapa para esta implementacion de Subject */
   int attachCount_; /*! Contador de llamadas al metodo Attach*/
};

} /** namespace suri */

#endif /* SUBJECT_H_ */
