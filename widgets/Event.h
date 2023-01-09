/** \file Event.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
