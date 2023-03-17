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

#include <string>
#include <map>

#include "Event.h"
#include "suri/ParameterCollection.h"

namespace suri {

Event Event::instance_;

/**
 * Dtor (no es virtual porque no se espera que alguien herede de aca)
 */
Event::~Event() {
   std::map<std::string, Callback*>::iterator it;
   for (it = events_.begin(); it != events_.end(); ++it)
      delete it->second;
   events_.clear();
}

/**
 * Permite establer un objeto que va a responder ante un cierto evento.
 *
 * @param[in] EventName Nombre del evento.
 * @param[in] pCallback Objeto callback
 */
bool Event::On(const std::string& EventName, Callback* pCallback) {
   if (pCallback == NULL)
      return false;

   std::map<std::string, Callback*>::iterator it;
   it = events_.find(EventName);
   if (it != events_.end()) {
      events_.erase(it);
   }

   events_[EventName] = pCallback;

   return true;
}

/**
 * Realiza una llamada a un cierto evento.
 *
 * @param[in] EventName Nombre del evento.
 */
bool Event::Call(const std::string& EventName, BaseParameter* pParam) {
   std::map<std::string, Callback*>::iterator it;
   it = events_.find(EventName);
   if (it != events_.end()) {
      (*it->second)(pParam);
      return true;
   }
   return false;
}

/**
 * Obtiene la unica instancia de la clase.
 */
Event* Event::GetInstance() {
   return &instance_;
}

}  // namespace suri
