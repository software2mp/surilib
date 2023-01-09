/*! \file NavigationToolCommandExecutionHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/NavigationToolCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/RasterElement.h"
#include "suri/RasterSpatialModel.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/Viewer2D.h"
#include "suri/Viewer3D.h"
#include "suri/PartContainerWidget.h"
#include "ViewerSynchronizationPart.h"
#include "suri/ViewportLinkManager.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/Configuration.h"
#include "TerrainGLCanvas.h"
#include "CoordinatesNavigatorPart.h"
#include "suri/CoordinatesNavigationInputInterface.h"
#include "GoButton.h"

namespace suri {
namespace core {

/** Constructor */
ViewportLinkCommandExecutionHandler::ViewportLinkCommandExecutionHandler(
      DataViewManager* pDataView) : pDataView_(pDataView),
            pViewportLink_(new ViewportLinkManager(
                  pDataView->GetViewportManager(), pDataView->GetViewcontextManager())) {
}

/** Destructor */
ViewportLinkCommandExecutionHandler::~ViewportLinkCommandExecutionHandler() {
   delete pViewportLink_;
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ViewportLinkCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("ViewportLink"))
      return false;
   ViewerSynchronizationPart *pviewportsync = new ViewerSynchronizationPart(
         pDataView_->GetViewportManager(), pDataView_->GetViewcontextManager(), pViewportLink_);
   PartContainerWidget *pcontainer =
         new PartContainerWidget(pviewportsync, _(caption_VIEWER_SYNCHRONIZATION));
   pcontainer->ShowModal();
   return true;
}

/** Constructor */
NavigationCommandExecutionHandler::NavigationCommandExecutionHandler(
      DataViewManager* pDataView, GenericTool* pTool, Navigator *pNavigator) :
            pDataView_(pDataView), pTool_(pTool), pNavigator_(pNavigator),
            pCoordinateInputWidget_(new CoordinatesNavigatorPart(pDataView->GetLibraryManager())),
            pGoButton_(NULL) {
}

/** Destructor */
NavigationCommandExecutionHandler::~NavigationCommandExecutionHandler() {
   delete pGoButton_;
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool NavigationCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("Navigation"))
      return false;
   ViewerWidget* pviewer = dynamic_cast<Viewer2D*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget *pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   // Si es un DynamicViewerWidget obtengo el visualizador activo
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<ViewerWidget*>(pdynamic->GetViewer());
   // Verifico que existan todos los elementos para la operacion
   if (pviewer && pNavigator_) {
      if (pCommand->IsActive()){
         if (pviewer)
         pNavigator_->StartPaintEvent();
      } else {
         pNavigator_->StopPaintEvent();
      }
      NavigationWidget *pnavwidget = pNavigator_->GetWidget();
      if (pnavwidget) {
         if (pGoButton_) {
            // Elimina el boton anterior
            pnavwidget->RemoveButtonFromTool(pGoButton_);
            // Elimino la ventana del boton en forma segura.
            pGoButton_->GetWindow()->Destroy();
         }
         if (pCommand->IsActive()) {
            std::string spatial =
                   pviewer->GetWorld() ? pviewer->GetWorld()->GetSpatialReference() :
                         LATLON_SPATIAL_REFERENCE;
            // Navigator va a utilizar coordenadas LatLong
            pNavigator_->SetSpatialReference(spatial);
            if (pnavwidget->GetInputWidget() == NULL) {
               pnavwidget->SetInputWidget(pCoordinateInputWidget_);
            } else {
               pCoordinateInputWidget_ = pnavwidget->GetInputWidget();
            }
            pCoordinateInputWidget_->SetSpatialReference(spatial);
            pCoordinateInputWidget_->SetDataViewManager(pDataView_);
            /** Si el visualizador esta en modo GIS deshabilito la navegacion en pixel linea **/
            if (dynamic_cast<Viewer2D*>(pviewer) &&
                  (dynamic_cast<Viewer2D*>(pviewer)->GetMode()== Viewer2D::Gis||
                        Configuration::GetParameter("app_short_name", "") != wxT("SoPI"))) {
               pCoordinateInputWidget_->EnablePixelLineNavigation(false);
            }

            bool found = false;
            Element* pactelement = NULL;
            LayerList* plist =
                  pDataView_->GetViewcontextManager()->GetSelectedViewcontext()->GetLayerList();
            for (int ix = 0, lenix = plist->GetElementCount(); ix < lenix && !found; ++ix) {
               pactelement = dynamic_cast<RasterElement*>(plist->GetElement(ix));
               if (pactelement && pactelement->IsActive()) found = true;
               else {
                  pactelement = NULL;
               }
            }

            if (pactelement) {
               pCoordinateInputWidget_->SetSpatialReference(
                     pactelement->GetSpatialReference().c_str());
            }
            Coordinates windowcenter;
            if (pNavigator_->GetCenterPoint(windowcenter)) {
                  pCoordinateInputWidget_->SetCoordinate(windowcenter);
               }
            // Agrega boton a NavigatorWidget
            pGoButton_ = new GoButton(pnavwidget->GetToolWindow(), pviewer,
                                      pactelement, pDataView_->GetViewportManager(),
                                      pDataView_->GetViewcontextManager(), pNavigator_);
            pnavwidget->AddButtonTool(pGoButton_, ToolGroupManager::First);
         } else if (pGoButton_) {
            // Actualiza mundo para no mostrar la cruz
            pviewer->GetWorld()->SetChanged();
            pviewer->GetWorld()->SendViewerUpdate();
            pGoButton_ = NULL;
         }
      }
   }
   return true;
}

