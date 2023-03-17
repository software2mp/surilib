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
