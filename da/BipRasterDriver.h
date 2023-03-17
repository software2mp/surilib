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

#ifndef BIPRASTERDRIVER_H_
#define BIPRASTERDRIVER_H_

// Includes Estandar
// Includes Suri
#include "suri/RawRasterDriver.h"
// Includes Wx
// Defines
// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {
namespace driver {

/**
 *  Clase encargada de la lectura/escritura sobre archivos raster con interlineado
 *  BIP
 */
class BipRasterDriver : public suri::core::raster::dataaccess::driver::RawRasterDriver {
   /** ctor copia **/
   BipRasterDriver(const BipRasterDriver&);

public:
   /** ctor **/
   BipRasterDriver(const std::string& Mux, const std::string& FileName,
                   int Pixels, int Lines);
   /** dtor **/
   virtual ~BipRasterDriver();
   /** string que identifica al interlineado del driver **/
   static std::string MuxIdentifier;

private:
   /** array que tiene el contenido de la banda en memoria **/
   char* pData_;
   /** Tamanio del bloque X e Y */
   virtual bool DoGetBlockSize(int &SizeX, int &SizeY) const;
   /** Carga el buffer con el subset **/
   virtual bool DoRead(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry);
   /** Escribe mas de una banda **/
   virtual bool DoWrite(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                        int Ulx, int Uly, int Lrx, int Lry);
   /** Retorna un buffer interno con los datos **/
   virtual void* DoGetBlock(int BlockX, int BlockY);
};

}  /** namespace driver **/
}  /** namespace dataaccess **/
}  /** namespace raster **/
}  /** namespace core **/

} /** namespace suri */

#endif /* BIPRASTERDRIVER_H_ */