/** Constructor */
FullExtentCommandExecutionHandler::FullExtentCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
FullExtentCommandExecutionHandler::~FullExtentCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool FullExtentCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("FullExtent"))
      return false;
   Viewer2D *pviewer = dynamic_cast<Viewer2D*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   if (pviewer) {
      Subset world;
      pviewer->GetWorld()->GetWorld(world);
      pviewer->ChangeWindow(world);
   }
   return true;
}

/** Constructor */
LayerExtentCommandExecutionHandler::LayerExtentCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
LayerExtentCommandExecutionHandler::~LayerExtentCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool LayerExtentCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("LayerExtent"))
      return false;
   ViewcontextInterface *pviewcontext =
                                    pDataView_->GetViewcontextManager()->GetSelectedViewcontext();
   std::list<SuriObject::UuidType> selectedlayers = pviewcontext->GetSelectedLayers();
   if (selectedlayers.size() == 1) {
      LayerInterface* player = pviewcontext->GetLayer(*selectedlayers.begin());
      Element *pelement = player->GetElement();
      if (pelement) {
         Viewer2D* pview = dynamic_cast<Viewer2D*>(
               pDataView_->GetViewportManager()->GetSelectedViewport());
         DynamicViewerWidget *pdynamic = dynamic_cast<DynamicViewerWidget*>(
               pDataView_->GetViewportManager()->GetSelectedViewport());
         if (!pview && pdynamic)
            pview = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
         if (pview) {
            World *pviewerworld = pview->GetWorld();
            if (pviewerworld) {
               ParameterCollection params;
               params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                            pelement->GetSpatialReference().c_str());
               params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                            pviewerworld->GetSpatialReference());
               TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
               CoordinatesTransformation *pcoordtrans = pfactory->Create(
                     TransformationFactory::kExact, params);
               TransformationFactoryBuilder::Release(pfactory);

               Subset elementwindow;
               pelement->GetElementExtent(elementwindow);
               if (pcoordtrans && pcoordtrans->Transform(elementwindow, 100) != 0) {
                  Subset world;
                  pviewerworld->GetWorld(world);
                  elementwindow = Intersect(world, elementwindow);
                  pview->ChangeWindow(elementwindow);
               }
               delete pcoordtrans;
            }
         }
      }
   }
   return true;
}

/** Constructor */
Zoom1on1CommandExecutionHandler::Zoom1on1CommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
Zoom1on1CommandExecutionHandler::~Zoom1on1CommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool Zoom1on1CommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("Zoom1on1"))
      return false;
   RasterElement *pelement = NULL;
   ViewcontextInterface *pviewcontext =
         pDataView_->GetViewcontextManager()->GetSelectedViewcontext();
   // Recorro la lista buscando un Raster activo
   if (!pviewcontext->GetLayerList() || pviewcontext->GetLayerList()->GetActiveCount() == 0) {
      return false;
   }

   LayerList::ElementListType::iterator it =
         pviewcontext->GetLayerList()->GetElementIterator();
   while (it != pviewcontext->GetLayerList()->GetElementIterator(false)) {
      if ((*it)->IsActive() && dynamic_cast<RasterElement*>(*it)) {
         pelement = dynamic_cast<RasterElement*>(*it);
         break;
      }
      it++;
   }
   if (!pelement) {
      REPORT_DEBUG("D:No se pudo encontrar el raster activo");
      return false;
   }

   // Obtengo el RasterSpatialModel y calculo el tamano del window para pixel 1:1
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
         pelement->GetRasterModel());
   if (!prastermodel) {
      return false;
   }

   Subset newwindow(0, 0, 0, 0);
   int x, y;
   Viewer2D *pviewer =  dynamic_cast<Viewer2D*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget *pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   if (pviewer) {
      World *pworld = pviewer->GetWorld();
      if (pworld) {
         pworld->GetViewport(x, y);
         newwindow.lr_.x_ = x;
         newwindow.lr_.y_ = y;
         prastermodel->Transform(newwindow.lr_);
         prastermodel->Transform(newwindow.ul_);
         RasterSpatialModel::Destroy(prastermodel);
         // encuentro la posicion final de la ventana
         Subset oldwindow;
         pworld->GetWindow(oldwindow);
         double oldcenter_x = (oldwindow.lr_.x_ + oldwindow.ul_.x_) / 2;
         double oldcenter_y = (oldwindow.lr_.y_ + oldwindow.ul_.y_) / 2;
         double difx = (newwindow.lr_.x_ - newwindow.ul_.x_) / 2;
         double dify = (newwindow.lr_.y_ - newwindow.ul_.y_) / 2;
         newwindow.lr_.x_ = oldcenter_x + difx;
         newwindow.ul_.x_ = oldcenter_x - difx;
         newwindow.lr_.y_ = oldcenter_y + dify;
         newwindow.ul_.y_ = oldcenter_y - dify;
         pviewer->ChangeWindow(newwindow);
      }
   }
   return true;
}

