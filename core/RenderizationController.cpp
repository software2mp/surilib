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

// Includes estandar

// Includes Suri
#include "suri/RenderizationController.h"
#ifdef __DEBUG__
#  include "WorldWidget.h"
#endif
#include "RenderizationManager.h"
#include "Mask.h"
#include "suri/Progress.h"
#include "suri/messages.h"
#include "suri/Element.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

/** ctor */
RenderizationController::RenderizationController() :
      pRenderizationList_(NULL), pMaskList_(NULL), pWorld_(NULL), pOutputCanvas_(NULL),
      pRenderizationManager_(NULL), pMaskRenderization_(NULL) {
   Finalize();
}

/** dtor */
RenderizationController::~RenderizationController() {
   Finalize();
}

/**
 * Renderiza el extent del mundo
 * @return  bool que indica si tuvo exito la renderizacion, false si el mundo no
 *          esta inicializado o si falla la renderizacion.
 */
bool RenderizationController::Render() {
   // si el mundo no esta inicializado, no puede renderizar
   if (!Initialize()) {
      return false;
   }
#ifdef __GRAPHICAL_DEBUG__
   WorldWidget *processpreview = new WorldWidget(pWorld_);
#endif
   bool retval = DoRender();
#ifdef __GRAPHICAL_DEBUG__
   delete processpreview;
#endif
   if (!retval) {
      REPORT_DEBUG("D:Error en la renderizacion de los elementos.");
   }

   Finalize();
   return retval;
}

/**
 * Setea la lista con los elementos a renderizar
 * @param[in] pList lista con elementos que se quieren renderizar.
 */
void RenderizationController::SetRenderizationList(LayerList *pList) {
   if (pRenderizationList_ && Initialized()) {
      Finalize();
   }
   pRenderizationList_ = pList;
   DoSetList();
}

/**
 * Setea la lista con los elementos que, renderizados, generan la mascara
 * @param[in] pList Lista con vectores que se usan como mascara.
 * Deben tener el estilo "mask"
 */
void RenderizationController::SetMaskList(LayerList *pList) {
   // si esta inicializado, tengo que reiniciar para que la mascara este por
   // adelante de la renderizacion de la lista
   if (Initialized()) {
      Finalize();
   }
   pMaskList_ = pList;
   DoSetMaskList();
}

/**
 * Setea el mundo que utilizara para la renderizacion
 * pWorld Mundo a renderizar
 * @param[in] pWorld mundo que se quiere renderizar.
 */
void RenderizationController::SetWorld(World *pWorld) {
   if (pWorld_ && Initialized()) {
      Finalize();
   }
   pWorld_ = pWorld;
   DoSetWorld();
}

/**
 * Setea la salida de la renderizacion
 * @param[in] pCanvas canvas sobre el que se va a renderizar
 */
void RenderizationController::SetOutputCanvas(Canvas *pCanvas) {
   if (pOutputCanvas_ && Initialized()) {
      Finalize();
   }
   pOutputCanvas_ = pCanvas;
   DoSetOutputCanvas();
}

/**
 *  Realiza una renderizacion y retorna el estado.
 * @return Valor del estado de la renderizacion
 */
bool RenderizationController::DoRender() {
   pRenderizationManager_->Render();
   return pRenderizationManager_->GetRenderizationStatus();
}

/** Inicializa el controlador */
/**
 *  Crea los renderizadores los conecta a las listas, mundo, canvas
 * y mascara. Deja listo para llamar a Render().
 *
 * @return True si pudo o estaba inicializado
 * @return False Si no se encuentra inicializado
 */
bool RenderizationController::Initialize() {
   // si el mundo no esta inicializado, no puede renderizar
   if (!Initialized()) {
      return false;
   }
   // Si ya esta listo para renderizar sale
   if (pRenderizationManager_ && (!pMaskList_ || pMaskRenderization_)) {
      return true;
   }
   Mask *prenderizationmask = NULL;
   // creo el renderizador para la mascara
   // debe hacerse previo al de la renderizacion para que reciba el
   // Viewer::Update() antes.
   if (pMaskList_) {
      pMaskRenderization_ = new RenderizationManager(pMaskList_, pWorld_);
      // No quiero que modifiquen el extent del mundo
      pMaskRenderization_->SetWorldExtentManager(NULL);
      // El canvas es una mascara
      prenderizationmask = new Mask;
      pMaskRenderization_->SetCanvas(prenderizationmask);

      prenderizationmask->SetNoDataValue(pMaskList_->GetNoDataValue());
      prenderizationmask->SetNoDataValueAvailable(pMaskList_->IsNoDataValueAvailable());
      prenderizationmask->SetAllBandsNdv(pMaskList_->GetAllBandsNdv());
   }
   // creo el renderizador y le paso la lista y el mundo
   pRenderizationManager_ = new RenderizationManager(pRenderizationList_, pWorld_);
   // No quiero que me modifiquen el extent del mundo
   pRenderizationManager_->SetWorldExtentManager(NULL);
   // Seteo la salida de la renderizacion
   pRenderizationManager_->SetCanvas(pOutputCanvas_);
   // cargo la mascara
   pRenderizationManager_->SetMask(prenderizationmask);

   return true;
}

/** retorna el estado de inicializacion */
/**
 *  Si tiene lista, canvas y mundo inicializado se considera inicializado
 *
 * @return True si se pueden generar los sistemas minimos de renderizacion
 */
bool RenderizationController::Initialized() {
   return pRenderizationList_ && pOutputCanvas_ && pWorld_ && pWorld_->IsInitialized();
}

/**
 * Pasa al estado inicial.
 *
 * \post Todos los sistemas de renderizacion quedan nulos, es necesario
 *       volver a llamar a Initialize().
 * \note No limpia las listas ni el canvas, solo el sistema de renderizacion
 */
void RenderizationController::Finalize() {
   if (pRenderizationManager_) {
      // le saca el canvas al administrador de renderizacion
      pRenderizationManager_->SetCanvas(NULL);
      // le saca la mascara
      pRenderizationManager_->SetMask(NULL);
   }
   if (pMaskRenderization_) {
      // elimino la mascara
      delete pMaskRenderization_->SetCanvas(NULL);
      // elimino el renderizador de la mascara
      delete pMaskRenderization_;
      pMaskRenderization_ = NULL;
   }
   delete pRenderizationManager_;
   pRenderizationManager_ = NULL;
}

/**
 * Retorna el estado del sistema para la ultima renderizacion
 * @return informa si hubo un error en la renderizacion de los
 * elementos o las mascaras
 */
bool RenderizationController::GetRenderizationStatus() const {
   return pRenderizationManager_ && pRenderizationManager_->GetRenderizationStatus()
         && (!pMaskRenderization_ || pMaskRenderization_->GetRenderizationStatus());
}
}
