/*! \file NavigationTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <list>

// Includes Suri
#include "resources.h"
#include "GoButton.h"
#include "WorldWidget.h"
#include "TerrainGLCanvas.h"
#include "ViewerSynchronizationPart.h"
#include "suri/Viewer2D.h"
#include "suri/Viewer3D.h"
#include "suri/DataTypes.h"
#include "suri/DragEvent.h"
#include "suri/MultiZoom.h"
#include "suri/ToolCommand.h"
#include "suri/EventCommand.h"
#include "suri/RasterElement.h"
#include "suri/NavigationTool.h"
#include "suri/NavigationButton.h"
#include "suri/RasterSpatialModel.h"
#include "suri/PartContainerWidget.h"
#include "suri/ViewportLinkManager.h"
#include "suri/PixelLineInputWidget.h"
#include "suri/LatitudeLongitudeInputWidget.h"
#include "suri/CoordinatesNavigationInputInterface.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "CoordinatesNavigatorPart.h"
#include "CrossHairPaintEvent.h"
#include "suri/Configuration.h"
#include "suri/DynamicViewerWidget.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
NavigationTool::NavigationTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager),
      pZoom_(NULL),
      pDrag_(NULL),
      pCrossHair_(NULL),
      pGoButton_(NULL),
      pViewportLink_(
            new ViewportLinkManager(GetViewportManager(), GetViewcontextManager())),
      pCoordinateInputWidget_(new CoordinatesNavigatorPart(GetLibraryManager())) {
   configurationVariableName_ = "navigation";
   Initialize();
}

/**
 * Destructor
 */
NavigationTool::~NavigationTool() {
   Finalize();
   delete pZoom_;
   delete pDrag_;
   delete pGoButton_;
   delete pCrossHair_;
}

/**
 * Ejecuta un comando
 */
void NavigationTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("ZoomIn")) {
      Viewer2D *pviewer = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
      // Si es un DynamicViewerWidget obtengo el visualizador activo
      if (!pviewer &&
            dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport())) {
         DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
               GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
      }
      if (pviewer) {
         wxSize size = pviewer->GetWindow()->GetSize();
         pviewer->ZoomIn(size.GetWidth() / 2, size.GetHeight() / 2);
      }
   } else if (id == GetId("ZoomOut")) {
      Viewer2D *pviewer = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
      // Si es un DynamicViewerWidget obtengo el visualizador activo
      if (!pviewer &&
            dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport())) {
         DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
               GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
      }
      if (pviewer) {
         wxSize size = pviewer->GetWindow()->GetSize();
         pviewer->ZoomOut(size.GetWidth() / 2, size.GetHeight() / 2);
      }
   } else if (id == GetId("Disabler")) {
      std::list < std::string > commandnames = GetCommandNames();
      std::list<std::string>::iterator it = commandnames.begin();
      while (it != commandnames.end()) {
         if (*it != "Disabler") {
            Command *pcommand = GetCommand(GetId(*it));
            if (pcommand)
               pcommand->SetEnabled(!pToolCommand->IsActive());
         }
         ++it;
      }
   } else if (id == GetId("Disactivator")) {
      std::list < std::string > commandnames = GetCommandNames();
      std::list<std::string>::iterator it = commandnames.begin();
      while (it != commandnames.end()) {
         std::string name = *it;
         if (name != "Disactivator") {
            Command *pcommand = GetCommand(GetId(name));
            if (pcommand)
               pcommand->SetActive(false);
         }
         ++it;
      }
   } else if (id == GetId("FullExtent")) {
      Viewer2D *pviewer = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
      // Si es un DynamicViewerWidget obtengo el visualizador activo
      if (!pviewer &&
            dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport())) {
         DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
               GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
      }
      if (pviewer) {
         Subset world;
         pviewer->GetWorld()->GetWorld(world);
         pviewer->ChangeWindow(world);
      }
   } else if (id == GetId("LayerExtent")) {
      ExecuteLayerExtent();
   } else if (id == GetId("Zoom1on1")) {
      ExecuteZoom1on1();
   } else if (id == GetId("SaveView")) {
      ExecuteSaveView();
   }
