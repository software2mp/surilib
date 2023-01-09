/*! \file GisWorldExtentManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar

// Includes Suri
#include "suri/Element.h"
#include "suri/World.h"
#include "suri/RasterElement.h"
#include "suri/ListView.h"
#include "GisWorldExtentManager.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in] pLayerList lista de elementos
 * @param[in] pWorld puntero al mundo
 */
GisWorldExtentManager::GisWorldExtentManager(
      LayerList* pLayerList, World* pWorld) : pLayerList_(pLayerList), pWorld_(pWorld) {
}

/**
 * Destructor
 */
GisWorldExtentManager::~GisWorldExtentManager() {
}

/**
 * Devuelve un puntero a LayerList
 * @return
 */
LayerList* GisWorldExtentManager::GetLayerList() {
   return pLayerList_;
}

/**
 * Devuelve un puntero a World
 * @return
 */
World* GisWorldExtentManager::GetWorld() {
   return pWorld_;
}


/**
 * Metodo que actualiza el mundo
 *
 * \todo (danieltesta-#4287): Revisar el uso del elemento asociado (GetAssociatedElement)
 */
void GisWorldExtentManager::SetExtent() {
   // TODO(Sebastian - TCK #2443): Implementar lo de Mayor Extent
// No bloqueo el mundo para que si hay mas de un Renderizador, este reciba la actualizacion
#ifdef __UNUSED_CODE__
   pWorld_->BlockViewerUpdate();
#endif

   Subset worldsubset;
   pWorld_->GetWorld(worldsubset);

   int joincount = 0;
   suri::LayerList::ElementListType::iterator it = pLayerList_->GetElementIterator(true);

   for (; it != pLayerList_->GetElementIterator(false); ++it) {
      Element* pelement = NULL;
      if ((*it)->HasAssociatedElement())
         pelement = (*it)->GetAssociatedElement();
      else
         pelement = *it;

      if (pelement->IsActive()) {
         ParameterCollection params;
         params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                      pelement->GetSpatialReference().c_str());
         params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                      pWorld_->GetSpatialReference());
         TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
         CoordinatesTransformation *pcoordinatetransform = pfactory->Create(TransformationFactory::kExact,
                                                           params);
         TransformationFactoryBuilder::Release(pfactory);
         if (pcoordinatetransform != NULL) {
            Subset extent;
            pelement->GetElementExtent(extent);

            if (extent != Subset(0, 0, 0, 0)) {
               int exitval = pcoordinatetransform->Transform(extent);
               if (exitval == 0) {
                  REPORT_DEBUG(
                        "D:Coordenadas de raster no se pudieron calcular en coordenadas de mundo.");
               }

               if (pWorld_->IsInitialized()) {
                  worldsubset = Join(worldsubset, extent);
                  joincount++;
               } else {
                  worldsubset = extent;
                  pWorld_->PreventNextViewerUpdate();
                  pWorld_->SetWorld(worldsubset);
                  pWorld_->PreventNextViewerUpdate(false);
                  pWorld_->SetWindow(worldsubset);
               }
              delete pcoordinatetransform;
            }

         } else {
            REPORT_DEBUG("D:Extent de raster imposible de calcular en coordenadas de mundo.");
         }
      }
   }
   if (joincount > 0) {
      pWorld_->SetWorld(worldsubset);
   }
   // No bloqueo el mundo para que si hay mas de un Renderizador, este reciba la actualizacion
#ifdef __UNUSED_CODE__
   pWorld_->UnblockViewerUpdate();
#endif
}

} /** namespace suri */
