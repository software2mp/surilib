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

#ifndef GOBUTTON_H_
#define GOBUTTON_H_

#include "suri/Button.h"



namespace suri {

class ViewerWidget;
class ViewportManagerInterface;
class ViewcontextManagerInterface;
class Viewer2D;
class RasterElement;
class Navigator;

/**
 * Obtiene coordenadas del NavigationWidget y se las da al Navegador.
 *
 * Boton que obtiene las coordenadas del GoButton y dibuja una cruz en el viewer.
 * Recibe un CallBack cuando el mundo es modificado y redibuja la cruz en la
 * posicion que corresponde.
 */
class GoButton : public Button {
public:
   /** ctor */
   GoButton(wxWindow *pToolbar, ViewerWidget* pViewer, Element* pElement = NULL,
            ViewportManagerInterface* pViewportManager = NULL,
            ViewcontextManagerInterface* pViewcontextManager = NULL,
            Navigator* pNavigator = NULL);

   /** dtor */
   virtual ~GoButton();

protected:
   /** Obtiene coordenadas del NavigationWidget y se las da al Navegador */
   virtual void OnButtonClicked(wxCommandEvent &Event);

   /** Metodo de actualizacion del mundo */
   /**
    *  Metodo Update que se ejecuta cuando pWorldWindow es modificado. Es el
    * callback que se registra en pWorldViewer_.
    */
   void DoOnPaint(wxPaintEvent &Event);

   /** Dibuja cruz invirtiendo colores */
   void DrawCrossWithInvertedColor(const Coordinates &Point1);

   /** Evita que cambie el cursor al navegar */
   void DoSetCursor(bool State) {
   }
private:
   /** Retorna navegador */
   Navigator* GetNavigator();
   ViewerWidget* pViewer_; /*! Se usa para obtener el navegador y para mostrar */
   /* cruz. */
   bool buttonClick_; /*! Recuerda el primer click para no mostrar la cruz */
   /* hasta que esto ocurra */
   int startX_; /*! Guarda la posicion horizontal de scroll inicial */
   int startY_; /*! Guarda la posicion vertical de scroll inicial */
   Subset lastWindow_; /*! Guarda la ventana inicial */
   Element* pElement_; /*! Elemento activo **/
   ViewportManagerInterface* pViewportManager_;
   ViewcontextManagerInterface* pViewcontextManager_;
   Navigator *pNavigator_; /*! permite recorrer el mundo usando puntos */

   /**
    * Envia eventos de simulacion de un click con el mouse.
    */
   void SendMouseClick(const Coordinates& LatLong, bool Transform = true);
   ViewerWidget* GetActiveViewer();
#ifdef __UNUSED_CODE__
   /**
    * Devuelve el visualizador que debe ser utilizado.
    * \deprecated: Utilizar GetActiveViewer. La herramienta de Navegacion se extiende a V3D.
    */
   Viewer2D* GetViewer2D();
#endif

   /**
    * Devuelve el elemento que debe ser utilizado.
    */
   RasterElement* GetElement();
};
}

#endif /*GOBUTTON_H_*/
