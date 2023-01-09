/*! \file XmlMetadataHandler.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef XMLMETADATAHANDLER_H_
#define XMLMETADATAHANDLER_H_

// Includes Estandar
// Includes Suri
#include "MetadataExporterInterface.h"
#include "MetadataImporterInterface.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Maneja las acciones de escritura y lectura de metadatos en formato XML implementando las
 * funciones definidas en las interfaces MetadataExporterInterface y MetadataImporterInterface
 */
class XmlMetadataHandler : public MetadataExporterInterface, public MetadataImporterInterface {
public:
   /** Constructor */
   XmlMetadataHandler();
   /** Desstructor */
   virtual ~XmlMetadataHandler();
   /** Realiza la lectura de un archivo xml de metadatos */
   virtual bool ImportMetadata(DatasourceInterface* pDatasource);
   /** Realiza la escritura de los metadatos en formato xml */
   virtual bool ExportMetadata(DatasourceInterface* pDatasource, std::string Filename);
};

} /** namespace suri */

#endif /* XMLMETADATAHANDLER_H_ */
