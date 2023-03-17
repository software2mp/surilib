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

#include "SourceSrsChangeNotification.h"
#include "suri/GeoreferenceGcpDriver.h"
#include "TablePart.h"

namespace suri {

/** Ctor */
SourceSrsChangeNotification::SourceSrsChangeNotification(
                                    const std::string &SourceSrs) {
   sourceSrs_ = SourceSrs;
}

/** Dtor */
SourceSrsChangeNotification::~SourceSrsChangeNotification() {
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado
 * @param[in] pObserver Observador que escucha la notificacion
 * @return true en caso de haber notificado correctamente, false en otro caso
 */
bool SourceSrsChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   bool retval = false;
   GeoreferenceGcpDriver* pdriver = dynamic_cast<GeoreferenceGcpDriver*>(pObserver);
   if (pdriver != NULL) {
      pdriver->SetGcpSourceSrs(sourceSrs_);
      retval = true;
   } else {
      TablePart* ptable = dynamic_cast<TablePart*>(pObserver);
      if (ptable != NULL) {
         ptable->SetSrs(sourceSrs_);
         retval = true;
      }
   }
   return retval;
}

} /* namespace suri */
