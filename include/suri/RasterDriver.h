/*! \file RasterDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERDRIVER_H_
#define RASTERDRIVER_H_

// Includes standard
#include <string>
#include <vector>

// Includes suri
#include "RasterSource.h"

/** namespace suri */
namespace suri {
/** RasterSource que es base de los Drivers de lectura de archivos raster */
/**
 *  Esta clase es la base de los lectores de archivos raster.
 *  Se instancia desde la clase Image derivada asociada y es propiedad de la
 * misma, es decir, la debe eliminar la clase Image.
 *  Tambien se presenta como una fuente de Raster RasterSource ya que tiene
 * las mismas propiedades y puede ser manejada como una mas.
 * \note hereda de RasterSource para formar parte de la tuberia de lectura
 *  de imagenes
 */
class RasterDriver : public RasterSource {
   /** Ctor. de Copia. */
   RasterDriver(const RasterDriver &RasterDriver);

public:
   /** Ctor */
   RasterDriver();
   /** Dtor */
   virtual ~RasterDriver();

   /** Identifica el nombre del driver */
   const std::string& GetDriverName() const {
      return driverName_;
   }
   /** Acceso al nombre del archivo */
   std::string GetFilename() const {
      return fileName_;
   }

// ----------------------------- RASTER -----------------------------
   /** Tamanio del raster X e Y */
   virtual void GetSize(int &SizeX, int &SizeY) const {
      SizeX = sizeX_;
      SizeY = sizeY_;
   }
   /** Tamanio X del raster */
   virtual int GetSizeX() const {
      return sizeX_;
   }
   /** Tamanio Y del raster */
   virtual int GetSizeY() const {
      return sizeY_;
   }
// -------- Lectura de datos --------
   /** Retorna un buffer interno con los datos */
   virtual void* GetBlock(int BlockX, int BlockY)=0;
   /** Carga el buffer con el subset */
   virtual bool Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry)=0;
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const=0;
   /** Tamanio X del bloque */
   virtual int GetBlockSizeX() const;
   /** Tamanio Y del bloque */
   virtual int GetBlockSizeY() const;
// -------------------------- MULTI-RASTER --------------------------
   /** Retorna la cantidad de bandas */
   int GetBandCount() const;
// --------------------------- DATA-TYPE ----------------------------
   /** Nombre del tipo de dato */
   virtual std::string GetDataType() const {
      return dataName_;
   }
   /** Tamanio del dato en bytes */
   virtual int GetDataSize() const {
      return dataSize_;
   }
protected:
   std::string driverName_; /*! Nombre del driver */
   std::string dataName_; /*! Nombre del tipo de dato */
   int dataSize_; /*! Tamanio en bytes del tipo de dato */
   std::string fileName_; /*! Nombre del archivo */
   int band_; /*! Banda representada */
   int bandCount_; /*! Cantidad de bandas */
   int sizeX_; /*! Tamanio en columnas */
   int sizeY_; /*! Tamanio en filas */
private:
};
}

#endif /*RASTERDRIVER_H_*/
