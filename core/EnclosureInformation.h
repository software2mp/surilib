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

#ifndef ENCLOSUREINFORMATION_H_
#define ENCLOSUREINFORMATION_H_

// Includes Estandar
#include<string>

// Includes Suri
#include "suri/DataTypes.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase que encapsula la informacion de una fuente de datos como recinto
 */
class EnclosureInformation {
public:
   typedef enum { Train = 0, Test =1} EnclosureType;
   /** ctor **/
   EnclosureInformation(const std::string& LayerName, const std::string& Query,
                        EnclosureType Etype,
                        const std::string& TypeName = TypeNameDefault,
                        const std::string& DescName = DescNameDefault);
   /** dtor **/
   virtual ~EnclosureInformation();
   /** Nombre del campo tipo de area por defecto*/
   static const std::string TypeNameDefault;
   /** Nombre del campo descripcion por defecto*/
   static const std::string DescNameDefault;
   /** Obtiene el nombre de la capa*/
   std::string GetLayerName() const;
   /** Crea una query basada en los campos asociados al recinto y la condicion que recibe */
   std::string SetQuery(const std::string& Condition = " IN(0)");
   /** Obtiene la query asociada al recinto*/
   std::string GetQuery() const;
   /** Obtiene el nombre del campo tipo de area*/
   std::string GetTypeAreaName() const;
   /** Obtiene el tipo de dato del campo tipo de area*/
   std::string GetTypeAreaFieldType() const;
   /**Devuelve el nombre del campo descripcion del area */
   std::string GetDescAreaName() const;
   /**  Obtiene el tipo de dato asociado al campo descripcion tipo de area */
   std::string GetDescFieldType() const;
   /** Devuelve el tipo de recinto (entrenamiento o test). **/
   EnclosureType GetEnclosureType() const;
   /** Tipo del campo "tipo" */
   typedef DataInfo<int> TypeRecord;
   /** Tipo del campo "descripcion" */
   typedef DataInfo<unsigned char> DescRecord;

private:
   std::string layerName_;
   std::string query_;
   EnclosureType enclosureType_;
   class AreaField {
   public:
      std::string name_, type_;
      AreaField(const std::string& Name, const std::string& Type) : name_(Name), type_(Type) {
      }
   };
   AreaField typeArea_;
   AreaField descArea_;
};

} /** namespace suri */

#endif /* ENCLOSUREINFORMATION_H_ */
