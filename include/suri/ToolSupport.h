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

#ifndef TOOLSUPPORT_H_
#define TOOLSUPPORT_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "suri/LayerList.h"
#include "suri/RasterElement.h"
#include "suri/VectorElement.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "VectorLayer.h"
#include "VectorDatasource.h"
#include "RasterDatasource.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase auxiliar para acceder a la imagen y subset del ambiente
 */
class ToolSupport {
public:
   /** Obtiene el raster seleccionado del contexto activo */
   static RasterElement* GetRaster(ViewcontextManagerInterface* pViewContextManager);
   /** Obtiene el raster seleccionado del contexto indicado */
   static RasterElement* GetRaster(ViewcontextManagerInterface* pViewContextManager,
                                                   SuriObject::UuidType ViewContextId);
   /** Obtiene el raster seleccionado del contexto activo */
   static RasterDatasource* GetRasterDatasource(DatasourceManagerInterface* pDatasourceManager,
                                                ViewcontextManagerInterface* pViewContextManager);
   /** Obtiene todos los elementos raster del contexto activo */
   static std::vector<RasterElement*> GetAllRaster(
         ViewcontextManagerInterface* pViewContextManager);
   /** Obtiene todos los elementos raster del contexto indicado */
   static std::vector<RasterElement*> GetAllRaster(
         ViewcontextManagerInterface* pViewContextManager,
         SuriObject::UuidType ViewContextId);

   /** Obtiene el raster activo del contexto indicado */
   static RasterElement* GetActiveRaster(ViewcontextManagerInterface* pViewContextManager,
                                                   SuriObject::UuidType ViewContextId);
   /**
    * Obtiene el VectorElement activo del contexto seleccionado
    */
   static VectorElement* GetVector(ViewcontextManagerInterface* pViewContextManager);
   /**
    * Obtiene el VectorLayer activo del contexto seleccionado
    */
   static VectorLayer* GetVectorLayer(ViewcontextManagerInterface* pViewContextManager);
   /**
    * Obtiene el VectorLayer activo del contexto
    */
   static VectorDatasource* GetVectorDatasource(
                     DatasourceManagerInterface* pDatasourceManager,
                     ViewcontextManagerInterface* pViewContextManager);
   /**
    * Obtiene el subset a partir del ViewportManager y el raster
    */
   static void GetSubset(ViewportManagerInterface* pViewportManager,
                         Subset& ViewerWindow, RasterElement* pRaster);
   /**
    * Auxiliar para obtener la lista de capas
    */
   static LayerList* GetLayerList(ViewcontextManagerInterface* pViewContextManager);
   /**
    * Auxiliar para obtener el mundo
    */
   static World* GetWorld(ViewportManagerInterface* pViewportManager);
   /**
    * Recorre la lista para ver si hay un elemento vectorial de tipo poligono
    */
   static bool ContainsPolygons(ViewcontextManagerInterface* pViewContextManager);
};

} /** namespace suri */

#endif /* TOOLSUPPORT_H_ */
