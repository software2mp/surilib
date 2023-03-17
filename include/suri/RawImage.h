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

#ifndef RAWIMAGE_H_
#define RAWIMAGE_H_

// Includes Estandar
#include <string>
#include <map>

// Includes Suri
#include "suri/Image.h"
#include "suri/RawRasterDriver.h"

// Includes Wx

// Defines

// forwards

namespace suri {
namespace core {
namespace raster {
namespace dataaccess {

/**
 *  Clase que representa una imagen en formato "crudo"
 *  Para estos tipos de imagenes es posible definir los offset de archivo, linea
 *  y banda (encabezado y cola) asi tambien como el interlineado que contiene
 *  (BIP, BSQ o BIL)
 */
class RawImage : public suri::Image {
public:
   /** constructor **/
   RawImage();
   /** dtor */
   virtual ~RawImage();
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                        int BandCount, int SizeX, int SizeY,
                        const std::string &DataType);
   /** Metodo que realiza la lectura completa del archivo en el modo de lectura
    * deseado. Carga las bandas e inicializa los drivers.*/
   virtual Image* Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType);
   /** Crea una imagen envi */
   static Image* Create() {
      return new RawImage;
   }
   /** Retorna el nombre de la clase */
   static std::string GetClassId() {
      return "RawImage";
   }
protected:
   /** Inicializacion de la clase */
   virtual void InitializeClass();
   /** Configura el driver con el interlineado que se pasa por parametro **/
   driver::RawRasterDriver* GetDriver(const std::string &Mux,
                                      const std::string &Filename, int SizeX,
                                      int SizeY);
   /** Metodo auxiliar que obtiene el offset configurado en las opciones de la imagen **/
   driver::RawRasterDriver::RawDriverOffset GetRawOffset();
   static std::map<std::string, int> DataTypes_;
};

} /** namespace dataacess */
} /** namespace raster */
} /** namespace core */
} /** namespace suri */

#endif /* RAWIMAGE_H_ */
