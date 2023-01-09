/*! \file AspectPreservingWorld.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
