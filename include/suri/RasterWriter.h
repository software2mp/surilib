/*! \file RasterWriter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERWRITER_H_
#define RASTERWRITER_H_

// Include standard
#include <vector>
#include <string>

/** namespace suri */
namespace suri {
/** Clase base para la escritura de archivos */
/**
 *  Clase base de la que derivan los "drivers de escritura" a.k.a Writers. Estos
 * son los que escriben los datos de imagen a un archivo.
 */
class RasterWriter {
   /** Ctor. de Copia. */
   RasterWriter(const RasterWriter &RasterWriter);

public:
   /** ctor */
   RasterWriter(const std::string &WriterName, const std::string &Filename);
   /** dtor */
   virtual ~RasterWriter();

   /** Identifica el nombre del driver */
   const std::string& GetWriterName() const {
      return writerName_;
   }
   /** Acceso al nombre del archivo */
   std::string GetFilename() const {
      return filename_;
   }
// ----------------------------- RASTER -----------------------------
   /** Acceso al tamanio */
   void GetSize(int &SizeX, int &SizeY) const;
   /** Acceso a la dimension X */
   int GetSizeX() const;
   /** Acceso a la dimension Y */
   int GetSizeY() const;
   /** Tamanio del raster */
   virtual void SetSize(const int SizeX, const int SizeY);
   /** Tamanio X */
   virtual void SetSizeX(const int Size);
   /** Tamanio Y */
   virtual void SetSizeY(const int Size);
   /** Escribe mas de una banda */
   virtual void Write(const std::vector<int> &BandIndex, std::vector<void*> &Data, int Ulx,
                      int Uly, int Lrx, int Lry)=0;
// ----------------------------- BLOQUE -----------------------------
   /** Tamanio del bloque X e Y */
   virtual void GetBlockSize(int &SizeX, int &SizeY) const;
   /** Tamanio X del bloque */
   int GetBlockSizeX() const;
   /** Tamanio Y del bloque */
   int GetBlockSizeY() const;
   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                             int &Lry);
// -------------------------- MULTI-RASTER --------------------------
   /** Retorna la cantidad de bandas */
   int GetBandCount() const;
   /** Carga la cantidad de bandas */
   void SetBandCount(int BandCount);
// --------------------------- DATA-TYPE ----------------------------
   /** Nombre del tipo de dato */
   std::string GetDataType() const;
   /** Tamanio del dato en bytes */
   int GetDataSize() const;
   /** Nombre del tipo de dato */
   virtual void SetDataType(const std::string& DataType);
protected:
   std::string writerName_; /*! Nombre del writer (formato) */
   std::string filename_; /*! Nombre del archivo */
   int bandCount_; /*! Cantidad de bandas */
   std::string dataType_; /*! Tipo de dato */
   int dataSize_; /*! Tamanio del dato */
   int sizeX_; /*! Dimension en X */
   int sizeY_; /*! Dimension en Y */
private:
   int nextUlx_; /*! proximo Ulx del bloque */
   int nextUly_; /*! proximo Uly del bloque */
   int nextLrx_; /*! proximo Lrx del bloque */
   int nextLry_; /*! proximo Lry del bloque */
   std::vector<int> nextBands_; /*! Las bandas */
};
}

#endif /*RASTERWRITER_H_*/
