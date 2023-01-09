/*! \file Renderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
