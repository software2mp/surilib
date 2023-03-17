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

// Includes Estandar
#include <string>
#include <map>
#include <fstream>

// Includes Suri
#include "GenericRasterHeaderDescriptionFile.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// Definicion de constantes
std::string GenericRasterHeaderDescriptionFile::NcolsHeader = "ncols";
std::string GenericRasterHeaderDescriptionFile::NrowsHeader = "nrows";
std::string GenericRasterHeaderDescriptionFile::NbandsHeader = "nbands";
std::string GenericRasterHeaderDescriptionFile::NbitsHeader = "nbits";
std::string GenericRasterHeaderDescriptionFile::PixelTypeHeader = "pixeltype";
std::string GenericRasterHeaderDescriptionFile::ByteorderHeader = "byteorder";
std::string GenericRasterHeaderDescriptionFile::LayoutHeader = "layout";
std::string GenericRasterHeaderDescriptionFile::SkipbytesHeader = "skipbytes";
std::string GenericRasterHeaderDescriptionFile::UlxMapHeader = "ulxmap";
std::string GenericRasterHeaderDescriptionFile::UlyMapHeader = "ulymap";
std::string GenericRasterHeaderDescriptionFile::XdimHeader = "xdim";
std::string GenericRasterHeaderDescriptionFile::YDimHeader = "ydim";
std::string GenericRasterHeaderDescriptionFile::BandGapBytesHeader = "bandgapbytes";
std::string GenericRasterHeaderDescriptionFile::BandRowBytesHeader = "bandrowbytes";
std::string GenericRasterHeaderDescriptionFile::TotalRowBytesHeader = "totalrowbytes";

GenericRasterHeaderDescriptionFile::GenericRasterHeaderDescriptionFile(
      const std::string& FileName) :
      fileName_(FileName) {
}

GenericRasterHeaderDescriptionFile::~GenericRasterHeaderDescriptionFile() {
}

/**
 * Lee desde el archivo de encabezado la informacion que posee y la almacena
 * en un mapa clave-valor.
 * @param[out] KeyValueMap mapa clave-valor en el que se almacena la
 * informacion del encabezado
 */
bool GenericRasterHeaderDescriptionFile::Read(
      std::map<std::string, std::string>& KeyValueMap) const {
   // TODO (Gabriel #5136): Falta tener en cuenta comentarios. Si una linea
   // no empieza con una palabra clave se toma como comentario.
   // Luego de indicar clave-valor lo demas es comentario.
   if (fileName_.empty())
      return false;
   std::ifstream fin(fileName_.c_str());
   std::string word;
   int keyvaluepos=0;
   std::string key, value;
   LoadDefaultHeaderValues(KeyValueMap);
   while (fin >> word) {
      if (keyvaluepos == 0) {
         key = ToLower(word);
      } else {
         value = word;
         KeyValueMap.insert(std::make_pair<std::string, std::string>(key, value));
      }
      keyvaluepos = (keyvaluepos+1) %2;
   }
   if (KeyValueMap.find(UlyMapHeader) == KeyValueMap.end()) {
      // ulymap default = nrows - 1
      KeyValueMap[UlyMapHeader] = NumberToString(
            StringToNumber<int>(KeyValueMap[NrowsHeader]) - 1);
   }
   fin.close();
   return true;
}
/**
 * Guarda en el archivo de encabeza la informacion definida en el mapa
 * clave-valor que se pasa por parametro.
 * @param[in] KeyValueMap mapa clave-valor de donde toma la informacion
 * para guardar en el encabezado
 **/
bool GenericRasterHeaderDescriptionFile::Write(
      const std::map<std::string, std::string>& KeyValueMap) {
   std::map<std::string, std::string>::const_iterator it = KeyValueMap.begin();
   std::ofstream fout(fileName_.c_str());
   for (; it != KeyValueMap.end(); ++it) {
      fout << it->first << " " << it->second << std::endl;
   }
   return true;
}

/**
 * Metodo auxiliar que configura los valores por defecto de los encabezados
 */
void GenericRasterHeaderDescriptionFile::LoadDefaultHeaderValues(
      std::map<std::string, std::string>& KeyValueMap) const {
   KeyValueMap[NbandsHeader] = "1";
   KeyValueMap[NbitsHeader] = "8";
   KeyValueMap[LayoutHeader] = "bil";
   KeyValueMap[SkipbytesHeader] = "0";
   KeyValueMap[UlxMapHeader] = "0";
   KeyValueMap[XdimHeader] = "1";
   KeyValueMap[YDimHeader] = "1";
   KeyValueMap[BandGapBytesHeader] = "0";
   KeyValueMap[ByteorderHeader] = "I";
}
} /** namespace suri */
