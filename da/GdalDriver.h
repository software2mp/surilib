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
