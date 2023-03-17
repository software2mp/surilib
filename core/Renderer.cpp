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

#include "suri/Renderer.h"

// Include suri
#include "suri/messages.h"
#include "suri/Element.h"
#include "suri/xmlnames.h"
#include "logmacros.h"

// Defines

/** namespace suri */
namespace suri {
/** mapa estatico para los renderizadores */
Renderer::RendererMapType *Renderer::pRenderers_ = NULL;

/** ctor */
Renderer::Renderer() : pPreviousRenderer_(NULL) {
}

/** dtor */
Renderer::~Renderer() {
}

/**
 *  Busca en la lista de Renderers el que se registro con el nombre
 * NodeName y utiliza el metodo virtual Create para crear un Renderer
 * derivado capaz de manejar el nodo.
 * @param[in] NodeName Nombre del nodo para el que se desea crear un renderizador.
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior, puede ser NULL.
 * @return Instancia del renderizador que maneja el NodeName.
 */
Renderer *Renderer::Create(const std::string &NodeName, Element *pElement,
                           Renderer *pPreviousRenderer) {
   std::pair<RendererMapType::iterator, RendererMapType::iterator> itpair;
   itpair = pRenderers_->equal_range(NodeName);
   // para la primera entrada
   Renderer *prenderer = NULL;
   while (itpair.first != itpair.second && prenderer == NULL) {
      prenderer = itpair.first->second->Create(pElement, pPreviousRenderer);
      itpair.first++;
   }
   if (!prenderer) {
      REPORT_AND_FAIL_VALUE("D:No se encontro el renderizador para \"%s\".", NULL,
                            NodeName.c_str());
   }
   // Guardo el renderizador anterior
   prenderer->pPreviousRenderer_ = pPreviousRenderer;
   return prenderer;
}

/**
 *  Agrega una instancia de renderizador capaz de manejar un nodo.
 * @param[in] pRenderer Instancia de renderizador del cual se utilizara el
 *            metodo virtual Create para obtener uno nuevo.
 */
void Renderer::Register(Renderer *pRenderer) {
   assert(pRenderer);
   if (!pRenderers_) {
      pRenderers_ = new RendererMapType;
   }
   pRenderers_->insert(std::make_pair(pRenderer->CreatedNode(), pRenderer));
   REPORT_DEBUG("D:Renderizador \"%s\" registrado con exito.",
                pRenderer->CreatedNode().c_str());
}

/**
 *  Elimina un renderizador de la lista.
 * @param[in] RendererKey Nombre del nodo del renderizador. Se usa para
 * identificar al renderizador.
 */
void Renderer::Unregister(const std::string &RendererKey) {
   if (!pRenderers_) {
      return;
   }

   // Elimino los elementos de memoria
   RendererMapType::iterator it = pRenderers_->find(RendererKey);
   while (it != pRenderers_->end()) {
      delete it->second;
      pRenderers_->erase(it);
      REPORT_DEBUG("D:Renderizador \"%s\" eliminado con exito.", RendererKey.c_str());
      it = pRenderers_->find(RendererKey);
   }
}

/**
 *  Destruye en forma interna al renderizador.
 * \post pRenderer sera NULL.
 * @param[in] pRenderer Renderizador a destruir (equivalente delete)
 */
void Renderer::Destroy(Renderer* &pRenderer) {
   delete pRenderer;
   pRenderer = NULL;
}

/**
 * Busca el nodo de renderizacion dentro del elemento
 * @param[in] pElement elemento del que se necesita el nodo de
 * renderizacion
 * @return el nodo de renderizacion o NULL si no lo encontro
 * \attention el nodo* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
wxXmlNode *Renderer::GetRenderizationNode(const Element *pElement) {
   if (!pElement) {
      REPORT_AND_FAIL_VALUE("Elemento invalido", NULL);
   }

   wxXmlNode *pnode = pElement->GetNode(wxT(""));
   if (!pnode) {
      REPORT_ERROR("D:Nodo raiz Nulo");
      return NULL;
   }

   pnode = pnode->GetChildren();
   while (pnode) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         return pnode;
      }
      pnode = pnode->GetNext();
   }
   return NULL;
}

/**
 * Indica si el Renderizador tiene cambios para aplicar en una renderizacion
 * posterior. Si hay sistema de Cache, esto indica que se deben actualizar los
 * datos de los renderizadores posteriores.
 * @return true Cuando el renderizador generara un set de datos distinto al de
 *              la utlima llamada.
 * @return false Cuando el renderizador generara un set de datos igual al de la
 *              ultima llamada.
 */
bool Renderer::IsDirty() {
   return pPreviousRenderer_ ? pPreviousRenderer_->IsDirty() : false;
}
}