#ifdef __ENABLE_WORLDWATCHER_TOOL__
   if (id == GetId("WorldWatcher")) {
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      new WorldWidget(pviewer->GetWorld());
   }
#endif
   if (id == GetId("ViewportLink")) {
      ViewerSynchronizationPart *pviewportsync = new ViewerSynchronizationPart(
            GetViewportManager(), GetViewcontextManager(), pViewportLink_);
      PartContainerWidget *pcontainer = new PartContainerWidget(pviewportsync,
                                                                _(caption_VIEWER_SYNCHRONIZATION));
      pcontainer->ShowModal();
   } else if (id == GetId("SphericalCamera") || id == GetId("EyeCamera")) {
      if (pToolCommand->IsActive()) {
         Viewer3D *pviewer = dynamic_cast<Viewer3D*>(GetViewportManager()->GetSelectedViewport());
         // Si es un DynamicViewerWidget obtengo el visualizador activo
         if (!pviewer &&
               dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport())) {
            DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
                  GetViewportManager()->GetSelectedViewport());
            pviewer = dynamic_cast<Viewer3D*>(pdynamic->GetViewer());
         }
         if (pviewer) {
            TerrainGLCanvas::ToggleButtonActionType action = TerrainGLCanvas::SphericCam;

            if (id == GetId("EyeCamera"))
               action = TerrainGLCanvas::EyeCam;
            if (id == GetId("SphericalCamera"))
               action = TerrainGLCanvas::SphericCam;

            pviewer->GetCanvas()->SetAction(action);
         }
      }
   } else if (id == GetId("Navigation") || id == GetId("RasterNavigation")) {
      ExecuteNavigation(pToolCommand->IsActive());
   }
}


/** Metodo que ejecuta el comando de navegacion **/
void NavigationTool::ExecuteNavigation(bool CommandActive) {
   Viewer2D *pviewer = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
   // Si es un DynamicViewerWidget obtengo el visualizador activo
   if (!pviewer &&
         dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport())) {
      DynamicViewerWidget *pdynamic =
            dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport());
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   }
   if (pviewer) {
      // Verifico que existan todos los elementos para la operacion
      Navigator *pnav = pviewer->GetNavigator();
      if (pnav) {
         if (!pCrossHair_) {
            pCrossHair_ = new CrossHairPaintEvent(pviewer, GetViewportManager(),
                                                  GetViewcontextManager(), pnav);
         }
         if (CommandActive)
            pviewer->PushPaintEvent(pCrossHair_);
         else
            pviewer->PopPaintEvent(pCrossHair_);

         NavigationWidget *pnavwidget = pnav->GetWidget();
         if (pnavwidget) {
            if (pGoButton_) {
               // Elimina el boton anterior
               pnavwidget->RemoveButtonFromTool(pGoButton_);
               // Elimino la ventana del boton en forma segura.
               pGoButton_->GetWindow()->Destroy();
            }
            if (CommandActive) {
               std::string spatial =
                      pviewer->GetWorld() ? pviewer->GetWorld()->GetSpatialReference() :
                            LATLON_SPATIAL_REFERENCE;
               // Navigator va a utilizar coordenadas LatLong
               pnav->SetSpatialReference(spatial);
               if (pnavwidget->GetInputWidget() == NULL) {
                  pnavwidget->SetInputWidget(pCoordinateInputWidget_);
               } else {
                  pCoordinateInputWidget_ = pnavwidget->GetInputWidget();
               }
               pCoordinateInputWidget_->SetSpatialReference(spatial);
               pCoordinateInputWidget_->SetDataViewManager(GetDataViewManager());
               /** Si el visualizador esta en modo GIS deshabilito la navegacion en pixel linea **/
               if (pviewer->GetMode()
                     == Viewer2D::Gis|| Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
                  pCoordinateInputWidget_->EnablePixelLineNavigation(false);
               }

               bool found = false;
               Element* pactelement = NULL;
               LayerList* plist =
                     GetViewcontextManager()->GetSelectedViewcontext()->GetLayerList();
               for (int ix = 0, lenix = plist->GetElementCount(); ix < lenix && !found; ++ix) {
                  pactelement = dynamic_cast<RasterElement*>(plist->GetElement(ix));
                  if (pactelement && pactelement->IsActive()) found = true;
                  else pactelement = NULL;
               }

               if (pactelement) {
                  pCoordinateInputWidget_->SetSpatialReference(
                        pactelement->GetSpatialReference().c_str());
               }
               Coordinates windowcenter;
               if (pnav->GetCenterPoint(windowcenter)) {
                  pCoordinateInputWidget_->SetCoordinate(windowcenter);
               }

               // Agrega boton a NavigatorWidget
               pGoButton_ = new GoButton(pnavwidget->GetToolWindow(), pviewer,
                                         pactelement, GetViewportManager(),
                                         GetViewcontextManager());
               pnavwidget->AddButtonTool(pGoButton_, ToolGroupManager::First);
            }
            else if (pGoButton_) {
               // Actualiza mundo para no mostrar la cruz
               pviewer->GetWorld()->SetChanged();
               pviewer->GetWorld()->SendViewerUpdate();
               pGoButton_ = NULL;
            }
         }
      }
   }
}

