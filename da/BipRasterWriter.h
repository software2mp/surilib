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

#ifndef BIPRASTERWRITER_H_
#define BIPRASTERWRITER_H_

// Includes standard
#include <fstream>

// Includes suri
#include "suri/RasterWriter.h"

/** namespace suri */
namespace suri {
/** Writer multiplexado en BIP */
/**
 *  Clase que hereda de RasterWriter y escribe datos en multiplexado BIP.
 *
 * \todo Hacer que pueda escribir random en X, Y y Bandas
 */
class BipRasterWriter : public RasterWriter {
   /** Ctor. de Copia. */
   BipRasterWriter(const BipRasterWriter &BipRasterWriter);

public:
   /** tipo de funcion usada para decimar */
   typedef void (*WriterFunc)(std::ostream &OutStream, std::vector<void*> Data, int,
                              int, int);
   /** ctor */
   BipRasterWriter(const std::string &Filename);
   /** dtor */
   virtual ~BipRasterWriter();

   /** Cambia el tipo de dato */
   virtual void SetDataType(const std::string &DataType);
   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                             int &Lry);
   /** Escritura de un bloque */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                      int Uly, int Lrx, int Lry);
protected:
private:
   std::ofstream ostream_; /*! Stream de escritura */
   bool fullWrite_; /*! flag que indica si se llego al fin del archivo */
   WriterFunc pWriteFunc_; /*! Funcion de escritura */
};
}

#endif /*BIPRASTERWRITER_H_*/
