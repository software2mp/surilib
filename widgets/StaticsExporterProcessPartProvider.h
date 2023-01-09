/*! \file StaticsExporterProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef STATICSEXPORTERPROCESSPARTPROVIDER_H_
#define STATICSEXPORTERPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "RasterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que provee los parts para la configuracion del proceso de estadistica **/
class StaticsExporterProcessPartProvider : public RasterProcessPartProvider {
   /** ctor copia **/
   StaticsExporterProcessPartProvider(const StaticsExporterProcessPartProvider&);

public:
   /** Destructor */
   virtual ~StaticsExporterProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);

protected:
   /** Constructor */
   explicit StaticsExporterProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* STATICSEXPORTERPROCESSPARTPROVIDER_H_ */
