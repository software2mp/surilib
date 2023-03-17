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

// Includes Estandar
#include <string>

// Includes Suri
#include "EnclosureValidator.h"
#include "VectorDatasource.h"
#include "suri/VectorElement.h"
#include "suri/VectorEditor.h"
#include "EnclosureManipulator.h"

// Includes Wx

// Defines
#define ENCLOSURE_VALIDATE_FIELDS_

// forwards

namespace suri {

/** Nombre del campo "tipo" */
const std::string EnclosureValidator::TypeField = "_tipo_Area";

/** Nombre del campo "descripcion" */
const std::string EnclosureValidator::DescField = "_desc_Area";

/**
 * Constructor
 */
EnclosureValidator::EnclosureValidator() {
}

/**
 * Destructor
 */
EnclosureValidator::~EnclosureValidator() {
}

/**
 * Verifica que una fuente de datos tenga los nodos que corresponden a un recito
 * Se define un XML valido como un xml del estilo:
 * \verbatim
 * <recinto>
 *   <query>SELECT * FROM layername WHERE_tipo_Area ne 0</query>
 *   <tipoarea tipo="int">_tipo_Area</tipoarea>
 *   <descripcion tipo="uchar">_tipo_Desc</descripcion>
 *  </recinto>
 *  \endverbatim
 *  No se valida el nodo query ya que es optativo.
 *  Se valido los tipos de datos de los nodos tipoarea y descripcion
 * @return true en caso de que la fuente de datos tengo los nodos xml que corresponde
 * @return false en caso de que el xml no sea valido
 */
bool EnclosureValidator::IsXmlValid(Element* pElement) const {
#ifdef ENCLOSURE_VALIDATE_FIELDS_
   wxXmlNode* pnode = pElement->GetNode(wxT(ENCLOSURE_NODE));
   if (pnode == NULL)
      return false;

   EnclosureManipulator manipulator;
   EnclosureInformation enclosureinformation =
         manipulator.GetEnclosureInformation(pElement);
   std::string areatypefield = enclosureinformation.GetTypeAreaFieldType();
   std::string areadescfield = enclosureinformation.GetDescFieldType();
   std::string query = enclosureinformation.GetQuery();

   bool hasvalidtypenode = (areatypefield.compare(EnclosureInformation::TypeRecord::Name) == 0);
   bool hasvaliddescnode = (areadescfield.compare(EnclosureInformation::DescRecord::Name) == 0);
   bool hasquerynode = !query.empty();

   return (hasvaliddescnode && hasvalidtypenode && hasquerynode);
#else
   return true;
#endif
}

/**
 * Valida que la fuente de datos sea valida como reciento. Es decir, sea vectorial
 * y de geometria poligono
 * @param[in] pDatasource fuente de datos a validar
 * @return true en caso de que la fuente de datos sea de un tipo valido como enclosure
 * @return false en caso contrario
 */
bool EnclosureValidator::IsValidDatasourceType(DatasourceInterface* pDatasource) const {
   if (!pDatasource)
      return false;
   return IsValidDatasourceType(pDatasource->GetElement());
}

/**
 * Valida que la fuente de datos sea valida como reciento. Es decir, sea vectorial
 * y de geometria poligono
 * @param[in] pElement fuente de datos a validar
 * @return true en caso de que la fuente de datos sea de un tipo valido como enclosure
 * @return false en caso contrario
 */
bool EnclosureValidator::IsValidDatasourceType(Element* pElement) const {
   VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pElement);
   if (!pvectorelement)
      return false;
   // Verifica si la geometrica del vector es la de un poligono
   wxString type = pvectorelement->GetTypeAsString();
   if (type != TYPE_DECORATOR(message_POLYGON)) {
        return false;
   }
   return true;
}
/**
 * Verifica que una fuente de datos tenga los campos que corresponden a un recito
 * @return true en caso de que la fuente de datos tengo los campos que corresponde
 * @return false en caso de que el xml no sea valido
 */
bool EnclosureValidator::IsFieldsValid(Element* pElement) const {
#ifdef ENCLOSURE_VALIDATE_FIELDS_
   VectorElement* pvectorelement = dynamic_cast<VectorElement*>(pElement);

   if (!IsValidDatasourceType(pElement))
      return false;
   EnclosureManipulator manipulator;
   EnclosureInformation enclosureinformation =
         manipulator.GetEnclosureInformation(pElement);

   // Obtengo los campos (tipo y descripcion)
   VectorEditor vectoreditor;
   vectoreditor.OpenVector(pvectorelement->GetUrl().c_str());
   wxFileName fname(pvectorelement->GetUrl());
   std::string name = WS2S(fname.GetName());
   if (!vectoreditor.OpenLayer(pvectorelement->GetActiveLayer()))
      return false;
   int typeaefieldposition =
         vectoreditor.GetFieldPosition(enclosureinformation.GetTypeAreaName());
   int descaefieldposition =
         vectoreditor.GetFieldPosition(enclosureinformation.GetDescAreaName());


   // Si no existe alguno de los campos, se sale
   if (typeaefieldposition == -1 || descaefieldposition == -1)
      return false;

   std::string typeaefieldtype = vectoreditor.GetFieldType(typeaefieldposition);
   std::string descaefieldtype = vectoreditor.GetFieldType(descaefieldposition);
   if (typeaefieldtype.compare(TypeRecord::Name) != 0
         || descaefieldtype.compare(DescRecord::Name) != 0)
      return false;
#endif
   return true;
}

/**
 * Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido
 * Se considera una fuente de datos valida como reciento si:
 *    Es una fuente de datos de tipo vector
 *    La geometria de la misma es la de un poligono
 *    Tiene los campos de tipo y descripcion del recinto
 *    Los tipos de datos de los campos son correcto
 * @return true si la fuente de datos es valida como recinto
 * @return false si la fuente de datos no es valida.
 */
bool EnclosureValidator::IsValid(DatasourceInterface* pDatasource) const {
   VectorDatasource* pvectordatasource = dynamic_cast<VectorDatasource*>(pDatasource);
   return pvectordatasource && IsValid(pDatasource->GetElement());
}

/**
 * Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido
 * Se considera una fuente de datos valida como reciento si:
 *    Es una fuente de datos de tipo vector
 *    La geometria de la misma es la de un poligono
 *    Tiene los campos de tipo y descripcion del recinto
 *    Los tipos de datos de los campos son correcto
 * @return true si la fuente de datos es valida como recinto
 * @return false si la fuente de datos no es valida.
 */
bool EnclosureValidator::IsValid(Element* pElement) const {
   return IsXmlValid(pElement) && IsFieldsValid(pElement);
}

} /** namespace suri */
