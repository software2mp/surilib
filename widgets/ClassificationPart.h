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

#ifndef CLASSIFICATIONPART_H_
#define CLASSIFICATIONPART_H_

// Includes standard
#include <vector>
// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"
#include "suri/VectorStyle.h"
#include "ClassInformation.h"
#include "ClassificationGetter.h"

// Includea wx
#include "wx/wx.h"
// Defined
// forward

/** namespace suri */
namespace suri {
/** Interfaz para los Part que obtienen parametros para una clasificacion */
/**
 * Interfaz que usa el ClassificationExporterWidget para generar el renderizaror
 * encargado de clasificar.
 * Tambien lo utiliza para obtener las clases y colores generados.
 */
class ClassificationPart : public Part, public ProcessAtributeProvider, public ClassificationGetter {
public:
    /** ctor */
   ClassificationPart() {}
   /** dtor */
   virtual ~ClassificationPart() {}
   /** Retorna la cantidad de clases */
   virtual int GetClassCount()=0;
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones
    * van de 0 a GetClassCount()-1.**/
   virtual bool GetClassProperties(std::vector<ClassInformation> &Properties)=0;
   /** Retorna el nodo que se debe agregar al elemento que se renderiza */
   virtual wxXmlNode* GetClassificationRendererNode(RasterElement* pRasterElement)=0;
   /** Metodo que indica la seleccion de bandas aciva para la clasificacion **/
   virtual void SetSelectedBands(const std::vector<int>& SelectedBands) {
      // se implementa vacio para no forzar esto mismo en las clases hijas que no requieran
      // hacer uso de este metodo
   }
};
}

#endif /* CLASSIFICATIONPART_H_ */
