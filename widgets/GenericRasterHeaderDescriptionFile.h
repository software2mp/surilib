/*! \file GenericRasterHeaderDescriptionFile.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
