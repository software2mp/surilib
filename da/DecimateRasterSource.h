/*! \file DecimateRasterSource.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DECIMATERASTERSOURCE_H_
#define DECIMATERASTERSOURCE_H_

// Includes suri
#include "suri/RasterDriver.h"

/** namespace suri */
namespace suri {
/** Clase fuente que decima con NN la lectura de datos */
/**
 *  Esta clase sirve para decimar la lectura de datos de una imagen.
 *
 *  Se intercala adelante de otra fuente. Se establece un tamanio de imagen
 * deseado y este se ocupa de calcular el paso necesario para lograr dicha
 * dimension.
 */
class DecimateRasterSource : public RasterSource {
   /** Ctor. de Copia. */
   DecimateRasterSource(const DecimateRasterSource &DecimateRasterSource);

public:
   /** tipo de funcion usada para decimar */
   typedef void (*DecimateFunc)(void*, void*, size_t, double);

   /** ctor */
   DecimateRasterSource(SourceSupervisor *pSupervisor, int NewX, int NewY = 0);
   /** dtor */
   virtual ~DecimateRasterSource();

// -------- Dimensiones --------
   /** Debe setear la nueva dimension */
   virtual void SetSize(int SizeX, int SizeY = 0);
   /** Debe setear la dimension X manteniendo la proporcion */
   virtual void SetSizeX(int SizeX);
   /** Debe setear la dimension Y manteniendo la proporcion */
   virtual void SetSizeY(int SizeY);
   /** obtiene el tamanio original */
   virtual void GetRealSize(int &SizeX, int &SizeY);
   /** obtiene el tamanio decimado */
   virtual void GetSize(int &SizeX, int &SizeY) const {
      SizeX = sizeX_;
      SizeY = sizeY_;
   }
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const;
// -------- Lectura de datos --------
   /** Retorna un buffer interno con los datos */
   virtual void* GetBlock(int BlockX, int BlockY);
   /** Carga el buffer con el subset */
   virtual bool Read(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry);
// -------- Ingesta de datos --------
   /** Actualiza */
   virtual void Update();

   /**
    * Resetea el estado de los datos referidos a las dimensiones recomendadas.
    */
   virtual void ResetRecommendedStatus() { }

protected:
private:
   int sizeX_; /*! cantidad de columnas en imagen nueva. */
   int sizeY_; /*! cantidad de filas en imagen nueva. */
   double stepX_; /*! paso en X */
   double stepY_; /*! paso en Y */
   DecimateFunc pFunction_; /*! Funcion decimadora */
   unsigned char *pBuffer_; /*! Puntero interno a datos usado por GetBuffer */
};
}

#endif /*DECIMATERASTERSOURCE_H_*/
