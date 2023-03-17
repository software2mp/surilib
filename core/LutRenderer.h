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

#ifndef LUTRENDERER_H_
#define LUTRENDERER_H_

// Includes standard
#include <list>
#include <map>

// Includes suri
#include "suri/Renderer.h"
#include "RgbColor.h"
#include "suri/LutArray.h"

// defines

/** namespace suri */
namespace suri {
class Element;

/** Clase que genera una imagen aplicando una Lut a las bandas de entrada */
/**
 * Genera una imagen aplicandole una tabla a las bandas de la entrada.
 * La imagen resultante puede tener menos bandas que la original pero
 * no pueden haber bandas sin tabla asignada.
 */
class LutRenderer : public Renderer {
   /** Ctor. de Copia. */
   LutRenderer(const LutRenderer &LutRenderer);

public:
   /** Ctor */
   LutRenderer();
   /** Dtor */
   virtual ~LutRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de LutRenderer(bandas, tipo de dato de salida, tipo de dato de entrada) */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() {
      }
      /** tipo de dato para la funcion de la lut */
      typedef void (*LutFunctionType)(void*, void*, size_t, std::map<double, double>);
      LutArray lut_; /*! Tabla con lut que uso para renderizar */
      LutFunctionType pFunction_; /*! tipo de funcion */
   };

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const LutRenderer::Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static LutRenderer::Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   virtual Renderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
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

// ----------------------- METODOS DE MODIFICACION DE LUT -------------------------

   /** Setea un nuevo Lut en un elemento raster, ya hecho el checkeo de si existe otro */
protected:

   /** Carga params desde elemento */
   static bool LoadFunction(Parameters &params, Element* pElement,
                            Renderer *pPreviousRenderer);

   /** Cambia el contenido de parametres y verifica su contenido */
   static bool ValidateTable(const Parameters &Params, Renderer *pPreviousRenderer);

   LutRenderer::Parameters parameters_; /*! Guarda informacion necesaria */
   /* para aplicar Render */
};

bool SetNewLutNode(Element* pElement, wxXmlNode *pRenderizationNode,
                   wxXmlNode* pLutNode);
}

#endif /*LUTRENDERER_H_*/
