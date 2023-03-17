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

#ifndef MAXLIKELIHOODPART_H_
#define MAXLIKELIHOODPART_H_

// Includes standard
// Includes Suri
#include "MaxLikelihoodAlgorithm.h"
#include "ParametersSelectionPart.h"
#include "suri/VectorStyle.h"
#include "ClusterClassificationPart.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(MaxLikelihoodPartWidgetEvent);

/** Genera nodo de renderizador que clasifica usando algoritmo maxima verosimilitud */
/**
 *	Esta clase al retornar la cantidad de clases asi como las porpiedades
 *	considera que la primer clase es siempre 'Sin Clase'.
 */
class MaxLikelihoodPart : public ClusterClassificationPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(MaxLikelihoodPartWidgetEvent);
   /** Ctor */
   MaxLikelihoodPart();
   /** Dtor */
   virtual ~MaxLikelihoodPart();

// --- Metodos de Part ---
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();

// -- Metodos de ParametersSelectionPart ---
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();

protected:
   /** Cambia el color del control */
   virtual void SetColourCtrlValue(VectorStyle::Color Color);
   /** Retorna el color del control */
   virtual VectorStyle::Color GetColourCtrlValue();
   /** Cambia el valor del slider */
   virtual void SetThresholdCtrlValue(double Threshold);
   /** Retorna el valor del slider */
   virtual double GetThresholdCtrlValue();
   /** modifica el texto que informa la posicion */
   virtual void OnScroll(wxScrollEvent& Event);

   /** Inicializo la clase */
   virtual MaxLikelihoodPart* InitializeClass() {
      return this;
   }
   EVENT_OBJECT_PROPERTY(MaxLikelihoodPartWidgetEvent); /*! Conecta los eventos */

   static const double DEFAULT_THRESHOLD;  /*! threshold inicial */
   static const VectorStyle::Color DEFAULT_COLOR;  /*! color inicial */
};
}  // namespace suri

#endif /* MAXLIKELIHOODPART_H_ */
