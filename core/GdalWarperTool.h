/*
 * Biblioteca de procesamiento GeoEspacial SuriLib.
 * Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 * Este producto contiene software desarrollado por
 * SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GDALWARPERTOOL_H
#define GDALWARPERTOOL_H

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
// Includes Wx
// Defines

namespace suri{

class GdalWarperTool{
public:
   /**
    * Constructor
    */
   GdalWarperTool(const std::string& SrcFilename, const std::string& DestFilename);
   /**
    * Constructor
    */
   GdalWarperTool(const std::string& SrcFilename, const std::string& DestFilename,
            const std::vector<int>& selectedBands);
   bool Warp(const std::string& InWkt, bool UseGCPs, int Order);
   /** Formato de salida default **/
   static const std::string kDefaultDriver;
private:
   const std::string srcFilename;
   const std::string destFilename;
   const std::vector<int> selectedBands;
};

} // namespace suri

#endif // GDALWARPERTOOL_H
