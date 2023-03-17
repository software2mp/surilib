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

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/VectorRenderizationController.h"
#include "suri/World.h"
#include "suri/Canvas.h"
#include "suri/LayerList.h"
#include "suri/Element.h"
#include "suri/DatasourceContainerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
VectorRenderizationController::VectorRenderizationController() :
      pRenderizationList_(NULL), pMaskList_(NULL), pWorld_(NULL),
      pOutputCanvas_(NULL) {
}

VectorRenderizationController::~VectorRenderizationController() {
}

/** Setea la lista con los elementos a renderizar */
void VectorRenderizationController::SetRenderizationList(DatasourceContainerInterface *pList) {
   // TODO(Gabriel - TCK #3623): Ver si se debe eliminar el viejo puntero
   pRenderizationList_ = pList;
}

/** Este metodo es un metodo dummy para respetar la interfaz de RenderizationController
 *  que se utiliza en capas Raster. **/
void VectorRenderizationController::SetMaskList(LayerList *pList) {
}

/** Setea el mundo a renderizar */
void VectorRenderizationController::SetWorld(World *pWorld) {
   // TODO(Gabriel - TCK #3623): Ver si se debe eliminar el viejo puntero
   pWorld_ = pWorld;
}

/** Setea la salida de la renderizacion */
void VectorRenderizationController::SetOutputCanvas(Canvas *pCanvas) {
   // TODO(Gabriel - TCK #3623): Ver si se debe eliminar el viejo puntero
   pOutputCanvas_ = pCanvas;
}

/** Renderiza */
bool VectorRenderizationController::Render() {
   // aplico el tamano antes del clear por si se necesita dentro del canvas
   if (pWorld_) {
      int vpx, vpy;
      pWorld_->GetViewport(vpx, vpy);
      pOutputCanvas_->SetSize(vpx, vpy);
   }
   pOutputCanvas_->Clear();
   REPORT_DEBUG("D:Renderizando.");
   // se recorre la lista desde el final hacia el principio
   DatasourceContainerInterface::UuidListType listids =
         pRenderizationList_->GetDatasourceList();
   DatasourceContainerInterface::UuidListType::iterator it = listids.begin();
   for (; it != listids.end() ; ++it) {
      DatasourceInterface* pds = pRenderizationList_->GetDatasource(*it);
      if (pds) {
         Element* pelement = pds->GetElement();
         REPORT_DEBUG("D:Renderizando elemento \"%s\".", pelement->GetName().c_str());
         RenderPipeline* ppipeline = RenderPipeline::Create(pelement, pOutputCanvas_);
         if (ppipeline != NULL) {
				bool status = true;
            PROFILE_CALL(status = ppipeline->Render(pWorld_, pOutputCanvas_, NULL));
				if (!status) {
					REPORT_DEBUG("D: el elemento %s no pudo ser renderizado. Url: %s", 
					pelement->GetName().c_str(), pelement->GetUrl().c_str());
					return false;
				}
         } else {
            REPORT_DEBUG("D:No se pudo obtener RenderPipeline para elemento: %s.",
                         pelement->GetName().c_str());
            return false;
         }
      }
   }
   REPORT_DEBUG("D:Fin renderizacion.");
   return true;
}
} /** namespace suri */
