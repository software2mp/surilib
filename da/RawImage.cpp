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
