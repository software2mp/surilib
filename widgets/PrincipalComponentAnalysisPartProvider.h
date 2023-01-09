/*! \file PrincipalComponentAnalysisPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PRINCIPALCOMPONENTANALYSISPARTPROVIDER_H_
#define PRINCIPALCOMPONENTANALYSISPARTPROVIDER_H_

#include <string>

#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessPartProviderInterface.h"

namespace suri {

class PrincipalComponentAnalysisPartProvider : public ProcessPartProviderInterface {
public:
   /**
    * Ctor.
    */
   explicit PrincipalComponentAnalysisPartProvider(ProcessAdaptLayer* pPal);

   /**
    * Dtor.
    */
   virtual ~PrincipalComponentAnalysisPartProvider();

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

#endif  // PRINCIPALCOMPONENTANALYSISPARTPROVIDER_H_
