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

#ifndef THRESHOLDRENDERER_H_
#define THRESHOLDRENDERER_H_

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "suri/Renderer.h"
#include "SRDHistogram.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Clase que escribe una mascara que contiene determinado porcentaje de los
 * valores mas significativos
 */
class ThresholdRenderer : public Renderer {
public:
   /**
    * Destructor
    */
   virtual ~ThresholdRenderer();
// ------------------------- ESTRUCTURA DE DATOS ----------------------------
   class Parameters {
   public:
      /** Valor que se utiliza para marcar que se supera el umbral **/
      int thresholdHigh;
      /** Valor que se utiliza para marcar que se no supera el umbral **/
      int thresholdLow;
      /** Porcentaje de pixeles con los que me quedo **/
      double thresholdPercent;
      /** Constructor **/
      Parameters();
   };
// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
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

private:
   raster::data::Histogram<float>* CreateHistograms(std::vector<void*> data,
                                                   int canvasSize);
   /** Recalcula los limites del histograma, utilizando un histograma temporal **/
   void AdjustLimits(std::vector<void*> data, int canvasSize,
                     int* pNumBins, double* minValues, double* maxValues);
   /** Aplica el umbral a los datos**/
   void ApplyThreshold(std::vector<void*>& data, double* threshold, int canvasSize);
   /** Cantidad de veces que se ajustan los limites del histograma **/
   int adjustmentLoops_;
   /** Valor que se utiliza para marcar que se supera el umbral **/
   int thresholdHigh_;
   /** Valor que se utiliza para marcar que se no supera el umbral **/
   int thresholdLow_;
   /** Porcentaje de pixeles con los que me quedo **/
   double thresholdPercent_;
   /** Cantidad de divisiones en el histograma **/
   static const int kHistogramBins;
};

} /** namespace suri */

#endif /* THRESHOLDRENDERER_H_ */
