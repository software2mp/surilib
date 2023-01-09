/** \file Event.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
