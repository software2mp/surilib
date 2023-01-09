/*! \file RasterLink.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
