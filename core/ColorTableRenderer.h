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

#ifndef COLORTABLERENDERER_H_
#define COLORTABLERENDERER_H_

// Includes standard
// Includes suri
#include "ColorTable.h"
#include "LutRenderer.h"

/** namespace suri */
namespace suri {
/** Clase para el manejo de tablas de color */
/**
 * A una banda le aplica 3 tablas y forma una imagen color del mismo tamanio
 * pero con tres bandas.
 * Implementa metodos de conversion de y a nodo XML para los parametros.
 * Metodos de conversion entre parametros de LutTable y de ColorTable
 */
class ColorTableRenderer : public LutRenderer {
   /** Ctor. de Copia. */
   ColorTableRenderer(const ColorTableRenderer &ColorTableRenderer);

public:
   /** Ctor */
   ColorTableRenderer();
   /** Dtor */
   virtual ~ColorTableRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de ColorTalbeRenderer */
   class Parameters {
   public:
      /** Ctor. */
      Parameters() {
      }
      ColorTable colorTable_; /*! Tabla con colores que uso para renderizar */
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(const Parameters &RenderParameters);
   /** genera los parametros a partir de un nodo */
   static Parameters GetParameters(const wxXmlNode *pNode);
// -------------------------- METODO DE CREACION ----------------------------
   /** Crea un renderizador a partir de un pElement */
   virtual Renderer *Create(Element *pElement, Renderer *pPreviousRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
// ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);

protected:
   /** Adapta Parameters de ColorTable a Lut */
   static bool ColorTableToLut(ColorTableRenderer::Parameters InParameters,
                               LutRenderer::Parameters &OutParameters);
   /** Adapta Parameters de Lut a ColorTable */
   static bool LutToColorTable(LutRenderer::Parameters &InParameters,
                               ColorTableRenderer::Parameters &OutParameters);
};
}

#endif /* COLORTABLERENDERER_H_ */
