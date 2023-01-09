/*! \file ConfusionMatrixProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONFUSIONMATRIXPROCESSPARTPROVIDER_H_
#define CONFUSIONMATRIXPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "StaticsExporterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que provee los parts para la configuracion del proceso de estadisticas */
class ConfusionMatrixProcessPartProvider : public suri::StaticsExporterProcessPartProvider {
   /** ctor copia */
   ConfusionMatrixProcessPartProvider(const ConfusionMatrixProcessPartProvider&);
   /** ctor **/
   explicit ConfusionMatrixProcessPartProvider(ProcessAdaptLayer* pPal);

public:

   /** dtor **/
   virtual ~ConfusionMatrixProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* CONFUSIONMATRIXPROCESSPARTPROVIDER_H_ */
