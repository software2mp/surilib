/*! \file RenderPipeline.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RENDERPIPELINE_H_
#define RENDERPIPELINE_H_

// Includes standard
#include <cstddef>
#include <vector>

/** namespace suri */
namespace suri {
class Element;
class World;
class Canvas;
class Mask;
class Renderer;

/** clase que contiene una cadena de Renderers */
/**
 *  Esta clase tiene una cadena de instancias de clases derivadas de Render
 * y que ejecuta en forma secuencial pasando el resultado de la anterior a la
 * siguiente.
 *  Es la responsable de crear los renderizadores basado en los nodos bajo
 * renderizacion a fin de obtener un resultado presentable por pantalla,
 * memoria o archivo.
 */
class RenderPipeline {
// -------------------- PRIVADOS POR USAR CREATE/DESTROY --------------------
   /** ctor */
   RenderPipeline();
   /** ctor de copia */
   RenderPipeline(const RenderPipeline&);
   /** dtor */
   virtual ~RenderPipeline();
public:
// --------------------- ESTATICOS CREACION/DESTRUCCION ---------------------
   /** Carga los renderers que figuran en el xml en un vector. */
   static RenderPipeline *Create(Element *pElement, Canvas *pOutputCanvas = NULL);
   /** Elimina el pipeline y deja pPipeline en NULL */
   static void Destroy(RenderPipeline* &pPipeline);
// ----------------------------- RENDERIZACION ------------------------------
   /** Renderiza los elementos del mundo en el canvas */
   virtual bool Render(const World *pWorld, Canvas* pCanvas, Mask* pMask);
   /** Obtiene el "bounding box" del elemento renderizado */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry) const;
// ----------------------------- ACTUALIZACION ------------------------------
   /** Metodo para actualizacion de elemento */
   virtual void Update(Element *pElement);
protected:
private:
   typedef std::vector<Renderer*> PipelineType; /*! vector con los Renderers */
   PipelineType renderers_; /*! mapa de renderizadores */
};
}

#endif /*RENDERPIPELINE_H_*/
