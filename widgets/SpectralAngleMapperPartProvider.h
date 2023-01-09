/*! \file SpectralAngleMapperPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SPECTRALANGLEMAPPERPARTPROVIDER_H_
#define SPECTRALANGLEMAPPERPARTPROVIDER_H_

// Includes Estandar
// Includes Suri
#include "FileExporterProcessPartProvider.h"
// Includes Wx
// Defines

namespace suri {

// forwards
class SpectralAngleMapperPart;

/** Clase encargada de proveer los parts para la configuracion del proceso de clasificacion SAM */
class SpectralAngleMapperPartProvider : public suri::FileExporterProcessPartProvider {
   /** Constructor copia **/
   SpectralAngleMapperPartProvider(const SpectralAngleMapperPartProvider&);
public:
   /** Constructor */
   SpectralAngleMapperPartProvider();
   /** Destructor */
   virtual ~SpectralAngleMapperPartProvider();
   /** Metodo para la creacion de un Part provider */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** En funcion de los parametros de entrada retorna el part **/
   SpectralAngleMapperPart* GetMapperPart();

protected:
   /** Constructor **/
   explicit SpectralAngleMapperPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* SPECTRALANGLEMAPPERPARTPROVIDER_H_ */
