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

#ifndef GDALRASTERWRITER_H_
#define GDALRASTERWRITER_H_

#include "suri/RasterWriter.h"

class GDALDataset;

/** namespace suri */
namespace suri {
/** Clase para el manejo del driver de escritura de GDAL */
/**
 * Clase para el manejo del driver de escritura de GDAL. Obtiene el bloque
 * de bandas / subset que debe escribir. Calcula el tamanio x e y de un bloque.
 */
class GdalRasterWriter : public RasterWriter {
public:
   /** ctor */
   GdalRasterWriter(GDALDataset *pDataset, const std::string &Filename);
   /** dtor */
   virtual ~GdalRasterWriter();

   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                             int &Lry);
   /** Escritura de un bloque */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                      int Uly, int Lrx, int Lry);
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const;
private:
   GDALDataset *pDataset_; /*! Dataset asociado */
};
}

#endif /* GDALRASTERWRITER_H_ */
