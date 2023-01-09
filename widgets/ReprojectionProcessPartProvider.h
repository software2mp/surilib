/*! \file ReprojectionProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REPROJECTIONPROCESSPARTPROVIDER_H_
#define REPROJECTIONPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "FileExporterProcessPartProvider.h"
#include "ReprojectionProcess.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de proveer los parts para la configuracion del proceso
 *  de reproyeccion
 */
class ReprojectionProcessPartProvider : public suri::FileExporterProcessPartProvider {
   /** ctor copia **/
   ReprojectionProcessPartProvider(const ReprojectionProcessPartProvider&);
   /** Ctor **/
   explicit ReprojectionProcessPartProvider(ProcessAdaptLayer* pPal);

public:
   /** Dtor **/
   virtual ~ReprojectionProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);

private:
   bool reprojection_;
   bool askGcpFile_;
   bool showResizePart_;
   ReprojectionProcess::ReprojectionParameters* pReprojectionParameters_;
};

} /** namespace suri */

#endif /* REPROJECTIONPROCESSPARTPROVIDER_H_ */
