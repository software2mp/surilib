/*! \file Decimate.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "Decimate.h"

// Includes suri
#include "suri/DataTypes.h"
#include "suri/RasterDriver.h"
#include "suri/AuxiliaryFunctions.h"
#include "DecimateRasterSource.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] SizeX cantidad de columnas deseadas.
 * @param[in] SizeY cantidad de filas deseadas (valor por defaul 0 para mantener
 *  la proporcion)
 */
Decimate::Decimate(int SizeX, int SizeY) :
      sizeX_(SizeX), sizeY_(SizeY) {
   SetSize(SizeX, SizeY);
}

/** Destructor */
Decimate::~Decimate() {
}

/**
 * Debe setear la nueva dimension
 *  Recorre la lista de fuentes manejadas (supervisadas) y modifica las
 * dimensiones.
 *
 * @param[in] SizeX nueva cantidad de columnas.
 * @param[in] SizeY nueva cantidad de filas.
 */
void Decimate::SetSize(int SizeX, int SizeY) {
   SourceSetType::iterator it = childSources_.begin();
   SourceSetType::iterator end = childSources_.end();
   while (it != end) {
      DecimateRasterSource *ptemp = dynamic_cast<DecimateRasterSource*>((*it));
      if (ptemp) {
         ptemp->SetSize(SizeX, SizeY);
      }
      it++;
   }
   sizeX_ = SizeX;
   sizeY_ = SizeY;
}

/**
 * Debe setear la dimension X manteniendo la proporcion
 *  Recorre la lista de fuentes manejadas (supervisadas) y modifica las
 * dimensiones.
 *
 * @param[in] SizeX nueva cantidad de columnas.
 */
void Decimate::SetSizeX(int SizeX) {
   SourceSetType::iterator it = childSources_.begin();
   SourceSetType::iterator end = childSources_.end();
   while (it != end) {
      DecimateRasterSource *ptemp = dynamic_cast<DecimateRasterSource*>((*it));
      if (ptemp) {
         ptemp->SetSize(SizeX, 0);
      }
      it++;
   }
   sizeX_ = SizeX;
}

/**
 * Debe setear la dimension Y manteniendo la proporcion
 *  Recorre la lista de fuentes manejadas (supervisadas) y modifica las
 * dimensiones.
 *
 * @param[in] SizeY nueva cantidad de filas.
 */
void Decimate::SetSizeY(int SizeY) {
   SourceSetType::iterator it = childSources_.begin();
   SourceSetType::iterator end = childSources_.end();
   while (it != end) {
      DecimateRasterSource *ptemp = dynamic_cast<DecimateRasterSource*>((*it));
      if (ptemp) {
         ptemp->SetSize(0, SizeY);
      }
      it++;
   }
   sizeY_ = SizeY;
}

/**
 * Permite obtener las dimensiones decimadas a partir de las reales.
 * Para obtener las dimensiones decimadas multiplica UL*Tamanio Deimado y
 * eso se divide por el tamano original (filas o columnas) de la fuente
 * (por ejemplo del Raster).
 * @param[in] RealX Ulx o Lrx original
 * @param[in] RealY Uly o Lry original
 * @param[out] ResizedX Ulx o Lrx decimado
 * @param[out] ResizedY Uly o Lry decimado
 */
void Decimate::Real2Resized(double RealX, double RealY, double &ResizedX,
                            double &ResizedY) const {
   SourceSetType::const_iterator it = childSources_.begin();
   if (it == childSources_.end()) {
      return;
   }
   DecimateRasterSource *ptemp = dynamic_cast<DecimateRasterSource*>((*it));
   if (!ptemp) {
      return;
   }
   int rx, ry;
   ptemp->GetRealSize(rx, ry);
   int x, y;
   ptemp->GetSize(x, y);
   ResizedX = RealX * x / rx;
   ResizedY = RealY * y / ry;
}

/**
 * Permite obtener las dimensiones reales a partir de las decimadas
 * Para obtener las dimensiones reales multiplica UL*tamano original
 * (filas o columnas) y todo eso se divide por el Tamanio Deimado de la fuente
 * (por ejemplo del Raster).
 * @param[in] ResizedX Ulx o Lrx decimado
 * @param[in] ResizedY Uly o Lry decimado
 * @param[out] RealX Ulx o Lrx original
 * @param[out] RealY Uly o Lry original
 */
void Decimate::Resized2Real(double ResizedX, double ResizedY, double &RealX,
                            double &RealY) const {
   SourceSetType::const_iterator it = childSources_.begin();
   if (it == childSources_.end()) {
      return;
   }
   DecimateRasterSource *ptemp = dynamic_cast<DecimateRasterSource*>((*it));
   if (!ptemp) {
      return;
   }
   int rx, ry;
   ptemp->GetRealSize(rx, ry);
   int x, y;
   ptemp->GetSize(x, y);
   RealX = ResizedX * rx / x;
   RealY = ResizedY * ry / y;
}

/**
 *  Crea una instancia de la fuente manejada con las dimensiones deseadas.
 *
 *  Este metodo se usa desde Image::PushSource() para agregar fuentes a las
 * bandas. La fuente creada entra en la lista.
 * @return RasterSource generado
 */
RasterSource *Decimate::Create() {
   return new DecimateRasterSource(this, sizeX_, sizeY_);
}
}
