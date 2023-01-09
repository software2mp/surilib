/*! \file SharpeningProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SHARPENINGPROCESSPARTPROVIDER_H_
#define SHARPENINGPROCESSPARTPROVIDER_H_

#include <string>

#include "suri/ProcessPartProviderInterface.h"
#include "suri/ProcessAdaptLayer.h"

namespace suri {

/**
 * Proveedor de parts para el proceso de fusion de bandas con pancromatica (HSV)
 */
class SharpeningProcessPartProvider : public suri::ProcessPartProviderInterface {
public:
   /**
    * Ctor.
    */
   explicit SharpeningProcessPartProvider(ProcessAdaptLayer* pPal);

   /**
    * Dtor.
    */
   virtual ~SharpeningProcessPartProvider();

   /**
    * Obtiene los parts necesarios para la configuracion visual del proceso.
    */
   virtual bool GetParts(std::vector<Part*>& ProcessParts);

   /**
    * Metodo para la creacion de un Part provider.
    */
   static ProcessPartProviderInterface* Create(const std::string& ProcessName, ProcessAdaptLayer* pPal);
};

}  // namespace suri

#endif  // SHARPENINGPROCESSPARTPROVIDER_H_
