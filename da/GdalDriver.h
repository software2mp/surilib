/*! \file GdalDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GDALDRIVER_H_
#define GDALDRIVER_H_

#ifdef __GDAL__

// Includes suri
#include "suri/RasterDriver.h"

class GDALDataset;

/** namespace suri */
namespace suri {
/** Clase derivada de RasterDriver para leer usando GDAL */
/**
 *  Deriva de la clase RasterDriver para realizar lectura raster utilizando la
 * biblioteca GDAL.
 */
class GdalDriver : public RasterDriver {
   /** Ctor. de Copia. */
   GdalDriver(const GdalDriver &GdalDriver);

public:
   /** ctor */
   GdalDriver(GDALDataset *pDataset, const std::string &Filename,
              const std::string &DataName, int Band, int DataSize);
   /** dtor */
   virtual ~GdalDriver();

   /** Retorna un buffer interno con los datos */
   virtual void* GetBlock(int BlockX, int BlockY);
   /** Lee la banda asociada */
   virtual bool Read(void* pBuffer, int Ulx, int Uly, int Lrx, int Lry);
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const;

   /**
    * Obtiene el size recomendado teniendo en cuenta la existencia de overviews.
    */
   virtual void CalcRecommendedSize(int XSize, int YSize, int& XRecomm, int& YRecomm);

   /**
    * Estable el tamanio recomendado.
    */
   virtual void SetRecommendedSize(int XRecomm, int YRecomm);

   /**
    * Resetea el estado de los datos referidos a las dimensiones recomendadas.
    */
   virtual void ResetRecommendedStatus();

protected:
private:
   GDALDataset *pDataset_; /*! Dataset usado */
   void *pData_; /*! buffer que contiene 1 bloque */

   int ovlevel_;
};
}

#endif   // __GDAL__
#endif /*GDALDRIVER_H_*/
