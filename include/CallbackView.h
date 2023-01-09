/*! \file CallbackView.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CALLBACKVIEW_H_
#define CALLBACKVIEW_H_

// Includes suri
#include "View.h"

// ------------------------------------------------------------------------------
// CALLBACK VIEWER
// ------------------------------------------------------------------------------

/** View que implementa un callback a un metodo de la clase template */
/**
 *  Esta clase hereda de viewer e implementa una conexion al Update() que llama
 * a un metodo de una instancia de la clase del template.
 *
 *  Se utiliza para crear propiedades de una clase que sean Viewers de modelos
 * distintos y llamen asi a metodos de la clase que los contiene.
 *
 * @see suri::RenderizationManager::pListViewer_
 * @see suri::RenderizationManager::pWorldViewer_
 * \note especializa view para llamar a una funcion de Callback en caso de un
 *       update
 */
template<class CallbackClass>
class CallbackView : public View {
   /** Ctor. de Copia. */
   CallbackView(const CallbackView &CallbackView);

public:
   /** tipo para el metodo de callback (es metodo de CallbackClass y */
   typedef void (CallbackClass::*pCallbackMethod)(Model*);
// ------------------------------------------------------------------------------

   /** ctor */
   CallbackView(CallbackClass *pObject, pCallbackMethod pUpdateCallbackMethod,
                pCallbackMethod pDeleteCallbackMethod = NULL) :
         pCallbackObject_(pObject), pUpdateCallbackMethod_(pUpdateCallbackMethod),
         pDeleteCallbackMethod_(pDeleteCallbackMethod) {
   }
// ------------------------------------------------------------------------------

   /** dtor */
// TIENE SENTIDO DADO QUE UN VIEWER PUEDE VER MAS DE UN MODEL????
   virtual ~CallbackView() {
      if (GetModel<Model>()) {
         GetModel<Model>()->UnregisterViewer(this);
      }
   }
// ------------------------------------------------------------------------------

   /** llama al metodo callback registrado para el update del modelo */
   virtual void Update(Model *pModel) {
      if (Model::IsValid(pModel) && pCallbackObject_ && pUpdateCallbackMethod_) {
         (pCallbackObject_->*pUpdateCallbackMethod_)(pModel);
      }
   }
// ------------------------------------------------------------------------------

   /** llama al metodo callback registrado para el delete del modelo */
   virtual void OnModelDelete(Model *pModel) {
      if (Model::IsValid(pModel) && pCallbackObject_ && pDeleteCallbackMethod_) {
         pModel->UnregisterViewer(this);
         (pCallbackObject_->*pDeleteCallbackMethod_)(pModel);
      }
   }
// ------------------------------------------------------------------------------

protected:

private:
   CallbackClass *pCallbackObject_; /*! Instancia del objeto que ejecuta los */
   /* callbacks */
   pCallbackMethod pUpdateCallbackMethod_; /*! Metodo de pObject_ a ejecutar */
   /* en Update() */
   pCallbackMethod pDeleteCallbackMethod_; /*! Metodo de pObject_ a ejecutar */
   /* en OnModelDelete() */
};

#endif /*CALLBACKVIEW_H_*/
