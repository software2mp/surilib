/*! \file GeographicLink.cpp */
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
