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

#include "AspectPreservingWorld.h"

// Includes standard
#include <cmath>

// Includes suri
#include "suri/Dimension.h"
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {
/** ctor */
AspectPreservingWorld::AspectPreservingWorld() :
      maxViewportX_(0), maxViewportY_(0) {
}

/** dtor */
AspectPreservingWorld::~AspectPreservingWorld() {
}

/** Cambia las coordenadas de la ventana */
/**
 * Si el aspecto(x/y) de la ventana no coincide con el del viewport modifico
 * uno de los lados del window. Si es necesario muevo el centro de la nueva ventana
 * para que este dentro del mundo y luego la guardo
 *
 * @param[in] WindowUlx valor de x de la punta sup izq de la nueva ventana
 * @param[in] WindowUly valor de y de la punta sup izq de la nueva ventana
 * @param[in] WindowLrx valor de x de la punta inf der de la nueva ventana
 * @param[in] WindowLry valor de y de la punta inf der de la nueva ventana
 */
void AspectPreservingWorld::SetWindow(double WindowUlx, double WindowUly,
                                      double WindowLrx, double WindowLry) {
   Subset NewWindow(WindowUlx, WindowUly, WindowLrx, WindowLry);

   Dimension dim(NewWindow);
   // aspecto de la ventana
   double waspect = std::abs(dim.GetWidth() / dim.GetHeight());
   int x, y;
   GetViewport(x, y);
   // aspecto del viewport
   double vaspect = (double) x / y;
   // si no respeta el aspecto => lo fuerzo
   if (!FLOAT_COMPARE(waspect, vaspect)) {
      // REPORT_DEBUG("D:Corrigiendo aspecto de la ventana");
      int signx, signy;
      dim.GetSign(signx, signy);
      if (vaspect > waspect) {  // X > Y
         NewWindow.lr_.x_ = NewWindow.ul_.x_
               + signx * std::abs(dim.GetHeight()) * vaspect;
      } else {
         NewWindow.lr_.y_ = NewWindow.ul_.y_
               + signy * std::abs(dim.GetWidth()) / vaspect;
      }

      // verifico que se haya aproximado el aspecto
      dim = Dimension(NewWindow);
      // REPORT_DEBUG("D:Aspecto VP %f, Aspecto WINDOW
      // %f",vaspect,std::abs(dim.GetWidth()/dim.GetHeight()));
   }
   // Verifico que el centro de la ventana quede dentro del mundo
   Coordinates wincenter((NewWindow.ul_.x_ + NewWindow.lr_.x_) / 2,
                         (NewWindow.ul_.y_ + NewWindow.lr_.y_) / 2);
   Subset world;
   GetWorld(world);

#if defined(__FORCE_WINDOW_CENTRE__)
   double worldcentrex = (world.ul_.x_ + world.lr_.x_) / 2,
   worldcentrey = (world.ul_.y_ + world.lr_.y_) / 2;
#endif   // __FORCE_WINDOW_CENTRE__
   // La ventana se restringe de manera que el centro de la misma no pueda salir del mundo
   if (!world.IsInside(wincenter)) {
      wincenter = world.NearestPoint(wincenter);
      // REPORT_DEBUG("D:Moviendo centro de la ventana");
      dim = Dimension(NewWindow);
      NewWindow.ul_.x_ = wincenter.x_ - dim.XSign() * dim.GetWidth() / 2;
      NewWindow.lr_.x_ = wincenter.x_ + dim.XSign() * dim.GetWidth() / 2;
      NewWindow.ul_.y_ = wincenter.y_ - dim.YSign() * dim.GetHeight() / 2;
      NewWindow.lr_.y_ = wincenter.y_ + dim.YSign() * dim.GetHeight() / 2;
   }

   // Con la ventana modificada, hago la llamada correspondiente
   World::SetWindow(NewWindow.ul_.x_, NewWindow.ul_.y_, NewWindow.lr_.x_,
                    NewWindow.lr_.y_);
}

/** Cambia las coordenadas del Viewport */
/**
 * Obtiene el nuevo extremo inferior derecho de la venta con las coordenadas de viewport,
 * Actualiza el viewport y despues la ventana.
 * \post
 *       -# La punta superior izquierda de la ventana no se modifica.
 *       -# La punta inferior derecha de la ventana cambia para tener el mismo aspecto del nuevo viewport
 *
 * @param[in] ViewportWidth dimension X del viewport
 * @param[in] ViewportHeight dimension Y del viewport
 */
void AspectPreservingWorld::SetViewport(int ViewportWidth, int ViewportHeight) {
   double windowulx, windowuly, windowlrx, windowlry;
   GetWindow(windowulx, windowuly, windowlrx, windowlry);
   V2WTransform(ViewportWidth, ViewportHeight, windowlrx, windowlry);
   PreventNextViewerUpdate();
   World::SetViewport(ViewportWidth, ViewportHeight);
   SetWindow(windowulx, windowuly, windowlrx, windowlry);
}

/** Carga las dimensiones maximas que puede tomar el viewport */
void AspectPreservingWorld::SetMaxViewport(int ViewportWidth, int ViewportHeight) {
   maxViewportX_ = ViewportWidth;
   maxViewportY_ = ViewportHeight;
}

/** Arregla el tamanio de la ventana */
/**
 *  Si es posible (es decir entra en el mundo) agranda la ventana de manera que
 * se acerque lo mas posible al aspecto del viewport.
 * @param[in] Window           ventana actual
 * @param[out] Window          ventana nueva
 * @param[out] WindowAspect    x/y de la ventana nueva
 * @param[in] ViewportAspect   x/y del viewport
 * \todo No se usa
 */
void AspectPreservingWorld::FixWindowSize(Subset &Window, double &WindowAspect,
                                          double &ViewportAspect) {
   return;
   Subset world;
   GetWorld(world);
   Dimension wodim(world), widim(Window);
   // si el mundo y la ventana tienen las mismas dimensiones no hace nada
   if (wodim == widim) {
      // REPORT_DEBUG("D:Dimensiones de mundo y ventana iguales, saliendo");
      return;
   }
   // dimensiones de la ventana dadas por el aspecto del viewport
   Coordinates wingrouth;
   wingrouth.x_ = widim.GetHeight() * ViewportAspect;
   wingrouth.y_ = widim.GetWidth() / ViewportAspect;
   int signx, signy;
   widim.GetSign(signx, signy);
   // agrando la ventana en X e Y hasta el maximo del mundo
   // si ya es mas grande que el mundo la dejo como esta
   if (wingrouth.x_ > widim.GetWidth()) {
      if (Window.ul_.x_ < signx * world.ul_.x_) {
         Window.lr_.x_ = Window.ul_.x_ + wingrouth.x_;
      }
      if (Window.ul_.x_ + wingrouth.x_ < signx * world.lr_.x_) {
         Window.lr_.x_ = Window.ul_.x_ + wingrouth.x_;
      }
   }
   if (wingrouth.y_ > widim.GetHeight()) {
      if (Window.ul_.y_ < signy * world.ul_.y_) {
         Window.lr_.y_ = Window.ul_.y_ + wingrouth.y_;
      }
      if (Window.ul_.y_ + wingrouth.y_ < signy * world.lr_.y_) {
         Window.lr_.y_ = Window.ul_.y_ + wingrouth.y_;
      } else {
         Window.lr_.y_ = world.lr_.y_;
      }
   }
   widim = Dimension(Window);
   WindowAspect = std::abs(widim.GetWidth() / widim.GetHeight());
   return;

   if (!FLOAT_COMPARE(WindowAspect, ViewportAspect)) {
      if (ViewportAspect > WindowAspect) {  // X > Y
         Window.lr_.x_ = Window.ul_.x_
               + signx * std::abs(widim.GetHeight()) * ViewportAspect;
      } else {
         Window.lr_.y_ = Window.ul_.y_
               + signy * std::abs(widim.GetWidth()) / ViewportAspect;
      }
      WindowAspect = std::abs(widim.GetWidth() / widim.GetHeight());
   }

   // Verifico que el centro de la ventana quede dentro del mundo
   double wincentrex = (Window.ul_.x_ + Window.lr_.x_) / 2, wincentrey = (Window.ul_.y_
         + Window.lr_.y_) / 2;
   double minx = std::min(world.ul_.x_, world.lr_.x_), miny = std::min(world.ul_.y_,
                                                                       world.lr_.y_),
         maxx = std::max(world.ul_.x_, world.lr_.x_), maxy = std::max(world.ul_.y_,
                                                                      world.lr_.y_);
   // restrinjo la ventana de manera que el centro de la misma no pueda
   // salir del mundo
   bool centermoved = false;

   if (wincentrex < minx) {
      wincentrex = minx + widim.GetWidth() / 2;
      centermoved = true;
   }
   if (wincentrex > maxx) {
      wincentrex = maxx - widim.GetWidth() / 2;
      centermoved = true;
   }
   if (wincentrey < miny) {
      wincentrey = miny + widim.GetHeight() / 2;
      centermoved = true;
   }
   if (wincentrey > maxy) {
      wincentrey = maxy - widim.GetHeight() / 2;
      centermoved = true;
   }
   // si hubo movimiento del centro, actualizo la ventana
   if (centermoved) {
      // REPORT_DEBUG("D:Moviendo centro de la ventana");
      widim = Dimension(Window);
      Window.ul_.x_ = wincentrex - widim.XSign() * widim.GetWidth() / 2;
      Window.lr_.x_ = wincentrex + widim.XSign() * widim.GetWidth() / 2;
      Window.ul_.y_ = wincentrey - widim.YSign() * widim.GetHeight() / 2;
      Window.lr_.y_ = wincentrey + widim.YSign() * widim.GetHeight() / 2;
   }
}

/** Arregla la posicion de la ventana */
void AspectPreservingWorld::FixWindowPosition(Subset &Window) {
}
// -------------------------------- WORLD PROXY --------------------------------
}
