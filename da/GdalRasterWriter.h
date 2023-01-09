/*! \file GdalRasterWriter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GDALRASTERWRITER_H_
#define GDALRASTERWRITER_H_

#include "suri/RasterWriter.h"

class GDALDataset;

/** namespace suri */
namespace suri {
/** Clase para el manejo del driver de escritura de GDAL */
/**
 * Clase para el manejo del driver de escritura de GDAL. Obtiene el bloque
 * de bandas / subset que debe escribir. Calcula el tamanio x e y de un bloque.
 */
class GdalRasterWriter : public RasterWriter {
public:
   /** ctor */
   GdalRasterWriter(GDALDataset *pDataset, const std::string &Filename);
   /** dtor */
   virtual ~GdalRasterWriter();

   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                             int &Lry);
   /** Escritura de un bloque */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                      int Uly, int Lrx, int Lry);
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const;
private:
   GDALDataset *pDataset_; /*! Dataset asociado */
};
}

#endif /* GDALRASTERWRITER_H_ */
