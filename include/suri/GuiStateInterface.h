/** \file GuiStateInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef GUISTATEINTERFACE_H_
#define GUISTATEINTERFACE_H_

// Includes standard
#include <string>

// Includes sopi
#include "suri/NotificationInterface.h"

namespace suri {

/**
 * Interfaz que deben implementar las clases que modifican el estado de
 * la GUI en respuesta un evento.
 */
class GuiStateInterface {
public:
   /** Dtor */
   virtual ~GuiStateInterface() {}

   /** Actualiza estado de la aplicacion a partir del evento */
   virtual bool StateChangeEvent(const std::string EventId,
                                       const std::string SourceId) = 0;
};

} /* namespace suri */


#endif /* GUISTATEINTERFACE_H_ */
