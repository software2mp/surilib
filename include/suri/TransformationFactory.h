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

#ifndef TRANSFORMATIONFACTORY_H_
#define TRANSFORMATIONFACTORY_H_

#include <string>

#include "suri/ParameterCollection.h"
#include "suri/CoordinatesTransformation.h"

namespace suri {

/**
 * Es la clase base donde se declara el metodo "Create" para la creacion de
 * objetos del tipo CoordinatesTransformation. Al mismo tiempo mantiene una
 * referencia a un sucesor para la cadena de responsabilidad.
 */
class TransformationFactory {
public:
   /**
    * Tipos de transformaciones disponibles.
    */
   static const std::string kExact;
   static const std::string kPolynomial;
   static const std::string kTriangulation;

   /**
    * Nombres de parametros.
    */
   static const std::string kParamWktIn;
   static const std::string kParamWktOut;
   static const std::string kParamGcps;
   static const std::string kParamOrder;

   /**
    * Destructor (elimina al sucesor)
    */
   virtual ~TransformationFactory();

   /**
    * Metodo a implementar por las clases de tranformaciones concretas para
    * crear una instancia concreta de transformacion.
    * @param[in] Type Tipo de transformacion que se desea crear.
    * @param[in] Params Parametros necesarios para la creacion de cada transformacion.
    * @return Devuelve una instancia a una transformacion de coordenadas.
    */
   virtual CoordinatesTransformation* Create(const std::string& Type,
                                             const suri::ParameterCollection& ParamsP) = 0;

protected:
   /**
    * Constructor protegido para evitar que se pueda crear un TransformFactory
    * pero que al mismo tiempo pueda ser utilizado por las clases que heredan para
    * configurar al sucesor.
    */
   explicit TransformationFactory(TransformationFactory* pSuccessor);

   TransformationFactory* pSuccessor_; /** Sucesor en la cadena de responsabilidad de creacion. */

   /**
    * Devuelve al sucesor.
    */
   inline TransformationFactory* GetSuccessor() const {
      return pSuccessor_;
   }

   /**
    * Configura al sucesor de esta factoria.
    */
   inline void SetSuccessor(TransformationFactory* pSuccessor) {
      pSuccessor_ = pSuccessor;
   }
};

}  // namespace suri

#endif  // TRANSFORMATIONFACTORY_H_