/** Ejecuta el comando LayerExtent **/
void NavigationTool::ExecuteLayerExtent() {
   ViewcontextInterface *pviewcontext = GetViewcontextManager()->GetSelectedViewcontext();
   std::list<SuriObject::UuidType> selectedlayers = pviewcontext->GetSelectedLayers();
   if (selectedlayers.size() == 1) {
      LayerInterface* player = pviewcontext->GetLayer(*selectedlayers.begin());
      Element *pelement = player->GetElement();
      if (pelement) {
         Viewer2D* pview = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
         // Si es un DynamicViewerWidget obtengo el visualizador activo
         if (!pview &&
               dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport())) {
            DynamicViewerWidget *pdynamic =
                  dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport());
            pview = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
         }
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
}

/** Metodo que ejecuta el comando Zoom1on1 **/
void NavigationTool::ExecuteZoom1on1() {
   RasterElement *pelement = NULL;
         ViewcontextInterface *pviewcontext = GetViewcontextManager()->GetSelectedViewcontext();
         // Recorro la lista buscando un Raster activo
         if (!pviewcontext->GetLayerList() || pviewcontext->GetLayerList()->GetActiveCount() == 0) {
            return;
         }

         LayerList::ElementListType::iterator it = pviewcontext->GetLayerList()->GetElementIterator();
         while (it != pviewcontext->GetLayerList()->GetElementIterator(false)) {
            if ((*it)->IsActive() && dynamic_cast<RasterElement*>(*it)) {
               pelement = dynamic_cast<RasterElement*>(*it);
               break;
            }
            it++;
         }
         if (!pelement) {
            REPORT_DEBUG("D:No se pudo encontrar el raster activo");
            return;
         }

         // Obtengo el RasterSpatialModel y calculo el tamano del window para pixel 1:1
         RasterSpatialModel *prastermodel = RasterSpatialModel::Create(pelement->GetRasterModel());
         if (!prastermodel) {
            return;
         }

         Subset newwindow(0, 0, 0, 0);
         int x, y;
         Viewer2D *pviewer = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
         // Si es un DynamicViewerWidget obtengo el visualizador activo
         if (!pviewer &&
               dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport())) {
            DynamicViewerWidget *pdynamic =
                  dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport());
            pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
         }
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
}


/** Metodo que ejecuta comando SaveView **/
void NavigationTool::ExecuteSaveView() {
   ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
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
   std::string visualizationpath = Configuration::GetParameter("app_visualization_dir","");
   wxFileDialog dlg(NULL, _(caption_SAVE_VISUALIZATION), wxT(visualizationpath), wxT(""),
                    filter_SAVE_VISUALIZATION, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() == wxID_OK) {
      outputbmp.ConvertToImage().SaveFile(dlg.GetPath());
   }
}
/**
 * Crea un comando
 */
