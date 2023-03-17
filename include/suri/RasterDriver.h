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
