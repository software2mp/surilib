/*! \file RenderizationController.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef RENDERIZATIONCONTROLLER_H_
#define RENDERIZATIONCONTROLLER_H_

/**
 *
 */

// Includes estandar
// Includes Suri
#include "suri/Canvas.h"
#include "suri/Subset.h"
#include "suri/World.h"
#include "suri/LayerList.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
// forwards
class RenderizationManager;

/** Encapsula los pasos de configuracion necesarios para realizar una renderizacion */
/**
 * Para usar esta clase se necesitan:
 * LayerList con todos los elementos que se quieren renderizar
 * LayerList con todas las mascaras. Estas deben tener el estilo mascara.
 * World configurado con la parte del mundo que se quiere renderizar
 * Canvas donde se guardara la renderizacion
 *
 * Luego de cargar estos elementos se puede renderizar.
 * \attention: Al finalizar el uso del controlador se debe sacar los
 * elementos del mismo y eliminarlos manualmente.
 */
class RenderizationController {
public:
   /** Ctor. */
   RenderizationController();
   /** Dtor. */
   virtual ~RenderizationController();

   /** Renderiza */
   virtual bool Render();
   /** Setea la lista con los elementos a renderizar */
   void SetRenderizationList(LayerList *pList);
   /** Setea la lista con los elementos que, renderizados, generan la mascara */
   void SetMaskList(LayerList *pList);
   /** Setea el mundo a renderizar */
   void SetWorld(World *pWorld);
   /** Setea la salida de la renderizacion */
   void SetOutputCanvas(Canvas *pCanvas);
protected:
   /** Virtual para procesar la lista de renderizacion */
   virtual void DoSetList() {
   }
   /** Virtual para procesar la lista de mascara */
   virtual void DoSetMaskList() {
   }
   /** Virtual para procesar el mundo */
   virtual void DoSetWorld() {
   }
   /** Virtual para procesar el canvas */
   virtual void DoSetOutputCanvas() {
   }
   /** Virtual para renderizar */
   virtual bool DoRender();
   /** Inicializa el controlador */
   bool Initialize();
   /** retorna el estado de inicializacion */
   bool Initialized();
   /** Limpia los objetos del controlador */
   void Finalize();
   /** Retorna el estado del sistema para la ultima renderizacion */
   bool GetRenderizationStatus() const;
   LayerList *pRenderizationList_; /*! Lista de elementos */
   LayerList *pMaskList_; /*! Lista de elementos que renderizados generan */
   /* una mascara */
   World *pWorld_; /*! Mundo */
   Canvas *pOutputCanvas_; /*! Canvas de salida */
private:
   RenderizationManager *pRenderizationManager_; /*! Renderizador de los */
   /* elementos */
   RenderizationManager *pMaskRenderization_; /*! Renderizador de la mascara */
};
}

#endif /* RENDERIZATIONCONTROLLER_H_ */
