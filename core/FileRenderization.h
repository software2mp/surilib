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

#ifndef FILERENDERIZATION_H_
#define FILERENDERIZATION_H_

/**
 *
 */

// Includes estandar
#include <string>

// Includes Suri
#include "suri/Option.h"
#include "MovingWindowController.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Controlador para la renderizacion a un archivo */
/**
 * Salva un canvas a un archivo usando el nombre de archivo y las opciones
 * recibidas.
 */
class FileRenderization : public MovingWindowController {
public:
   /** Ctor */
   FileRenderization();
   /** Dtor */
   virtual ~FileRenderization();
   /** Modifica el mundo de manera de recorrer el extent completo y asi renderizar */
   virtual bool Render();
   /** Setter del nombre del archivo de salida */
   void SetFilename(const std::string &Filename);
   /** Setter de las opciones de imagen de salida */
   void SetOption(const Option& Options);
   void SetFeathering(const Subset& Intersection, const std::vector<void*>& Data,
                                                       unsigned char NoDataValue);
   void SetNoDataValue(unsigned char Value) { noDataValue_ = Value;}
protected:
private:
   std::string fileName_; /*! Nombre del archivo de salida */
   Option option_; /*! Opciones de imagen de la salida */
   Subset intersection_;
   unsigned char noDataValue_;
   std::vector<void*> featheringData_;
};
}

#endif /* FILERENDERIZATION_H_ */
