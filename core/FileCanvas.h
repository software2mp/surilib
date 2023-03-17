/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
