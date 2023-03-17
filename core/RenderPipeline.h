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
