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

#ifndef SRSHSVSHARPENING_H_
#define SRSHSVSHARPENING_H_

#include "SRSSharpening.h"
#include "suri/ParameterCollection.h"

#include "gdal.h"

namespace suri {
namespace raster {
namespace sharpening {

/**
 * Clase que implementa el metodo de fusion de bandas con pancromatica por HSV.
 */
class HSVSharpening : public Sharpening {
public:
   static const int kDestBandCount = 3;

   /**
    * Ctor.
    */
   HSVSharpening(const std::string& SrcFilename, const std::string& PanFilename,
                 const std::string& DestFilename, const std::vector<int>& SrcBands,
                 int PanBand);

   /**
    * Dtor.
    */
   virtual ~HSVSharpening();

   /**
    * Implementa el procedimiento de fusion de bandas con pancromatica por HSV.
    */
   virtual void Transform(CBSHARPENINGPROGRESS ProgressCB = NULL, void* pProgressCBData = NULL);

   /**
    * Crea una instancia de HSVSharpening.
    */
   static Sharpening* Create(suri::ParameterCollection& Params);

private:
   /**
    * Copia informacion de proyeccion, georreferenciacion, metadatos, etc
    * desde el dataset de origen al de destino.
    */
   void CopyStandardData(GDALDatasetH SrcDs, GDALDatasetH DestDs);
};

}  // namespace sharpening
}  // namespace raster
}  // namespace suri

#endif /* SRSHSVSHARPENING_H_ */
