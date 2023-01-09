/*! \file GdalRasterWriter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
