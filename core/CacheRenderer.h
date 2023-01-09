/*! \file CacheRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
