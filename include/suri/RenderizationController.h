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
