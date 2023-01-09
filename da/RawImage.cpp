/*! \file RawImage.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <map>
#include <utility>

// Includes Suri
#include "suri/DataTypes.h"
#include "suri/RawImage.h"
#include "suri/AuxiliaryFunctions.h"
#include "BipRasterDriver.h"
#include "BsqRasterDriver.h"
#include "BilRasterDriver.h"
// Includes Wx
// Defines
// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {

std::map<std::string, int> RawImage::DataTypes_;
/** ctor **/
RawImage::RawImage() {
}

/** dtor **/
RawImage::~RawImage() {
}

/** Inicializacion de la clase */
void RawImage::InitializeClass() {
   // inicializo tipo de dato ?
   DataTypes_.insert(std::make_pair(DataInfo<unsigned char>::Name, 1));
   DataTypes_.insert(std::make_pair(DataInfo<short>::Name, 2));
   DataTypes_.insert(std::make_pair(DataInfo<unsigned short>::Name, 12));
   DataTypes_.insert(std::make_pair(DataInfo<int>::Name, 3));
   DataTypes_.insert(std::make_pair(DataInfo<unsigned int>::Name, 13));
   DataTypes_.insert(std::make_pair(DataInfo<float>::Name, 4));
   DataTypes_.insert(std::make_pair(DataInfo<double>::Name, 5));
}

/** Metodo que realiza la lectura completa del archivo en el modo de lectura
 * deseado. Carga las bandas e inicializa los drivers.**/
Image* RawImage::Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType) {
   /**
    * si ya se encuentra cargado el array de bandas se supone que anteriormente
    * se cargo la imagen con lo que se evita la lectura de la misma
    */
   if (!bandVector_.empty())
      return this;
   pWriter_ = GetDriver(GetOption(Mux), Filename, SizeX, SizeY);
   pWriter_->SetBandCount(BandCount);
   pWriter_->SetDataType(DataType);
   for (int b = 0; b < BandCount; ++b) {
      driver::RawRasterDriver* pdriver = GetDriver(GetOption(Mux), Filename, SizeX, SizeY);
      pdriver->SetDataType(DataType);
      pdriver->SetBandReaderIndex(b);
      pdriver->SetBandCount(BandCount);
      RasterBand* pband = RasterBand::Create(DataType, pdriver, this);
      if (pband) {
         bandVector_.push_back(pband);
         driverVector_.push_back(pdriver);
      }
   }
   return this;
}

/** Inspecciona el archivo, para saber si es del formato que maneja */
bool RawImage::Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType) {
   // TODO: ver si se debe leer un pedazo del archivo para entender un poco su contenido
   if (GetOption(Format) == "raw") {
      return true;
   }
   return false;
}

/** Configura el driver con el interlineado que se pasa por parametro **/
driver::RawRasterDriver* RawImage::GetDriver(const std::string &Mux,
                                  const std::string &Filename,
                                  int SizeX, int SizeY) {
   driver::RawRasterDriver::RawDriverOffset rawoffset = GetRawOffset();
   // se arma la cadena de responsabilidades para los drivers. Esto podria
   // estar en una clase aparte

   /** TODO(Gabriel TCK #5180): Debe extenderse la lectura de los drivers para soportar
      * indicar endianess de los datos. Por ahora se utiliza el de la maquina
      * en ejecucion
      **/
   driver::RawRasterDriver* pdriver = new driver::BsqRasterDriver(Mux, Filename,
                                                                  SizeX, SizeY);
   pdriver->Adducesor(new driver::BilRasterDriver(Mux, Filename, SizeX, SizeY));
   pdriver->Adducesor(new driver::BipRasterDriver(Mux, Filename, SizeX, SizeY));
   pdriver->SetOffset(rawoffset);
   return pdriver;
}

/** Metodo auxiliar que obtiene el offset configurado en las opciones de la imagen **/
driver::RawRasterDriver::RawDriverOffset RawImage::GetRawOffset() {
   driver::RawRasterDriver::RawDriverOffset rawoffset;
   std::string offset = GetOption("LineHeaderOffset");
   rawoffset.lineOffset_.headerOffset_ = StringToNumber<int>(offset);
   offset = GetOption("LineTailOffset");
   rawoffset.lineOffset_.tailOffset_ = StringToNumber<int>(offset);

   offset = GetOption("FileHeaderOffset");
   rawoffset.fileOffset_.headerOffset_ = StringToNumber<int>(offset);
   offset = GetOption("FileTailOffset");
   rawoffset.fileOffset_.tailOffset_ = StringToNumber<int>(offset);

   offset= GetOption("BandHeaderOffset");
   rawoffset.bandOffset_.headerOffset_ = StringToNumber<int>(offset);
   offset = GetOption("BandTailOffset");
   rawoffset.bandOffset_.tailOffset_ = StringToNumber<int>(offset);
   return rawoffset;
}
} /** namespace dataacess */
} /** namespace raster */
} /** namespace core */
} /** namespace suri */
