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

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "DecimationRenderer.h"
#include "suri/Canvas.h"
#include "suri/DataTypes.h"
#include "suri/xmlnames.h"
// Includes Wx
// Defines
// forwards

namespace suri {

// Forwards
/**
 * Decimo una banda a la mitad de ancho y alto
 */
float* DecimateData(const float* floatData, int canvassizex, int canvassizey);

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::DecimationRenderer);

DecimationRenderer::~DecimationRenderer() {}

Renderer *DecimationRenderer::Create(Element *pElement,
                                     Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }
   DecimationRenderer *pEntropy = new DecimationRenderer;
   return pEntropy;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del renderer
 */
std::string DecimationRenderer::CreatedNode() const {
   return wxT(DECIMATION_NODE);
}

/**
 * Calcula la registrabilidad de las baldosas segun la cantidad entropia que
 * poseen en cada subregion.
 * @param[in] pWorldWindow mundo con informacion del subset a renderizar
 * @param[in] pCanvas datos sobre los que se aplicara el filtro
 * @param[out] pCanvas datos obtenidos de aplicar el filtro
 * @param[in] pMask mascara
 */
bool DecimationRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                       Mask* pMask) {
   bool prevrenderizationstatus = true;
   if (pPreviousRenderer_ && pCanvas) {
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, pCanvas, pMask);
   } else {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar filtro.", false);
   }
   int canvassizex, canvassizey, bandcount = 0;
   std::string datatype;
   // Leo el tamanio del canvas retornado
   pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey, bandcount, datatype);
   // Leo el tamanio del canvas retornado
   pCanvas->GetSize(canvassizex, canvassizey);
   std::string canvasInputType = pCanvas->GetDataType();
   if (canvassizex < 1 || canvassizey < 1) {
      REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
   }
   if (datatype.compare(DataInfo<float>::Name) != 0) {
      REPORT_AND_FAIL_VALUE("D:Tipo de dato del canvas erroneo", false);
   }
   // Si es de tamanio 1, no hace falta resizear
   if (canvassizey == 1){
      return true;
   }
   // Cargo bandas
   std::vector<int> bands(bandcount);
   std::vector<void*> data(bandcount);
   std::vector<void*> decimatedData(bandcount);
   for (int b = 0; b < bandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];
   }
   // Cargo bandas
   pCanvas->Read(bands, data);
   // Calculo la entropia
   for (unsigned int i = 0; i < bands.size(); ++i) {
      float* floatData = static_cast<float*>(data[i]);
      decimatedData[i] = DecimateData(floatData, canvassizex, canvassizey);
      delete[] floatData;
   }
   pCanvas->SetSize(canvassizex / 2, canvassizey / 2);
   pCanvas->Write(bands, decimatedData);
   // Libero recursos
   for (unsigned int i = 0; i < decimatedData.size(); ++i) {
      delete[] static_cast<float*>(decimatedData[i]);
   }
   return true && prevrenderizationstatus;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]  pWorld: puntero a la ventana del mundo.
 * @param[in]  Ulx: uper-left x
 * @param[in]  Uly: uper-left y
 * @param[in]  Lrx: lower-right x
 * @param[in]  Lry: lower-right y
 */
void DecimationRenderer::GetBoundingBox(const World *pWorld, double &Ulx,
                                               double &Uly, double &Lrx, double &Lry) {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetBoundingBox(pWorld, Ulx, Uly, Lrx, Lry);
   }
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void DecimationRenderer::GetInputParameters(int &SizeX, int &SizeY,
                                                   int &BandCount,
                                                   std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio de salida en X por el renderizador.
 * @param[out] SizeY Tamanio de salida en Y por el renderizador.
 * @param[out] BandCount Bandas generadas.
 * @param[out] DataType Tipo de dato.
 */
void DecimationRenderer::GetOutputParameters(int &SizeX, int &SizeY,
                                                    int &BandCount,
                                                    std::string &DataType) const {
   GetInputParameters(SizeX, SizeY, BandCount, DataType);
   SizeX /= 2;
   SizeY /= 2;
}


// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]  pElement: puntero al elemento.
 */
void DecimationRenderer::Update(Element *pElement) {
   // wxXmlNode *pNode = pElement->GetNode(wxT(""));
}

/**
 * Decimo una banda a la mitad de ancho y alto
 */
float* DecimateData(const float* floatData, int inputWidth, int inputHeight) {
      // Ancho de salida
   int outputWidth = inputWidth / 2;
   int outputHeight = inputHeight / 2;
   float* pOutput = new float[outputHeight * outputWidth];
   for (int iy = 0; iy < outputHeight; ++iy) {
      int inputOffsetY = iy * 2 * inputWidth;
      int outputOffsetY = iy * outputWidth;
      for (int ix = 0; ix < outputWidth; ++ix) {
         pOutput[outputOffsetY + ix] = floatData[inputOffsetY + ix * 2];
      }
   }
   return pOutput;
}

} /** namespace suri */
