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

#ifndef DAUBECHIESRENDERER_H_
#define DAUBECHIESRENDERER_H_

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/Renderer.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class DaubechiesSplitStrategy;
class DaubechiesTransformStrategy;

class DaubechiesRenderer : public Renderer {
   /** Ctor. de Copia. */
   DaubechiesRenderer(const DaubechiesRenderer&);

public:
   /** Ctor */
   DaubechiesRenderer();
   /** Dtor */
   virtual ~DaubechiesRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de DaubechiesRenderer */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() :
            filterName_("") {
      }

      std::string filterName_; /*! Nombre del filtro a aplicar */
   };

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
   /** Cambia el contenido de parametres y verifica su contenido */
   virtual void SetParameters(const Parameters &params);
   Parameters parameters_; /*! Guarda informacion necesaria para aplicar Render */
   DaubechiesSplitStrategy* pSplitStrategy_;
   DaubechiesTransformStrategy* pTransformStrategy_;
   // Cantidad de bandas nuevas por banda transformada.
   static const int RendererOutputBands;
};

} /** namespace suri */

#endif /* DAUBECHIESRENDERER_H_ */