Command* NavigationTool::CreateCommand(const std::string &CommandName) {
   Command *pcommand = NULL;
   if (CommandName == "ViewportLink") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_VIEWPORT_LINK);
      pcommand->SetDecoration(tool_VIEWPORT_LINK);
      pcommand->SetLocation("ID_SINCRONIZE_VIEWERS_MENUTIEM");
   } else if (CommandName == "Navigation") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-128);
      pcommand->SetDescription(tooltip_NAVIGATION_WIDGET);
      pcommand->SetDecoration(tool_NAVIGATION_WIDGET);
      pcommand->SetLocation("ID_CONATINERMID|ID_NAVIGATION_MENUTIEM");
   } else if (CommandName == "FullExtent") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(tooltip_ZOOM_FULL_EXTENT);
      pcommand->SetDecoration(button_ZOOM_FULL_EXTENT);
      pcommand->SetLocation("ID_CONATINERMID|ID_ZOOM_FULL_EXTENT_MENUTIEM");
   } else if (CommandName == "LayerExtent") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(tooltip_ZOOM_LAYER_EXTENT);
      pcommand->SetDecoration(button_ZOOM_LAYER_EXTENT);
      pcommand->SetLocation("ID_CONATINERMID|ID_ZOOM_TO_LAYER_MENUTIEM");
   } else if (CommandName == "Zoom1on1") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(tooltip_ZOOM_ONE_TO_ONE);
      pcommand->SetDecoration(button_ZOOM_ONE_TO_ONE);
      pcommand->SetLocation("ID_CONATINERMID|ID_ZOOM_1_1_MENUTIEM");
   } else if (CommandName == "SaveView") {
      // reemplazar con un command independiente
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button|Menu");
      pcommand->SetDescription(caption_SAVE_VISUALIZATION);
      pcommand->SetDecoration(button_SAVE_VIEW);
      pcommand->SetLocation("ID_CONATINERMID|ID_SAVE_VISIALIZATION_MENUTIEM");
   } else if (CommandName == "Drag") {
      pDrag_ = new DragEvent(GetViewportManager(), GetViewcontextManager());
      SetMouseEvent(pDrag_);
      pcommand = new EventCommand(pDrag_, NULL, SuriObject::NullIntUuid, "ToggleButton|Menu",
                                  tool_HAND, tooltip_DRAG, "ID_CONATINERMID|ID_SCROLL_MENUTIEM");
   } else if (CommandName == "MultiZoom") {
      pZoom_ = new MultiZoom(GetViewportManager(), GetViewcontextManager());
      SetMouseEvent(pZoom_);
      SetPaintEvent(pZoom_);
      pcommand = new EventCommand(pZoom_, pZoom_, SuriObject::NullIntUuid, "ToggleButton|Menu",
                                  tool_ZOOM, tooltip_ZOOM, "ID_CONATINERMID|ID_ZOOM_MENUTIEM");
   } else if (CommandName == "ZoomIn") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("ZoomIn");
      pcommand->SetDecoration(tool_ZOOM_IN);
      pcommand->SetLocation("ID_ZOOM_IN_MENUTIEM");
   } else if (CommandName == "ZoomOut") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription("ZoomOut");
      pcommand->SetDecoration(tool_ZOOM_OUT);
      pcommand->SetLocation("ID_ZOOM_OUT_MENUTIEM");
   } else if (CommandName == "SphericalCamera") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetGroupId(-128);
      pcommand->SetDescription(tooltip_SPHERICAL_CAMERA);
      pcommand->SetDecoration(tool_SPHERICAL_CAMERA);
      pcommand->SetLocation("ID_SPHERICAL_CAM_MENUITEM");
   } else if (CommandName == "EyeCamera") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetGroupId(-128);
      pcommand->SetDescription(tooltip_EYE_CAMERA);
      pcommand->SetDecoration(tool_EYE_CAMERA);
      pcommand->SetLocation("ID_EYE_CAM_MENUTIEM");
   }
#ifdef __ENABLE_WORLDWATCHER_TOOL__
   else if (CommandName == "WorldWatcher") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(tooltip_PANORAMA_WINDOW);
      pcommand->SetDecoration("");
      pcommand->SetLocation("ID_CONATINERMID");
   }
#endif
   return pcommand;
}

/**
 * Destruye un comando
 */
void NavigationTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Crea un wodget
 */
Widget* NavigationTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /* namespace suri */
