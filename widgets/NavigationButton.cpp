/*! \file NavigationButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Surilib
#include "suri/NavigationButton.h"
#include "suri/Configuration.h"
#include "CoordinatesNavigatorPart.h"
#include "CrossHairPaintEvent.h"
#include "GoButton.h"
#include "suri/Button.h"
#include "suri/Coordinates.h"
#include "suri/CoordinatesTransform.h"
#include "suri/DataViewManager.h"
#include "suri/LayerList.h"
#include "suri/messages.h"
#include "suri/NavigationWidget.h"
#include "suri/Navigator.h"
#include "suri/RasterElement.h"
#include "suri/ToolGroupManager.h"
#include "suri/ViewcontextInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/ViewerWidget.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/Widget.h"
#include "suri/Wkt.h"
#include "suri/World.h"

#include "resources.h"

namespace suri {

/**
 * Constructor
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pViewer viewer asociado
 */
NavigationButton::NavigationButton(wxWindow *pToolbar, ViewerWidget* pViewer) :
      ToggleButton(pToolbar, wxT(tool_SIMUR_NAVIGATION_WIDGET),
                   wxT(tooltip_NAVIGATION_WIDGET), BUTTON_NULL),
      pViewer_(pViewer), pGoButton_(NULL), pCrossHair_(NULL) {
}

/**
 * Destructor
 */
NavigationButton::~NavigationButton() {
}

/**
 * Setea el estado del boton. Ingresa/Saca boton de NavigationWidget.
 * @param[in] State estado en el que va a estar el boton
 * @return true siempre
 * \todo si no existe algun elemento para realizar la operacion deberia devolver
 * el resultado de ToggleButton::DoSetValue
 */
bool NavigationButton::DoSetValue(bool State) {
   // Verifico que existan todos los elementos para la operacion
   if (!pViewer_ || !pViewer_->GetNavigator()
         || !pViewer_->GetNavigator()->GetWidget()) {
      suri::ToggleButton::DoSetValue(false);
      return true;
   } else {
      suri::ToggleButton::DoSetValue(State);
   }
   Navigator *pnav = pViewer_->GetNavigator();
   if (pnav) {
      NavigationWidget *pnavwidget = pnav->GetWidget();
      if (pGoButton_) {
         // Elimina el boton anterior
         pnavwidget->RemoveButtonFromTool(pGoButton_);
         // Elimino la ventana del boton en forma segura.
         pGoButton_->GetWindow()->Destroy();
      }
      if (!State) {
         // Actualiza mundo para no mostrar la cruz
         if (pCrossHair_) {
            pViewer_->PopPaintEvent(pCrossHair_);
            delete pCrossHair_;
            pCrossHair_ = NULL;
         }
         pViewer_->GetWorld()->SetChanged();
         pViewer_->GetWorld()->SendViewerUpdate();
         pGoButton_ = NULL;
         return true;
      }

      // Navigator va a utilizar el srs geografico de mundo. La conversion
      // se realiza de WGS84 (EPSG:4326) al SRS del mundo.
      pnav->SetSpatialReference(pViewer_->GetWorld()->GetSpatialReference());

      DataViewManager* pdvmanager = pnavwidget->GetDataViewManager();

      if (pCrossHair_ == NULL) {
         pCrossHair_ = new CrossHairPaintEvent(
               pViewer_, pnavwidget->GetDataViewManager()->GetViewportManager(),
               pdvmanager->GetViewcontextManager(), pnav);
         pViewer_->PushPaintEvent(pCrossHair_);
      }

      CoordinatesNavigationInputInterface* pcoordinateinput = NULL;
      if (pnavwidget->GetInputWidget() == NULL) {
         pcoordinateinput = new CoordinatesNavigatorPart(NULL, false);
         pnavwidget->SetInputWidget(pcoordinateinput);
      } else {
         pcoordinateinput = pnavwidget->GetInputWidget();
      }
      // Fix para ticket #6288
      if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
         pcoordinateinput->EnablePixelLineNavigation(false);
      }
      pcoordinateinput->SetDataViewManager(pdvmanager);
      pcoordinateinput->SetSpatialReference(pnav->GetSpatialReference());
      bool found = false;
      Element* pactelement = NULL;
      LayerList* plist = pdvmanager->GetViewcontextManager()->GetSelectedViewcontext()->GetLayerList();
      for (int ix = 0, lenix = plist->GetElementCount(); ix < lenix && !found; ++ix) {
         pactelement = dynamic_cast<RasterElement*>(plist->GetElement(ix));
         if (pactelement && pactelement->IsActive()) found = true;
         else pactelement = NULL;
      }

      // El ingreso de coordenadas (para Seymour) siempre se hace en
      // grados, minutos y segundos. Esta la posibilidad se usar la
      // version decimal de las coordenadas. Va cableado WGS84.
      pcoordinateinput->SetSpatialReference(pViewer_->GetWorld()->GetSpatialReference());

      Coordinates windowcenter;
      if (pnav->GetCenterPoint(windowcenter)) {
         pcoordinateinput->SetCoordinate(windowcenter);
      }

      // Agrega boton a NavigatorWidget
      pGoButton_ = new GoButton(
            pnavwidget->GetToolWindow(), pViewer_, pactelement,
            pnavwidget->GetDataViewManager()->GetViewportManager(),
            pnavwidget->GetDataViewManager()->GetViewcontextManager());
      pnavwidget->AddButtonTool(pGoButton_, ToolGroupManager::First);
   }

   return true;
}

}  // namespace suri
