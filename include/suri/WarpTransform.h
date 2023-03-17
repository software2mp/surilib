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


#ifndef WARPTRANSFORM_H_
#define WARPTRANSFORM_H_

// Includes standard

// Includes Suri
#include "suri/Coordinates.h"
// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Una clase que representa una funcion de transformacion (x,y)=>(x',y').
 * Se espera que los coeficientes de la funcion puedan ser calculados a
 * partir de una serie de puntos de control, o puedan ser cargados manualmente
 * para obtener la transformacion de una serie de puntos automaticamente.
 */
class WarpTransform {
public:
   WarpTransform();
   virtual ~WarpTransform();

   /**
    *Permite determinar si se pudo determinar la funcion de transformacion.
    *@return bool indicando el exito de la determinacion.
    */
   virtual bool IsOk() = 0;
   /**
    *Permite limpiar los coeficientes y demas parametros de la funcion de
    *transformacion
    */
   virtual void Clear() = 0;
   /**
    * Mapea la lista de coordenadas de entrada a la de salida, aplicando
    * la funcion de transformacion calculada previamente
    * @return bool indicando el exito de la aplicacion de la funcion de
    * transformacion.
    * @param[in] Origin Lista de cordenadas origen.
    * @param[out] Destination Lista de coordenadas de destino
    */
   virtual bool ApplyTransform(const CoordinateListType &Origin,
                               CoordinateListType &Destination) = 0;
   /**
    * Mapea la coordenada de entrada a la coordenada de salida,
    * aplicando la funcion de transformacion calculada previamente
    * @return bool indicando el exito de la aplicacion de la funcion de
    * transformacion
    * @param[in] Origin: Coordenada de origen
    * @param[out] Destination: Coordenada de destino
    */
   virtual bool ApplyTransform(const Coordinates &Origin, Coordinates &Destination) = 0;
};
}
#endif /* WARPTRANSFORM_H_ */

