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

#ifndef PARAMETERSSELECTIONPART_H_
#define PARAMETERSSELECTIONPART_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
// Includes Wx
// Includes App
// Defines

/** Definido si ParametricClassificationPart tambien se usa para clasificacion no supervisada */
#define NOT_SUPERVISED_CODE

// forwards
#include "ClassificationPart.h"

/** namespace suri */
namespace suri {
/** Interfaz que deben implementar los parts que representen clasificaciones parametricas */
/**
 * Si se quiere agregar una clasificacion parametrica que requiera las
 * estadisticas de clases que se forman con poligonos en la imagen se debe
 * implementar esta interfaz y la de ClassificationAlgorithm.
 * ParametricClassificationPart primero llama a AddClass con las estadisticas
 * correspondientes a cada clase seleccionada por el usuario.
 * Luego obtiene el ClassificationAlgorithm configurado con datos de
 * de clasificacion.
 * Finalmente configura la tabla de colores del raster generado con
 * lo que devuelva GetClassProperties
 */
class ParametersSelectionPart : public Part {
   /** Macro que declara metodos de la clase */
HAS_FACTORY(ParametersSelectionPart, std::string);
public:
   /** Ctor */
   ParametersSelectionPart();
   /** Dtor */
   virtual ~ParametersSelectionPart();
   
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName()=0;
   /** Retorna un puntero al algoritmo de clasificacion creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager()=0;
};
}  // namespace suri

#endif /* PARAMETERSSELECTIONPART_H_ */
