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
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

// Includes suri
#include "GoButton.h"
#include "suri/messages.h"
#include "suri/ViewerWidget.h"
#include "resources.h"
#include "suri/RasterElement.h"
#include "SpatialReference.h"
#include "suri/RasterSpatialModel.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/Configuration.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/Viewer2D.h"
#include "suri/CoordinatesNavigationInputInterface.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/wxSuriMouseEvent.h"

#include "wx/event.h"
#include "wx/app.h"

namespace {

/**
 * Funcion que redondea un numero.
 *
 * @param[in] Number Numero a redondear.
 * @param[in] Precision Precision en decimales.
 * @return Numero redondeado (long)
 * \todo (danieltesta-#3719): verificar si se puede mejorar el procedimiento
 */
long Round(double Number, int Precision) {
   double auxnumber = 0;

   std::stringstream ss;
   ss << std::setprecision(Precision) << std::fixed << Number;
   ss >> auxnumber;

   double diff = (auxnumber * std::pow(10.0, Precision)
         - static_cast<int>(auxnumber) * std::pow(10.0, Precision))
         / std::pow(10.0, Precision);

   if (diff >= 0.5) return std::ceil(auxnumber);

   return std::floor(auxnumber);
}
}  // anonymous namespace

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pViewer puntero al viewer
 */
GoButton::GoButton(wxWindow *pToolbar, ViewerWidget* pViewer, Element* pElement,
                   ViewportManagerInterface* pViewportManager,
                   ViewcontextManagerInterface* pViewcontextManager, Navigator* pNavigator) :
      Button(pToolbar, wxT(button_GO), wxT(tooltip_GoButton), WINDOW_PAINT),
      pViewer_(pViewer), buttonClick_(false), startX_(0), startY_(0),
      pElement_(pElement), pViewportManager_(pViewportManager),
      pViewcontextManager_(pViewcontextManager), pNavigator_(pNavigator) {
   if (!pViewer_ || !pViewer_->GetNavigator()) {
      SetEnabled(false);
   }
}

/**
 * Destructor
 */
GoButton::~GoButton() {
}


ViewerWidget* GoButton::GetActiveViewer() {
   ViewerWidget* pviewer = dynamic_cast<Viewer2D*>(pViewportManager_->GetSelectedViewport());
   DynamicViewerWidget* pdynamic =
         dynamic_cast<DynamicViewerWidget*>(pViewportManager_->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = pdynamic->GetViewer();
   return pviewer;
}



/**
 * Obtiene coordenadas del NavigationWidget y se las da al Navegador
 * @param[in] Event evento click sobre el boton
 */
void GoButton::OnButtonClicked(wxCommandEvent &Event) {
   ViewerWidget* pviewer = GetActiveViewer();
   if (pviewer == NULL) return;

   Navigator* pnavigator = GetNavigator();
   if (pnavigator == NULL) return;

   NavigationWidget* pnavwidget = pnavigator->GetWidget();
   RasterElement* praster = dynamic_cast<RasterElement*>(GetElement());
   if (pnavwidget != NULL && praster != NULL) {
      Coordinates  point;
      pnavwidget->GetCoordinate(point);

      std::string navsrs = pnavwidget->GetInputWidget()->GetSpatialReference();
      std::string rastersrs = praster->GetSpatialReference().c_str();

      if (SpatialReference::IsPixelLineSpatialRef(navsrs)) {
         RasterSpatialModel* prsm = RasterSpatialModel::Create(praster->GetRasterModel());
         prsm->Transform(point);
         RasterSpatialModel::Destroy(prsm);
      }

      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn, navsrs);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut, rastersrs);
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);

      if (!pct || !pct->Transform(point)) {
         REPORT_DEBUG("D:Coordenadas incorrectas");
         SendMouseClick(point, false);
      } else {
         CoordinatesTransformation *pnct = pnavigator->GetTransformation();
         Coordinates clickpoint(point);
         pnct->Transform(clickpoint);
         SendMouseClick(clickpoint);
         GetNavigator()->ShowPoint(point);
      }

      delete pct;
   }
}

/**
 * Metodo Update que se ejecuta cuando pWorldWindow es modificado. Es el
 * callback que se registra en pWorldViewer_.
 * Dibuja cruz en viewer
 * @param[in] Event evento
 */
void GoButton::DoOnPaint(wxPaintEvent &Event) {
   ViewerWidget* pviewer = GetActiveViewer();

   if (!pviewer || !GetNavigator()) {
      SetEnabled(false);
   }

   Coordinates coord(0, 0, 0);
   // Veo que las coordenadas del navegador sean validas y se muestren
   if ((buttonClick_) && (GetNavigator()->GetLastPoint(coord))
         && (GetNavigator()->IsShow(coord))) {
      if (pviewer->GetWorld() != NULL) {
         Subset currentwindow;
         pviewer->GetWorld()->GetWindow(currentwindow);
         if (lastWindow_.lr_ != currentwindow.lr_
               || lastWindow_.ul_ != currentwindow.ul_) {
            lastWindow_ = currentwindow;
            startX_ = pviewer->GetWindow()->GetScrollPos(wxHORIZONTAL);
            startY_ = pviewer->GetWindow()->GetScrollPos(wxVERTICAL);
         }

         if (startX_ != pviewer->GetWindow()->GetScrollPos(wxHORIZONTAL)
               || startY_ != pviewer->GetWindow()->GetScrollPos(wxVERTICAL)) {
            return;
         }
      }

      // Si se pudo transformar las coordenadas lat long a coord de mundo dibujo cruz
      if (GetNavigator()->GetLastPointInWorldCoordinates(coord)) {
         DrawCrossWithInvertedColor(coord);
      }
   }
}

