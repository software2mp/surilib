/*! \file AppConfigPartsProviderInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef APPCONFIGPARTSPROVIDERINTERFACE_H_
#define APPCONFIGPARTSPROVIDERINTERFACE_H_

#include <vector>

namespace suri {

//Forwards
class Part;

/** 
 * Interace que define el comportamiento esperado de un proveedor de Parts de configuracion
 * general de la aplicacion
**/
class AppConfigPartsProviderInterface {
   /** Constructor copia **/
   AppConfigPartsProviderInterface(const AppConfigPartsProviderInterface&);

public:
   /* Constructor */
   AppConfigPartsProviderInterface() { }
   /* Destructor */
   virtual ~AppConfigPartsProviderInterface() { }
   /** Devuelve los parts existentes */
   virtual std::vector<suri::Part *> GetConfigParts() = 0;
};

} // End namespace suri.


#endif /* APPCONFIGPARTSPROVIDERINTERFACE_H_ */
