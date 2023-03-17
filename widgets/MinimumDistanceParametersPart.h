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

#ifndef MINIMUMDISTANCEPARAMETERSPART_H_
#define MINIMUMDISTANCEPARAMETERSPART_H_

#include "ClusterClassificationPart.h"

namespace suri {
/** Permite setear los parametros del algoritmo de clasificacion distancia minima. */
/**
 * Permite setear los parametros del algoritmo de clasificacion distancia minima.
 * Consta de un area para escribir el umbral dentro del cual se considerara
 * que un punto pertenece a la clase.
 */
class MinimumDistanceParametersPart : public ClusterClassificationPart {
public:
   /** Ctor */
   MinimumDistanceParametersPart();
   /** Dtor */
   virtual ~MinimumDistanceParametersPart();

// --- Metodos de Part ---
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();


   // -- Metodos de ParametersSelectionPart ---
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();

   // -- Metodos internos ---
   /** Cambia el color del control */
   virtual void SetColourCtrlValue(VectorStyle::Color Color);
   /** Retorna el color del control */
   virtual VectorStyle::Color GetColourCtrlValue();
   /** Cambia el valor del threshold */
   virtual void SetThresholdCtrlValue(double Threshold);
   /** Retorna el valor del threshold */
   virtual double GetThresholdCtrlValue();

   static const double DEFAULT_THRESHOLD;  /*! threshold inicial */
   static const VectorStyle::Color DEFAULT_COLOR;  /*! color inicial */
};
}

#endif /* MINIMUMDISTANCEPARAMETERSPART_H_ */
