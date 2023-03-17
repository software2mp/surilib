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

#ifndef ENCLOSUREVALIDATOR_H_
#define ENCLOSUREVALIDATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceValidatorInterface.h"
#include "suri/DataTypes.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * El EnclosureValidator se encarga de validar los casos en que un VectorDatasource
 * se pueda utilizar como un recinto (Enclosure)
 */
class EnclosureValidator : public suri::DatasourceValidatorInterface {
public:
   /** Constructor */
   EnclosureValidator();
   /** Destructor */
   virtual ~EnclosureValidator();
   /** Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido */
   virtual bool IsValid(DatasourceInterface* pDatasource) const;
   bool IsValid(Element* pElement) const;
   /*Valida que la fuente de datos sea valida como reciento. Es decir, sea vectorial
    * y de geometria poligono */
   virtual bool IsValidDatasourceType(DatasourceInterface* pDatasource) const;
    /*Valida que la fuente de datos sea valida como reciento. Es decir, sea vectorial
    * y de geometria poligono */
   bool IsValidDatasourceType(Element* pElement) const;
  /** Nombre del campo "tipo" */
   static const std::string TypeField;
   /** Nombre del campo "descripcion" */
   static const std::string DescField;
   /** Tipo del campo "tipo" */
   typedef DataInfo<int> TypeRecord;
   /** Tipo del campo "descripcion" */
   typedef DataInfo<unsigned char> DescRecord;

private:
   /** Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido */
   bool IsXmlValid(Element* pElement) const;
   /** Verifica que una fuente de datos tenga los campos que corresponden a un recito */
   bool IsFieldsValid(Element* pElement) const;
};

} /** namespace suri */

#endif /* ENCLOSUREVALIDATOR_H_ */
