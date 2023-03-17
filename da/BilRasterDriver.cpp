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

// Includes Estandar
#include <string.h>
#include <fstream>
#include <algorithm>

// Includes Suri
#include "BilRasterDriver.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "logmacros.h"

// Includes Wx
// Defines
// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {
namespace driver {
/** string que identifica al interlineado del driver **/
std::string BilRasterDriver::MuxIdentifier = "BIL";

/** ctor **/
BilRasterDriver::BilRasterDriver(const std::string& Mux,
                                 const std::string& FileName,
                                 int Pixels, int Lines) :
      RawRasterDriver(Mux, "BilRasterDriver", FileName, Pixels, Lines), pData_(NULL) {
   driverName_ = "BilRasterDriver";
}

/** dtor **/
BilRasterDriver::~BilRasterDriver() {
   if (pData_)
      delete[] pData_;
}

/** Tamanio del bloque X e Y. Inicialmente se utilizara como tamanio del bloque
 *  toda la imagen */
bool BilRasterDriver::DoGetBlockSize(int &SizeX, int &SizeY) const {
   if (ToUpper(mux_).compare(MuxIdentifier) != 0)
      return false;
   SizeX = npixels_;
   SizeY = nlines_;
   return true;
}

/** Carga el buffer con la informacion de la banda configurada teniendo
 *  el cuenta el subset solicitado.
 *  En la primera iteracion se cargara toda la imagen en memoria y no se tendra
 *  en cuenta el offset configurado en el driver.
 *  Precondicion: Ya se almaceno la memoria para pbuffer
 *  El interlineado BIL ordena la bandas por columna.
 *  Explicacion mas detallada
 *  http://webhelp.esri.com/arcgisdesktop/9.2/index.cfm?
 *  TopicName=BIL,_BIP,_and_BSQ_raster_files **/
bool BilRasterDriver::DoRead(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry) {
   // verifico si la configuracion permite la lectura del archivo
   if (ToUpper(mux_).compare(MuxIdentifier) != 0 || bandReaderIndex_ < 0 ||
         npixels_ <= 0 || nlines_ <= 0)
      return false;
   /**
    * En caso de no haber realizado ninguna lectura sobre el archivo
    * cargo la informacion en memoria
    * **/
   int pixelsize = SizeOf(dataType_);
   if (pData_ == NULL) {
      pData_ = new char[npixels_ * nlines_];
      std::ifstream file(filename_.c_str(), std::ios::in | std::ios::binary);
      if (file.is_open()) {
         int hoffset = offset_.fileOffset_.headerOffset_;
         hoffset += file.tellg();
         file.seekg(hoffset, std::ios_base::beg);
         char* preadpos = static_cast<char*>(pData_);
			int rstep = 0;
         for (int row = 0; row < nlines_; ++row) {
            /** Me posiciono en la primera columna de la linea actual de la banda **/
            int step = file.tellg();
            step += offset_.bandOffset_.headerOffset_;
            step += (bandReaderIndex_ * pixelsize * npixels_) + rstep;
            file.seekg(step, std::ios_base::beg);
            for (int col = 0; col < npixels_; ++col) {
               // TODO: Tener en cuenta los unsigned
               char* pbandpixel = new char[pixelsize];
               // leo de a pixel
               file.read((char*) pbandpixel, pixelsize);
               memcpy(preadpos, pbandpixel, pixelsize);
               /** muevo el puntero al sig. pixel **/
               preadpos += pixelsize;
               delete[] pbandpixel;
            }
             /** Una vez recorrida las columnas de la fila que corresponden a la
             *  banda realizo un salto de linea. Esto quiere decir que tengo
             *  que saltar las columnas de las bandas que restan de la linea
             **/
           	rstep = 0;
            rstep += offset_.bandOffset_.tailOffset_;
            rstep += (RasterDriver::bandCount_ - bandReaderIndex_ - 1) * npixels_
                  * pixelsize;
         }
         file.close();
      } else
         return false;
   }


   int udim = Lrx >= Ulx ? Lrx - Ulx : Ulx - Lrx;
   int ldim = Lry >= Uly ? Lry - Uly : Uly - Lry;
   /** Adelante el puntero de lectura en funcion de las coordenadas del subset **/
   char* preadpos = pData_;
   /** Salteo la cantidad de pixeles para ulx **/
   preadpos += Ulx * pixelsize;
   /** Salteo la cantidad de lineas para uly **/
   preadpos += (Uly * npixels_ * pixelsize);
   char* pwritepos = static_cast<char*>(pBuffer);
   memcpy(pwritepos, preadpos, udim * ldim * pixelsize);
   return true;
}

/** Escribe mas de una banda.
 *  En la primera iteracion no se tendra en cuenta el offset configurado en el driver
 *  y se supone que la informacion qeu se escribe de la banda es la banda completa
 *  Precondicion la posicion 0 del array de Data corresponde a la posicion 0
 *  del indice de la banda contenido en BandIndex **/
bool BilRasterDriver::DoWrite(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                     int Ulx, int Uly, int Lrx, int Lry) {
   if (ToUpper(mux_).compare(MuxIdentifier) != 0 || BandIndex.empty() || Data.empty())
      return false;
   // Ordeno el vector de bandas para almacenar en orden
   std::vector<int> bands = BandIndex;
   std::vector<int>::iterator it = bands.begin();
   std::ofstream file(filename_.c_str(), std::ios::out | std::ios::binary);
   if (file.is_open()) {
      int pixelsize = SizeOf(dataType_);
      for (int i = 0; it != bands.end(); ++it, ++i) {
         for (int row = 0; row < nlines_; ++row) {
            /** Me posiciono en la primera columna de la linea actual de la banda **/
            file.seekp(*it * pixelsize * npixels_, std::ios_base::cur);
            /** direcciono al byte **/
            char* pdata = static_cast<char*>(Data[i]);
            file.write (pdata, pixelsize*npixels_);
            /** posiciono el file pointer para que salte lo que queda de la linea**/
            file.seekp((RasterWriter::bandCount_ - *it) * (npixels_ * pixelsize),
                       std::ios_base::cur);
         }
      }
      file.close();
   }
   return true;
}

/** Retorna un buffer interno con los datos **/
void* BilRasterDriver::DoGetBlock(int BlockX, int BlockY) {
   if (ToUpper(mux_).compare(MuxIdentifier) != 0 || BlockX == 0 || BlockY == 0
         || BlockX > RasterDriver::sizeX_ || BlockY > RasterDriver::sizeY_ || !pData_)
      return NULL;
   int pixelsize = SizeOf(dataType_);
   char* pblock = new char[pixelsize * BlockX * BlockY];
   memcpy(pblock, pData_, BlockX * BlockY * pixelsize);
   return pblock;
}

}  /** namespace driver **/
}  /** namespace dataaccess **/
}  /** namespace raster **/
}  /** namespace core **/
} /** namespace suri */
