/*! \file VectorProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORPROCESSPARTPROVIDER_H_
#define VECTORPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/ProcessPartProviderInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de proveer los parts para la configuracion de un proceso
 *  vectorial **/
class VectorProcessPartProvider : public suri::ProcessPartProviderInterface {
   /** ctor copia **/
   VectorProcessPartProvider(const VectorProcessPartProvider&);

public:
   /** Dtor */
   virtual ~VectorProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);

protected:
   /** Ctor */
   explicit VectorProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* VECTORPROCESSPARTPROVIDER_H_ */
