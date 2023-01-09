/*! \file ToolSupport.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <list>
#include <vector>

// Includes Suri
#include "suri/ToolSupport.h"
#include "suri/VectorElement.h"
#include "suri/ViewcontextInterface.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "RasterDatasource.h"
#include "RasterLayer.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Obtiene una LayerList a partir de un ViewcontextManager
 * @param[in]  ViewContextManager: Puntero al Administrador de Contextos
 * @return     Devuelve la LayerList si tuvo exito, sino NULL
 */
LayerList* ToolSupport::GetLayerList(ViewcontextManagerInterface* ViewContextManager) {
   LayerList *playerlist = ViewContextManager->GetSelectedViewcontext()->GetLayerList();
   if (playerlist == NULL) {
      return NULL;
   }
   return playerlist;
}

/**
 * Obtiene el Raster seleccionado del contexto activo
 * @param[in]  ViewContextManager: Puntero al Administrador de Contextos
 * @return     Devuelve el si tuvo exito (el contexto activo tiene un raster seleccionado),
 *             sino devuelve NULL
 */
RasterElement* ToolSupport::GetRaster(ViewcontextManagerInterface* ViewContextManager) {
   SuriObject::UuidType pviewcontextid = ViewContextManager->GetSelectedViewcontextId();
   return GetRaster(ViewContextManager, pviewcontextid);
}

/**
 * Obtiene el Raster seleccionado del contexto indicado
 * @param[in]  ViewContextManager: Puntero al Administrador de Contextos
 * @param[in]  pViewContextId: Id del viewcontex
 * @return     Devuelve el si tuvo exito (el contexto tiene un raster seleccionado),
 *             sino devuelve NULL
 */
RasterElement* ToolSupport::GetRaster(ViewcontextManagerInterface* ViewContextManager,
                                                SuriObject::UuidType pViewContextId) {
   ViewcontextInterface *pviewcontext = ViewContextManager->GetViewcontext(pViewContextId);
   RasterElement *praster = NULL;
   if (pviewcontext == NULL)
      return NULL;
   std::list<SuriObject::UuidType> selectedlayers =
                                       pviewcontext->GetSelectedLayers();
   std::list<SuriObject::UuidType>::const_iterator it = selectedlayers.begin();
   for (; it != selectedlayers.end() && praster == NULL; ++it) {
      LayerInterface* player = pviewcontext->GetLayer(*it);
      if (player)
         praster = dynamic_cast<RasterElement*>(player->GetElement());
   }
   return praster;
}


/** Obtiene el raster seleccionado del contexto activo */
RasterDatasource* ToolSupport::GetRasterDatasource(DatasourceManagerInterface* pDatasourceManager,
                                      ViewcontextManagerInterface* pViewContextManager) {
   RasterLayer* prasterlayer = NULL;
   ViewcontextInterface* pselectedviewcontext = pViewContextManager->GetSelectedViewcontext();
   if (pselectedviewcontext != NULL) {
      std::list<SuriObject::UuidType> selectedlayers =
                                       pselectedviewcontext->GetSelectedLayers();
      std::list<SuriObject::UuidType>::const_iterator it = selectedlayers.begin();
      for (; it != selectedlayers.end() && prasterlayer == NULL; ++it) {
         LayerInterface* player = pselectedviewcontext->GetLayer(*it);
         if (player)
            prasterlayer = dynamic_cast<RasterLayer*>(player);
      }
   }
   RasterDatasource* prasterdatasource = NULL;
   if (prasterlayer) {
      DatasourceInterface* pdatasource = pDatasourceManager->GetDatasource(
            prasterlayer->GetAssociatedDatasourceId());
      prasterdatasource = dynamic_cast<RasterDatasource*>(pdatasource);
   }
   return prasterdatasource;
}

/**
 * Obtiene todos los elementos raster del contexto activo
 */