/**
 * Dibuja cruz en coordenadas que recive.
 * @param[in] Point coordenadas en sistema de referencia de mundo donde se
 * dibuja la cruz
 */
void GoButton::DrawCrossWithInvertedColor(const Coordinates &Point) {
   ViewerWidget* pviewer = GetActiveViewer();
   if (!pviewer) {
      return;
   }
   Coordinates pixel;
   // Obtengo coordenadas en pixel-linea
   pviewer->GetWorld()->InverseTransform(Point, pixel);
   // Dibujo lineas
   wxClientDC wxdc(pviewer->GetWindow());
   wxdc.SetLogicalFunction(wxINVERT);
   wxdc.SetBrush(wxBrush("red", wxTRANSPARENT));
   wxdc.CrossHair(static_cast<int>(pixel.x_), static_cast<int>(pixel.y_));
}

/**
 * Envia eventos de simulacion de un click con el mouse.
 *
 * \todo (danieltesta-#3719): verificar la funcion Round y ver si
 * se puede conseguir una mejor solucion.
 */
void GoButton::SendMouseClick(const Coordinates& LatLong, bool Transform) {
   ViewerWidget* pviewer = GetActiveViewer();

   Coordinates point(LatLong);

   if (Transform) {
      CoordinatesTransformation* pct = pviewer->GetTransformation();
      if (pct) pct->Transform(point, true);
   }

   long x = Round(point.x_, 1);
   long y = Round(point.y_, 1);

   Configuration::SetParameterEx(VETYPE_PARAM_NAME, VETYPE_PARAM_SIMULATED);

   wxMouseEvent mdldlick(wxEVT_LEFT_DOWN);
   mdldlick.m_x = x;
   mdldlick.m_y = y;
   mdldlick.SetEventObject(pviewer->GetWindow());
   pviewer->GetWindow()->GetEventHandler()->AddPendingEvent(mdldlick);
   std::string vetypemode = Configuration::GetParameterEx(VETYPE_PARAM_MODE_NAME,
                                                          VETYPE_PARAM_MODE_UNDEFINED);
   if (vetypemode.compare(VETYPE_PARAM_MODE_EDIT) == 0) {
      wxMouseEvent mmclick(wxEVT_MOTION);
      mmclick.m_x = x;
      mmclick.m_y = y;
      mmclick.m_leftDown = true;
      mmclick.SetEventObject(pviewer->GetWindow());
      pviewer->GetWindow()->GetEventHandler()->AddPendingEvent(mmclick);
   }

   wxSuriMouseEvent mluclick(wxEVT_LEFT_UP);
   mluclick.m_x = x;
   mluclick.m_y = y;
   mluclick.SetPosition(point.x_, point.y_);
   mluclick.SetEventObject(pviewer->GetWindow());
   pviewer->GetWindow()->GetEventHandler()->AddPendingEvent(mluclick);
}

#ifdef __UNUSED_CODE__
/**
 * Devuelve el visualizador que debe ser utilizado.
 * \deprecated: Utilizar GetActiveViewer. La herramienta de Navegacion se extiende a V3D.
 */
Viewer2D* GoButton::GetViewer2D() {
   Viewer2D* pviewer = dynamic_cast<Viewer2D*>(pViewportManager_->GetSelectedViewport());
   DynamicViewerWidget* pdynamic =
         dynamic_cast<DynamicViewerWidget*>(pViewportManager_->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   return pviewer;
}
#endif

/**
 * Devuelve el elemento que debe ser utilizado.
 */
RasterElement* GoButton::GetElement() {
   bool found = false;

   RasterElement* pactelement = NULL;
   ViewerWidget* pviewer = GetActiveViewer();
   ViewcontextInterface* pviewcontext = pViewcontextManager_->GetViewcontext(
         pviewer->GetViewcontextUuid());
   if (pviewcontext) {
      LayerList* plist = pviewcontext->GetLayerList();
      for (int ix = 0, lenix = plist->GetElementCount(); ix < lenix && !found; ++ix) {
         pactelement = dynamic_cast<RasterElement*>(plist->GetElement(ix));
         if (pactelement && pactelement->IsActive())
            found = true;
         else
            pactelement = NULL;
      }
   } else {
      pactelement = dynamic_cast<RasterElement*>(pElement_);
   }

   // Esto es un workaround para el modo GIS
   if (pactelement && pactelement->HasAssociatedElement())
      pactelement = dynamic_cast<RasterElement*>(pactelement->GetAssociatedElement());
   /////////////////////////////////////////

   return pactelement;
}

/**
 * Retorna navegador
 * @return puntero a Navigator
 * @return NULL si no existe
 */
Navigator* GoButton::GetNavigator() {
   return pNavigator_ ? pNavigator_ : pViewer_->GetNavigator();
}

}  // namespace suri
