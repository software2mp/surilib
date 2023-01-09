/*
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GDALTRANSLATOR_H
#define GDALTRANSLATOR_H

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
// Includes Wx
// Includes GDAL
// Defines
// forwards

class GDALRasterBand;

namespace suri {

class GcpList;
class Subset;
class World;

class GdalTranslator{
public:
   /**
    * Constructor
    */
   GdalTranslator(const std::string& SrcFilename, const std::string& DestFilename,
               const std::vector<int>& selectedBands);
   /**
    * Realiza una transformación a partir de una lista de gcps suri
    */
   void Translate(const GcpList& gcps);
   /** Formato de salida default **/
   static const std::string kDefaultDriver;
   
private:
   const std::string srcFilename;
   const std::string destFilename;
   const std::vector<int> selectedBands;
   void CopyBandInfo(GDALRasterBand* SrcBand, GDALRasterBand* DstBand);
   bool IsSelectedBand(int nSrcBand) const;
};

}

#endif // GDALTRANSLATOR_H
