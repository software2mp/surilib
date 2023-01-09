/*! \file BandMathProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BANDMATHPROCESSPARTPROVIDER_H_
#define BANDMATHPROCESSPARTPROVIDER_H_

#include "FileExporterProcessPartProvider.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de proveer los parts para la configuracion del proceso
 *  de algebra de bandas
 */
class BandMathProcessPartProvider : public suri::FileExporterProcessPartProvider {
   /** ctor copia **/
   BandMathProcessPartProvider(const BandMathProcessPartProvider&);
   /** ctor **/
   explicit BandMathProcessPartProvider(ProcessAdaptLayer* pPal);

public:
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** dtor **/
   virtual ~BandMathProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
};

} /** namespace suri */

#endif /* BANDMATHPROCESSPARTPROVIDER_H_ */
