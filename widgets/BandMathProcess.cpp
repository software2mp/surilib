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
#include <map>

// Includes suri
#include "suri/BandMathProcess.h"
#include "suri/DataTypes.h"
#include "BandMathRenderer.h"
#include "DataCastRenderer.h"
#include "suri/DataViewManager.h"
#include "suri/XmlFunctions.h"

// Defines
/** Offset (en bytes) para calcular el tamanio libre necesario para salvar */
#define EXTRA_SAVE_SPACE_REQUIRED 1000000

namespace suri {

/** Nombre del proceso **/
const std::string BandMathProcess::kProcessName = "BandMathProcess";
/** Clave para la obtencion del tipo de dato en que se expresa el algebra de bandas **/
const std::string BandMathProcess::DataTypeKeyAttr = "DataType";
/** Clave para el atributo de equacion **/
const std::string BandMathProcess::EquationKey = "Equation";
/** Clave para el atributo de variables de la ecuacion **/
const std::string BandMathProcess::EquationVariablesKey = "EquationVariables";

/** Constructor */
BandMathProcess::BandMathProcess(Element *pInputElement,
                                 const Subset &ViewerSubset,
                                 DataViewManager* pDataViewManager) :
                  FileExporterProcess(pInputElement, ViewerSubset,
                                      pDataViewManager) {
   SetProcessName(kProcessName);
   pRenderizationObject_ = new FileRenderization();
   showSpectralSelectionPart_ = false;  // Oculta part seleccion espectral
}

/** Destructor */
BandMathProcess::~BandMathProcess() {
}

/**
 * Permite configurar la salida del proceso antes de correrlo
 * @param[in] pRasterElement puntero al elemento a configurar
 * @return bool que indica si tuvo exito
 */
bool BandMathProcess::ConfigureRaster(RasterElement *pRasterElement) {
   wxXmlNode *pbandmathnode = GetBandMathRenderizationNode();
   wxXmlNode *pdatacastnode = GetDataCastRenderizationNode();
   wxXmlNode *prnode = pRasterElement->GetNode(wxT(RENDERIZATION_NODE));
   if (!pbandmathnode || !prnode) {
      delete pbandmathnode;
      delete pdatacastnode;
      return false;
   }

   pRasterElement->AddNode(prnode, pbandmathnode);
   if (pdatacastnode)
      pRasterElement->AddNode(prnode, pdatacastnode);
   return true;
}

/**
 * Calcula el tamanio del elemento creado por la herramienta.
 * Para esta herramienta el tamanio se obtiene como el producto entre el tamanio
 * del tipo de salida y el tamanio del viewport (el elemento
 * resultante tiene una sola banda).
 * @return neededspace : tamanio del elemento creado por la herramienta.
 */
wxLongLong BandMathProcess::GetNeededSpace() {
   wxLongLong neededspace = 0;
   int hight = 0, width = 0;
   // TODO(Gabriel - TCK #2315): Las clases hijas no deberian usar el World
   // directamente tendrian que usar el mundo de salida?
//   pInputWorld_->GetViewport(hight, width);
   World* pworld = (pOutputWorld_)? pOutputWorld_ : pInputWorld_;
   pworld->GetViewport(hight, width);
   std::string datatype;
   if (pAdaptLayer_
         && pAdaptLayer_->GetAttribute<std::string>(DataTypeKeyAttr, datatype)) {
      neededspace = hight * width * SizeOf(datatype) + EXTRA_SAVE_SPACE_REQUIRED;
   }
   return neededspace;
}

/**
 * Retorna el nodo configurado con ecuacion ingresada por el usuario
 * @return Nodo que configura BandMathRenderer. El puntero es responsabilidad
 * del codigo invocante
 */
wxXmlNode* BandMathProcess::GetBandMathRenderizationNode() {
   BandMathRenderer::Parameters::Equation equationparam;
   pAdaptLayer_->GetAttribute<std::string>(EquationKey, equationparam.equation_);
   pAdaptLayer_->GetAttribute<std::map<std::string, int> >(EquationVariablesKey,
                                                           equationparam.bandNames_);

   BandMathRenderer::Parameters parameters;
   parameters.equations_.push_back(equationparam);
   return BandMathRenderer::GetXmlNode(parameters);
}

/**
 * Retorna el nodo datacast configurado con el tipo de dato de salida
 * @return Nodo que configura DataCastRenderer. El puntero es responsabilidad
 * del codigo invocante
 */
wxXmlNode* BandMathProcess::GetDataCastRenderizationNode() {
   DataCastRenderer::Parameters parameters;
   std::string datatype;
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<std::string>(DataTypeKeyAttr, datatype)) {
      return NULL;
   }
   parameters.destinationDataType_ = datatype;
   if (parameters.destinationDataType_.empty())
      return NULL;
   return DataCastRenderer::GetXmlNode(parameters);
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> BandMathProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params; 
   std::string datatype;
   pAdaptLayer_->GetAttribute<std::string>(DataTypeKeyAttr, datatype);      
   params.push_back(CreateKeyValueNode("tipo-de-dato", datatype));
   BandMathRenderer::Parameters::Equation equationparam;
   pAdaptLayer_->GetAttribute<std::string>(EquationKey, equationparam.equation_);
   params.push_back(CreateKeyValueNode("ecuacion", equationparam.equation_));
   return params;
}

}  // namespace

