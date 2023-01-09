/*! \file FileCanvas.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILECANVAS_H_
#define FILECANVAS_H_

// Includes suri
#include "suri/Option.h"
#include "MemoryCanvas.h"
#include "suri/Subset.h"
/** namespace suri */
namespace suri {
// Forward
class Image;

/** Canvas cuyo destino es un archivo */
/**
 *  Este canvas representa un archivo en disco. Se utiliza para salvar la
 * renderizacion a un archivo.
 * \note hereda de Canvas para permitir que el pipeline de renderizacion pueda
 * escribir resultados a disco
 */
class FileCanvas : public MemoryCanvas {
   /** Ctor. de Copia. */
   FileCanvas(const FileCanvas &FileCanvas);

public:
   /** Ctor */
   FileCanvas(Image *pImage);
   /** Ctor */
   FileCanvas(const std::string &Filename, const int SizeX, const int SizeY,
              const Option &ImageOptions);
   /** Dtor */
   virtual ~FileCanvas();
   /** Pasa al buffer interno la renderizacion sobre wxDC */
   virtual void Flush(const Mask *pMask);
// ----------------------------- BLOQUE -----------------------------
   /** Bloque de bandas/subset que debe escribir */
   virtual bool GetNextBlock(std::vector<int> &BandIndex, int &Ulx, int &Uly, int &Lrx,
                             int &Lry);
   void AddFeathering();
   /** Devuelve el la ruta con el que fue creado el archivo **/
   std::string GetFileName() const;

   void SetFeathering(const Subset& Intersection, const std::vector<void*>& Data,
                                                       unsigned char NoDataValue);
protected:
   Image *pImage_; /*! Imagen asociada */
   std::string filename_; /*! Nombre del archivo de salida */
   int imageWidth_; /*! ancho de la imagen */
   int imageHeight_; /*! alto de la imagen */
   int extraDataSize_; /*! Cantidad de datos a escribir. */
   std::vector<void*> extraData_; /*! Datos extra a escribir en el siguiente */
   /* flush */
   Option imageOptions_; /*! Opciones de imagen */
   //// para feathering
   /** area en pixel linea donde intersecan los rasters q forman la imagen de salida **/
   unsigned char noDataValue_;
   Subset intersection_;
   std::vector<void*> featheringData_; /** Nuevos valores de pixeles **/
   void SetBackgroundColour();
};
}

#endif /*FILECANVAS_H_*/