/** Constructor */
SaveViewCommandExecutionHandler::SaveViewCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
SaveViewCommandExecutionHandler::~SaveViewCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool SaveViewCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("SaveView"))
      return false;
   ViewerWidget *pviewer = AS_VW(pDataView_->GetViewportManager()->GetSelectedViewport());
   wxDC *pscreendc = new wxClientDC(pviewer->GetWindow());
   wxMemoryDC *ptargetdc = new wxMemoryDC;
   int width, height;
   pscreendc->GetSize(&width, &height);
   wxBitmap outputbmp(width, height);
   ptargetdc->SelectObject(outputbmp);
   ptargetdc->Blit(0, 0, width, height, pscreendc, 0, 0);
   ptargetdc->SelectObject(wxNullBitmap);
   delete ptargetdc;
   delete pscreendc;
   std::string visualizationpath = Configuration::GetParameter("app_visualization_dir", "");
   wxFileDialog dlg(NULL, _(caption_SAVE_VISUALIZATION), wxT(visualizationpath), wxT(""),
                    filter_SAVE_VISUALIZATION, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() == wxID_OK) {
      outputbmp.ConvertToImage().SaveFile(dlg.GetPath());
   }
   return true;
}

/** Constructor */
ZoomInCommandExecutionHandler::ZoomInCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
ZoomInCommandExecutionHandler::~ZoomInCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ZoomInCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("ZoomIn"))
      return false;
   Viewer2D *pviewer = dynamic_cast<Viewer2D*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   if (pviewer) {
      wxSize size = pviewer->GetWindow()->GetSize();
      pviewer->ZoomIn(size.GetWidth() / 2, size.GetHeight() / 2);
   }
   return true;
}

/** Constructor */
ZoomOutCommandExecutionHandler::ZoomOutCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
ZoomOutCommandExecutionHandler::~ZoomOutCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ZoomOutCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("ZoomOut"))
      return false;
   Viewer2D *pviewer = dynamic_cast<Viewer2D*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataView_->GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   if (pviewer) {
      wxSize size = pviewer->GetWindow()->GetSize();
      pviewer->ZoomOut(size.GetWidth() / 2, size.GetHeight() / 2);
   }
   return true;
}

/** Constructor */
CameraCommandExecutionHandler::CameraCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
CameraCommandExecutionHandler::~CameraCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool CameraCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("EyeCamera") &&
         pCommand->GetId() != pTool->GetId("SphericalCamera"))
      return false;
   if (pCommand->IsActive()) {
      Viewer3D *pviewer = dynamic_cast<Viewer3D*>(
            pDataView_->GetViewportManager()->GetSelectedViewport());
      DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
            pDataView_->GetViewportManager()->GetSelectedViewport());
      if (!pviewer && pdynamic)
         pviewer = dynamic_cast<Viewer3D*>(pdynamic->GetViewer());
      if (pviewer) {
         TerrainGLCanvas::ToggleButtonActionType action = TerrainGLCanvas::SphericCam;

         if (pCommand->GetId() == pTool->GetId("EyeCamera"))
            action = TerrainGLCanvas::EyeCam;
         if (pCommand->GetId() == pTool->GetId("SphericalCamera"))
            action = TerrainGLCanvas::SphericCam;

         pviewer->GetCanvas()->SetAction(action);
      }
   }
   return true;
}

} /** namespace core */
} /** namespace suri */
