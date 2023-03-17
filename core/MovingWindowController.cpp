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

// Includes estandar

// Includes Suri
#include "MovingWindowController.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/World.h"
#include "suri/Dimension.h"
#include "suri/Progress.h"

// Includes Wx

// Includes App

// Defines

/** Cantidad de lineas de la imagen que utiliza como buffer de lectura-escritura. */
#define MAX_BUFFER_LINES_SIZE 500
/** Cantidad de columnas de la imagen que utiliza como buffer de lectura-escritura. */
#define MAX_BUFFER_PIXELS_SIZE 10000000

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
MovingWindowController::MovingWindowController() :
      viewportWidth_(0), viewportHeight_(0), initialWindow_(Subset()) {
}

/**
 * Destructor
 */
MovingWindowController::~MovingWindowController() {
}

/** Determina cual es la siguiente ventana a renderizar */
/**
 * Toma la ventana actual y la desplaza de izquierda a derecha y arriba a abajo
 * ajustandola (y al viewport para mantener el tamano de pixel) en caso de ser
 * necesario para no excederse de los limites del extent.
 *
 * \pre La ventana del mundo debe representar un area dentro del extent.
 * @param[out] NextWindow Es el valor de la ventana desplazada.
 * @param[in] NextWindow Si viene en estado de recien creada ( == Subset()),
 *            considera que se trata de la primer iteracion y retorna la ventana
 *            actual del mundo sin modificar.
 * @return True En caso de no haber terminado de barrer todo el extent
 * @return False Cuando se recorrio todo el extent.
 */
bool MovingWindowController::GetNext(Subset &NextWindow) {
   // al ser la primera vez
   if (NextWindow == Subset()) {
      pWorld_->GetWindow(initialWindow_);
      NextWindow = initialWindow_;
      pWorld_->GetViewport(viewportWidth_, viewportHeight_);
      return true;
   }
   Subset window, extent;
   pWorld_->GetWindow(window);
   pWorld_->GetWorld(extent);
   Dimension windowdimension(window);
   // Tamano actual del viewport
   int vpw = 0, vph = 0;
   pWorld_->GetViewport(vpw, vph);
   // muevo a la izquierda
   NextWindow.ul_.x_ = window.lr_.x_;
   // si me pase en X del extent, vuelvo atras e incremento en Y
   if (windowdimension.XSign() * NextWindow.ul_.x_ + EPSILON_REL
         >= windowdimension.XSign() * extent.lr_.x_) {
      NextWindow.ul_.x_ = extent.ul_.x_;
      NextWindow.ul_.y_ = window.lr_.y_;
      // restoreo el tamano del viewport por si cambio en la iteracion anterior
      vpw = viewportWidth_;
      // si tambien me paso en Y, finalice
      if (windowdimension.YSign() * NextWindow.ul_.y_ + EPSILON_REL
            >= windowdimension.YSign() * extent.lr_.y_) {
         NextWindow = Subset();
         // reseteo el tamano del viewport
         pWorld_->BlockViewerUpdate();
         pWorld_->SetViewport(viewportWidth_, viewportHeight_);
         pWorld_->SetWindow(initialWindow_);
         pWorld_->UnblockViewerUpdate();
         return false;
      }
      // recalculo el tamano de la ventana
      Coordinates temp;
      pWorld_->Transform(Coordinates(vpw, vph), temp);
      windowdimension = Dimension(window.ul_, temp);
   }
   // calculo el LR en funcion del ancho de la ventana
   NextWindow.lr_.x_ = NextWindow.ul_.x_
         + windowdimension.XSign() * windowdimension.GetWidth();
   NextWindow.lr_.y_ = NextWindow.ul_.y_
         + windowdimension.YSign() * windowdimension.GetHeight();

   // Si se pasa el LR, lo achico de manera que no tome partes de afuera del extent.
   if (windowdimension.XSign() * NextWindow.lr_.x_ + EPSILON_REL
         >= windowdimension.XSign() * extent.lr_.x_) {
      NextWindow.lr_.x_ = extent.lr_.x_;
      Coordinates tempul, templr;
      pWorld_->InverseTransform(NextWindow.ul_, tempul);
      pWorld_->InverseTransform(NextWindow.lr_, templr);
      Dimension dim(Subset(tempul, templr));
      // Sumo EPSILON_REL al ancho porque al truncar se pierde un pixel. Al hacer
      // InverseTransform el ancho vale (x-1).9999999999 y trunca a x-1.
      vpw = SURI_TRUNC(int, dim.GetWidth() + EPSILON_REL);
   }
   // si tambien me paso en Y, finalice
   if (windowdimension.YSign() * NextWindow.lr_.y_ + EPSILON_REL
         >= windowdimension.YSign() * extent.lr_.y_) {
      NextWindow.lr_.y_ = extent.lr_.y_;
      Coordinates tempul, templr;
      pWorld_->InverseTransform(NextWindow.ul_, tempul);
      pWorld_->InverseTransform(NextWindow.lr_, templr);
      Dimension dim(Subset(tempul, templr));
      // Sumo EPSILON_REL al alto porque al truncar se pierde un pixel. Al hacer
      // InverseTransform el alto vale (x-1).9999999999 y trunca a x-1.
      vph = SURI_TRUNC(int, dim.GetHeight() + EPSILON_REL);
   }
   // cambio el tamano del viewport para que coincida con la ventana modificada
   pWorld_->BlockViewerUpdate();
   pWorld_->SetViewport(vpw, vph);
   pWorld_->UnblockViewerUpdate();

   assert(NextWindow != window);
   // si por alguna razon da que la nueva ventana no se modifico, salgo como
   // si hubiera terminado de recorrer
   if (NextWindow == window) {
      return false;
   }
   // para que siga iterando
   return true;
}

