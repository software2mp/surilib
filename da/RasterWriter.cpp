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

#include "suri/RasterWriter.h"

// Includes suri
#include "suri/DataTypes.h"
#include "logmacros.h"

/** namespace suri */
namespace suri {
/** ctor */
/**
 * @param[in] WriterName el nombre del Writer para pasarlo para arriba.
 * @param[in] Filename nombre del archivo donde voy a escribir
 */
RasterWriter::RasterWriter(const std::string &WriterName, const std::string &Filename) :
      writerName_(WriterName), filename_(Filename), bandCount_(0),
      dataType_(DataInfo<void>::Name), dataSize_(DataInfo<void>::Size), sizeX_(0),
      sizeY_(0), nextUlx_(-1), nextUly_(-1), nextLrx_(-1), nextLry_(-1) {
}

/** dtor */
RasterWriter::~RasterWriter() {
}

// ----------------------------- RASTER -----------------------------
/**
 * Tamanio del raster X e Y
 * @param[out] SizeX ancho de la imagen
 * @param[out] SizeY alto de la imagen
 */
void RasterWriter::GetSize(int &SizeX, int &SizeY) const {
   SizeX = GetSizeX();
   SizeY = GetSizeY();
}

/**
 * Tamanio X del raster
 * @return ancho de la imange
 */
int RasterWriter::GetSizeX() const {
   return sizeX_;
}

/**
 * Tamanio Y del raster
 * @return alto de la imagen
 */
int RasterWriter::GetSizeY() const {
   return sizeY_;
}
/**
 * Tamanio del raster
 * @param[in] SizeX nuevo ancho de la imagen
 * @param[in] SizeY nuevo alto de la imagen
 */
void RasterWriter::SetSize(const int SizeX, const int SizeY) {
   if (sizeX_ != 0 && sizeY_ != 0) {
      REPORT_AND_FAIL("D:RasterWriter: Dimensiones ya establecidas");
   }

   sizeX_ = SizeX;
   sizeY_ = SizeY;
}

/**
 * Tamanio X
 * @param[in] Size nuevo ancho de la imagen
 */
void RasterWriter::SetSizeX(const int Size) {
   SetSize(Size, sizeY_);
}

/**
 * Tamanio Y
 * @param[in] Size nuevo alto de la imagen
 */
void RasterWriter::SetSizeY(const int Size) {
   SetSize(sizeX_, Size);
}

// ----------------------------- BLOQUE -----------------------------
/**
 * Tamanio del bloque X e Y
 * @param[out] SizeX ancho de un bloque
 * @param[out] SizeY alto de un bloque
 */
void RasterWriter::GetBlockSize(int &SizeX, int &SizeY) const {
   SizeX = GetSizeX();
   SizeY = GetSizeY();
}

/**
 * Tamanio X del bloque
 * @return ancho de un bloque
 */
int RasterWriter::GetBlockSizeX() const {
   int temp, dummy;
   GetBlockSize(temp, dummy);
   return temp;
}

/**
 * Tamanio Y del bloque
 * @return altura de un bloque
 */
int RasterWriter::GetBlockSizeY() const {
   int temp, dummy;
   GetBlockSize(dummy, temp);
   return temp;
}

/**
 *  Este metodo carga los datos para el bloque siguiente de escritura.
 * Utiliza el tamanio de bloque para calcular el siguiente subset que va de
 * izquierda a derecha y de arriba a abajo para todas las bandas en
 * simultaneo.
 *
 *  En caso de desear otro tipo de lectura, sobrecargar este metodo con el
 * comportamiento deseado.
 *
 *  En caso de que el comportamiento de bloque sea el deseado, pero no el de
 * las bandas, sobrecargar, llamar a este metodo y luego reinicializar las
 * bandas como se desee.
 * @param[out] BandIndex indice de la capas del vector
 * @param[out] Ulx upper left x del proximo bloque
 * @param[out] Uly upper left y del proximo bloque
 * @param[out] Lrx lower right x del proximo bloque
 * @param[out] Lry lower right y del proximo bloque
 * @return False cuando se pide 1 mas que el tamanio del Raster.
 */
bool RasterWriter::GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly,
                                int &Lrx, int &Lry) {
   // cargo todas las bandas porque en bip las bajo todas juntas
   nextBands_.clear();
   for (int i = 0; i < GetBandCount(); i++)
      nextBands_.push_back(i);
   int blocksizex, blocksizey;
   GetBlockSize(blocksizex, blocksizey);
   int x, y;
   GetSize(x, y);
   if (nextUlx_ == -1 || nextUly_ == -1 || nextLrx_ == -1 || nextLry_ == -1) {
      // Inicializo la siguiente lectura como la inicial por si deseo volver a
      // empezar a escribir.
      nextUlx_ = 0;
      nextUly_ = 0;
      nextLrx_ = blocksizex;
      nextLry_ = blocksizey;
   } else
   // Si el Y del UL es mayor que el tamanio => termine
   if (nextUly_ >= y) {
      nextUlx_ = -1;
      nextUly_ = -1;
      nextLrx_ = -1;
      nextLry_ = -1;
      return false;
   }
   // Asigno el subset del bloque
   Ulx = nextUlx_;
   Uly = nextUly_;
   Lrx = nextLrx_;
   Lry = nextLry_;
   BandIndex = nextBands_;
   // Incremento Ulx en un bloque
   nextUlx_ += blocksizex;
   // Si me paso lo llevo a la primer columna
   if (nextUlx_ >= x) {
      nextUlx_ = 0;
   }
   // el X del LR es 1 tamanio de bloque mas que el X de UL
   nextLrx_ = nextUlx_ + blocksizex;
   // Si se pasa, lo lleva al maximo (el tamanio de la imagen)
   nextLrx_ = std::min(nextLrx_, x);
   // Si el X es 0 avanzo el Y en 1 bloque
   if (nextUlx_ == 0) {
      nextUly_ += blocksizey;
   }
   // el Y del LR es 1 tamanio de bloque mas que el Y del UL
   nextLry_ = nextUly_ + blocksizey;
   // Si se pasa, lo lleva al maximo (el tamanio de la imagen)
   nextLry_ = std::min(nextLry_, y);
   return true;
}

// -------------------------- MULTI-RASTER --------------------------
/**
 * Retorna la cantidad de bandas
 * @return cantidad de bandas
 */
int RasterWriter::GetBandCount() const {
   return bandCount_;
}

/**
 * Carga la cantidad de bandas
 * @param[in] BandCount cantidad de capas en raster
 */
void RasterWriter::SetBandCount(int BandCount) {
   if (bandCount_ != 0) {
      REPORT_AND_FAIL("D:RasterWriter: No se puede cambiar la cantidad de bandas");
   }
   bandCount_ = BandCount;
}

// --------------------------- DATA-TYPE ----------------------------
/**
 * @return nombre del tipo de dato
 */
std::string RasterWriter::GetDataType() const {
   return dataType_;
}

/**
 * @return tamanio del dato en bytes
 */
int RasterWriter::GetDataSize() const {
   return dataSize_;
}

/**
 * Nombre del tipo de dato
 * @param[in] DataType nuevo tipo de datos del raster
 */
void RasterWriter::SetDataType(const std::string& DataType) {
   if (dataType_ != DataInfo<void>::Name) {
      REPORT_AND_FAIL("D:RasterWriter: Tipo de dato ya establecido.");
   }
   dataType_ = DataType;
   dataSize_ = SizeOf(dataType_);
}
}
