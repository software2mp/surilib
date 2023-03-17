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

// Includes standart
#include <limits>
#include <vector>
#include <string>

// Includes Suri
#include "suri/ClassificationProcess.h"
#include "ColorTable.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/VectorElement.h"
#include "suri/VectorStyle.h"
#include "suri/DataTypes.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "suri/DataViewManager.h"
#include "ClassInformation.h"
#include "ClassificationGetter.h"
#include "suri/XmlFunctions.h"

// Includes wx
#include "wx/wx.h"

// Defines
/** Offset (en bytes) para calcular el tamanio libre necesario para salvar */
#define EXTRA_SAVE_SPACE_REQUIRED 1000000

/** namespace suri */
namespace suri {
/** Nombre del proceso **/
const std::string ClassificationProcess::kProcessName = "ClassificationProcess";
/** Clave para atributo de tipo de clasificacion**/
const std::string ClassificationProcess::ClassificationType = "ClassificationType";
/** Clave para la obtencion de funcion encargada de obtener las propiedades
 *  de las clases y el nodo xml resultantes de la clasificacion*/
const std::string ClassificationProcess::ClassGetterAttrKey = "ClassPropFunc";

/**
 * Constructor
 * @param[in] pLayerList lista de elementos (poligonos de clases)
 * @param[in] pInputElement elemento que se quiere clasificar
 * @param[in] WindowSubset subset que se quiere clasificar del elemento
 * @param[out] pOutputElement elemento resultado de clasificar
 * @param[in] ClassificationType tipo de clasificacion
 */
ClassificationProcess::ClassificationProcess(
      LayerList *pLayerList, Element *pInputElement, const Subset &WindowSubset,
      std::string ClassificationType, DataViewManager* pDataViewManager,
      bool ShowSpectralPart) :
      FileExporterProcess(pInputElement, WindowSubset, pDataViewManager),
      pLayerList_(pLayerList),
      pDatasourceManager_(pDataViewManager ? pDataViewManager->GetDatasourceManager() : NULL),
      pViewcontextManager_(pDataViewManager ? pDataViewManager->GetViewcontextManager() : NULL),
      classificationType_(ClassificationType) {
   showSpectralSelectionPart_ = ShowSpectralPart;
   SetProcessName(kProcessName);
}

/** Dtor. */
ClassificationProcess::~ClassificationProcess() {
}

/**
 * Otiene el tamanio del objeto a salvar. Toma en consideracion que
 * el formato de salida es unsigned char.
 * @return tamanio (en bytes) libre necesario para salvar
 * \todo ver de donde se puede obtener el formato de salida en forma dinamica
 */
wxLongLong ClassificationProcess::GetNeededSpace() {
   wxLongLong neededspace;
   int bandscount = 0;

   if (HasValidSpectralData()) bandscount = GetSelectedRasterBands().size();

   int hight, width;
   // TODO(Gabriel - TCK #2315): Las clases hijas no deberian usar el World
   // directamente tendrian que usar el mundo de salida?
   // pInputWorld_->GetViewport(hight, width);
   World* pworld = (pOutputWorld_)? pOutputWorld_ : pInputWorld_;
   pworld->GetViewport(hight, width);
   neededspace = bandscount * hight * width
         * SizeOf(DataInfo<unsigned char>::Name)+ EXTRA_SAVE_SPACE_REQUIRED;

   return neededspace;
}

/**
 * Le pide al part de clasificacion el nodo y lo agrega a la lista de
 * renderizadores del raster.
 * @param[in] pRasterElement raster que se quiere clasificar
 * @return informa si pudo agregar el clasificador.
 */
bool ClassificationProcess::ConfigureRaster(RasterElement *pRasterElement) {
   // Determino las bandas que se van a renderizar
   FileExporterProcess::ConfigureRaster(pRasterElement);

   // Le pide al clasificador el nodo y lo agrega al raster
   wxXmlNode *pclassificationnode = NULL;
   suri::ClassificationGetter* pnodefunc = NULL;
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<ClassificationGetter*>(ClassGetterAttrKey,
               pnodefunc)) {
      return false;
   }

   pnodefunc->SetSelectedBands(GetSelectedRasterBands());

   // Esto lo usa mas que nada el proceso de clasificacion por angulo espectral.
   if (!pnodefunc->IsValid())
      return false;

   pclassificationnode = pnodefunc->GetClassificationRendererNode(pRasterElement);
   wxXmlNode *prnode = pRasterElement->GetNode(wxT(RENDERIZATION_NODE));
   if (prnode) {
      pRasterElement->AddNode(prnode, pclassificationnode);
   }
   return true;
}

