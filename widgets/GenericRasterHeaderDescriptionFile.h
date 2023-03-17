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

#ifndef GENERICRASTERHEADERDESCRIPTIONFILE_H_
#define GENERICRASTERHEADERDESCRIPTIONFILE_H_

// Includes Estandar
#include <string>
#include <map>

// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Clase que representa un archivo de encabezado que describe la informacion del
 * archivo raster asociado.
 *
 */
class GenericRasterHeaderDescriptionFile {
public:
   GenericRasterHeaderDescriptionFile(const std::string& FileName);
   virtual ~GenericRasterHeaderDescriptionFile();
   /**
    * Lee desde el archivo de encabezado la informacion que posee y la almacena
    * en un mapa clave-valor.
    * @param[out] KeyValueMap mapa clave-valor en el que se almacena la
    * informacion del encabezado
    */
   bool Read(std::map<std::string, std::string>& KeyValueMap) const;
   /**
    * Guarda en el archivo de encabeza la informacion definida en el mapa
    * clave-valor que se pasa por parametro.
    * @param[in] KeyValueMap mapa clave-valor de donde toma la informacion
    * para guardar en el encabezado
    **/
   bool Write(const std::map<std::string, std::string>& KeyValueMap);

   /** Constantes para opciones de encabezado **/
   static std::string NcolsHeader;
   static std::string NrowsHeader;
   static std::string NbandsHeader;
   static std::string NbitsHeader;
   static std::string PixelTypeHeader;
   static std::string ByteorderHeader;
   static std::string LayoutHeader;
   static std::string SkipbytesHeader;
   static std::string UlxMapHeader;
   static std::string UlyMapHeader;
   static std::string XdimHeader;
   static std::string YDimHeader;
   static std::string BandRowBytesHeader;
   static std::string TotalRowBytesHeader;
   static std::string BandGapBytesHeader;

private:
   /**
    * Metodo auxiliar que configura los valores por defecto de los encabezados
    */
   void LoadDefaultHeaderValues(std::map<std::string, std::string>& KeyValueMap) const;
   std::string fileName_;
};

} /** namespace suri */

#endif /* GENERICRASTERHEADERDESCRIPTIONFILE_H_ */
