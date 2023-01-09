/*! \file MetadataImporterInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef METADATAIMPORTERINTERFACE_H_
#define METADATAIMPORTERINTERFACE_H_

// Includes Estandar
// Includes Suri
#include "suri/DatasourceInterface.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Define la interfaz del importador de archivos de metadatos */
class MetadataImporterInterface {
public:
   /** Realiza la lectura de un archivo de metadatos */
   virtual bool ImportMetadata(DatasourceInterface* pDatasource)=0;
};

} /** namespace suri */

#endif /* METADATAIMPORTERINTERFACE_H_ */
