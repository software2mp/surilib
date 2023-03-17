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
#include <vector>
#include <string>

// Includes suri
#include "suri/IndexProcess.h"
#include "suri/RasterElement.h"
#include "suri/messages.h"
#include "suri/DataTypes.h"
#include "BandMathRenderer.h"
#include "suri/XmlFunctions.h"

// Defines
/** Offset (en bytes) para calcular el tamanio libre necesario para salvar */
#define EXTRA_SAVE_SPACE_REQUIRED 1000000
/** Expresion de la ecuacion de algebra de bandas para indices normalizados */
#define NORMALIZED_INDEX_EQUATION "(b1 - b2)/(b1 + b2)"

/** Indice del choicebook para el NDVI */
#define NDVI_INDEX 0
/** Indice del choicebook para el NDWI */
#define NDWI_INDEX 1
/** Indice del choicebook para el RVI */
#define RVI_INDEX 2
/** Indice del choicebook para el SMI */
#define SMI_INDEX 3

/** namespace suri */
namespace suri {

/** Nombre del proceso **/
const std::string IndexProcess::kProcessName = "IndexProcess";
/** Clave para atributo de seleccion de indice **/
const std::string IndexProcess::IndexKeyAttr = "SelectedIndex";
/** Clave para el atributo de ecuaciones */
const std::string IndexProcess::EquationKey = "Equation";

/**
 * Constructor
 * Agrega el nombre a la herramienta.
 * Obtiene el url del elemento sobre el cual se selecciono la herramienta
 * Crea una instancia del filerenderization.
 * @param[in] pInputElement: elemento sobre el que se aplica la herramienta
 * @param[in] ViewerSubset: subset con las coordenadas de la ventana del elemento
 * activo, que puede ser distinto al elemento sobre el cual se aplica la
 * herramienta.
 * @param[out] pOutputElement: referencia al elemento creado por la herramienta
 */
IndexProcess::IndexProcess(Element *pInputElement,const Subset &ViewerSubset,
                           DataViewManager* pDataViewManager) :
                   FileExporterProcess(pInputElement, ViewerSubset,
                                       pDataViewManager) {
   pRenderizationObject_ = new FileRenderization();
   showSpectralSelectionPart_ = false;  // Oculta part seleccion espectral
   SetProcessName(kProcessName);
}

/**
 * Destructor
 */
IndexProcess::~IndexProcess() {
   if (pAdaptLayer_) {
      BandMathRenderer::Parameters::Equation* pequ = NULL;
      pAdaptLayer_->GetAttribute<BandMathRenderer::Parameters::Equation*>(EquationKey, pequ);
      delete pequ;
   }
}

/**
 * Retorna las bandas selecciondas por el usuario
 * @return vector con las bandas seleccionadas por el usuario
 */
void IndexProcess::GetSelectedRasterBands(std::vector<int>& SelectedBands) {
   pAdaptLayer_->GetAttribute<std::vector<int> >(ProcessAdaptLayer::SelectedBandsKey,
                                                 SelectedBands);
}

/**
 * Indica si las bandas se configuraron correctamente
 * @return bool que indica si las bandas se configuraron correctamente
 */
bool IndexProcess::HasValidSpectralData() {
#ifdef __UNUSED_CODE__
   return pIndexPart_ && pIndexPart_->HasValidData();
#endif
   return true;
}

/** Permite configurar la salida del proceso antes de correrlo */
bool IndexProcess::ConfigureRaster(RasterElement *pRasterElement) {
   wxXmlNode *pindexpnode = GetIndexRenderizationNode();
   wxXmlNode *prnode = pRasterElement->GetNode(wxT(RENDERIZATION_NODE));
   if (pindexpnode && prnode)
      pRasterElement->AddNode(prnode, pindexpnode);

   return true;
}

/**
 * Calcula el tamanio del elemento creado por la herramienta.
 * Para esta herramienta el tamanio se obtiene como el producto entre el tamanio
 * de un double (tipo de dato del elemento) y el tamanio del viewport (el elemento
 * resultante tiene una sola banda).
 * @return neededspace : tamanio del elemento creado por la herramienta.
 */
wxLongLong IndexProcess::GetNeededSpace() {
   wxLongLong neededspace = 0;
   int hight = 0, width = 0;
   // TODO(Gabriel - TCK #2315): Las clases hijas no deberian usar el World
   // directamente tendrian que usar el mundo de salida?
//   pInputWorld_->GetViewport(hight, width);
   World* pworld = (pOutputWorld_)? pOutputWorld_ : pInputWorld_;
   pworld->GetViewport(hight, width);
   neededspace = hight * width * DataInfo<double>::Size + EXTRA_SAVE_SPACE_REQUIRED;
   return neededspace;
}

/**
 * Crea el nodo de indice para agregar al nodo renderizacion.
 * Carga en parameters la formula y las bandas. Luego llama a GetXmlNode para
 * construir el nodo.
 * \attention El nodo devuelto (y su destruccion) es responsabilidad del que
 * lo solicita.
 */
wxXmlNode * IndexProcess::GetIndexRenderizationNode() {
   // Obtengo ecuacion de part de indice
   BandMathRenderer::Parameters::Equation *pequation;
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<BandMathRenderer::Parameters::Equation *>(
               EquationKey, pequation)) {
      REPORT_DEBUG("D: Error al generar el nodo de BandMathRenderer");
      return NULL;
   }

   // Cargo ecuacion en parametro y genero nodo.
   BandMathRenderer::Parameters parameters;
   parameters.equations_.push_back(*pequation);
   return BandMathRenderer::GetXmlNode(parameters);
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> IndexProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;    
   int index = -1;
   // TODO(Gabriel - TCK #7139): Si se actualizan los indices se deberia actualizar esto.
   if (pAdaptLayer_ && pAdaptLayer_->GetAttribute<int>(IndexKeyAttr, index)) {
      std::string indexname;
      switch (index) {
         case NDVI_INDEX:
            indexname = "NDVI";
            break;
         case NDWI_INDEX:
            indexname = "NDWI";
            break;
         case RVI_INDEX:
            indexname = "RVI";
            break;
         case SMI_INDEX:
            indexname = "SMI";
            break;
         default:
            break;
      }
      params.push_back(CreateKeyValueNode("indice", indexname));
   }
   return params;
}

}  // namespace suri
