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

#ifndef EVENT_H_
#define EVENT_H_

#include <string>
#include <map>

#include "suri/ParameterCollection.h"

namespace suri {

/**
 * Clase base para responder a eventos (Functor)
 */
class Callback {
public:
   virtual ~Callback() { }

   /** Operador para llamada */
   void operator()(BaseParameter* pParam) {
      Handle(pParam);
   }

   /** Metodo a reimplementar en cada evento */
   virtual void Handle(BaseParameter* pParam = NULL) = 0;
};

/**
 * Clase que permite de manera global definir objetos para responder
 * a eventos.
 */
class Event {
public:
    /** Dtor (no es virtual porque no se espera que alguien herede de aca) */
    ~Event();

    /** Permite establer un objeto que va a responder ante un cierto evento */
    bool On(const std::string& EventName, Callback* pCallback);

    /** Realiza una llamada a un cierto evento */
    bool Call(const std::string& EventName, BaseParameter* pParam = NULL);

    /** Obtiene la unica instancia de la clase */
    static Event* GetInstance();

private:
    /**
     * Ctor.
     */
    Event() { }

    static Event instance_;
    std::map<std::string, Callback*> events_;
};

}  // namespace suri

#endif  // EVENT_H_
