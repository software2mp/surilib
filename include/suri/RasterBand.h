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
