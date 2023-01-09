/*! \file RasterProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERPROCESSPARTPROVIDER_H_
#define RASTERPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/ProcessPartProviderInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// Forwards
class Element;

/** Clase encargada de proveer los Parts necesarios para
 *  la configuracion de un proceso raster */
class RasterProcessPartProvider : public ProcessPartProviderInterface {
   /** ctor copia **/
   RasterProcessPartProvider(const RasterProcessPartProvider&);

public:
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** dtor **/
   virtual ~RasterProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** ctor **/
   explicit RasterProcessPartProvider(ProcessAdaptLayer* pPal);
protected:

   /** Metodo que agrega el part de seleccion espacial */
   bool CreateSpectralSectionPart(std::vector<Part*> &ProcessParts,
                                  std::vector<Element*>& InputElements);
};

} /** namespace suri */

#endif /* RASTERPROCESSPARTPROVIDER_H_ */
