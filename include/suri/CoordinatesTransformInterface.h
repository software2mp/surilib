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

#ifndef COORDINATETRANSFORMINTERFACE_H_
#define COORDINATETRANSFORMINTERFACE_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/Subset.h"
#include "suri/Coordinates.h"
#include "suri/GcpList.h"

// Includes Wx
// Defines
/** Puntos a tener en cuenta en una transformacion de coordenadas */
#define DEFAULT_CALCULATION_POINTS 300
// forwards

namespace suri {

// forwards
class GCPList;

/*!
 * Interaz que representa el comportamiento en comun a todo Transformacion Geometrica
 */
class CoordinatesTransformInterface {
   CoordinatesTransformInterface(const CoordinatesTransformInterface&);

public:
   /** Destruye el objeto */
   static void Destroy(CoordinatesTransformInterface * &pCTransform);

   /** Crea el objeto a partir del WKT */
   /**
    *  Para transformar entre un PCS y su respectivo GCS (forma 1)
    *  @param[in] SRWkt WKT del sistema de coordenadas
    *  @return objeto CoordinatesTransform creado
    */
   static CoordinatesTransformInterface * Create(const std::string& TransformationType,
                                                 const std::string &SRWkt);

   /** Crea el objeto a partir de los WKTs */
   /**
    * Para transformar entre un PCS/GCS y otro PCS/GCS (forma 2)
    * @param[in] SRWktIn WKT del sistema de coordenadas de entrada
    * @param[in] SRWktOut WKT del sistema de coordenadas de salida
    * @return objeto CoordinatesTransform creado
    */
   static CoordinatesTransformInterface * Create(const std::string& TransformationType,
                                                 const std::string &SRWktIn,
                                                 const std::string &SRWktOut);

   /** Transforma de X-Y(in) a X-Y(out) */
   virtual int TransformCoordinates(Coordinates &InOut, bool Inverse = false) const=0;

   /** Transforma de X-Y(in) a X-Y(out) */
   virtual int TransformCoordinates(std::vector<Coordinates> &InOut, bool Inverse =
         false) const=0;
   /** Transforma de Subset(in) a Subset(out) */
   virtual int TransformCoordinates(Subset &InOut, int CalculationPoints =
         DEFAULT_CALCULATION_POINTS,
                                    bool Inverse = false) const=0;
   /** Configura el orden sobre el algoritmo para la transformacion **/
   virtual void SetOrder(const int Order) = 0;
   /** Configura el orden sobre el algoritmo para la transformacion **/
   virtual void SetGcpList(GcpList* pCcpList) = 0;

   /** Devuelve el WKT del sistema de coordenadas de entrada */
   virtual std::string GetWktIn() const=0;

   /** Devuelve el WKT del sistema de coordenadas de salida */
   virtual std::string GetWktOut() const=0;

   /** Indica si es la transformacon identidad */
   virtual bool IsIdentity() const=0;

protected:
   CoordinatesTransformInterface();
   virtual ~CoordinatesTransformInterface();
};
} /** namespace suri */

#endif /* COORDINATETRANSFORMINTERFACE_H_ */
