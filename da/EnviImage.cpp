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

#include "EnviImage.h"

// Includes standard
#include <fstream>

// Includes suri
#include "BipRasterWriter.h"
#include "suri/DataTypes.h"

/** namespace suri */
namespace suri {
/** Tipo de dato */
std::map<std::string, int> EnviImage::enviDataType_;

/** Constructor default. */
EnviImage::EnviImage() {
}

/** Destructor. */
EnviImage::~EnviImage() {
   if (pWriter_) {
      std::ofstream headerstream;
      headerstream.open((pWriter_->GetFilename() + ".hdr").c_str());
      headerstream << "ENVI" << std::endl << "description = { " << GetOption("desc")
                   << " }" << std::endl << "samples = " << GetSizeX() << std::endl
                   << "lines = " << GetSizeY() << std::endl << "bands = "
                   << GetBandCount() << std::endl << "file type = ENVI Standard"
                   << std::endl << "data type = "
                   << enviDataType_[pWriter_->GetDataType()] << std::endl
                   << "interleave = " << GetOption("mux") << std::endl
                   << "byte order = 0" << std::endl;
      headerstream.close();
   }
}

/** Inspecciona el archivo, para saber si es del formato que maneja */
/**
 *  Esta funcion analiza el archivo y el tipo de acceso para determinar si es
 * el formato que esta clase maneja.
 *
 *  En caso de ImageAccess==WriteOnly, recibe parametros extra y determina si
 * puede crear un archivo para escritura con los parametros deseados.
 *
 * @param[in] Filename Nombre del archivo.
 * @param[in] ImageAccess tipo de acceso a la imagen
 * @param[in] BandCount cantidad de bandas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeX cantidad de columnas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeY cantidad de filas (solo para ImageAccess==WriteOnly)
 * @param[in] DataType tipo de dato raster (solo para ImageAccess==WriteOnly)
 * @return bool True: se reconocio el formato; False: error o no se reconocio
 *  el formato
 */
bool EnviImage::Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                        int BandCount, int SizeX, int SizeY,
                        const std::string &DataType) {
   if (GetOption(Format) == "envi") {
      return true;
   }
   return false;
}

/** Carga la imagen */
/**
 *  Metodo que realiza la lectura completa del archivo en el modo de lectura
 * deseado. Carga las bandas e inicializa los drivers.
 *
 * @param[in] Filename Nombre del archivo.
 * @param[in] ImageAccess tipo de acceso a la imagen
 * @param[in] BandCount cantidad de bandas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeX cantidad de columnas (solo para ImageAccess==WriteOnly)
 * @param[in] SizeY cantidad de filas (solo para ImageAccess==WriteOnly)
 * @param[in] DataType tipo de dato raster (solo para ImageAccess==WriteOnly)
 * @return puntero a Image
 * \todo hacer que se cargue en forma automatica los defaults
 */
Image* EnviImage::Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType) {
   if (ImageAccess == WriteOnly || ImageAccess == ReadWrite) {
      pWriter_ = new BipRasterWriter(Filename);
      pWriter_->SetSize(SizeX, SizeY);
      pWriter_->SetBandCount(BandCount);
      pWriter_->SetDataType(DataType);
      // \todo hacer que se cargue en forma automatica los defaults
      SetOption("mux", "bip");
      // hasta que solucione el writeback
      // ojo que con esto queda inconsistente el blocksize (ver Image::GetBlockSize)
      /*		for (int i=0;i<BandCount;i++)
       {
       RasterBand *pband = RasterBand::Create(DataType);
       pband->SetSize(SizeX,SizeY);
       bandVector_.push_back(pband);
       }*/
   }
   return this;
}

/** Inicializacion de la clase */
void EnviImage::InitializeClass() {
   enviDataType_.insert(std::make_pair(DataInfo<unsigned char>::Name, 1));
   enviDataType_.insert(std::make_pair(DataInfo<short>::Name, 2));
   enviDataType_.insert(std::make_pair(DataInfo<unsigned short>::Name, 12));
   enviDataType_.insert(std::make_pair(DataInfo<int>::Name, 3));
   enviDataType_.insert(std::make_pair(DataInfo<unsigned int>::Name, 13));
   enviDataType_.insert(std::make_pair(DataInfo<float>::Name, 4));
   enviDataType_.insert(std::make_pair(DataInfo<double>::Name, 5));
}
}
