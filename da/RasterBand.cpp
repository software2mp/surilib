/*! \file RasterBand.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