/** Configura la ventana del mundo para usar un viewport (buffer) dado */
/**
 * Transforma la ventana y el viewport para que se genere una lectura utilizando
 * un buffer del tamano deseado.
 * \pre Window debe ser igual al extent.
 * \pre Window.ul_ + el tamano del buffer no debe exceder el extent
 * \pre Viewport debe tener el tamano de la matriz que corresponde al de salida
 *      deseado para el extent. (es decir, el tamano de matriz de la imagen
 *      de salida)
 * \post El viewport toma el tamano del buffer y pWorld_->Window el
 *       correspondiente a dicho tamano en coordenadas de mundo.
 * \post Si Pixels > que el ancho de salida, entonces el buffer toma el ancho
 * \post Si Lines > que el alto de salida, entonces el buffer toma dicho alto
 * \post Ambas dimensiones del buffer seran menores o iguales que los define
 *       MAX_BUFFER_*_SIZE definidos arriba.
 * @param[out] Pixels Cantidad de pixels deseados en el buffer
 * @param[in] Lines Catidad de lineas deseadas en el buffer
 */
void MovingWindowController::SetBufferSize(int Pixels, int Lines) {
   if (!pWorld_ || !pWorld_->IsInitialized()) {
      REPORT_DEBUG("D: Mundo mal inicializado");
      return;
   }
   pWorld_->BlockViewerUpdate();
   int width = 0, height = 0;
   // el viewport debe traer el tamano raster de salida deseado
   pWorld_->GetViewport(width, height);
   Subset window;
   pWorld_->GetWindow(window);
   // la dimension del buffer que mejor se aproxima a lo deseado
   int bufferw = std::min(std::min(Pixels, width), MAX_BUFFER_PIXELS_SIZE), bufferh =
         std::min(std::min(Lines, height), MAX_BUFFER_LINES_SIZE);
   pWorld_->Transform(Coordinates(bufferw, bufferh), window.lr_);
   pWorld_->SetWindow(window);
   pWorld_->SetViewport(bufferw, bufferh);
   pWorld_->UnblockViewerUpdate();
}

/**
 * Utiliza los defines de maximas columnas y lineas para un buffer de ventana.
 *
 */
void MovingWindowController::SetBestBufferSize() {
   SetBufferSize(MAX_BUFFER_PIXELS_SIZE, MAX_BUFFER_LINES_SIZE);
}

/**
 * Setea el mundo que utilizara para la renderizacion
 *
 *  - World (extent) : Es la porcion de mundo que debe renderizarse
 *  - Window : Se utiliza para obtener la relacion de escala, se fuerza a que
 *            sea igual al extent del mundo.
 *  - Viewport : Tamanio de salida deseado (cantidad de pixeles que se desea
 *               que representen el extent). Representa el tamanio raster de
 *               salida.
 * Ejemplo:
 *  Se tiene una imagen para exportar, el extent del mundo (world) sera el
 * extent de la imagen. La ventana (window), coincidira con el mundo (world) y
 * Viewport tendra la dimension deseada del raster de salida.
 *
 * \pre World debe representar la porcion de mundo que se renderizara.
 * \pre Viewport debe tener el tamanio en pixeles de la imagen de salida.
 */
void MovingWindowController::DoSetWorld() {
   if (pWorld_) {
      // fuerzo que la ventana tenga el mismo tamanio que el mundo
      Subset extent;
      pWorld_->GetWorld(extent);
      pWorld_->BlockViewerUpdate();
      pWorld_->SetWindow(extent);
      pWorld_->UnblockViewerUpdate();
   }
}

/**
 *  Utiliza GetNext para obtener la siguiente ventana movil y
 * asi modificar el mundo, lo cual envia un update que realiza una
 * renderizacion.
 *
 *  GetNext funciona de manera tal que se recorre el extent completo.
 */
bool MovingWindowController::DoRender() {
   Subset next;
   Progress progression(-1, wxT(message_RENDERING_PROGRESS));
   // itera sobre el extent del mundo
   // genera Updates del sistema de progreso, permite cancelarlo
   bool terminate = false; /*! determina si se debe abortar la renderizacion */
   while (GetNext(next) && !terminate) {
      pWorld_->SetWindow(next);
      terminate = progression.Update() || !GetRenderizationStatus();
   }
   // Retorna el estado de la renderizacion
   return !terminate;
}
}
