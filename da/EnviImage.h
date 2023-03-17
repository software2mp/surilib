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

#ifndef ENVIIMAGE_H_
#define ENVIIMAGE_H_

// Includes standard
#include <map>

// Includes suri
#include "suri/Image.h"

/** namespace suri */
namespace suri {
/** Clase que maneja imagenes estandar Envi HDR */
/**
 * Maneja imagenes estandar Envi HDR: verifica el archivo de la imagen para
 * saber si maneja el formato, carga la imagen y permite crear imagenes en este
 * formato.
 */
class EnviImage : public Image {
   /** Ctor. de Copia. */
   EnviImage(const EnviImage &EnviImage);
public:
   /** ctor */
   EnviImage();
   /** dtor */
   virtual ~EnviImage();
   /** Inspecciona el archivo, para saber si es del formato que maneja */
   virtual bool Inspect(const std::string &Filename, ImageAccessType ImageAccess,
                        int BandCount, int SizeX, int SizeY,
                        const std::string &DataType);
   /** Carga la imagen */
   virtual Image* Load(const std::string &Filename, ImageAccessType ImageAccess,
                       int BandCount, int SizeX, int SizeY,
                       const std::string &DataType);
   /** Crea una imagen envi */
   static Image* Create() {
      return new EnviImage;
   }
   /** Retorna el nombre de la clase */
   static std::string GetClassId() {
      return "EnviImage";
   }
protected:
   /** Inicializacion de la clase */
   virtual void InitializeClass();
private:
   static std::map<std::string, int> enviDataType_; /*! Tipo de dato */
};
}

#endif /*ENVIIMAGE_H_*/
