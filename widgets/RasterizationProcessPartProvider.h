/*! \file RasterizationProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERIZATIONPROCESSPARTPROVIDER_H_
#define RASTERIZATIONPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "FileExporterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class RasterizationProcessPartProvider: public FileExporterProcessPartProvider {
   /* ctor copia **/
   RasterizationProcessPartProvider(const RasterizationProcessPartProvider&);

public:
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** dtor **/
   virtual ~RasterizationProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);

protected:
   void GetRasterizationParts(std::vector<Part*> &ProcessParts);
   void GetRasterizationOutputSizeParts(std::vector<Part*> &ProcessParts);
   /** ctor */
   explicit RasterizationProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /*RASTERIZATIONPROCESSPARTPROVIDER_H_*/


