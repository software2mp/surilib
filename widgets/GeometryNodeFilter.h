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

#ifndef GEOMETRYNODEFILTER_H_
#define GEOMETRYNODEFILTER_H_

// Includes Estandar
// Includes Suri
#include "suri/NodeFilterInterface.h"

// Includes Wx
// Defines

namespace suri {

// Forwards
class VectorElement;
class DatasourceManagerInterface;

/*!
 * Clase que representa un filtro para
 * el recorrido de los nodos de un arbol
 * que filtra por tipo de geometria
 */
class GeometryNodeFilter : public NodeFilterInterface {
public:
   /** posibles filtros, pueden sumarse aplicando OR */
   static const int POLYGON_FILTER = 0x01;
   static const int POINT_FILTER = 0x02;
   static const int LINE_FILTER = 0x04;
   /** constructor **/
   GeometryNodeFilter(DatasourceManagerInterface* pDatasourceManager,
                      int FilterType);
   /** destructor **/
   virtual ~GeometryNodeFilter();
   /**
    * Clona instancia de filtro.
    * @return copia de filtro.
    */
   virtual NodeFilterInterface* Clone() const;
   /**
    * Retorna true si el nodo esta filtrado.
    * @param[in] pNode nodo a analizar
    * @return bool que indica si hay que filtrarlo
    */
   virtual bool IsFiltred(TreeNodeInterface* pNode) const;

private:
   /** Verifica el contenido de un grupo para saber si el mismo se filtra **/
   bool IsGroupFiltered(TreeNodeInterface* pNode) const;
   /** Verifica el contenido de un nodo hoja para ver si se debe filtrar el mismo **/
   bool IsNodeFiltered(TreeNodeInterface* pNode) const;
   /** administrador de fuente de datos para la obtencion de las caracteristicas de la
    *  fuente de datos a filtrar */
   DatasourceManagerInterface* pDatasourceManager_;
   /** configuracion activa del filtro **/
   int filterType_;
};

} /** namespace suri */

#endif /* GEOMETRYNODEFILTER_H_ */