std::vector<RasterElement*> ToolSupport::GetAllRaster(
      ViewcontextManagerInterface* ViewContextManager) {
   SuriObject::UuidType pviewcontextid = ViewContextManager->GetSelectedViewcontextId();
   return GetAllRaster(ViewContextManager, pviewcontextid);
}

/**
 * Obtiene todos los elementos raster del contexto indicado.
 */
std::vector<RasterElement*> ToolSupport::GetAllRaster(
      ViewcontextManagerInterface* ViewContextManager,
      SuriObject::UuidType pViewContextId) {
   ViewcontextInterface *pviewcontext = ViewContextManager->GetViewcontext(
         pViewContextId);
   RasterElement *praster = NULL;
   if (pviewcontext == NULL)
      return std::vector<RasterElement*>();
   std::list<SuriObject::UuidType> selectedlayers = pviewcontext->GetSelectedLayers();
   std::vector<RasterElement*> retvec;
   std::list<SuriObject::UuidType>::const_iterator it = selectedlayers.begin();
   for ( ; it != selectedlayers.end(); ++it) {
      LayerInterface* player = pviewcontext->GetLayer(*it);
      // verifico que sea un puntero valido
      if (player) {
         praster = dynamic_cast<RasterElement*>(player->GetElement());
         // verifico que se pudo hacer el cast
         if (praster)
            retvec.push_back(praster);
      }
   }
   return retvec;
}

/**
 * Obtiene el Raster seleccionado del contexto indicado
 * @param[in]  ViewContextManager: Puntero al Administrador de Contextos
 * @param[in]  pViewContextId: Id del viewcontex
 * @return     Devuelve el si tuvo exito (el contexto tiene un raster seleccionado),
 *             sino devuelve NULL
 */
RasterElement* ToolSupport::GetActiveRaster(ViewcontextManagerInterface* ViewContextManager,
                                                SuriObject::UuidType pViewContextId) {
   ViewcontextInterface *pviewcontext = ViewContextManager->GetViewcontext(pViewContextId);
   LayerList* playerlist = pviewcontext->GetLayerList();
   RasterElement *praster = NULL;
   int activecount = playerlist->GetActiveCount(LayerList::AllGroups);
   for (int i = 0; i < activecount && praster == NULL; i++) {
      Element* pelement = playerlist->GetActiveElement(i, LayerList::AllGroups);
      praster = dynamic_cast<RasterElement*>(pelement);
   }
   return praster;
}

/**
 * Obtiene el Vector activo del contexto seleccionado
 * @param[in]  ViewContextManager: Puntero al Administrador de Contextos
 * @return     Devuelve el VectorElement si tuvo exito
 *             sino devuelve NULL
 */
VectorElement* ToolSupport::GetVector(ViewcontextManagerInterface* ViewContextManager) {
   VectorLayer* pvectorlayer = GetVectorLayer(ViewContextManager);
   VectorElement* pvectorelement = pvectorlayer ?
         dynamic_cast<VectorElement*>(pvectorlayer->GetElement()) : NULL;
   return pvectorelement;
}

/**
 * Obtiene el VectorLayer activo del contexto seleccionado
 * @param[in]  ViewContextManager: Puntero al Administrador de Contextos
 * @return     Devuelve VectorLayer si tuvo exito
 *             sino devuelve NULL
 */
VectorLayer* ToolSupport::GetVectorLayer(ViewcontextManagerInterface* ViewContextManager) {
   VectorLayer* pvectorlayer = NULL;
   ViewcontextInterface* pselectedviewcontext = ViewContextManager->GetSelectedViewcontext();
   if (pselectedviewcontext != NULL) {
      std::list<SuriObject::UuidType> selectedlayers =
                                       pselectedviewcontext->GetSelectedLayers();
      std::list<SuriObject::UuidType>::const_iterator it = selectedlayers.begin();
      for (; it != selectedlayers.end() && pvectorlayer == NULL; ++it) {
         LayerInterface* player = pselectedviewcontext->GetLayer(*it);
         if (player)
            pvectorlayer = dynamic_cast<VectorLayer*>(player);
      }
   }
   return pvectorlayer;
}

