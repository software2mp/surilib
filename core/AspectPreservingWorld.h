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

#ifndef ASPECTPRESERVINGWORLD_H_
#define ASPECTPRESERVINGWORLD_H_

// Includes suri
#include "suri/World.h"

/** namespace suri */
namespace suri {
/** World que evita deformacion de aspecto en transformacion de window a viewport */
/**
 * Modifica la ventana para conservar la realcion entre los lados(x/y)
 * del window y del viewport.
 * \note agrega funcionalidad a World para conservar el aspecto de la imagen
 */
class AspectPreservingWorld : public World {
   /** Ctor. de Copia. */
   AspectPreservingWorld(const AspectPreservingWorld &);

public:
   /** ctor */
   AspectPreservingWorld();
   /** dtor */
   virtual ~AspectPreservingWorld();
// --------------------- SET -----------------------
   /** Cambia las coordenadas de la ventana */
   virtual void SetWindow(double WindowUlx, double WindowUly, double WindowLrx,
                          double WindowLry);
//	virtual void SetWindow(const Subset &Window){World::SetWindow(Window);}
   /** Cambia las coordenadas del Viewport */
   virtual void SetViewport(int ViewportWidth, int ViewportHeight);
   /** Carga las dimensiones maximas que puede tomar el viewport */
   void SetMaxViewport(int ViewportWidth, int ViewportHeight);
protected:
   /** Arregla el tamanio de la ventana */
   void FixWindowSize(Subset &Window, double &WindowAspect, double &ViewportAspect);
   /** Arregla la posicion de la ventana */
   void FixWindowPosition(Subset &Window);
private:
   int maxViewportX_; /*! Ancho maximo de viewport */
   int maxViewportY_; /*! Alto maximo de viewport */
};
}

#endif /*ASPECTPRESERVINGWORLD_H_*/
