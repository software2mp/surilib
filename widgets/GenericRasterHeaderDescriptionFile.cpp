/*! \file GenericRasterHeaderDescriptionFile.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
