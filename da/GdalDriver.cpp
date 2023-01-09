/*! \file GdalDriver.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "GdalDriver.h"

#ifdef __GDAL__

// Includes standard
#include <math.h>
#ifdef __DEBUG__
#include <iostream>
#endif

// Includes suri
#include "suri/messages.h"

#include "logmacros.h"

// Includes otros
#include "gdal_priv.h"

/** namespace suri */
namespace suri {
/**
 *  Inicializa las variables de RasterDriver.
 *
 *  Inicializa el miembro pDataset_ con el dataset de la imagen (permite
 * usar el cache de GDAL). No debe destruir el dataset (le pertenece a la
 * imagen).
 *
 *  Carga como nombre del driver "Gdal::" + la descripcion provista por el
 * driver del dataset.
 *
 * @param[in] pDataset es el dataset de GDAL utilizado para la lectura.
 * @param[in] Filename es el nombre del archivo a abrir.
 * @param[in] DataName es el nombre del tipo de dato que maneja.
 * @param[in] Band es la banda que lee este driver.
 * @param[in] DataSize es el tamanio del tipo de dato en bytes.
 */
GdalDriver::GdalDriver(GDALDataset *pDataset, const std::string &Filename,
                       const std::string &DataName, int Band, int DataSize) :
      pDataset_(pDataset), pData_(NULL) {
   driverName_ = std::string("Gdal::") + pDataset->GetDriver()->GetDescription();
   dataName_ = DataName;
   dataSize_ = DataSize;
   fileName_ = Filename;
   band_ = Band;
   bandCount_ = pDataset_->GetRasterCount();
   sizeX_ = pDataset_->GetRasterXSize();
   sizeY_ = pDataset_->GetRasterYSize();
   ovlevel_ = -1;
}

/** dtor */
GdalDriver::~GdalDriver() {
   delete[] static_cast<unsigned char*>(pData_);
}

/**
 * Retorna un buffer interno con los datos
 * @param[in] BlockX Columna del bloque
 * @param[in] BlockY Fila del bloque
 * @return datos en bloque indicado
 */
void* GdalDriver::GetBlock(int BlockX, int BlockY) {
   delete[] static_cast<unsigned char*>(pData_);
   GDALRasterBand *pband = pDataset_->GetRasterBand(band_ + 1);
   pData_ = new unsigned char[GetSizeX() * GetSizeY() * GetDataSize()];
   pband->ReadBlock(BlockX, BlockY, pData_);
   return pData_;
}

/** Lee la banda asociada */
/**
 *  Realiza la lectura de un subset y lo carga en pData.
 *
 *  Utiliza la lectura bloqueada de GDAL (con cache).
 *
 *  Deberia funcionar con cualquier tamanio de bloque de lectura GDAL.
 * @param[out] pData datos leidos, en el tipo de dato del driver.
 *            Debe tener memoria reservada.
 * @param[in] Ulx Coordenada X superior
 * @param[in] Uly Coordenada Y superior
 * @param[in] Lrx Coordenada X inferior
 * @param[in] Lry Coordenada X inferior
 * @return bool que indica si pudo leer la imagen
 */
bool GdalDriver::Read(void* pData, int Ulx, int Uly, int Lrx, int Lry) {
   GDALRasterBand* prawband = pDataset_->GetRasterBand(band_ + 1);
   GDALRasterBand* pband = NULL;

   if (ovlevel_ > -1) {
      pband = prawband->GetOverview(ovlevel_);
   } else {
      pband = prawband;
   }

   // Seteo paso de buffer antes de modificar Lrx y Ulx
   int buffer_step = (Lrx - Ulx);

   // verifico los tamanios
   Ulx = std::max(0, Ulx);
   Uly = std::max(0, Uly);
   Lrx = std::min(Lrx, pband->GetXSize());
   Lry = std::min(Lry, pband->GetYSize());

   int block_x_size, block_y_size;
   pband->GetBlockSize(&block_x_size, &block_y_size);

   // block inicial en x
   int iblockx = Ulx / block_x_size;
   // block final en x
   int fblockx = (int) ceil((double) Lrx / block_x_size);
   int iblocky = Uly / block_y_size;
   int fblocky = (int) ceil((double) Lry / block_y_size);

   for (int j = iblocky; j < fblocky; j++) {
      int rowcount = std::min((j + 1) * block_y_size, Lry)
            - std::max(j * block_y_size, Uly);
      for (int i = iblockx; i < fblockx; i++) {
         int colcount = std::min((i + 1) * block_x_size, Lrx)
               - std::max(i * block_x_size, Ulx);

         GDALRasterBlock *prasterblock = pband->GetLockedBlockRef(i, j);

         if (!prasterblock) {
            REPORT_AND_FAIL_VALUE("D:No se pudo inicializar el bloque GDAL.", false);
         }
         // obtengo el puntero de lectura
         char *pblock = static_cast<char*>(prasterblock->GetDataRef());
         // Calculo y aplico el offset de lectura
         int block_x_offset = std::max(i * block_x_size, Ulx) - i * block_x_size;
         int block_y_offset = std::max(j * block_y_size, Uly) - j * block_y_size;
         int block_offset = block_x_offset + block_y_offset * block_x_size;
         pblock += block_offset * dataSize_;
         char *pbuff = static_cast<char*>(pData);
         // Calculo y aplico el offset de escritura
         int buff_x_offset = std::max(i * block_x_size, Ulx) - Ulx;
         int buff_y_offset = std::max(j * block_y_size, Uly) - Uly;
         int buff_offset = buff_x_offset + buff_y_offset * buffer_step;
         pbuff += buff_offset * dataSize_;
         for (int c = 0; c < rowcount; c++)
            memcpy(pbuff + c * buffer_step * dataSize_,
                   pblock + c * block_x_size * dataSize_, colcount * dataSize_);
         prasterblock->DropLock();
      }
   }

   return true;
}

// ----------------------------- BLOQUE -----------------------------
/**
 * Tamanio del bloque X e Y
 * @param[in] SizeX cantidad de columnas del bloque
 * @param[in] SizeY cantidad de filas del bloque
 */
void GdalDriver::GetBlockSize(int &SizeX, int &SizeY) const {
   if (pDataset_) {
      GDALRasterBand *pband = pDataset_->GetRasterBand(band_ + 1);
      if (pband) {
         pband->GetBlockSize(&SizeX, &SizeY);
      }
   } else {
      REPORT_AND_FAIL("D:Error en el tamano de bloque de datos");
   }
}

/**
 * Obtiene el size recomendado teniendo en cuenta la existencia de overviews.
 */
void GdalDriver::CalcRecommendedSize(int XSize, int YSize, int& XRecomm, int& YRecomm) {
   // Por si falla el chequeo de overviews.
   XRecomm = XSize;
   YRecomm = YSize;

   // Chequeo overviews.
   if (pDataset_ != NULL) {
      GDALRasterBand* pband = pDataset_->GetRasterBand(1);
      if (pband != NULL && pband->GetOverviewCount() > 0) {
         for (int ix = pband->GetOverviewCount() -1; ix > -1; --ix) {
            GDALRasterBand* povband = pband->GetOverview(ix);
            XRecomm = povband->GetXSize();
            YRecomm = povband->GetYSize();
            if (XSize < XRecomm && YSize < YRecomm) {
               break;
            }
         }
         if (XRecomm < XSize && YRecomm < YSize) {
            XRecomm = XSize;
            YRecomm = YSize;
         }
      } else {
         GetSize(XRecomm, YRecomm);
      }
   }
}

/**
 * Estable el tamanio recomendado.
 */
void GdalDriver::SetRecommendedSize(int XRecomm, int YRecomm) {
   RasterSource::SetRecommendedSize(XRecomm, YRecomm);

   if (pDataset_ != NULL) {
      GDALRasterBand* prawband = pDataset_->GetRasterBand(band_ + 1);

      if (HasRecommendedSize() && prawband->GetOverviewCount() > 0) {
         for (int ix = prawband->GetOverviewCount() - 1; ix > -1; --ix) {
            GDALRasterBand* povband = prawband->GetOverview(ix);
            if (xRecomm_ <= povband->GetXSize() && yRecomm_ <= povband->GetYSize()) {
               ovlevel_ = ix;
               break;
            }
         }
      }
   }
}

/**
 * Resetea el estado de los datos referidos a las dimensiones recomendadas.
 */
void GdalDriver::ResetRecommendedStatus() {
   ovlevel_ = -1;
}

}  // namespace suri

#endif   // __GDAL__
