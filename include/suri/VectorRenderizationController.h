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
