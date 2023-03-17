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
#include <vector>

// Includes Suri
#include "ClassifiedRasterDatasourceValidator.h"
#include "RasterDatasource.h"
#include "ClassInformation.h"
#include "ClassifiedRasterDatasourceManipulator.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
ClassifiedRasterDatasourceValidator::ClassifiedRasterDatasourceValidator() {
}

/**
 * Destructor
 */
ClassifiedRasterDatasourceValidator::~ClassifiedRasterDatasourceValidator() {
}

/**
 * Valida el contenido xml de una fuente de datos
 * Se condisera como xml valido un xml con los siguientes nodos:
 * \verbatim
 * <elemento ..>
 *   ...
 *   <clasificacion>
 *       <clase indice="0">
 *           <nombre> nombre </nombre>
 *           <color> valor_hexa </color>
 *           <recinto> id</recinto>
 *       </clase> ....
 *   </clasificacion>
 *   ...
 * </elemento>
 * \endverbatim
 * Para la validacion solo se tiene el nodo nombre para buscar una clase "sin clase"
 * @param[in] pDatasource referencia a la fuente de datos que se le quiere validar el xml
 * @return true en caso de que contenga al menos una clase "sin clase"
 * @return false en otro caso
 */
bool ClassifiedRasterDatasourceValidator::IsXmlValid(DatasourceInterface* pDatasource) const {
   ClassifiedRasterDatasourceManipulator manipulator;
   std::vector<ClassInformation> classvector = manipulator.GetClassInformationList(pDatasource);
   std::vector<ClassInformation>::const_iterator it = classvector.begin();
   bool exitsnoclass = false;
   for (; it != classvector.end() && !exitsnoclass; ++it) {
      exitsnoclass = ((*it).GetIndex() == ClassInformation::NoClassIndex);
   }
   return exitsnoclass;
}

/**
 * Verifica si un tipo es entero
 * @param[in] Type tipo de dato a verificar
 * @return true si el tipo es entero
 * @return false si el tipo no es entero
 */
bool ClassifiedRasterDatasourceValidator::IsIntegerType(const std::string& Type) const {
   if (Type.compare(DataInfo<unsigned char>::Name) == 0 || Type.compare(DataInfo<short>::Name) == 0
         || Type.compare(DataInfo<unsigned short>::Name) == 0
         || Type.compare(DataInfo<int>::Name) == 0
         || Type.compare(DataInfo<unsigned int>::Name) == 0) {
      return true;
   }
   return false;
}

/**
 * Efectua la validacion del objeto en cuestion y devuelve verdadero si es valido
 * Se considera como una fuente de datos clasificada a una fuente de datos que tiene:
 *       la fuente de datos es de tipo raster
 *      como minimo dos clases
 *      tiene que existir la clase no clase
 *      1 sola banda
 *      el tipo de dato de la banda tiene que ser entero
 * @param[in] pDatasource fuente de datos a validar
 * @return true si la fuente de datos es valida como una clasificada
 * @return false si la fuente de datos no es valida como clasificada
 */
bool ClassifiedRasterDatasourceValidator::IsValid(DatasourceInterface* pDatasource) const {
   RasterDatasource* pvd = dynamic_cast<RasterDatasource*>(pDatasource);
   if (pvd == NULL)
      return false;
   if (pvd->GetBandCount() != 1)
      return false;
   if (!IsIntegerType(pvd->GetDataType()))
      return false;
   bool xmlvalid = IsXmlValid(pDatasource);
   return xmlvalid;
}

} /** namespace suri */
