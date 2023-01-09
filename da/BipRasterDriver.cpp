/*! \file BipRasterDriver.cpp */
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
#include "BipRasterDriver.h"
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
std::string BipRasterDriver::MuxIdentifier = "BIP";

/** ctor **/
BipRasterDriver::BipRasterDriver(const std::string& Mux,
                                 const std::string& FileName,
                                 int Pixels, int Lines) :
      RawRasterDriver(Mux, "BipRasterDriver", FileName, Pixels, Lines), pData_(NULL) {
   driverName_ = "BipRasterDriver";
}

/** dtor **/
BipRasterDriver::~BipRasterDriver() {
   if (pData_)
      delete[] pData_;
}
/** Tamanio del bloque X e Y. Inicialmente se utilizara como tamanio del bloque
 *  toda la imagen*/
bool BipRasterDriver::DoGetBlockSize(int &SizeX, int &SizeY) const {
   if (ToUpper(mux_).compare(MuxIdentifier) != 0)
      return false;
   SizeX = npixels_;
   SizeY = nlines_;
   return true;
}

/** Carga el buffer con el subset **/
bool BipRasterDriver::DoRead(void *pBuffer, int Ulx, int Uly, int Lrx, int Lry) {
   // Debe configurarse correctamente el interlineado y la banda sobre la cual leer
   if (ToUpper(mux_).compare(MuxIdentifier) != 0 || bandReaderIndex_ < 0)
      return false;
   int pixelsize = SizeOf(dataType_);
   if (pData_ == NULL) {
      pData_ = new char[npixels_ * nlines_];
      std::ifstream file(filename_.c_str(), std::ios::in | std::ios::binary);
      if (file.is_open()) {
         // le sumo el offset del archivo                                                           
			int step = file.tellg();
         step += offset_.fileOffset_.headerOffset_;                                                   
         file.seekg(step, std::ios_base::beg);                                                      
			char* pbuffpos = static_cast<char*>(pData_);
         for (int p = 0; p < npixels_*nlines_; ++p) {
				int bread = pixelsize*RasterDriver::bandCount_;
				if (p % npixels_ == 0) {
					// comienzo de linea
					bread += offset_.lineOffset_.headerOffset_;	
				} else if (p + 1 % npixels_ == 0) {
					// fin de linea
					bread += offset_.lineOffset_.tailOffset_;
				} 
            char* pbandpixel = new char[bread];
            // leo de a pixel
            file.read((char*)pbandpixel, bread);
				char* ppixel = pbandpixel+bandReaderIndex_;
            ppixel += (p % npixels_ == 0) ? offset_.lineOffset_.headerOffset_ : 0;
				memcpy(pbuffpos, ppixel, pixelsize);
            pbuffpos += pixelsize;
				delete[] pbandpixel;
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

/** Escribe mas de una banda **/
bool BipRasterDriver::DoWrite(const std::vector<int> &BandIndex, std::vector<void*> &Data,
                     int Ulx, int Uly, int Lrx, int Lry) {
   if (ToUpper(mux_).compare(MuxIdentifier) != 0)
      return false;
   // TODO: Implementar
   return true;
}

/** Retorna un buffer interno con los datos **/
void* BipRasterDriver::DoGetBlock(int BlockX, int BlockY) {
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
