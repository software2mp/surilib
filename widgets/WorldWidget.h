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

#ifndef WORLDWIDGET_H_
#define WORLDWIDGET_H_

// Include suri
#include "suri/Widget.h"
#include "suri/World.h"
#include "suri/WorldView.h"

// Includes wx
#include "wx/wx.h"

/** namespace suri */
namespace suri {
/** utilizado para tener disponible o no la herramienta que muestra la ventana dentro del mundo */
#ifdef __DEBUG__
#  define __ENABLE_WORLDWATCHER_TOOL__
#endif   // __DEBUG__
#ifdef __ENABLE_WORLDWATCHER_TOOL__

/** Declara clase WorldWidgetEvent */
DECLARE_EVENT_OBJECT(WorldWidgetEvent);

/** Widget que muestra la posicion de la ventana dentro del mundo */
/**
 * Muestra dos recuadros, uno representa al mundo(extent de la imagen)
 * y el otro a la ventana.
 * \todo Con muy poco trabajo se puede hacer una ventana "Panorama"
 */
class WorldWidget : public Widget {
   /** Ctor. de Copia. */
   WorldWidget(const WorldWidget &WorldWidget);

public:
   /** Declara friend la clase de evento WorldWidgetEvent */
   FRIEND_EVENT_OBJECT(WorldWidgetEvent);
   /** Ctor */
   WorldWidget(World *pWorld);
   /** Dtor */
   virtual ~WorldWidget();
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();

protected:
   /** evento que dibuja un rectangulo para el mundo y otro para la ventana */
   void OnPaint(wxPaintEvent &Event);
   /** Actualiza las coordenadas del centro del mundo */
   void WorldUpdate(Model *pWorld);
   /** se llama cuando se elimina el world */
   void WorldDelete(Model *pWorld);
private:
   EVENT_OBJECT_PROPERTY(WorldWidgetEvent); /*! Objeto para conectar eventos */
   WorldView<WorldWidget> *pWorldViewer_; /*! View del mundo */
   double offsetX_; /*! coord x del centro del mundo */
   double offsetY_; /*! coord y del centro del mundo */
   double scaleFactor_; /*! relacion de tamano entre viewer y mundo */
};

// -------------------------- CLASE PARA VER EL MUNDO --------------------------
#endif   // __ENABLE_WORLDWATCHER_TOOL__
}

#endif /*WORLDWIDGET_H_*/
