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

#include "GdalRasterWriter.h"

#ifdef __GDAL__

// Includes GDAL
#include "gdal.h"
#include "gdal_priv.h"

/** namespace suri */
namespace suri {
/**
 * Constructor.
 * @param[in] pDataset Objeto GDAL que representa la imagen.
 * @param[in] Filename nombre del Archivo.
 * @return instancia de la clase GdalRasterWriter
 */
GdalRasterWriter::GdalRasterWriter(GDALDataset *pDataset, const std::string &Filename) :
      RasterWriter("GdalRasterWriter", Filename), pDataset_(pDataset) {
}

/** Destructor. */
GdalRasterWriter::~GdalRasterWriter() {
}

/**
 * Obtiene el bloque de bandas/subset que debe escribir. Utiliza el metodo
 * RasterWriter::GetNextBlock.
 * @param[out] BandIndex Vector con los indices de las bandas
 * @param[out] Ulx UpperLeft en x del subset
 * @param[out] Uly UpperLeft en y del subset
 * @param[out] Lrx LowerRight en x del subset
 * @param[out] Lry LowerRight en y del subset
 * @return el resultado de invocar a RasterWriter::GetNextBlock
 */
bool GdalRasterWriter::GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly,
                                    int &Lrx, int &Lry) {
   return RasterWriter::GetNextBlock(BandIndex, Ulx, Uly, Lrx, Lry);
}

/**
 * Escritura de un bloque
 * @param[in] BandIndex Vector con los indices de las bandas
 * @param[in] Data vector de punteros. Buffer del que se toman los datos.
 * @param[in] Ulx UpperLeft en x del subset
 * @param[in] Uly UpperLeft en y del subset
 * @param[in] Lrx LowerRight en x del subset
 * @param[in] Lry LowerRight en y del subset
 */
void GdalRasterWriter::Write(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                             int Ulx, int Uly, int Lrx, int Lry) {
   for (int i = 0; i < GetBandCount(); i++) {
      GDALRasterBand *pband =
            pDataset_ ? pDataset_->GetRasterBand(BandIndex[i] + 1) : NULL;
      if (pband) {
         pband->RasterIO(GF_Write, Ulx, Uly, (Lrx - Ulx), (Lry - Uly), Data[i],
                         (Lrx - Ulx), (Lry - Uly), pband->GetRasterDataType(), 0, 0);
      }
   }
}

// ----------------------------- BLOQUE -----------------------------
/**
 * Tamanio del bloque X e Y
 * @param[out] SizeX cantidad de columnas del bloque
 * @param[out] SizeY cantidad de filas del bloque
 */
void GdalRasterWriter::GetBlockSize(int &SizeX, int &SizeY) const {
   GDALRasterBand *pband = pDataset_ ? pDataset_->GetRasterBand(1) : NULL;
   if (pband) {
      pband->GetBlockSize(&SizeX, &SizeY);
   }
}
}

#endif   // __GDAL__
