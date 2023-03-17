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

#ifndef SRENHANCEMENTRENDERER_H_
#define SRENHANCEMENTRENDERER_H_

#include "suri/Renderer.h"
#include "NoDataValue.h"

namespace suri {
namespace render {

class EnhancementRenderer : public suri::Renderer {
public:
   /**
    * Ctor.
    */
   EnhancementRenderer();

   /**
    * Ctor 2.
    */
   EnhancementRenderer(int bandcount, int* pnumbins, double* plowvalue,
                       double* phighvalue, int** pbins, bool Active, std::string& Name);

   /**
    * Dtor.
    */
   ~EnhancementRenderer();

   /**
    * Crea un renderizador a partir de un pElement.
    */
   virtual Renderer *Create(Element *pElement,
                            Renderer *pPreviousRenderer = NULL) const;

   /**
    * Nombre del renderizador == al nombre del nodo.
    */
   virtual std::string CreatedNode() const;

   /**
    * Renderiza el elemento dado un World en un Canvas.
    */
   virtual bool Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask);

   /**
    * Obtiene el "bounding box" del elemento renderizado.
    */
   virtual void GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                               double &Lrx, double &Lry);

   /**
    * Obtiene los parametros de entrada del renderer.
    */
   virtual void GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                   std::string &DataType) const;

   /**
    * Obtiene los parametros de salida del renderer.
    */
   virtual void GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                    std::string &DataType) const;

   /**
    * Actualizacion de parametros para representar un elemento.
    */
   virtual void Update(Element *pElement);

private:
   int bandCount_;  // Cantidad de bandas.
   int* pNumBins_;  // Numero de bins por banda.
   double* pLowValue_;  // Valor mas chico por banda.
   double* pHighValue_;  // Valor mas grande por banda.
   int** pBins_;  // LUT
   bool active_;  // Si esta activo o no.
   std::string name_;  // Nombre del realce.

   /**
    * Traduce los valores de la imagen al valor de intesidad correspondiente.
    */
   void Translate(const std::string& DataType, int* pLut, void* pInData, void* pOutData,
                  int DataSize, int NumBins, double Min, double Max, double NoDataValue,
                  bool NoDataValueAvailable);
};

}  // namespace render
}  // namespace suri

#endif  // SRENHANCEMENTRENDERER_H_
