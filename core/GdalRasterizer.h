/*! \file GdalRasterizer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

#ifndef GDALRASTERIZER_H
#define GDALRASTERIZER_H

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
// Includes Wx
// Includes Wx
#include "wx/progdlg.h"
#include "wx/filename.h"
// Includes GDAL
// Defines
// forwards

class VectorDatasource;
class GDALDataset;
#include "gdal.h"
namespace suri {


class GdalRasterizer{
public:
   GdalRasterizer(VectorDatasource* pVectorDatasource,
               const std::vector<int>& SelectedBands,
               const std::string& DestUrl,
               const std::string& DataType);

   bool Rasterize(const std::string& BurnAttr, int Width, int Height);
private:

   void CreateTypeMap();

   GDALDataset * CreateGdalRasterDataset(int Width, int Height);

   std::map<std::string, GDALDataType> sopiToGdalTypeMap_;
   /** Formato de salida default **/
   static const std::string kDefaultDriver;

   const std::string destUrl_;

   const std::vector<int> selectedBands_;

   VectorDatasource* pVectorDatasource_;

   std::string dataType_;
};

}

#endif // GDALRASTERIZER_H