/**
 * Obtiene el VectorDatasource activo del contexto seleccionado
 * @param[in]  DatasourceManager: Puntero al Administrador de datasources
 * @param[in]  ViewContextManager: Puntero al Administrador de Contextos
 * @return     VectorDatasource si tuvo exito,
 *             sino devuelve NULL
 */
VectorDatasource* ToolSupport::GetVectorDatasource(
                        DatasourceManagerInterface* pDatasourceManager,
                           ViewcontextManagerInterface* pViewContextManager) {
   VectorLayer* pvectorlayer = ToolSupport::GetVectorLayer(pViewContextManager);
   if (!pvectorlayer)
      return NULL;
   DatasourceInterface* pdatasource = pDatasourceManager->GetDatasource(
                                     pvectorlayer->GetAssociatedDatasourceId());
   VectorDatasource* pvectordatasource = dynamic_cast<VectorDatasource*>(pdatasource);
   return pvectordatasource;
}

/**
 * Obtiene el World (mundo) del Viewport seleccionado
 * @param[in]  ViewportManager: Puntero al Administrador de Viewports
 * @return     Devuelve el si tuvo exito (el viewport seleccionado tiene un mundo inicializado),
 *             sino devuelve NULL
 */
World* ToolSupport::GetWorld(ViewportManagerInterface* ViewportManager) {
   if (ViewportManager == NULL || ViewportManager->GetSelectedViewport() == NULL) {
      return NULL;
   }
   World *pworld = AS_VW(ViewportManager->GetSelectedViewport())->GetWorld();
   if (pworld == NULL || !pworld->IsInitialized()) {
      return NULL;
   }
   return pworld;
}

/**
 * Obtiene el Subset a partir del Viewport activo y el Raster seleccionado
 * @param[in]  ViewportManager: Puntero al Administrador de Viewports
 * @param[in]  ViewerWindow: Subset a ser devuelto
 * @param[in]  PRaster: Raster seleccionado
 */
void ToolSupport::GetSubset(ViewportManagerInterface* ViewportManager,
                            Subset& ViewerWindow, RasterElement* pRaster) {
   ViewerWindow = Subset();
   // Obtiene el mundo del elemento desplegado
   World *pworld = ToolSupport::GetWorld(ViewportManager);
   if (pworld == NULL || pRaster == NULL) {
      return;
   }
   // Crea un CoordinateTransform para pasar de coordenadas de mundo a Lat/Long
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn, pworld->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, pRaster->GetSpatialReference().c_str());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pcoordinatetransform = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   // Transforma las coordenadas de la ventana al sistema Lat/Long
   if (pcoordinatetransform != NULL) {
      pworld->GetWindow(ViewerWindow);
      pcoordinatetransform->Transform(ViewerWindow);
      delete pcoordinatetransform;
   }
}

/**
 * Recorre la lista para ver si hay un elemento vectorial de tipo poligono
 * @return true: si hay algun elemento vectorial de tipo poligono.
 */
bool ToolSupport::ContainsPolygons(ViewcontextManagerInterface* ViewContextManager) {
   LayerList* layerlist = ToolSupport::GetLayerList(ViewContextManager);
   LayerList::ElementListType::iterator it = layerlist->GetElementIterator();
   LayerList::ElementListType::iterator itend = layerlist->GetElementIterator(false);
   VectorElement* vector;
   std::string vectortype;
   for (; it != itend; it++) {
      vector = dynamic_cast<suri::VectorElement*>(layerlist->GetElement(*it));
      if (vector) {
         vectortype = vector->GetTypeAsString().c_str();
         if (!vectortype.compare(TYPE_DECORATOR(message_POLYGON).c_str())) {
            return true;
         }
      }
   }
   return false;
}

} /** namespace suri */
