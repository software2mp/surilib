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

#ifndef CACHERENDERER_H_
#define CACHERENDERER_H_

// Includes standard
#include <map>
// Includes Suri
#include "suri/Renderer.h"
#include "suri/Subset.h"
// Includes Wx
// Defines

namespace suri {
// forwards
class MemoryCanvas;
class Mask;

/** Renderizador que cachea la renderizacion de los que tiene adelante */
class CacheRenderer : public Renderer {
   /** ctor de copia */
   CacheRenderer(const CacheRenderer&);
   /** dtor */
   virtual ~CacheRenderer();
   /** clase que maneja un canvas cacheado */
   class CanvasCache {
   public:
      /** ctor */
      CanvasCache();
      /** dtor */
      ~CanvasCache();
      MemoryCanvas *pCanvas_; /*! Canvas en memoria */
      Mask *pMask_; /*! Mascara de la renderizacion */
      Subset renderedWindow_; /*! Ultima ventana donde se renderizo el canvas */
   };
public:
   /** tipo donde guardo los caches de los elementos */
   typedef std::multimap<Element*, MemoryCanvas*> CachedElementsMap;
// ------------------------- METODOS DE BUSQUEDA ----------------------------
   /** Obtiene los caches asociados a un elemento */
   static CachedElementsMap GetCacheForElement(const Element *pElement);
   /** ctor */
   CacheRenderer();
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   virtual Renderer *Create(Element *pElement,
                            Renderer *pPreviousRenderer = NULL) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
// ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Renderiza el elemento dado un World en un Canvas */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
   /** Obtiene el "bounding box" del elemento renderizado */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);
   /** Obtiene los parametros de entrada del renderer */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;
   /** Obtiene los parametros de salida del renderer */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;
// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   /**
    *  Metodo abstracto de actualizacion del renderizador. Examina el elemento
    * y actualiza sus parametros de manera de reflejar los cambios que hayan
    * sido aplicados.
    * @param[in] pElement Elemento a analizar por cambios.
    */
   virtual void Update(Element *pElement) {
   }
   /** Indica si el renderizador tiene cambios para aplicar */
   virtual bool IsDirty();
protected:
private:
   CanvasCache cache_; /*! Elemento cacheado */
   static CachedElementsMap cachedElements_; /*! Mapeo el elemento con sus caches */
};
}  // namespace suri

#endif /* CACHERENDERER_H_ */
