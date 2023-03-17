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
