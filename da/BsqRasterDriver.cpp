/*! \file BsqRasterDriver.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string.h>
#include <fstream>

// Includes Suri
#include "BsqRasterDriver.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"

// Includes Wx
// Defines
// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {
namespace driver {
/** string que identifica al interlineado del driver **/
std::string BsqRasterDriver::MuxIdentifier = "BSQ";

/** Constructor que se inicializa a partir de un archivo **/
BsqRasterDriver::BsqRasterDriver(const std::string& Mux,
                                 const std::string& FileName,
                                 int Pixels, int Lines) :
      RawRasterDriver(Mux, "BsqRasterDriver", FileName, Pixels, Lines), pData_(NULL) {
   driverName_ = "BsqRasterDriver";
}

BsqRasterDriver::~BsqRasterDriver() {
   if (pData_)
      delete[] pData_;
}

/** Tamanio del bloque X e Y. Inicialmente se utilizara como tamanio del bloque
 *  toda la imagen */
bool BsqRasterDriver::DoGetBlockSize(int &SizeX, int &SizeY) const {
   if (ToUpper(mux_).compare(MuxIdentifier) != 0)
      return false;
   SizeX = npixels_;
   SizeY = nlines_;
   return true;
}

/** Carga el buffer con el subset **/
bool BsqRasterDriver::DoRead(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry) {
   // Debe configurarse correctamente el interlineado y la banda sobre la cual leer
   if (ToUpper(mux_).compare(MuxIdentifier) != 0 || bandReaderIndex_ < 0)
      return false;
   int pixelsize = SizeOf(dataType_);
   if (pData_ == NULL) {
      pData_ = new char[npixels_ * nlines_];
      std::ifstream file(filename_.c_str(), std::ios::in | std::ios::binary);
      if (file.is_open()) {

         int step = file.tellg();
         // le sumo el offset del archivo
         step += pixelsize * npixels_ * nlines_ * bandReaderIndex_ +
               offset_.fileOffset_.headerOffset_;
         file.seekg(step, std::ios_base::beg);
         char* pbuffpos = static_cast<char*>(pData_);
			int rstep = 0;
         for (int p = 0; p < npixels_ * nlines_; ++p) {
            // comienzo de una linea
            if (p % npixels_ == 0) {
               int boffset = file.tellg();
               // le sumo el offset del archivo
               boffset += offset_.bandOffset_.headerOffset_ + rstep;
               file.seekg(boffset, std::ios_base::beg);
            }
            char* pbandpixel = new char[pixelsize];
            // leo de a pixel
            file.read((char*) pbandpixel, pixelsize);
            memcpy(pbuffpos, pbandpixel, pixelsize);
            pbuffpos += pixelsize;
            delete[] pbandpixel;
            // final de linea
            rstep = 0;
				if (p + 1 % npixels_ == 0) {
               // le sumo el offset del archivo
               rstep = offset_.bandOffset_.tailOffset_;
            }
         }
         file.close();
      } else {
         return false;

      }
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

/** Escribe mas de una banda **/
bool BsqRasterDriver::DoWrite(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                     int Ulx, int Uly, int Lrx, int Lry) {
   if (ToUpper(mux_).compare(MuxIdentifier) != 0)
      return false;

   // TODO: Implementar
   return true;
}

/** Retorna un buffer interno con los datos **/
void* BsqRasterDriver::DoGetBlock(int BlockX, int BlockY) {
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
