/*! \file MetadataExporterInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef METADATAEXPORTERINTERFACE_H_
#define METADATAEXPORTERINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Define la interfaz del exportador de archivos de metadatos */
class MetadataExporterInterface {
public:
   /** Realiza la escritura de los metadatos en el formato correspondiente */
   virtual bool ExportMetadata(DatasourceInterface* pDatasource, std::string Filename)=0;
};

} /** namespace suri */

#endif /* METADATAEXPORTERINTERFACE_H_ */
