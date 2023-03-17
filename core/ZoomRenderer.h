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

#ifndef ZOOMRENDERER_H_
#define ZOOMRENDERER_H_

// Includes suri
#include "suri/Renderer.h"

/** Forwards */
class wxXmlNode;

/** namespace suri */
namespace suri {
/** Renderer que realiza interpolacion NN (Nearest Neighbour) */
/**
 * Usa interpolacion NN (Nearest Neighbour) para generar pixeles intermedios
 * cuando el nroPixeles en el viewport es mayor que en canvas
 * \note hereda de renderer para formar parte del pipeline de renderizacion
 */
class ZoomRenderer : public Renderer {
   /** Ctor. de Copia. */
   ZoomRenderer(const ZoomRenderer &ZoomRenderer);

public:
   /** ctor */
   ZoomRenderer();
   /** dtor */
   virtual ~ZoomRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de zoom */
   class Parameters {
   public:
      /** enum con tipos de interpolacion */
      typedef enum {
         NearestNeighbour, CubicCombolution, None
      } InterpolationType;
      /** interfaz de las funciones de interpolacion */
      typedef void (*InterpolationFunctionType)(void*, void*, size_t, double, double);
      InterpolationType interpolation_; /*! Tipo de interpolacion que se usa */
      /* cuando hay mas pixeles de salida que */
      /* de entrada */
      InterpolationFunctionType function_; /*! Funcion que aplica zoom. */
      std::string spatialModel_; /*! Referencia espacial del raster */
      std::string rasterModel_; /*! Relacion entre pixel-linea y la ref */
      /* espacial del raster */
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual ZoomRenderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
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
   virtual void Update(Element *pElement);
protected:
private:
   Parameters parameters_; /*! Parametros de la instancia */
};
}

#endif /*ZOOMRENDERER_H_*/
