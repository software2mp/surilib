/*
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GDALRESIZER_H
#define GDALRESIZER_H

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

class Subset;
class World;

class GdalResizer{
public:
   GdalResizer(const std::string& SrcFilename, const std::string& DestFilename,
               const std::vector<int>& selectedBands);
   void Resize(const World* pOutputWorld);
   void Resize(const Subset& outputSubset,
               int outXSize, int outYSize,
               double pixelXSize, double pixelYSize);
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

#endif // GDALRESIZER_H
