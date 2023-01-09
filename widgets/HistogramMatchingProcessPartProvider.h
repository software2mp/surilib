/*! \file HistogramMatchingPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef HISTOGRAMMATCHINGPROCESSPARTPROVIDER_H_
#define HISTOGRAMMATCHINGROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "FileExporterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class HistogramMatchingProcessPartProvider: public FileExporterProcessPartProvider {
   /* ctor copia **/
   HistogramMatchingProcessPartProvider(const HistogramMatchingProcessPartProvider&);

public:
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** dtor **/
   virtual ~HistogramMatchingProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);

protected:
   /** ctor */
   explicit HistogramMatchingProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /*HISTOGRAMMATCHINGPROCESSPARTPROVIDER_H_ */
