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
#include <string>

// Includes Suri
#include "RasterLink.h"
#include "suri/RasterSpatialModel.h"
#include "suri/RasterElement.h"
#include "suri/ViewcontextInterface.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx

// Defines

// forwards

namespace suri {

RasterLink::RasterLink(bool LinkCenter) :
      linkCenter_(LinkCenter) {
}

RasterLink::~RasterLink() {
}

bool RasterLink::IsCentered() const {
   return linkCenter_;
}

bool RasterLink::Execute() {
   ViewcontextInterface *pviewcontext = GetViewcontextManager()->GetViewcontext(
         GetMasterViewport()->GetViewcontextUuid());
   int count = pviewcontext->GetLayerList()->GetActiveCount(6);
   if ( (count == 1) || (count == 2) ) {
      RasterElement *pactiveraster =
            dynamic_cast<RasterElement*>(pviewcontext->GetLayerList()->GetActiveElement(0, 6));
      if (pactiveraster) {
         World *pvpworld =
               AS_VW(GetMasterViewport()) ? AS_VW(GetMasterViewport())->GetWorld() :
                                            NULL;
         RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
               pactiveraster->GetRasterModel());
         ParameterCollection params;
         params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                      pvpworld->GetSpatialReference());
         params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                      pactiveraster->GetSpatialReference().c_str());
         TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
         CoordinatesTransformation *pct = pfactory->Create(
               TransformationFactory::kExact, params);
         TransformationFactoryBuilder::Release(pfactory);
         if (prastermodel && pct) {
            Subset window;
            pvpworld->GetWindow(window);
            // transforma del sistema del viewport al del raster
            pct->Transform(window);
            // transforma del sistema del raster a PixelLinea
            prastermodel->InverseTransform(window.ul_);
            prastermodel->InverseTransform(window.lr_);
            std::vector<ViewportInterface*> linkedvps = GetLinkedViewports();
            for (std::vector<ViewportInterface*>::iterator it = linkedvps.begin();
                  it != linkedvps.end(); ++it) {
               World *plinkedworld = AS_VW(*it) ? AS_VW(*it)->GetWorld() : NULL;
               if (plinkedworld) {
                  RasterElement *plinkedraster =
                        dynamic_cast<RasterElement*>(GetViewcontextManager()->GetViewcontext(
                              (*it)->GetViewcontextUuid())->GetLayerList()->GetActiveElement(0, 6));
                  if (plinkedraster) {
                     RasterSpatialModel *plinkedrastermodel =
                           RasterSpatialModel::Create(plinkedraster->GetRasterModel());
                     ParameterCollection params;
                     params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                                  plinkedraster->GetSpatialReference().c_str());
                     params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                                  plinkedworld->GetSpatialReference());
                     TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
                     CoordinatesTransformation *plinkedct = pfactory->Create(
                           TransformationFactory::kExact, params);
                     TransformationFactoryBuilder::Release(pfactory);
                     if (plinkedrastermodel && plinkedct) {
                        Subset newwindow(window);
                        // transforma de PixelLinea a sistema del raster destino
                        plinkedrastermodel->Transform(newwindow.ul_);
                        plinkedrastermodel->Transform(newwindow.lr_);
                        // transforma de sistema del raster al del mundo destino
                        plinkedct->Transform(newwindow,
                                                        DEFAULT_CALCULATION_POINTS,
                                                        true);

                        plinkedworld->BlockViewerUpdate();
                        Coordinates oldcenter = newwindow.GetCenter();
                        if (linkCenter_) {
                           plinkedworld->GetWindow(newwindow);
                        } else {
                           plinkedworld->SetWindow(newwindow);
                           plinkedworld->GetWindow(newwindow);
                        }
                        Coordinates center = newwindow.GetCenter();
                        double distX = oldcenter.x_ - center.x_;
                        double distY = oldcenter.y_ - center.y_;
                        newwindow = newwindow + Coordinates(distX, distY);
                        plinkedworld->UnblockViewerUpdate();
                        plinkedworld->SetWindow(newwindow);
                     }
                     delete plinkedct;
                     RasterSpatialModel::Destroy(plinkedrastermodel);
                  }
               }
            }
         }
         delete pct;
         RasterSpatialModel::Destroy(prastermodel);
      }
   }
   return false;
}

} /** namespace suri */
