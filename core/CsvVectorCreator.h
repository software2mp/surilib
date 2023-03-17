/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
