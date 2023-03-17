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

#ifndef CATEGORIZEDVECTORRENDERER_H_
#define CATEGORIZEDVECTORRENDERER_H_

// Includes Estandar
#include <set>
#include <map>
#include <string>

// Includes Suri
#include "VectorRenderer.h"

// Includes Wx
// Defines
// forwards

namespace suri {
namespace vector {
namespace render {

class CategorizedVectorRenderer : public suri::VectorRenderer {
public:
   // ------------------------- ESTRUCTURA DE DATOS ----------------------------
   /** parametros de VectorRenderer(URL, georrefernecia, estilos de capas) */
   class CategorizedParameters : public VectorRenderer::Parameters{
   public:
      CategorizedParameters() {
         activelayer_ = 0;
      }
      /*!
       *  Indice y estilos de las capas. Por cada capa
       *  pueden existir n estilos */
      typedef std::vector< std::vector< std::pair<std::string, std::string > > > LayerStyleMap;
      LayerStyleMap categorizedlayerstyle_;
      /**
       *  Vector con los nombres de los estilos. Debe ser el mismo orden que los
       *  estilos cargados en el mapa de estilos
       */
      std::vector<std::string> categorizedStyleNames_;
   };

   CategorizedVectorRenderer();
   virtual ~CategorizedVectorRenderer();
   /** Renderiza una capa vectorial */
   virtual bool RenderLayer(int LayerIndex, const World *pWorldWindow, Canvas* pCanvas,
                            Mask* pMask);
   // -------------------------- METODO DE CREACION ----------------------------
   /** creador + inicializador */
   virtual VectorRenderer *Create(Element *pElement, Renderer *pLastRenderer) const;
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
   // ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
   /** genera un nodo XML a partir de los parametros */
   static wxXmlNode *GetXmlNode(
         const CategorizedVectorRenderer::CategorizedParameters &RenderParameters);
   /** Obtiene el nodo de interes a partir de un elemento */
   static wxXmlNode *GetXmlNode(const Element *pElement);
   /** Reemplaza el nodo renderizado por el nuevo */
   static bool SetXmlNode(Element *pElement, wxXmlNode *pNode);
   /** genera los parametros a partir de un nodo */
   static CategorizedParameters GetParameters(const wxXmlNode *pNode);
   /**
    * Actualizacion de parametros para representar un elemento
    * @param[in] pElement elemento del vector con el que quiero actualizar
    * el renderizador
    */
   virtual void Update(Element *pElement);

protected:
   /** Metodo auxiliar para cargar parametros de renderizacion **/
   static void LoadRenderizationParameters(const wxXmlNode *pnode,
                                           CategorizedParameters *pParams);
   /** Metodo auxiliar encargado de cargar el mapa de estilos clasificados **/
   static void LoadClassStyle(const wxXmlNode *pClassNode,
                              CategorizedParameters *pParams);
   CategorizedParameters params_;
};

} /** namespace render */
} /** namespace vector */
} /** namespace suri */

#endif /* CATEGORIZEDVECTORRENDERER_H_ */
