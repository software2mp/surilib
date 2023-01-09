/*! \file FileVectorExporterProcessPartProvider.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILEVECTOREXPORTERPROCESSPARTPROVIDER_H_
#define FILEVECTOREXPORTERPROCESSPARTPROVIDER_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "VectorProcessPartProvider.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de proveer los parts para la configuracion de un proceso vectorial **/
class FileVectorExporterProcessPartProvider : public VectorProcessPartProvider {
   /** ctor copia **/
   FileVectorExporterProcessPartProvider(const FileVectorExporterProcessPartProvider&);

public:
   /** dtor **/
   virtual ~FileVectorExporterProcessPartProvider();
   /** Obtiene los parts necesarios para la configuracion visual del proceso */
   virtual bool GetParts(std::vector<Part*> &ProcessParts);
   /** Metodo para la creacion de un Part provider
    *  @return instancia del creador correspondiente
    *  @return NULL en caso contrario
    */
   static ProcessPartProviderInterface* Create(
         const std::string& ProcessName, ProcessAdaptLayer* pPal);

protected:
   /** ctor **/
   explicit FileVectorExporterProcessPartProvider(ProcessAdaptLayer* pPal);
};

} /** namespace suri */

#endif /* FILEVECTOREXPORTERPROCESSPARTPROVIDER_H_ */