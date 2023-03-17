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

// Includes standard

// Includes suri
#include "suri/Element.h"
#include "suri/World.h"
#include "suri/RasterElement.h"
#include "suri/ListView.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/ActiveRasterWorldExtentManager.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

/**
 * Constructor
 * @param[in] pLayerList lista de elementos
 * @param[in] pWorld puntero al mundo
 */
ActiveRasterWorldExtentManager::ActiveRasterWorldExtentManager(
      LayerList *pLayerList, World *pWorld) : pLayerList_(pLayerList), pWorld_(pWorld) {
}

/**
 * Destructor
 */
ActiveRasterWorldExtentManager::~ActiveRasterWorldExtentManager() {
}

/**
 * Devuelve un puntero a LayerList
 * @return
 */
LayerList* ActiveRasterWorldExtentManager::GetLayerList() {
   return pLayerList_;
}

/**
 * Devuelve un puntero a World
 * @return
 */
World* ActiveRasterWorldExtentManager::GetWorld() {
   return pWorld_;
}

/**
 * Le asigna al extent del mundo un subset que contenga los extent de todas las imagenes activas.
 *
 * \todo (danieltesta-#4287): Revisar el uso del elemento asociado (GetAssociatedElement)
 */
void ActiveRasterWorldExtentManager::SetExtent() {
// No bloqueo el mundo para que si hay mas de un Renderizador, este reciba la actualizacion
#ifdef __UNUSED_CODE__
   pWorld_->BlockViewerUpdate();
#endif
   // Recorro la lista buscando elementos raster activos
   suri::LayerList::ElementListType::iterator it = pLayerList_->GetElementIterator(
         true);
   while (it != pLayerList_->GetElementIterator(false)) {
      Element *pelement = ((*it)->HasAssociatedElement()) ? (*it)->GetAssociatedElement() : *it;
      if (pelement->IsActive() && dynamic_cast<suri::RasterElement*>(pelement)) {
         ParameterCollection params;
         params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                      pelement->GetSpatialReference().c_str());
         params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                      pWorld_->GetSpatialReference());
         TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
         CoordinatesTransformation *pct = pfactory->Create(
               TransformationFactory::kExact, params);
         TransformationFactoryBuilder::Release(pfactory);
         if (pct) {
            Subset rasterextent;
            pelement->GetElementExtent(rasterextent);
            if (rasterextent != Subset(0, 0, 0, 0)) {
               // Encuentro extent en coordenadas de world
               if (pct->Transform(rasterextent) == 0) {
                  REPORT_DEBUG(
                        "D:Coordenadas de raster no se pudieron calcular en coordenadas de mundo.");
               }

               delete pct;
               Subset worldsubset;
               pWorld_->GetWorld(worldsubset);
               // como hay 1 solo raster, usar append no genera problemas
               // al pasar a mas de 1 o que los vectores influyan en el extent
               // se debera modificar esta logica (eliminarla de World y pasarla
               // a una clase mas especifica)
               pWorld_->AppendWorld(rasterextent);
            }
         } else {
            REPORT_DEBUG(
                  "D:Extent de raster imposible de calcular en coordenadas de mundo.");
         }
      }
      it++;
   }
// No bloqueo el mundo para que si hay mas de un Renderizador, este reciba la actualizacion
#ifdef __UNUSED_CODE__
   pWorld_->UnblockViewerUpdate();
#endif
}

}
