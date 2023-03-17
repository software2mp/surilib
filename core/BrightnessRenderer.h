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

#ifndef BRIGHTNESSRENDERER_H_
#define BRIGHTNESSRENDERER_H_

// Includes standard
#include <vector>
#include <map>

// Includes suri
#include "suri/Renderer.h"

/** forward */
class wxXmlNode;

/** namespace suri */
namespace suri {
/** Renderer que modifica el brillo y contraste de un canvas */
/**
 * Modifica el brillo y contraste de un canvas.
 * Tambien genera, obtiene o remplaza el nodo XML con el brillo y contraste
 * para un elemento raster.
 * \note hereda de renderer para formar parte del pipeline de renderizacion
 */
class BrightnessRenderer : public Renderer {
   /** Ctor. de Copia. */
   BrightnessRenderer(const BrightnessRenderer &BrightnessRenderer);

public:
   /** Ctor. */
   BrightnessRenderer();
   /** Dtor. */
   virtual ~BrightnessRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** Parametros de BrightnessRenderer(bands_, outputDataType_, inputDataType_, */
// brightness_,contrast_,BrightnessFunc)
   class Parameters {
   public:
      /** Tipo de dato para la funcion */
      typedef void (*BrightnessFunc)(void*, void*, size_t, double, double);
      std::vector<int> bands_; /*! \todo sacar: pasar a deteccion on-render */
      std::string outputDataType_; /*! tipo de dato a la salida */
      std::string inputDataType_; /*! tipo de dato a la entrada */
      double brightness_; /*! brillo que se le aplica al elemento */
      double contrast_; /*! contraste que se le aplica al elemento */
      BrightnessFunc pFunction_; /*! Funcion que aplica cambios segun tipo */
      /* de dato */
      /** Ctor. */
      Parameters() :
            brightness_(0), contrast_(0), pFunction_(NULL) {
      }
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** Obtiene el nodo de interes a partir de un elemento */
   static wxXmlNode *GetXmlNode(const Element *pElement);
   /** Reemplaza el nodo renderizado por el nuevo */
   static bool SetXmlNode(Element *pElement, wxXmlNode *pNode);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
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
protected:
private:
   Parameters parameters_; /*! Guarda informacion necesaria para aplicar Render */
};
}

#endif /*BRIGHTNESSRENDERER_H_*/
