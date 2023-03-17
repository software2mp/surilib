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

// Includes standard
#include <utility>
#include <string>

// Includes Suri
#include "CacheRenderer.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/World.h"
#include "MemoryCanvas.h"
#include "Mask.h"

// Includes Wx

// Defines

/** Macro para registrar el renderer */
AUTO_REGISTER_RENDERER(suri::CacheRenderer);

namespace suri {
/** Inicializacion de la variable estatica */
CacheRenderer::CachedElementsMap CacheRenderer::cachedElements_;

// ------------------------- METODOS DE BUSQUEDA ----------------------------
/** Obtiene los caches asociados a un elemento */
CacheRenderer::CachedElementsMap CacheRenderer::GetCacheForElement(
      const Element *pElement) {
   std::pair<CachedElementsMap::iterator, CachedElementsMap::iterator> itpair;
   itpair = cachedElements_.equal_range(const_cast<Element*>(pElement));
   return CachedElementsMap(itpair.first, itpair.second);
}

CacheRenderer::CanvasCache::CanvasCache() :
      pCanvas_(NULL), pMask_(NULL) {
}

CacheRenderer::CanvasCache::~CanvasCache() {
   delete pCanvas_;
   delete pMask_;
}

CacheRenderer::CacheRenderer() {
}

CacheRenderer::~CacheRenderer() {
   // TODO: eliminar del mapa
// cachedElements_.erase(std::make_pair(pElement,prenderer->cache_.pCanvas_));
}

// -------------------------- METODO DE CREACION ----------------------------
/** Crea un renderizador a partir de un pElement */
/**
 *  Metodo virtual abstracto puro utilizado para crear con el metodo
 * estatico Create() instancias de clases derivadas registradas con el
 * metodo RegisterViewer().
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior.
 */
Renderer *CacheRenderer::Create(Element *pElement, Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }
   CacheRenderer *prenderer = new CacheRenderer;
   prenderer->cache_.pCanvas_ = new MemoryCanvas;
   prenderer->pPreviousRenderer_ = pPreviousRenderer;
   cachedElements_.insert(std::make_pair(pElement, prenderer->cache_.pCanvas_));
   return prenderer;
}

/** Nombre del renderizador == al nombre del nodo */
/**
 *  Retorna el nombre del nodo que maneja el renderizador.
 * @return Nombre del nodo manejado por la clase derivada.
 */
std::string CacheRenderer::CreatedNode() const {
   return CANVAS_CACHE_NODE;
}

// ----------------------- METODOS DE RENDERIZACION -------------------------
/** Renderiza el elemento dado un World en un Canvas */
bool CacheRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   if (!pPreviousRenderer_) {
      return false;
   }
   Subset window;
   pWorldWindow->GetWindow(window);
   bool retval = true;
   // Si el anterior no cambio y la ventana es la misma, utiliza el cache
   if (!pPreviousRenderer_->IsDirty() && window == cache_.renderedWindow_) {
      if (cache_.pMask_) {
         pMask->InitializeAs(cache_.pMask_);
         Canvas *ptemp = cache_.pMask_;
         dynamic_cast<Canvas*>(pMask)->Write(ptemp, NULL);
         cache_.pMask_ = dynamic_cast<Mask*>(ptemp);
      }
      pCanvas->SetBandCount(cache_.pCanvas_->GetBandCount(), true);
      pCanvas->InitializeAs(cache_.pCanvas_);
      if (cache_.pMask_) {
         cache_.pMask_->ApplyMask(cache_.pCanvas_, pCanvas);
      }
   } else {  // sino renderiza a travez del anterior
      /**
       * Copio los datos del pCanvas al canvas cache por si tengo que mostrar algo bajo
       * el elemento que estoy renderizando
       **/
      std::vector<void*> auxData(pCanvas->GetBandCount());
      std::vector<int> bands(pCanvas->GetBandCount());
      for (int x= 0; x < pCanvas->GetBandCount(); x++) {
         bands[x] = x;
         auxData[x] = new unsigned char[pCanvas->GetSizeX() * pCanvas->GetSizeY()
                                           * pCanvas->GetDataSize()];
      }
      cache_.pCanvas_->InitializeAs(pCanvas);
      cache_.pCanvas_->Clear();
      if(pCanvas->GetDataSize()){
         pCanvas->Read(bands, auxData);
         cache_.pCanvas_->Write(bands,auxData);
      }
      for(int i = 0; i < pCanvas->GetBandCount();i++)
         delete[] static_cast<unsigned char*> (auxData[i]);
      if (!cache_.pMask_ && pMask) {
         // si no se creo cache_.pMask
         cache_.pMask_ = new Mask;
         cache_.pMask_->InitializeAs(pMask);
         cache_.pMask_->Clear();
      }
      retval = pPreviousRenderer_->Render(pWorldWindow, cache_.pCanvas_, cache_.pMask_);
      pCanvas->InitializeAs(cache_.pCanvas_);
      if (cache_.pMask_) {  // cache_.pMask_ solo existe si existe pMask
         pMask->InitializeAs(cache_.pMask_);  // inicializo la mascara
         // copia los datos del canvas propagado al cache
         dynamic_cast<Canvas*>(pMask)->Write(cache_.pMask_, NULL);
         // aplica la mascara
         pMask->ApplyMask(cache_.pCanvas_, pCanvas);
      } else {
         // no se propago mascara, copia los datos del cache al canvas que sale
         pCanvas->Write(cache_.pCanvas_, pMask);
      }
      cache_.renderedWindow_ = window;
   }
   return retval;
}

/** Obtiene el "bounding box" del elemento renderizado */
/**
 * \pre el subset debe ser seteado antes de llamar a esta funcion con un
 *      valor invalido.
 * \post Se refleja el subset de mundo que renderiza.
 * \post No se modifican los parametros en caso de falla o subset nulo
 * @param[in] pWorld Mundo sobre el cual se debe devolver el BB
 * @param[out] Ulx ULx de mundo renderizado.
 * @param[out] Uly ULy de mundo renderizado.
 * @param[out] Lrx LRx de mundo renderizado.
 * @param[out] Lry LRy de mundo renderizado.
 */
void CacheRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                   double &Lrx, double &Lry) {
   Ulx = -1;
   Uly = -1;
   Lrx = -1;
   Lry = -1;
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetBoundingBox(pWorld, Ulx, Uly, Lrx, Lry);
   }
}

/** Obtiene los parametros de entrada del renderer */
/**
 *  Permite obtener los parametros de entrada que requiere el renderizador,
 * es decir, los datos que espera de pPreviousRenderer.
 *
 *  En caso de que no tenga algun requerimiento, el parametro tomara los
 * siguientes valores.
 * @code
 *  SizeX = -1
 *  SizeY = -1
 *  BandCount = -1
 *  DataType = DataInfo<void>::Name
 * @endcode
 *
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void CacheRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                       std::string &DataType) const {
   SizeX = -1;
   SizeY = -1;
   BandCount = -1;
   DataType = DataInfo<void>::Name;
}

/** Obtiene los parametros de salida del renderer */
/**
 *  Permite obtener los parametros de salida del renderizador.
 *
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void CacheRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                        std::string &DataType) const {
   SizeX = -1;
   SizeY = -1;
   BandCount = -1;
   DataType = DataInfo<void>::Name;
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/** Indica si el renderizador tiene cambios para aplicar */
bool CacheRenderer::IsDirty() {
   return pPreviousRenderer_ ? pPreviousRenderer_->IsDirty() : true;
}
}
