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
