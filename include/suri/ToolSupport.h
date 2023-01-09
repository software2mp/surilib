/*! \file ToolSupport.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
