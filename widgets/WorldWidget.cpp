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

#include "WorldWidget.h"

// Includes suri
#include "suri/Dimension.h"
#include "suri/messages.h"

/** namespace suri */
namespace suri {
#ifdef __ENABLE_WORLDWATCHER_TOOL__
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(WorldWidgetEvent, WorldWidget)IMPLEMENT_EVENT_CALLBACK(OnPaint, OnPaint(event), wxPaintEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor.
 * @param[in] pWorld View del mundo.
 * @return instancia de la clase WorldWidget
 */
WorldWidget::WorldWidget(World *pWorld) :
      Widget(NULL), NEW_EVENT_OBJECT(WorldWidgetEvent),
      pWorldViewer_(NULL),
      offsetX_(0),
      offsetY_(0),
      scaleFactor_(2.0) {
   windowTitle_ = _(caption_PANORAMA);
   CreateTool();
   pWorldViewer_ = new WorldView<WorldWidget>(this, &WorldWidget::WorldUpdate,
                                              &WorldWidget::WorldDelete);
   pWorld->RegisterViewer(pWorldViewer_);
   WorldUpdate(pWorld);
}

/** dtor */
WorldWidget::~WorldWidget() {
   delete pWorldViewer_;
}

/**
 * Crea la ventana que muestra la ventana y el mundo
 * @return true si pudo crear la ventana de la herramienta.
 * @return false si no pudo crear la ventana de la herramienta.
 */
bool WorldWidget::CreateToolWindow() {
   pToolWindow_ = new wxPanel(pParentWindow_);
   pToolWindow_->SetMinSize(wxSize(400, 400));
   CONNECT_TOOL_EVENT(WorldWidgetEvent, OnPaint, wxEVT_PAINT, wxPaintEventHandler);
   return true;
}

/** evento que dibuja un rectangulo para el mundo y otro para la ventana */
/**
 * Dibuja el rectangulo del mundo en el centro (usando scaleFactor para calcular
 * el tamano) y luego dibuja la ventana con la posicion y tamano correspondientes.
 * @param[in] Event evento que se llama al actualizar imagen en wxwindow
 * \todo Calcula mal. Cambia la relacion de aspecto segun el tamano de la ventana
 */
void WorldWidget::OnPaint(wxPaintEvent &Event) {
   World *pworld = pWorldViewer_->GetWorld();
   if (!pworld) {
      return;
   }
   Subset world;
   Subset window;
   pworld->GetWorld(world);
   Dimension dim(world);
   int x, y;
   pToolWindow_->GetClientSize(&x, &y);
   double factorx = dim.XSign() * scaleFactor_
         * std::max(dim.GetWidth(), dim.GetHeight()) / x, factory = dim.YSign()
         * scaleFactor_ * std::max(dim.GetWidth(), dim.GetHeight()) / y;
   wxPaintDC paintdc(pToolWindow_);

   paintdc.SetBrush(*wxTRANSPARENT_BRUSH);
   paintdc.SetPen(*wxRED_PEN);
   paintdc.DrawRectangle((int) ((world.ul_.x_ - offsetX_) / factorx + x / 2.0),
                         (int) ((world.ul_.y_ - offsetY_) / factory + y / 2.0),
                         (int) (dim.XSign() * dim.GetWidth() / factorx),
                         (int) (dim.YSign() * dim.GetHeight() / factory));
   pworld->GetWindow(window);
   dim = Dimension(window);
   paintdc.SetPen(*wxGREEN_PEN);
   paintdc.DrawRectangle((int) ((window.ul_.x_ - offsetX_) / factorx + x / 2.0),
                         (int) ((window.ul_.y_ - offsetY_) / factory + y / 2.0),
                         (int) (dim.XSign() * dim.GetWidth() / factorx),
                         (int) (dim.YSign() * dim.GetHeight() / factory));
}

/**
 * Actualiza las coordenadas del centro del mundo
 * @param[in] pWorld mundo que se esta desplegando
 */
void WorldWidget::WorldUpdate(Model *pWorld) {
   World *pworld = pWorldViewer_->GetWorld();
   if (!pworld) {
      return;
   }
   Subset world;
   pworld->GetWorld(world);
   offsetX_ = world.ul_.x_ + (world.lr_.x_ - world.ul_.x_) / 2.0;
   offsetY_ = world.ul_.y_ + (world.lr_.y_ - world.ul_.y_) / 2.0;
   pToolWindow_->Refresh();
}

/**
 * Se llama cuando se elimina el world
 * @param[in] pWorld mundo que se esta eliminando
 */
void WorldWidget::WorldDelete(Model *pWorld) {
   delete this;
}

// -------------------------- CLASE PARA VER EL MUNDO --------------------------
#endif   // __ENABLE_WORLDWATCHER_TOOL__
}
