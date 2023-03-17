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

// Includes Estandar
#include <map>
#include <utility>

// Includes Suri
#include "suri/Subject.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
Subject::Subject() :
      observers_(), attachCount_(InvalidCount) {
}

/**
 * Destructor
 */
Subject::~Subject() {
}

/**
 * Registra un Observer para que pueda recibir notificaciones
 */
int Subject::Attach(ObserverInterface* pObserver) {
   attachCount_++;
   SubjectAddResultPairType result =
         observers_.insert(std::make_pair(attachCount_, pObserver));
   if (result.second == false) {
      return Subject::InvalidCount;
   }
   return attachCount_;
}

/**
 * Quita un Observer para que pueda recibir notificaciones
 */
bool Subject::Detach(int ObserverId) {
   int erased = observers_.erase(ObserverId);
   return (erased > 0);
}

/**
 * Notifica a todos los observadores sobre una modificacion del sujeto
 */
void Subject::Notify(NotificationInterface* pNotification) {
   SubjectHashmapType::const_iterator it = observers_.begin();
   for (; it != observers_.end(); it++) {
      it->second->Update(pNotification);
   }
}

} /** namespace suri */
