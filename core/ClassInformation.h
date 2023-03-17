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

#ifndef CLASS_H_
#define CLASS_H_

#include <string>

#include "suri/SuriObject.h"
#include "suri/VectorStyle.h"

namespace suri {

/**
 * Informacion sobre una clase de un proceso de clasificacion
 */
class ClassInformation {
public:
   /**
    * Constructor.
    */
   ClassInformation(
         int Index = NoClassIndex,
         const std::string& Name = NoClassName,
         const VectorStyle::Color& Color = VectorStyle::Color(NoClassRedColor,
                                                              NoClassGreenColor,
                                                              NoClassBlueColor,
                                                              NoClassAlphaColor),
         const SuriObject::UuidType& Id = SuriObject::NullClassId);

   /**
    * Destructor.
    */
   virtual ~ClassInformation();

   /** Tipo de Dato del Indice */
   typedef int IndexType;

   /** Obtiene el indice de la clase */
   IndexType GetIndex() const;

   /** Obtiene el nombre de la clase */
   std::string GetName() const;

   /** Obtiene el Id del Datasource */
   SuriObject::UuidType GetDatasourceId();

   /** Obtiene el color de la clase */
   VectorStyle::Color GetColor();

   /** Setea el nombre de la clase */
   void SetName(const std::string& Name);

   /** Setea el color de la clase */
   void SetColor(const VectorStyle::Color& Color);

   /** Determina si dos conjuntos de informacion de clases son iguales */
   bool Equals(ClassInformation& ClassInfo);

   /** Tipo de clase "Sin Clase" */
   static const int InvalidClassIndex = 0;

   /** Tipo de clase "Valor no valido" */
   static const std::string NDVName;

   /** Indice de la clase "Valor no valido" */
   static const int NDVIndex;

   /** Tipo de clase "Sin Clase" */
   static const std::string NoClassName;

   /** Indice de la clase "Sin Clase" */
   static const int NoClassIndex;

   /** Color Rojo de la clase "Sin Clase" */
   static const unsigned char NoClassRedColor;

   /** Color Azul de la clase "Sin Clase" */
   static const unsigned char NoClassBlueColor;

   /** Color Verde de la clase "Sin Clase" */
   static const unsigned char NoClassGreenColor;

   /** Color Alfa de la clase "Sin Clase" */
   static const unsigned char NoClassAlphaColor;

   /** Nombre por defecto para las clases con indices no encontrados */
   static const std::string DefaultClassNotFoundName;

private:
   /** Setea el indice de la clase */
   void SetIndex(IndexType Index);

   /** Cambia el Id del recinto */
   void SetDatasourceId(const SuriObject::UuidType& Id);

   /** Nomre de la clase */
   std::string name_;

   /** Indice de la clase */
   IndexType index_;

   /** Id del recinto que la origino */
   SuriObject::UuidType id_;

   /** Color de la clase */
   VectorStyle::Color color_;
};

} /** namespace suri */

#endif /* CLASS_H_ */
