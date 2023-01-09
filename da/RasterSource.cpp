/*! \file RasterSource.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/RasterSource.h"

// Includes standard
#include <cassert>

// Includes suri
#include "logmacros.h"
#include "suri/DataTypes.h"
#include "suri/SourceSupervisor.h"

// Defines
/** quiero que se defina por default que el supervisor sea estricto */
#ifndef __STRIC_SOURCE_SUPERVISOR__
#define __STRIC_SOURCE_SUPERVISOR__ 1
#endif

/** namespace suri */
namespace suri {
/**
 * constructor
 * @param[in] pSupervisor supervisor de la clase
 */
RasterSource::RasterSource(SourceSupervisor *pSupervisor) :
      pSource_(NULL), xRecomm_(-1), yRecomm_(-1), pSupervisor_(pSupervisor) {
}

/** dtor */
RasterSource::~RasterSource() {
   // Si tiene supervisor se elimina de la lista
   if (pSupervisor_) {
      pSupervisor_->DelSource(this);
   }
}

/**
 * @param[out] SizeX ancho de la imagen
 * @param[out] SizeY alto de la imagen
 */
void RasterSource::GetSize(int &SizeX, int &SizeY) const {
   if (pSource_) {
      pSource_->GetSize(SizeX, SizeY);
   } else {
      REPORT_AND_FAIL("D:IRasterSource no tiene fuente y no implementa GetSize");
   }
}

/**
 * Tamanio X del raster
 * @return ancho de la imange
 */
int RasterSource::GetSizeX() const {
   int temp, dummy;
   GetSize(temp, dummy);
   return temp;
}

/**
 * Tamanio Y del raster
 * @return alto de la imange
 */
int RasterSource::GetSizeY() const {
   int temp, dummy;
   GetSize(dummy, temp);
   return temp;
}

/**
 * Tamanio del bloque X e Y
 * @param[out] SizeX ancho de un bloque
 * @param[out] SizeY alto de un bloque
 */
void RasterSource::GetBlockSize(int &SizeX, int &SizeY) const {
   if (pSource_) {
      pSource_->GetBlockSize(SizeX, SizeY);
   } else {
      REPORT_AND_FAIL("D:IRasterSource no tiene fuente y no implementa GetBlockSize");
   }
}

/**
 * Tamanio X del bloque
 * @return ancho de un bloque
 */
int RasterSource::GetBlockSizeX() const {
   int temp, dummy;
   GetBlockSize(temp, dummy);
   return temp;
}

/**
 * Tamanio Y del bloque
 * @return alto de un bloque
 */
int RasterSource::GetBlockSizeY() const {
   int temp, dummy;
   GetBlockSize(dummy, temp);
   return temp;
}

/**
 * @return nombre del tipo de dato
 */
std::string RasterSource::GetDataType() const {
   if (pSource_) {
      return pSource_->GetDataType();
   }
   REPORT_AND_RETURN_VALUE("D:Retornando tipo de dato %s.", DataInfo<void>::Name,
                           DataInfo<void>::Name);
}

/**
 * @return tamanio del dato en bytes
 */
int RasterSource::GetDataSize() const {
   if (pSource_) {
      return pSource_->GetDataSize();
   }
   REPORT_AND_RETURN_VALUE("D:Retornando tamano de dato %d.", DataInfo<void>::Size,
                           DataInfo<void>::Size);
}

/** Agrega una fuente */
/**
 *  Agrega una fuente al frente de la lista
 *  Actualiza las fuentes.
 *  @param[in] pSource nueva fuente
 */
void RasterSource::PushSource(RasterSource *pSource) {
   if (!pSource) {
      return;
   }
   pSource->pSource_ = pSource_;
   pSource_ = pSource;
   UpdateSource();
}

/** Elimina la ultima fuente */
/**
 *  Saca la primer fuente de la lista.
 *  @return La fuente sacada, el que llama debe destruirla
 */
RasterSource *RasterSource::PopSource() {
   if (!pSource_) {
      return NULL;
   }
   RasterSource *psource = pSource_;
   pSource_ = pSource_->pSource_;
   return psource;
}

/** Actualiza las fuentes */
/**
 *  Llama a Update de las fuentes de atras para adelante para propagar cualquier
 * cambio.
 */
void RasterSource::UpdateSource() {
   if (pSource_ == NULL) {
      return;
   }
   pSource_->UpdateSource();
   Update();
}

/**
 * Obtiene el size recomendado teniendo en cuenta la existencia de overviews.
 */
void RasterSource::CalcRecommendedSize(int XSize, int YSize, int& XRecomm, int& YRecomm) {
   if (pSource_ != NULL) {
      pSource_->CalcRecommendedSize(XSize, YSize, XRecomm, YRecomm);
   } else {
      XRecomm = XSize;
      YRecomm = YSize;
   }
}

/**
 * Estable el tamanio recomendado.
 */
void RasterSource::SetRecommendedSize(int XRecomm, int YRecomm) {
   xRecomm_ = XRecomm;
   yRecomm_ = YRecomm;
}

/**
 * Indica si fue establecida un tamanio recomendado.
 */
bool RasterSource::HasRecommendedSize() {
   return xRecomm_ > -1 && yRecomm_ > -1;
}

/**
 * Resetea el estado de los datos referidos a las dimensiones recomendadas.
 */
void RasterSource::ResetRecommendedStatus() {
   if (pSource_ != NULL) {
      pSource_->ResetRecommendedStatus();
   } else {
      xRecomm_ = -1;
      yRecomm_ = -1;
   }
}

}  // namespace suri
