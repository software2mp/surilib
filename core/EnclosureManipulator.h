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

#ifndef ENCLOSUREMANIPULATOR_H_
#define ENCLOSUREMANIPULATOR_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DatasourceManipulatorInterface.h"
#include "EnclosureInformation.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Manipulador de VectorDatasources validados como Recintos (Enclosure)
 */
class EnclosureManipulator : public suri::DatasourceManipulatorInterface {
public:
   /** Constructor */
   explicit EnclosureManipulator(bool TrainArea = true);
   /** Destructor */
   virtual ~EnclosureManipulator();
   /** Crea una nueva fuente de datos que cumple con los requisitos de validacion de Enclosure */
   virtual DatasourceInterface* Create(const std::string& FileName,
                                       const Option& Option) const;
   /** Efectua la logica de agregado para un Datasource validado */
   virtual bool AddNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de limpieza para un Datasource validado */
   virtual bool CleanNature(DatasourceInterface* pDatasource) const;
   /** Efectua la logica de copia de un Datasource a otro */
   virtual bool CopyNature(DatasourceInterface* pSourceDatasource,
                    DatasourceInterface* pDestinationDatasource) const;
   /** Devuelve la informacion de recinto de la fuente de datos*/
   EnclosureInformation GetEnclosureInformation(
                        DatasourceInterface* pDatasource) const;
   /** Devuelve la informacion de recinto de la fuente de datos*/
   EnclosureInformation GetEnclosureInformation(
                        Element* pElement) const;

	/** Metodo que agrega la configuracion como area de clasificacion a un elemento **/                        
   bool AddNature(Element* pElement) const;
   // Definiciones para nombres de optiones
   static const std::string SpatialReferenceOptionName;
   static const std::string DriverOptionName;

private:
   /** Metodo auxiliar para crear los campos para un recito */
   bool CreateEnclosureFields(EnclosureInformation &EnclosureInformation,
                                    DatasourceInterface* pDatasource) const;
   /** Metodo auxiliar para crear los campos para un recito */
   bool CreateEnclosureFields(EnclosureInformation &EnclosureInformation,
                              Element* pElement) const;
   /** Metodo auxiliar para crear los nodos xml para un recito \deprecated */
   bool CreateEnclosureXmlNodes(EnclosureInformation &EnclosureInformation,
                                Element* pElement) const;
   /** Metodo auxiliar para la creacion del contenido del nodo consulta del xml del recinto */
   std::string CreateQuery(const std::string& LayerName, const std::string& FieldName,
                           const std::string& Condition = " IN(0)") const;
   /** Obtiene el nombre de la capa activa de la fuente de datos */
   std::string GetActiveLayerName(const std::string DatasourceUrl,
                                  int LayerIndex) const;
   bool trainArea_;
};

} /** namespace suri */

#endif /* ENCLOSUREMANIPULATOR_H_ */