/**
 * Carga en el elemento la tabla con LUT generada a partir de los
 * vectores seleccionados. Los colores y los intervalos los obtiene
 * del ClassificationPart
 * @return informa si pudo configurar elemento
 */
bool ClassificationProcess::ConfigureOutput() {
   if (!FileExporterProcess::ConfigureOutput())
      return false;

   // Genero tabla de colores con clases seleccionadas
   ColorTable colortable;
   colortable.Activate(true);
   /**
    * Se agrega como primer color de la tabla de colores el "valor no
    * valido de la imagen clasificada (el 0)
    */
   colortable.AddColor(ClassInformation::InvalidClassIndex, 1, 1, 1);

   // Agrego colores
   std::vector<ClassInformation> properties;
   suri::ClassificationGetter* pclasspropfunc = NULL;
   if (!pAdaptLayer_
         || !pAdaptLayer_->GetAttribute<ClassificationGetter*>(ClassGetterAttrKey,
               pclasspropfunc)) {
      return false;
   }
   Element* pNewElement = NULL;
   //pNewElement es inicializado por FileExporterProcess
   pAdaptLayer_->GetAttribute<Element*>(
               ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
   pclasspropfunc->GetClassProperties(properties);
   size_t classcount = properties.size();
   DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                  pNewElement);
   ClassifiedRasterDatasourceManipulator manipulator;
   manipulator.AddNature(pdatasource);
   for (size_t i = 0; i < classcount; i++) {
      colortable.AddColor(properties[i].GetIndex(),
                          properties[i].GetColor().red_,
                          properties[i].GetColor().green_,
                          properties[i].GetColor().blue_);
      if (i == classcount - 1) {
         colortable.AddColor(std::numeric_limits<double>::max(),
                             properties[i].GetColor().red_,
                             properties[i].GetColor().green_,
                             properties[i].GetColor().blue_);
      }
      manipulator.AddClassInformation(&properties[i], pdatasource, true);
   }

   // Genero nodo con datos parameters
   wxXmlNode* pctnode = NULL;
   if (!ColorTable::ToXml(colortable, pctnode)) {
      REPORT_DEBUG("Fallo al generar nodo de tabla de colores");
      return false;
   }

   // Modifico la LUT del raster
   wxXmlNode* prenderernode = pNewElement->GetNode(wxT(RENDERIZATION_NODE));
   wxString oldlutnodepath = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pNewElement->GetNode(oldlutnodepath.c_str());
   if (poldlutnode) {
      prenderernode->InsertChildAfter(pctnode, poldlutnode);
      prenderernode->RemoveChild(poldlutnode);
   } else {
      prenderernode->AddChild(pctnode);
   }
   /** se configura como valor no valido el valor de la clase no valida **/
   pdatasource->GetElement()->SetNoDataValue(ClassInformation::NDVIndex);

   ExportMetadataToFile(pdatasource, pdatasource->GetElement()->GetUrl().c_str());

   return pDatasourceManager_->AddDatasource(pdatasource);
}

/** Metodo que especializa la configuracion del ProcessAdaptLayer para
 *  los procesos especificos
 */
void ClassificationProcess::DoOnProcessAdaptLayerSet() {
   FileExporterProcess::DoOnProcessAdaptLayerSet();
   pAdaptLayer_->AddAttribute<LayerList*>(ProcessAdaptLayer::LayerListKeyAttr,
                                            pLayerList_);
   pAdaptLayer_->AddAttribute<std::string>(ClassificationProcess::ClassificationType,
                                           classificationType_);
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> ClassificationProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;
   suri::ClassificationGetter* pclasificationfunc = NULL;
   if (pAdaptLayer_ && pAdaptLayer_->
                  GetAttribute<ClassificationGetter*>(ClassGetterAttrKey, pclasificationfunc)) {
      params.push_back(new wxXmlNode(*pclasificationfunc->
                                    GetClassificationRendererNode(
                                    dynamic_cast<RasterElement*>(inputElements_[0]))));
      std::vector<ClassInformation> properties;
      pclasificationfunc->GetClassProperties(properties);
      for (size_t i = 0;  i < properties.size(); ++i) {
         wxXmlNode* pclasspropnode = CreateKeyValueNode("definicion-clase", "");
         CreateKeyValueNode("nombre", properties[i].GetName(), pclasspropnode);
         // CreateKeyValueNode("id", properties[i].GetId(), pclasspropnode);
         CreateKeyValueNode("color", properties[i].GetColor().GetColorAsHexaString(), pclasspropnode);
         params.push_back(pclasspropnode);
      }

   }
   return params;
}
 
}  /** namespace suri **/
