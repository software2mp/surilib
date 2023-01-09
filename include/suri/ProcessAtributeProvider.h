/*! \file ProcessAtributeProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PROCESSATTRIBUTEPROVIDER_H_
#define PROCESSATTRIBUTEPROVIDER_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "ProcessAdaptLayer.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que repressenta a un Part que participa en la configuracion
 *  de un proceso de la aplicacion
 */
class ProcessAtributeProvider {
public:
   /** ctor **/
   ProcessAtributeProvider() : pPal_(NULL) {}

   virtual ~ProcessAtributeProvider() {}
   /** obtiene la capa de adaptacion de proceso **/
   ProcessAdaptLayer* GetProcessLayer() {
      return pPal_;
   }
   /** Configura la capa de proceso **/
   void SetProcessLayer(ProcessAdaptLayer* pProccessLayer) {
      pPal_ = pProccessLayer;
   }

protected:
   ProcessAdaptLayer* pPal_;
};

} /** namespace suri */

#endif /* PROCESSATTRIBUTEPROVIDER_H_ */
