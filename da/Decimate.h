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

#ifndef DECIMATE_H_
#define DECIMATE_H_

// Includes standard
#include <map>
#include <string>

// Includes suri
#include "suri/SourceSupervisor.h"

/** namespace suri */
namespace suri {
/** Clase para realizar decimacion a nivel fuente */
/**
 *  Esta clase supervisa a clases derivadas de RasterSource que realizan
 * decimacion. La misma se agrega a una imagen a traves del metodo
 * Image::PushSource(). Este metodo utiliza la interfaz SourceSupervisor para
 * requerirle las fuentes para cada una de las bandas. Desde el codigo cliente
 * se utiliza esta clase derivada para cambiar la dimension de las fuentes
 * creadas al agregarlas a las bandas.
 */
class Decimate : public SourceSupervisor {
   /** Ctor. de Copia. */
   Decimate(const Decimate &Decimate);

public:
   /** ctor */
   Decimate(int SizeX, int SizeY = 0);
   /** dtor */
   virtual ~Decimate();

   /** Debe setear la nueva dimension */
   virtual void SetSize(int SizeX, int SizeY = 0);
   /** Debe setear la dimension X manteniendo la proporcion */
   virtual void SetSizeX(int SizeX);
   /** Debe setear la dimension Y manteniendo la proporcion */
   virtual void SetSizeY(int SizeY);
   /** Permite obtener las dimensiones decimadas a partir de las reales */
   virtual void Real2Resized(double RealX, double RealY, double &ResizedX,
                             double &ResizedY) const;
   /** Permite obtener las dimensiones reales a partir de las decimadas */
   virtual void Resized2Real(double ResizedX, double ResizedY, double &RealX,
                             double &RealY) const;

protected:
   /** Crea las fuentes que supervisa */
   virtual RasterSource *Create();
private:
   int sizeX_; /*! Tamanio de columnas decimado */
   int sizeY_; /*! Tamanio de filas decimado */
};
}

#endif /*DECIMATE_H_*/
