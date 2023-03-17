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

#ifndef CLASSIFICATIONCLASSSELECTION_H_
#define CLASSIFICATIONCLASSSELECTION_H_

// Includes standard

// Includes Suri
#include "suri/PartCollection.h"
#include "suri/LayerList.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/** Permite seleccionar las clases sobre las que se quiere calcular la estadistica */
/**
 * Permite seleccionar una clase.
 * Ademas, permite enmascarar los valores no validos de la imagen de
 * entrada.
 * Por ultimo permite seleccionar el color y nombre de la clase 'sin clase'
 */
class ClassificationClassSelectionPart : public PartCollection {
public:
   /** Ctor */
   ClassificationClassSelectionPart(LayerList* pLayerList,
                                    bool NonClassifiedClassParametersSelector = true);
   /** Dtor */
   virtual ~ClassificationClassSelectionPart();

   /** Modifica contenido de la ventana */
// Oculta selector de propiedades de la clase 'sin clase'
   bool ConfigureWidget();

private:
   bool nonClassifiedClassParametersSelector_; /*! Flag. Si es true se */
   /*! agrega part para seleccionar areas de entrenamiento */
};
}

#endif /* CLASSIFICATIONCLASSSELECTION_H_ */
