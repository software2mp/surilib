/*! \file CsvVectorCreator.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CSVVECTORCREATOR_H_
#define CSVVECTORCREATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/VectorEditionTable.h"
#include "suri/DatasourceManagerInterface.h"
#include "VectorDatasource.h"

namespace suri {
/** Clase encargada de importar un archivo csv al proyecto */
class CsvVectorCreator {
public:
   /** Constructor */
   CsvVectorCreator(std::string Path, int Xcolumn, const int Ycolumn, int StartFromRow,
                    std::string SpatialReference, std::string Token, bool IsPersonalized,
                    bool HasHeader);
   /** Destructor */
   virtual ~CsvVectorCreator();
   /** Crea el archivo de soporte correspondiente para importar */
   DatasourceInterface* Create();
private:
   /** Crea un archivo temporal que pueda ser leido por el driver vrt de gdal */
   DatasourceInterface* CreateFile(Option Metadata);
   /** Agrega la fuente de datos utilizando el driver vrt de gdal */
   DatasourceInterface* CreateVrt(Option Metadata);
   /** Establece el valor obtenido del archivo en la celda correspondiente */
   bool SetValues(std::string line, VectorEditionTable* Table);
   /** Metodo auxiliar que obtiene la informacion de metainformacion que configura el usuario */
   void GetMetadataOptions(Option& Metadata);
   /** url del archivo csv */
   std::string path_;
   /** Posicion de la columna X */
   int xcolumn_;
   /** Posicion de la columna Y */
   int ycolumn_;
   /** Fila desde la cual debe comenzar a importarse la informacion */
   int startfromrow_;
   /** Seleccion espacial */
   std::string spatialreference_;
   /** Separador */
   std::string token_;
   /** Indica si el token es personalizado */
   bool ispersonalized_;
   /** Indica si el archivo posee header */
   bool hasheader_;
};

} /** namespace suri */

#endif /* CSVVECTORCREATOR_H_ */
