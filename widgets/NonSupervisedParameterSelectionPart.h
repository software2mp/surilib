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

// Includes standard
// Includes Suri
#include "ClassificationPart.h"
#include "suri/StatisticsFunctions.h"
#include "ClassificationAlgorithmInterface.h"
// Includes Wx
// Defines

#ifndef NONSUPERVISEDPARAMETERSELECTIONPART_H_
#define NONSUPERVISEDPARAMETERSELECTIONPART_H_

#include "ParametersSelectionPart.h"

namespace suri {
/** Factory para almacenar algoritmos de clasificacion no supervisada */
/**
 * Factory para almacenar algoritmos de clasificacion no supervisada.
 * Todos los algoritmos de clasif no supervisada deberan heredar
 * y se registraran en esta
 *
 */
class NonSupervisedParameterSelectionPart : public suri::ParametersSelectionPart {
public:
   /** Macro que declara metodos de la clase */
HAS_FACTORY(NonSupervisedParameterSelectionPart, std::string);
public:

   /** Ctor */
   NonSupervisedParameterSelectionPart();
   /** Dtor */
   virtual ~NonSupervisedParameterSelectionPart();

   /** Elimina todas las clases cargadas. */
   virtual bool Reset()=0;
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName()=0;
   /** Agrega clases que se usan para clasificar */
   virtual bool AddClass(int ClassId, Statistics Stats)=0;
   /** Retorna un puntero al clasificador creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager()=0;
   /** Retorna la cantidad de clases */
   virtual int GetClassCount()=0;
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones van de 0 a */
// GetClassCount()-1.
   virtual bool GetClassProperties(ClassInformation &Properties)=0;

#ifdef NOT_SUPERVISED_CODE
   /** Informa si el part necesita que se configuren las clases. */
   virtual bool NeedClasses() {
      return true;
   }
#endif

protected:

private:
};
}

#endif /* NONSUPERVISEDPARAMETERSELECTIONPART_H_ */
