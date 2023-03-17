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

