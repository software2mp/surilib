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

#ifndef WORLDVIEW_H_
#define WORLDVIEW_H_

// Includes suri
#include "World.h"
#include "CallbackView.h"
#include "logmacros.h"

/** namespace suri */
namespace suri {
/** Template Vista para World */
/**
 * Provee la funcionalidad de Vista con llamada a callback, posee metodos
 * de consulta para determinar modificacion de World.
 *
 * \note especializa la clase CallbackView para tratar con el World
 */
template<class CallbackClass>
class WorldView : public CallbackView<CallbackClass> {
   /** Ctor. de Copia. */
   WorldView(const WorldView &WorldView);

public:
   /** rehago el typedef */
   typedef typename CallbackView<CallbackClass>::pCallbackMethod pCallbackMethod;

   /** ctor */
   WorldView(CallbackClass *pObject, pCallbackMethod pUpdateCallbackMethod,
             pCallbackMethod pDeleteCallbackMethod = NULL);
   /** dtor */
   virtual ~WorldView();
// ----------------------------- CALLBACKVIEWER -----------------------------
   /** llama al metodo callback registrado */
   virtual void Update(Model *pModel);
// ----------------------------- CONSULTA MUNDO -----------------------------
   /** Obtiene el mundo */
   World *GetWorld() const;
   /** Metodo para saber si se modifico el mundo */
   bool WorldChanged() const;
   /** Metodo para saber si se modifico la ventana */
   bool WindowChanged() const;
   /** Metodo para saber si se modifico el viewport */
   bool ViewportChanged() const;
   /** Metodo para saber si se modifico el SpatialReference */
   bool SpatialReferenceChanged() const;
protected:
   /** resetea el estado de las variables */
   void ResetStatus();
private:
   bool worldChanged_; /*! indica si cambio el mundo desde ultimo */
   /* update */
   bool windowChanged_; /*! indica si cambio la ventana desde ultimo */
   /* update */
   bool viewportChanged_; /*! indica si cambio el viewport desde ultimo */
   /* update */
   bool spatialReferenceChanged_; /*! indica si cambio la spatialReference desde */
   /* ultimo update */
   Subset worldSubset_; /*! guarda el subset del mundo al final del */
   /* update */
   Subset windowSubset_; /*! guarda el subset de la ventana al final */
   /* del update */
   int viewportH_; /*! guarda dimension X del viewport al final */
   /* del update */
   int viewportW_; /*! guarda dimension Y del viewport al final */
   /* del update */
   std::string worldWkt_; /*! guarda info del SpatialReference al final */
   /* del update */
};

/**
 * @param[in] pObject objeto que observo
 * @param[in] pUpdateCallbackMethod metodo que se llama cuando se modifica
 * el objeto
 * @param[in] pDeleteCallbackMethod metodo que se llama cuando se elimina
 * el objeto
 */
template<class CallbackClass>
WorldView<CallbackClass>::WorldView(CallbackClass *pObject,
                                    pCallbackMethod pUpdateCallbackMethod,
                                    pCallbackMethod pDeleteCallbackMethod) :
      CallbackView<CallbackClass>(pObject, pUpdateCallbackMethod,
                                  pDeleteCallbackMethod),
      worldChanged_(false), windowChanged_(false), viewportChanged_(false),
      spatialReferenceChanged_(false), worldSubset_(Subset()), windowSubset_(Subset()),
      viewportH_(-1), viewportW_(-1), worldWkt_("") {
}

/** dtor */
template<class CallbackClass>
WorldView<CallbackClass>::~WorldView() {
}

// ----------------------------- CALLBACKVIEWER -----------------------------
/**
 * llama al metodo callback registrado
 * @param[in] pModel modelo del objeto modificado
 */
template<class CallbackClass>
void WorldView<CallbackClass>::Update(Model *pModel) {
   World *pworld = GetWorld();
   /** quiere decir que recibe un update de algo que no conoce */
   if (pworld != pModel) {
      return;
   }
   Subset world, window;
   pworld->GetWorld(world);
   pworld->GetWindow(window);
   if (world.ul_ != worldSubset_.ul_ || world.lr_ != worldSubset_.lr_) {
      worldChanged_ = true;
   }
   if (window.ul_ != windowSubset_.ul_ || window.lr_ != windowSubset_.lr_) {
      windowChanged_ = true;
   }
   if (worldWkt_ != pworld->GetSpatialReference()) {
      spatialReferenceChanged_ = true;
   }
   int vpw, vph;
   pworld->GetViewport(vpw, vph);
   if (viewportW_ != vpw || viewportH_ != vph) {
      viewportChanged_ = true;
   }
   CallbackView<CallbackClass>::Update(pModel);
   ResetStatus();
}

// ----------------------------- CONSULTA MUNDO -----------------------------
/** Obtiene el mundo */
/**
 * @return mundo que observa
 */
template<class CallbackClass>
World* WorldView<CallbackClass>::GetWorld() const {
   World *pworld = View::GetModel<World>();
   if (!pworld) {
      REPORT_AND_FAIL_VALUE("D:Error al obtener el mundo.", NULL);
   }
   return pworld;
}

/** Metodo para saber si se modifico el mundo */
/**
 * @return indica si el mundo fue modificado
 */
template<class CallbackClass>
bool WorldView<CallbackClass>::WorldChanged() const {
   return worldChanged_;
}

/** Metodo para saber si se modifico la ventana */
/**
 * @return indica si la ventana fue modificada
 */
template<class CallbackClass>
bool WorldView<CallbackClass>::WindowChanged() const {
   return windowChanged_;
}

/** Metodo para saber si se modifico el viewport */
/**
 * @return indica si el viewport fue modificado
 */
template<class CallbackClass>
bool WorldView<CallbackClass>::ViewportChanged() const {
   return viewportChanged_;
}

/** Metodo para saber si se modifico el viewport */
/**
 * @return indica si se modifico la referencia espacial
 */
template<class CallbackClass>
bool WorldView<CallbackClass>::SpatialReferenceChanged() const {
   return spatialReferenceChanged_;
}

/** resetea el estado de las variables */
template<class CallbackClass>
void WorldView<CallbackClass>::ResetStatus() {
   worldChanged_ = false;
   windowChanged_ = false;
   viewportChanged_ = false;
   spatialReferenceChanged_ = false;
   World *pworld = GetWorld();
   if (!pworld) {
      return;
   }
   pworld->GetWorld(worldSubset_);
   pworld->GetWindow(windowSubset_);
   pworld->GetViewport(viewportW_, viewportH_);
   worldWkt_ = pworld->GetSpatialReference();
}
}

#endif /*WORLDVIEW_H_*/
