/*! \file RasterBand.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERBAND_H_
#define RASTERBAND_H_

// Includes standard
#include <deque>

// Includes suri
#include "RasterSource.h"
#include "RasterDriver.h"

/** namespace suri */
namespace suri {
class Image;

/** RasterSource que almacena datos del raster */
/**
 *  Esta clase representa una banda de una imagen. Guarda datos en memoria y
 * provee acceso de a bloques o por pixel (lento con conversion a double)
 *  El uso de esta clase es para almacenar los datos en memoria (a modo de
 * cache) para trabajar con los mismos (mostrarlos, procesarlos, etc.)
 *  El uso de un driver no es mandatorio, pero, si bien el driver participa
 * de la lista de sources, esta clase se niega a sacarlo de la lista
 * y retornarlo frente a un PopSource (la clase creadora del driver debe ser
 * la responsable de eliminarlo)
 * \note hereda de RasterSource para formar parte de la tuberia de lectura
 *  de imagenes
 */
class RasterBand : public RasterSource {
   /** Ctor. de Copia. */
   RasterBand(const RasterBand &RasterBand);

public:
   /** Ctor */
   RasterBand();
   /** Dtor */
   virtual ~RasterBand();

// ------------------------ METODOS STATICOS ------------------------
   /** Crea una banda del tipo deseado */
   static RasterBand *Create(const std::string &DataType, RasterDriver *pDriver = NULL,
                             Image *pParentImage = NULL);

// ----------------------------- RASTER -----------------------------
   /** Tamanio del raster X e Y */
   virtual void GetSize(int &SizeX, int &SizeY) const;
   /** Tamanio */
   virtual void SetSize(const int SizeX, const int SizeY);
   /** Tamanio X */
   void SetSizeX(const int Size);
   /** Tamanio Y */
   void SetSizeY(const int Size);
   /** Retorna un vector con buffers internos de cada banda del subset pedido */
   virtual void* GetBlock(int BlockX, int BlockY)=0;
   /** Llena los punteros del vector con el subset de las bandas pedidas */
   virtual bool Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry)=0;
   /** Escribe un bloque de banda */
   virtual void Write(void *pData, int Ulx, int Uly, int Lrx, int Lry)=0;
   /** Escribe un bloque de banda */
   virtual void Write(void *pData, int BlockX, int BlockY)=0;
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const;
// ----------------------------- SOURCES ----------------------------
   /** Necesita un pop especial */
   virtual RasterSource *PopSource();

   /** Retorna la imagen padre */
   Image *GetImage() const {
      return pParentImage_;
   }
   /** Retorna el driver */
   RasterDriver *GetDriver() const {
      return pDriver_;
   }
protected:
   int sizeX_; /*! tamanio raster en x */
   int sizeY_; /*! tamanio raster en y */
private:
   RasterDriver *pDriver_; /*! Driver de lectura */
   Image *pParentImage_; /*! Imagen que la contiene (puede ser NULL) */
};
}

#endif /*RASTERBAND_H_*/
