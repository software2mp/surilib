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

#ifndef STACKINGRENDERER_H_
#define STACKINGRENDERER_H_

// Includes Estandar
#include <map>
#include <string>
#include <vector>

// Includes Suri
#include "RasterRenderer.h"
#include "suri/Renderer.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase encargada de renderizar la combinacion de bandas desde mas de una fuente de datos.
 * Compone a varios RasterRenderer para realizar la combinacion de bandas de cada fuente de datos.
 */
class StackingRenderer : public suri::Renderer {
   /** ctor copia */
   StackingRenderer(const StackingRenderer&);

public:
   /** constructor **/
   StackingRenderer();
   /** destructor **/
   virtual ~StackingRenderer();
    /**
     * Crea un renderizador a partir de un pElement
     * Metodo virtual abstracto puro utilizado para crear con el metodo
     * estatico Create() instancias de clases derivadas registradas con el
     * metodo RegisterViewer().
     * @param[in] pElement Elemento a renderizar.
     * @param[in] pPreviousRenderer Renderizador anterior.
     */
    virtual StackingRenderer *Create(Element *pElement,
                             Renderer *pPreviousRenderer = NULL) const;
   // ----------------------- METODOS DE RENDERIZACION -------------------------
   /** Renderiza el elemento dado un World en un Canvas */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);
   /** Nombre del renderizador == al nombre del nodo */
   virtual std::string CreatedNode() const;
   // ----------------------- METODOS DE ACTUALIZACION -------------------------
   /** Actualizacion de parametros para representar un elemento */
   virtual void Update(Element *pElement);
   /** Obtiene los parametros de entrada del renderer */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;
   /** Obtiene los parametros de salida del renderer */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;
   /** Obtiene el "bounding box" del elemento renderizado */
   /**
    * \pre el subset debe ser seteado antes de llamar a esta funcion con un
    *      valor invalido.
    * \post Se refleja el subset de mundo que renderiza.
    * \post No se modifican los parametros en caso de falla o subset nulo
    * @param[in] pWorld Mundo sobre el cual se debe devolver el BB
    * @param[out] Ulx ULx de mundo renderizado.
    * @param[out] Uly ULy de mundo renderizado.
    * @param[out] Lrx LRx de mundo renderizado.
    * @param[out] Lry LRy de mundo renderizado.
    */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);

private:
   /** Metodo auxiliar que se encarga de cargar los parametros de renderizacion
    *  a partir del xml que contiene el element que se pasa por parametro
    */
   void LoadRendererParameters(Element *pElement);
   /** A partir del nodo de apilamiento se cargan las diferentes combinaciones
    *  de bandas que se encuentran definidas */
   void LoadCombinationParameters(wxXmlNode* pStackingNode);
   /** configuracion para combinacion de bandas de diferentes archivos **/
   typedef std::map<std::string, RasterRenderer::Parameters*> StackingMap;
   StackingMap stackingParameters_;
   RasterRenderer::Parameters generalParameters_;
   /** cantidad de bandas que participan en el apilamiento**/
   int bandCount_;
   typedef std::vector<RasterRenderer*> RendererVector;
   RendererVector pRenderers_;
};

} /** namespace suri */

#endif /* STACKINGRENDERER_H_ */
