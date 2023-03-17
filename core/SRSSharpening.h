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

#ifndef SRSSHARPENING_H_
#define SRSSHARPENING_H_

#include <string>
#include <vector>

namespace suri {
namespace raster {
namespace sharpening {

typedef int (*CBSHARPENINGPROGRESS)(double Complete, const char* pMessage,
                                    void* pProgressArg);

/**
 * Clase base para los distintos metodos de fusion de bandas con pancromatica.
 */
class Sharpening {
public:
   /**
    * Ctor.
    */
   Sharpening(const std::string& SrcFilename, const std::string& PanFilename,
              const std::string& DestFilename, const std::vector<int>& SrcBands,
              int PanBand);

   /**
    * Dtor.
    */
   virtual ~Sharpening();

   /**
    * Metodo a implementar en cada clase hija para ejecutar el
    * procedimiento de fusion de bandas con pancromatica.
    */
   virtual void Transform(CBSHARPENINGPROGRESS ProgressCB = NULL,
                          void* pProgressCBData = NULL) = 0;

protected:
   std::string srcFilename_;
   std::string panFilename_;
   std::string destFilename_;
   std::vector<int> srcBands_;
   int panBand_;
};

}  // namespace sharpening
}  // namespace raster
}  // namespace suri

#endif  // SRSSHARPENING_H_
