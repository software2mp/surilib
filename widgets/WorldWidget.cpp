/*! \file WorldWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
