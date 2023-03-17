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
#include <vector>
// Includes Suri
#include "LayerInterface.h"
#include "VectorElement.h"
#include "VectorEditor.h"
#include "VectorDatasource.h"
#include "LayerHtmlListItem.h"
// Includes Wx
// Defines
// forwards


#ifndef __USE_FEATURE_SELECTION__
#define __USE_FEATURE_SELECTION__
namespace suri {

class UseFeatureSelection{
protected:
  /**
   * Devuelve un  HotLinkVectorLayer* con un elemento generado en memoria a partir de
   * la seleccion de pElement.
   */
   LayerInterface* CreateSelectionList(Element* pElement);
   /**
    * Crea un vector en memoria del tipo indicado.
    */
   VectorEditor* CreateMemoryVector(
                          const std::string& VectorName, std::map<std::string, std::string>& Geoms,
                           const std::string& Srs);
   /**
    *Crea un LayerHtmlListItem dado un elmento con una seleccion , devuelve NULL  si el elemento
    *es NULL, o si el elemento no tiene una seleccion.
    */
   ui::LayerHtmlListItem* GetItemSelection(Element* pElement);

   /** Devuelve el Datasource indicado por el id, NULL en caso de no hallarlo */
   VectorDatasource* GetVectorDatasourceFromLayer(SuriObject::UuidType LayerId);

   /** Devuelve el vector datasource dado su id */
   VectorDatasource* GetVectorDatasource(SuriObject::UuidType DatasourceId);

   /** fuente de datos de la seleccion en memoria **/
   std::vector<VectorDatasource*> pSelection_;

   /** Dado un elemento devuelve el datasource de su seleccion */
   VectorDatasource* GetSelectedVectorDatasource(Element* pElement);

   /** mapea los ids de las layers con los datasources */
   std::map<SuriObject::UuidType, VectorDatasource*> selectionMapFromLayer_;
};

}
#endif
