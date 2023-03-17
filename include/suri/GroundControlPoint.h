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

#ifndef GROUNDCONTROLPOINT_H_
#define GROUNDCONTROLPOINT_H_

#include <vector>

#include "suri/Coordinates.h"

namespace suri {

/**
 * Clase que reprensenta un punto de control.
 */
class GroundControlPoint {
public:
   /**
    * Constructor copia
    */
   GroundControlPoint(const GroundControlPoint& other);
   /**
    * Construye un punto de control a partir de las componentes
    * de las coordenadas de origen y destino.
    */
   GroundControlPoint(const double &Xsrc = 0.0, const double &Ysrc = 0.0,
                      const double &Zsrc = 0.0, const double &Xdest = 0.0,
                      const double &Ydest = 0.0, const double &Zdest = 0.0);

   /**
    * Construye un punto de control con las coordenadas de origen y destino.
    */
   GroundControlPoint(const Coordinates& Source, const Coordinates& Destination);

   /**
    * Destructor.
    */
   virtual ~GroundControlPoint();

   /**
    * Obtiene las coordenadas de origen.
    */
   Coordinates GetSource() const;

   /**
    * Obtiene las coordenadas de destino.
    */
   Coordinates GetDestination() const;

   /**
    * Obtiene todas las coordenadas de origen de un vector de puntos de control.
    */
   static std::vector<Coordinates> ExtractSourceCoordinates(
         const std::vector<GroundControlPoint>& vGcp);

private:
   Coordinates source_;  /** Coordenadas de origen */
   Coordinates destination_;  /** Coordenadas de destino */
};


/**
 * Clase que reprensenta un punto de control con informacion extra.
 */
class GroundControlPointExtended : public GroundControlPoint {
public:
   /**
    * Constructor copia
    */
   GroundControlPointExtended(const GroundControlPointExtended& other);
   /**
    * Construye un punto de control a partir de las componentes
    * de las coordenadas de origen y destino e indicando si el
    * punto de control esta habilitado o no.
    */
   GroundControlPointExtended(const double &Xsrc = 0.0, const double &Ysrc = 0.0,
                              const double &Zsrc = 0.0, const double &Xdest = 0.0,
                              const double &Ydest = 0.0, const double &Zdest = 0.0,
                              bool EnabledP = true);

   /**
    * Construye un punto de control con las coordenadas de origen y destino e indicando si el
    * punto de control esta habilitado o no.
    */
   GroundControlPointExtended(const Coordinates& SourceP, const Coordinates& DestinationP,
                              bool EnabledP = true);

   /**
    * Destructor.
    */
   virtual ~GroundControlPointExtended();

   /**
    * Indica si el punto de control esta habilitado para su uso o no.
    */
   bool IsEnabled() const;

private:
   bool enabled_;
};

}

#endif  // GROUNDCONTROLPOINT_H_
