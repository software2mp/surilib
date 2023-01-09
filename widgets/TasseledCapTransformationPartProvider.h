/*! \file TasseledCapTransformationPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASSELEDCAPTRANSFORMATIONPARTPROVIDER_H_
#define TASSELEDCAPTRANSFORMATIONPARTPROVIDER_H_

#include <string>

#include "suri/ProcessPartProviderInterface.h"
#include "suri/ProcessAdaptLayer.h"

namespace suri {

class TasseledCapTransformationPartProvider : public suri::ProcessPartProviderInterface {
public:
   /**
    * Ctor.
    */
   explicit TasseledCapTransformationPartProvider(ProcessAdaptLayer* pPal);

   /**
    * Dtor.
    */
   virtual ~TasseledCapTransformationPartProvider();

   /**
    * Obtiene los parts necesarios para la configuracion visual del proceso.
    */
   virtual bool GetParts(std::vector<Part*>& ProcessParts);

   /**
    * Metodo para la creacion de un Part provider.
    */
   static ProcessPartProviderInterface* Create(const std::string& ProcessName,
                                               ProcessAdaptLayer* pPal);

};

}  // namespace suri

#endif  // TASSELEDCAPTRANSFORMATIONPARTPROVIDER_H_
