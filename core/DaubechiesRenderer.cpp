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

// Includes standard
#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

// Includes Suri
#include "DaubechiesRenderer.h"
#include "DaubechiesSplitStrategy.h"
#include "DaubechiesTransformStrategy.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/Canvas.h"
#include "suri/World.h"
#include "MemoryCanvas.h"

// Includes Wx
#include "wx/xml/xml.h"
#include "wx/string.h"
// Includes App
// Defines

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::DaubechiesRenderer);

/** namespace suri */
namespace suri {

const int DaubechiesRenderer::RendererOutputBands = 3;

/** Ctor */
DaubechiesRenderer::DaubechiesRenderer() {}

/** Dtor */
DaubechiesRenderer::~DaubechiesRenderer() {}

/**
 * Crea un renderizador a partir de un pElement
 * @param[in] pElement elemento a partir del cual se crea el renderizador
 * @param[in] pPreviousRenderer renderer anterior en el pipeline
 * @return pfilter nodo renderizacion
 * @return NULL si no existe el renderizador anterior o no existe el elemento
 */
Renderer *DaubechiesRenderer::Create(Element *pElement,
                                            Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }

   // Si no existe nodo convolucion con una tabla retorno false
   wxString nodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(CreatedNode().c_str());

   wxXmlNode *pnode = pElement->GetNode(nodepath);
   if (!pnode) {
      REPORT_ERROR(
            "D:No se pudo encontrar el nodo XML en el elemento \"%s\"",
             pElement->GetName().c_str());
      return NULL;
   }

   // Leo parametros de nodo
   Parameters params;
   // params = GetParameters(pElement->GetNode(wxT("")));

   // Cargo el tipo de dato de entrada
   int x, y, b;
   std::string datatype;
   pPreviousRenderer->GetOutputParameters(x, y, b, datatype);

   // Creo nuevo renderizador
   DaubechiesRenderer *pTransform = new DaubechiesRenderer;
   pTransform->SetParameters(params);
   return pTransform;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del renderer
 */
std::string DaubechiesRenderer::CreatedNode() const {
   return wxT(DAUBECHIES_NODE);
}

/**
 * Antes de llamar a renderizar agranda el mundo con un borde usando
 * World::Transform, por lo tanto el canvas retornado tendra
 * pixeles de mas en los bordes.
 * Con los resultados aplica la transformada y achica el canvas
 * al tamanio original(sin bordes para filtro)
 * @param[in] pWorldWindow mundo con informacion del subset a renderizar
 * @param[in] pCanvas datos sobre los que se aplicara el filtro
 * @param[out] pCanvas datos obtenidos de aplicar el filtro
 * @param[in] pMask mascara
 */
bool DaubechiesRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                       Mask* pMask) {
   bool prevrenderizationstatus = true;
   Subset auxwourld;
   pWorldWindow->GetWindow(auxwourld);

   World world(*pWorldWindow);
   // Calculo el nuevo viewport
   Subset windowsubset;
   int newviewportheight, newviewportwidth;
   pWorldWindow->GetViewport(newviewportwidth, newviewportheight);
   // Si no es par, le sumo un pixel
   newviewportheight += newviewportheight % 2;
   Coordinates newviewportlr(newviewportwidth, newviewportheight);
   Coordinates newviewportul(0, 0);

   // Calculo la ventana del nuevo Viewport
   world.Transform(newviewportlr, windowsubset.lr_);
   world.Transform(newviewportul, windowsubset.ul_);

   // Modifico el window con nuevos valores
   world.SetWindow(windowsubset);
   world.SetViewport(newviewportwidth, newviewportheight);

   // Cambio tipo de dato en canvas y llamo a Render
   int canvassizex, canvassizey, bandcount = 0;
   std::string datatype;
   if (pPreviousRenderer_ && pCanvas) {
      pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey, bandcount, datatype);
      pCanvas->SetDataType(datatype);
      prevrenderizationstatus = pPreviousRenderer_->Render(&world, pCanvas, pMask);
   } else {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar filtro.", false);
   }

   // Leo el tamanio del canvas retornado
   pCanvas->GetSize(canvassizex, canvassizey);
   std::string canvasInputType = pCanvas->GetDataType();
   if (canvassizex < 1 || canvassizey < 1) {
      REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
   }

   // leo el canvas retornado
   std::vector<int> bands(bandcount);
   std::vector<void*> data(bandcount);
   for (int b = 0; b < bandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];
   }
   // Cargo bandas
   pCanvas->Read(bands, data);
   // Salida
   std::vector<void*> outputDataBands;
   // Transformo
   for (int b = 0; b < bandcount; b++) {
   DaubechiesTransform2D daubechiesTransform(data[b], canvassizex,
                                                   canvassizey,
                                                   canvasInputType);
      float* tempTransform = daubechiesTransform.Transform();
      DaubechiesSplitBoth splitStrategy;
      std::vector<void*> splitOutput;
      splitOutput = splitStrategy.SplitBand(tempTransform, canvassizex,
                              canvassizey, DataInfo<float>::Size);
      outputDataBands.insert(outputDataBands.end(),
                                splitOutput.begin(), splitOutput.begin() + 3);
      delete[] tempTransform;
      delete[] static_cast<unsigned char*>(data[b]);
      // Borro la ultima banda, que descarto
      delete[] static_cast<unsigned char*>(splitOutput[3]);
   }
   // Salida
   unsigned int outputBands = outputDataBands.size();
   bands.clear();
   for (unsigned int i = 0; i < outputBands; ++i) {
      bands.push_back(i);
   }
   // Guardo cambios
   // Configuro
   pCanvas->SetSize(canvassizex / 2, canvassizey / 2);
   pCanvas->SetDataType(DataInfo<float>::Name);
   pCanvas->SetBandCount(outputBands);
   pCanvas->Write(bands, outputDataBands);

   // libero el temporario
   for (unsigned int b = 0; b < outputBands; b++) {
      delete[] static_cast<unsigned char*>(outputDataBands[b]);
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
void DaubechiesRenderer::GetBoundingBox(const World *pWorld, double &Ulx,
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
void DaubechiesRenderer::GetInputParameters(int &SizeX, int &SizeY,
                                                   int &BandCount,
                                                   std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void DaubechiesRenderer::GetOutputParameters(int &SizeX, int &SizeY,
                                                    int &BandCount,
                                                    std::string &DataType) const {
   GetInputParameters(SizeX, SizeY, BandCount, DataType);
   /**
    *  Como hago una transformada 2D, decimo alto y ancho, y me quedo con
    *  las bandas LL, HL y LH
    */
   BandCount *= RendererOutputBands;
   SizeX /= 2;
   SizeY /= 2;
   DataType = DataInfo<float>::Name;
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]  pElement: puntero al elemento.
 */
void DaubechiesRenderer::Update(Element *pElement) {
   // wxXmlNode *pNode = pElement->GetNode(wxT(""));
   try {
      Parameters params;
   } catch(...) {
      REPORT_DEBUG("D:Error en los parametros");
   }
}

/**
 * Cambia el contenido de parametes interno. El unico
 * parametro que no modifica es la funcion. El filtro debe tener un numero
 * impar de columnas y filas.
 * @param[in] Params nuevos valores que debe tomar parameters
 * @return false si el ancho / alto de los filtros es menor o igual a cero o par
 * @return true en otro caso
 */
void DaubechiesRenderer::SetParameters(const Parameters &Params) {
   this->parameters_ = Params;
}
}

