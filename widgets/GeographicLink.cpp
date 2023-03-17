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
#include "GeographicLink.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx

// Defines

// forwards

namespace suri {

GeographicLink::GeographicLink(bool LinkCenter) :
      linkCenter_(LinkCenter) {
}

GeographicLink::~GeographicLink() {
}

bool GeographicLink::IsCentered() const {
   return linkCenter_;
}

bool GeographicLink::Execute() {
   World *pvpworld =
         AS_VW(GetMasterViewport()) ? AS_VW(GetMasterViewport())->GetWorld() : NULL;

   Subset window;
   pvpworld->GetWindow(window);
   std::vector<ViewportInterface*> linkedvps = GetLinkedViewports();
   for (std::vector<ViewportInterface*>::iterator it = linkedvps.begin();
         it != linkedvps.end(); ++it) {
      World *plinkedworld = AS_VW(*it) ? AS_VW(*it)->GetWorld() : NULL;
      if (plinkedworld) {
         ParameterCollection params;
         params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                      pvpworld->GetSpatialReference());
         params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                      plinkedworld->GetSpatialReference());
         TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
         CoordinatesTransformation *pct = pfactory->Create(
               TransformationFactory::kExact, params);
         TransformationFactoryBuilder::Release(pfactory);

         // Actualiza la ventana, y luego mueve el centro para que coincidan
         if (pct) {
            Subset newwindow(window);
            pct->Transform(newwindow);
            plinkedworld->BlockViewerUpdate();
            Coordinates oldcenter = newwindow.GetCenter();
            if (linkCenter_) {
               plinkedworld->GetWindow(newwindow);
            } else {
               plinkedworld->SetWindow(newwindow);
               plinkedworld->GetWindow(newwindow);
            }
            Coordinates center = newwindow.GetCenter();
            newwindow = newwindow
                  + Coordinates(oldcenter.x_ - center.x_, oldcenter.y_ - center.y_);
            plinkedworld->UnblockViewerUpdate();
            plinkedworld->SetWindow(newwindow);
            delete pct;
         }
      }
   }
   return true;
}

} /** namespace suri */
