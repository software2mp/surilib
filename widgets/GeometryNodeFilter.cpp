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
#include "suri/messages.h"
#include "GeometryNodeFilter.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/VectorElement.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in] pDatasourceManager administrador de fuente de datos
 * @param[in] FilterType tipo de filtro que se desea aplicar sobre las geometrias
 */
GeometryNodeFilter::GeometryNodeFilter(DatasourceManagerInterface* pDatasourceManager,
                                       int FilterType) :
                                       pDatasourceManager_(pDatasourceManager),
                                       filterType_(FilterType) {
}

/** Destructor **/
GeometryNodeFilter::~GeometryNodeFilter() {
}

/**
 * Clona instancia de filtro.
 * @return copia de filtro.
 */
NodeFilterInterface* GeometryNodeFilter::Clone() const {
   return new GeometryNodeFilter(pDatasourceManager_, filterType_);
}

/**
 * Retorna true si el nodo esta filtrado.
 * @param[in] pNode nodo a analizar
 * @return bool que indica si hay que filtrarlo
 */
bool GeometryNodeFilter::IsFiltred(TreeNodeInterface* pNode) const {
   if (pNode->AllowsChildren()) {
      return IsGroupFiltered(pNode);
   }
   return IsNodeFiltered(pNode);
}

/** Verifica el contenido de un grupo para saber si el mismo se filtra **/
bool GeometryNodeFilter::IsGroupFiltered(TreeNodeInterface* pNode) const {
   return IsNodeFiltered(pNode);
}

/** Verifica el contenido de un nodo hoja para ver si se debe filtrar el mismo **/
bool GeometryNodeFilter::IsNodeFiltered(TreeNodeInterface* pNode) const {
   DatasourceInterface* pdatasource = pDatasourceManager_->GetDatasource(pNode->GetContent());
   if (!pdatasource)
      return true;
   VectorElement* pelement = dynamic_cast<VectorElement*>(pdatasource->GetElement());
   if (!pelement)
      return true;

   bool filtered = true;
   std::string type = pelement->GetTypeAsString().c_str();
   if (type.compare(TYPE_DECORATOR(message_POLYGON).c_str()) == 0) {
      filtered = !(filterType_ & POLYGON_FILTER);
   } else if (type.compare(TYPE_DECORATOR(message_POINT).c_str()) == 0) {
      filtered = !(filterType_ & POINT_FILTER);
   } else if (type.compare(TYPE_DECORATOR(message_LINE).c_str()) == 0) {
      filtered = !(filterType_ & LINE_FILTER);
   }
   return filtered;
}
} /** namespace suri */
