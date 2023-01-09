/*! \file IndexProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INDEXPROCESSPARTPROVIDER_H_
#define INDEXPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "FileExporterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de proveer los parts necesarios para la configuracion
 *  del proceso de calculo de indice
 */
class IndexProcessPartProvider : public suri::FileExporterProcessPartProvider {
   /** ctor copia */
   IndexProcessPartProvider(const IndexProcessPartProvider&);
   /** Constructor **/
   explicit IndexProcessPartProvider(ProcessAdaptLayer* pPal);

public:
   /** Destructor **/
   virtual ~IndexProcessPartProvider();
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

#endif /* INDEXPROCESSPARTPROVIDER_H_ */
