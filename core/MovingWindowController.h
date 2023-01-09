/*! \file MovingWindowController.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MOVINGWINDOWCONTROLLER_H_
#define MOVINGWINDOWCONTROLLER_H_

// Includes estandar

// Includes Suri
#include "suri/RenderizationController.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Controlador de renderizacion que barre con la ventana por todo el extent */
/**
 * Hereda de RenderizationControler, para renderzar recorre la imagen usando
 * ventanas de tamanio adecuado para no sobrecargar la maquina. Antes de
 * renderizar se debe configurar el tamanio deseado o se puede llamar a
 * SetBestBufferSize para hacerlo automaticamente.
 */
class MovingWindowController : public RenderizationController {
public:
   /** ctor */
   MovingWindowController();
   /** dtor */
   virtual ~MovingWindowController();
   /** Determina cual es la siguiente ventana a renderizar */
   virtual bool GetNext(Subset &NextWindow);
   /** Configura la ventana del mundo para usar un viewport (buffer) dado */
   virtual void SetBufferSize(int Pixels, int Lines);
   /** Configura la ventana del mundo con el mejor buffer */
   virtual void SetBestBufferSize();
protected:
   /** Configura el mundo */
   virtual void DoSetWorld();
   /** modifica el mundo de manera de recorrer el extent completo y asi renderizar */
   virtual bool DoRender();
private:
   int viewportWidth_; /*! Ancho del viewport inicial */
   int viewportHeight_; /*! Alto del viewport inicial */
   Subset initialWindow_; /*! Ventana inicial */
};
}

#endif /* MOVINGWINDOWCONTROLLER_H_ */
