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

#include "suri/RasterBand.h"

// Includes suri
#include "suri/RasterDriver.h"
#include "RasterFactory.h"
#include "logmacros.h"

/** namespace suri */
namespace {
/** Clase que registra todos los RasterBands */
class Startup {
public:
   /** ctor */
   Startup() {
      suri::RegisterAllRasterBands();
   }
   /** dtor */
   ~Startup() {
   }
};

Startup startup;
}

namespace suri {
/** ctor */
RasterBand::RasterBand() :
      sizeX_(0), sizeY_(0), pDriver_(NULL), pParentImage_(NULL) {
}

/** dtor */
RasterBand::~RasterBand() {
}

/**
 * Crea una banda del tipo deseado
 * @param[in] DataType tipo de datos en banda
 * @param[in] pDriver lector del archivo raster
 * @param[in] pParentImage clase que contiene a la banda
 * @return RasterBand generado, NULL si no pudo crear clase
 */
RasterBand *RasterBand::Create(const std::string &DataType, RasterDriver *pDriver,
                               Image *pParentImage) {
   if (DataType == "void") {
      return NULL;
   }
   RasterBand *pband = TheBandFactory::Instance().CreateClass(DataType);
   pband->pDriver_ = pDriver;
   pband->PushSource(pDriver);
   pband->pParentImage_ = pParentImage;
   return pband;
}

// ----------------------------- RASTER -----------------------------
/**
 * Retorna tamanio la banda X e Y
 * @param[out] SizeX ancho de la banda
 * @param[out] SizeY alto de la banda
 */
void RasterBand::GetSize(int &SizeX, int &SizeY) const {
   if (!pSource_) {
      SizeX = sizeX_;
      SizeY = sizeY_;
   } else {
      pSource_->GetSize(SizeX, SizeY);
   }
}

/**
 * Modifica tamanio de la banda
 * @param[in] SizeX nuevo ancho de la imagen
 * @param[in] SizeY nuevo alto de la imagen
 */
void RasterBand::SetSize(const int SizeX, const int SizeY) {
   if (pSource_) {
      REPORT_AND_FAIL("D:RasterBand: La banda posee una fuente de datos.");
   }

   sizeX_ = SizeX;
   sizeY_ = SizeY;
}

/**
 * Tamanio X
 * @param[in] Size nuevo ancho de la imagen
 */
void RasterBand::SetSizeX(const int Size) {
   SetSize(Size, sizeY_);
}

/**
 * Tamanio Y
 * @param[in] Size nuevo alto de la imagen
 */
void RasterBand::SetSizeY(const int Size) {
   SetSize(sizeX_, Size);
}

// ----------------------------- BLOQUE -----------------------------
/**
 * Tamanio del bloque X e Y
 * @param[out] SizeX ancho del bloque
 * @param[out] SizeY alto del bloque
 */
void RasterBand::GetBlockSize(int &SizeX, int &SizeY) const {
   if (!pSource_) {
      SizeX = sizeX_;
      SizeY = sizeY_;
   } else {
      pSource_->GetBlockSize(SizeX, SizeY);
   }
}

// ----------------------------- SOURCES ----------------------------
/** Elimina una fuente */
/**
 *  Saca la primer fuente del stack. En caso de que se trate de un driver de
 * lectura, no lo saca.
 * @return RasterSource que se extrajo
 * \pre La fuente no debe ser el driver.
 */
RasterSource *RasterBand::PopSource() {
   if (GetDriver() == pSource_) {
      return NULL;
   }
   return RasterSource::PopSource();
}
}
