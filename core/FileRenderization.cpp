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

#include "FileRenderization.h"

// Includes estandar
#include <cmath>
#include <string>

// Includes Suri
#include "suri/Image.h"
#include "suri/DataTypes.h"
#include "suri/RasterSpatialModel.h"
#include "RenderizationManager.h"
#include "suri/Dimension.h"
#include "suri/AuxiliaryFunctions.h"
#include "FileCanvas.h"

// Includes Wx

// Includes App

// Defines
/** Cantidad de lineas de la imagen que utiliza como buffer de lectura/escritura. Debo garantizar */
// que sea mayor que el tamanio de bloque
#define BUFFER_LINES_SIZE 500
/** Cantidad de columnas de la imagen que utiliza como buffer de lectura/escritura. */
#define BUFFER_PIXELS_SIZE width

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
FileRenderization::FileRenderization() :
      fileName_(""), option_(Option()) {
   noDataValue_ = 0;
}

/**
 * Destructor
 */
FileRenderization::~FileRenderization() {
}

void FileRenderization::SetFeathering(const Subset &Intersection, const std::vector<void*>& Data,
                                                                      unsigned char NoDataValue){
   noDataValue_ = NoDataValue;
   intersection_ = Intersection;
   featheringData_ = Data;
}
/**
 * Modifica el mundo de manera de recorrer el extent completo y asi renderizar
 * @return resultado de la rendererizacion
 */
bool FileRenderization::Render() {
   if (!pWorld_ || !pRenderizationList_ || fileName_.empty()) {
      return false;
   }
   // calculo el modelo raster para la imagen
   int width = 0, height = 0;
   // el viewport debe traer el tamano raster de salida deseado
   pWorld_->GetViewport(width, height);

   Subset worldextent, viewport(Coordinates(), Coordinates(width, height));
   pWorld_->GetWorld(worldextent);
   RasterSpatialModel *prm = RasterSpatialModel::Create(viewport.ul_, viewport.lr_,
                                                        worldextent.ul_,
                                                        worldextent.lr_);
   Option imageoptions(option_);
   if (prm) {
      imageoptions.SetOption("Matrix", prm->GetWkt());
   }
   RasterSpatialModel::Destroy(prm);
   // Cargo la referencia espacial del mundo
   imageoptions.SetOption("Geo", pWorld_->GetSpatialReference());

   // Inicializo la salida
   FileCanvas *poutput = new FileCanvas(fileName_, width, height, imageoptions);
   poutput->SetFeathering(intersection_, featheringData_, noDataValue_);
   // Modifico la ventana al ancho total de la imagen de entrada
   // y alto tal que me de una cantidad BUFFER_LINES_SIZE de pixeles
   // este es el tamano en pixels de la imagen de entrada que se utilizara como
   // buffer de escritura/lectura
   // Luego modifico el viewport para que tenga dicho tamanio
   SetBufferSize(width, height);
   // Fin - Configuracion de la salida
   SetOutputCanvas(poutput);
   bool returnvalue = MovingWindowController::Render();
   if (!returnvalue) {
      REPORT_DEBUG("D:Error en la renderizacion del elemento: %s", fileName_.c_str());
   }

   delete poutput;
   return returnvalue;
}

/**
 * Setter del nombre del archivo de salida
 * @param[in] Filename Nombre del archivo de salida
 */
void FileRenderization::SetFilename(const std::string &Filename) {
   fileName_ = Filename;
}

/**
 * Setter de las opciones de imagen de salida
 * @param[in] Option opciones de imagen
 */
void FileRenderization::SetOption(const Option& Options) {
   option_ = Options;
}

}
