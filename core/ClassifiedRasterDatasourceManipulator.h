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

#ifndef CLASSIFIEDRASTERDATASOURCEMANIPULATOR_H_
#define CLASSIFIEDRASTERDATASOURCEMANIPULATOR_H_

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "ClassifiedRasterDatasourceValidator.h"
#include "ClassInformation.h"
#include "suri/DatasourceManipulatorInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/VectorStyle.h"

// Includes Wx
#include "wx/xml/xml.h"

// Defines

// forwards

namespace suri {

/**
 * Manipulador de RasterDatasources validados como rasters clasificados
 */
class ClassifiedRasterDatasourceManipulator : public suri::DatasourceManipulatorInterface {
public:
   /** Constructor */
   ClassifiedRasterDatasourceManipulator();
   /** Destructor */
   virtual ~ClassifiedRasterDatasourceManipulator();
   /** Crea una nueva fuente de datos que cumple con los requisitos de validacion */
   virtual DatasourceInterface* Create(const std::string& FileName, const Option& Option) const;
   /** Efectua la logica de agregado para un Datasource validado */
   virtual bool AddNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de limpieza para un Datasource validado */
   virtual bool CleanNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de copia de un Datasource a otro */
   virtual bool CopyNature(DatasourceInterface* pSourceDatasource,
                 DatasourceInterface* pDestinationDatasource) const;
   /** Agrega una clase a la fuente de datos clasificada */
   bool AddClassInformation(ClassInformation* pClassInformation, DatasourceInterface* pDatasource,
                            bool Replace = false) const;
   /** Devuelve un id para la creacion de un nuevo datasource */
   ClassInformation::IndexType GetClassInformationNewId(DatasourceInterface* pDatasource) const;
   /** Obtiene una clase de la fuente de datos clasificada a partir de un indice */
   ClassInformation GetClassInformation(DatasourceInterface* pDatasource,
                                        ClassInformation::IndexType Index) const;
   /** Obtiene la cantidad de clases dentro de una fuente de datos clasificada */
   int GetClassInformationCount(DatasourceInterface* pDatasource) const;
   /** Devuelve una lista con las clases dentro de la fuente de datos */
   std::vector<ClassInformation> GetClassInformationList(DatasourceInterface* pDatasource) const;
   /** Definicion de nombres para la opcion SizeX */
   static const std::string SizeXOptionName;
   /** Definicion de nombres para la opcion SizeY */
   static const std::string SizeYOptionName;
   /** Definicion de nombres para la opcion Driver */
   static const std::string DriverOptionName;

private:
   /** Valor para cantidad de clases invalida */
   static const int InvalidCount;
   /** Validador de Raster Clasificado */
   ClassifiedRasterDatasourceValidator validator_;
   /** Metodo auxiliar para la creacion de los nodos xml correspondientes \deprecated */
   void CreateXmlNodes(DatasourceInterface* pDatasource) const;
   /** Metodo auxiliar para la creacion de una clase a partir del xml */
   ClassInformation CreateClassInformationFromXml(wxXmlNode* pNode,
                                                  ClassInformation::IndexType Index = -1,
                                                  bool ClassFound = true) const;
   /** Agrega los nodos xml de una clase a la fuente de datos clasificada */
   void CreateXmlClassNode(ClassInformation* pClassInformation,
                           DatasourceInterface* pDatasource) const;
   /** Modifica los nodos xml de una clase a la fuente de datos clasificada */
   bool ModifyXmlClassNode(ClassInformation* pClassInformation,
                           DatasourceInterface* pDatasource) const;
   /** Devuelve el nodo xml correspondiente a la clase que se indica por parametro */
   wxXmlNode* GetClassInformationXmlNode(DatasourceInterface* pDatasource,
                                         ClassInformation::IndexType Index) const;
};

} /** namespace suri */

#endif /* CLASSIFIEDRASTERDATASOURCEMANIPULATOR_H_ */
