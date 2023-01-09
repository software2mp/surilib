/*! \file FileExporterProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILEEXPORTERPROCESSPARTPROVIDER_H_
#define FILEEXPORTERPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "RasterProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de proveer los parts que configuran un proceso que exporta
 *  el resultado a un archivo
 */
class FileExporterProcessPartProvider : public RasterProcessPartProvider {
   /* ctor copia **/
   FileExporterProcessPartProvider(const FileExporterProcessPartProvider&);

public:
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);
   /** dtor **/
   virtual ~FileExporterProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);

protected:
   /** ctor */
   explicit FileExporterProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* FILEEXPORTERPROCESSPARTPROVIDER_H_ */
