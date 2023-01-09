/*! \file VectorRenderizationController.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORRENDERIZATIONCONTROLLER_H_
#define VECTORRENDERIZATIONCONTROLLER_H_

// Includes Estandar

// Includes Suri
#include "RenderPipeline.h"

// Includes Wx

// Defines

// forwards

namespace suri {

// forwards
class LayerList;
class Canvas;
class World;
class DatasourceContainerInterface;

/*!
 *  Clase que encapsula los pasos de configuracion necesarios para realizar
 *  una renderizacion.
 */
class VectorRenderizationController {
public:
   /** ctor **/
   VectorRenderizationController();
   /** dtor **/
   virtual ~VectorRenderizationController();
   /** Renderiza */
   virtual bool Render();
   /** Setea la lista con los elementos a renderizar */
   void SetRenderizationList(DatasourceContainerInterface *pList);
   /** Este metodo es un metodo dummy para respetar la interfaz de RenderizationController
    *  que se utiliza en capas Raster. **/
   void SetMaskList(LayerList *pList);
   /** Setea el mundo a renderizar */
   void SetWorld(World *pWorld);
   /** Setea la salida de la renderizacion */
   void SetOutputCanvas(Canvas *pCanvas);

protected:
   DatasourceContainerInterface *pRenderizationList_; /*! Lista de elementos */
   /*! Lista de elementos que renderizados generan una mascara */
   LayerList *pMaskList_;
   World *pWorld_; /*! Mundo */
   Canvas *pOutputCanvas_; /*! Canvas de salida */
};

} /** namespace suri */

#endif /* VECTORRENDERIZATIONCONTROLLER_H_ */
