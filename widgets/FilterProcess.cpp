/*! \file FilterExporterWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <vector>
#include <limits>

// Includes suri
#include "suri/FilterProcess.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "ColorTable.h"
#include "suri/DataViewManager.h"
#include "suri/XmlFunctions.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xml/xml.h"

/** namespace suri */
namespace suri {

/** Nombre del proceso **/
const std::string FilterProcess::kProcessName = "FilterProcess";
/** Nombre del proceso hijo **/
const std::string FilterProcess::kChildProcessName = "FilterLibraryProcess";
/** Clave para el atributo de nodo xml de filtro **/
const std::string FilterProcess::FilterNodeKey = "FilterNode";
/** Clave para el atributo de nodo xml de ecuaciones */
const std::string FilterProcess::EquationNodeKey = "EquationNode";
/** Clave para el atributo para el tipo de filtro **/
const std::string FilterProcess::FilterTypeKey = "FilterType";
/** Clave para el atributo para flags de filtros */
const std::string FilterProcess::FilterFlagsKey = "FilterFlags";

const char* FilterProcess::FilterTypeString[] =
                  { "PasaBajo", "PasaAlto", "Deteccion de bordes",
                  "Minoria", "Mayoria", "Filtro biblioteca"};

/**
 * Constructor
 * Asigna el tipo de dato del elemento creado por la herramienta.
 * @param[in] pInputElement: puntero al elemento sobre el que se aplica la herramienta
 * @param[in] WindowSubset subset de la imagen desplegada cuando se selecciono la herramienta
 * @param[out] pOutputElement referencia al elemento creado por la herramienta
 */
FilterProcess::FilterProcess(Element *pInputElement,
                                           const Subset &WindowSubset,
                                           int Filters,
                                           DataViewManager* pDataViewManager,
                                           bool ShowSpectralSelectionPart) :
      FileExporterProcess(pInputElement, WindowSubset, pDataViewManager),
      pDatasourceManager_(pDataViewManager ?
                           pDataViewManager->GetDatasourceManager() : NULL)  {
   dataType_ = DataInfo<double>::Name;
   // Oculta/Despliega part seleccion espectral
   showSpectralSelectionPart_ = ShowSpectralSelectionPart;
   flags_ = Filters;
   SetProcessName(kProcessName);
}

/**
 * Destructor
 */
FilterProcess::~FilterProcess() {
}

/**
 * Pemite configurar el RasterElement que se va a renderizar
 * @param[in] pRasterElement elemento raster a renderizar
 * @return true siempre
 */
bool FilterProcess::ConfigureRaster(RasterElement *pRasterElement) {
   FileExporterProcess::ConfigureRaster(pRasterElement);

   SetFilterOptions(pRasterElement);
   dataType_ = DataInfo<double>::Name;

   // Guardo las clases para configurarlas en la salida
   DatasourceInterface* pdatasource = DatasourceInterface::Create(
                                          "RasterDatasource", pRasterElement);
   ClassifiedRasterDatasourceManipulator manipulator;
   classInformation_ =  manipulator.GetClassInformationList(pdatasource);
   DatasourceInterface::Destroy(pdatasource);
   return true;
}

/**
 * Obtiene la cantidad de bandas selecionadas. Crea un nodo de renderizacion, le
 * pasa la cantidad y bandas y si el filtro es de deteccion de bordes devuelve
 * una lista con el doble de bandas (repeticion de cada banda seleccionada).
 * Invoca al metodo que agrega el nodo de renderizacion de bandas, le agrega
 * el nodo de filtro, obtiene el nodo de ecuacion de bandas y si existe lo
 * agrega al nodo de filtro.
 * @param[in] pRaster elemento raster al que se le setean las opciones
 */
void FilterProcess::SetFilterOptions(RasterElement *pRaster) {
   if (!pRaster || !HasValidSpectralData())
      return;

   std::vector<int> bands = GetSelectedRasterBands();
   wxXmlNode *pfilternode = NULL;
   pAdaptLayer_->GetAttribute<wxXmlNode*>(FilterNodeKey, pfilternode);
   if (pfilternode) {
      wxXmlNode *prnode = pRaster->GetNode(wxT(RENDERIZATION_NODE));
      if (prnode) {
         pRaster->AddNode(prnode, pfilternode);
         wxXmlNode *pequationnode = NULL;
         pAdaptLayer_->GetAttribute<wxXmlNode*>(EquationNodeKey, pequationnode);
         if (pequationnode) {
            // Chequeo si el nodo tiene una ecuacion definida ya que con los filtros
            // de convolucion puede que no venga una.
            if (HasEquation(pequationnode) == true)
               pRaster->AddNode(prnode, pequationnode);
         }
         // Si el tipo de filtro es FilterSelectionPart::EdgeDetectionFilter
         // duplico las bandas a la entrada.
         FilterType ftype = LowPassFilter;
         pAdaptLayer_->GetAttribute<FilterType>(FilterTypeKey, ftype);
         if (ftype == EdgeDetectionFilter) {
            // Obtiene el nodo combinacion de bandas
            std::vector<int> selectedbands = GetSelectedRasterBands();

            std::vector<int> bandlistdoubled;
            std::vector<int>::iterator iterbands = selectedbands.begin();
            for (; iterbands != selectedbands.end(); iterbands++) {
               bandlistdoubled.push_back(*iterbands);
               bandlistdoubled.push_back(*iterbands);
            }
            selectedbands.assign(bandlistdoubled.begin(), bandlistdoubled.end());

            std::string bands = join(selectedbands.begin(), selectedbands.end());

            wxXmlNode *pcombinationnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                                        wxT(BAND_COMBINATION_NODE));
            pcombinationnode->AddChild(
                  new wxXmlNode(pcombinationnode, wxXML_TEXT_NODE, wxEmptyString,
                                wxT(bands)));

            // Agrega el nodo combinacion al nodo renderizacion
            pRaster->AddNode(prnode, pcombinationnode);
         }
      }
   }
}

/**
 * Guarda en la imagen de salida la tabla de colores de la entrada.
 * @return informa si pudo configurar elemento
 */
bool FilterProcess::ConfigureOutput() {
   if (!FileExporterProcess::ConfigureOutput())
      return false;

   Element* pNewElement = NULL;
   //pNewElement es inicializado por FileExporterProcess
   pAdaptLayer_->GetAttribute<Element*>(
               ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
   DatasourceInterface* pdatasource = DatasourceInterface::Create(
                                          "RasterDatasource", pNewElement);

   if (flags_ == POSTCLASSIFICATION_FILTER) {
      // Genero tabla de colores con clases seleccionadas
      ClassifiedRasterDatasourceManipulator manipulator;
      manipulator.AddNature(pdatasource);

      /**
       * Codigo igual al de classification process. 
       * Refactor pendiente
       */
      ColorTable colortable;
      colortable.Activate(true);
      ClassInformation::IndexType lastindex = 0;
      VectorStyle::Color lastcolor = 0;
      for (size_t i = 0; i < classInformation_.size(); i++) {
         colortable.AddColor(classInformation_[i].GetIndex(),
                             classInformation_[i].GetColor().red_,
                             classInformation_[i].GetColor().green_,
                             classInformation_[i].GetColor().blue_);
         manipulator.AddClassInformation(&classInformation_[i], pdatasource, true);

         if (lastindex < classInformation_[i].GetIndex()) {
            lastcolor = classInformation_[i].GetColor();
            lastindex = classInformation_[i].GetIndex();
         }
      }
      colortable.AddColor(std::numeric_limits<double>::max(),  lastcolor.red_,
                                            lastcolor.green_, lastcolor.blue_);

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
   }

   if (pDatasourceManager_)
      pDatasourceManager_->AddDatasource(pdatasource);

   ExportMetadataToFile(pdatasource, pdatasource->GetElement()->GetUrl().c_str());
   return true;
}

/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
void FilterProcess::DoOnProcessAdaptLayerSet() {
   FileExporterProcess::DoOnProcessAdaptLayerSet();
   std::vector<int> bands = GetSelectedRasterBands();
   pAdaptLayer_->AddAttribute<std::vector<int> >(ProcessAdaptLayer::SelectedBandsKey,
                                                 bands);
   pAdaptLayer_->AddAttribute<int>(FilterProcess::FilterFlagsKey, flags_);
   // Se indica que no es necesario el part de seleccion espectral
   pAdaptLayer_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr,
                                    showSpectralSelectionPart_);
}   
/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> FilterProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;
   wxXmlNode *pfilternode = NULL;
   if (pAdaptLayer_->GetAttribute<wxXmlNode*>(FilterNodeKey, pfilternode))
      params.push_back(CreateKeyValueNode("configuracion-filtro", Xml2String(pfilternode)));
   
   wxXmlNode *pequationnode = NULL;
   pAdaptLayer_->GetAttribute<wxXmlNode*>(EquationNodeKey, pequationnode);
   params.push_back(CreateKeyValueNode("ecuacion-filtro", Xml2String(pequationnode)));
   FilterType ftype = LowPassFilter;
   pAdaptLayer_->GetAttribute<FilterType>(FilterTypeKey, ftype);
   params.push_back(CreateKeyValueNode("filtro", FilterProcess::FilterTypeString[ftype]));
   return params;
}

/**
 * Verifica si el nodo contiene una ecuacion definida.
 */
bool FilterProcess::HasEquation(const wxXmlNode* pNode) const {
   bool hasequ = true;
   wxXmlNode* pchild = pNode->GetChildren();
   while (pchild != NULL) {
      if (pchild->GetName().Cmp("ecuacion") == 0) {
         wxString equ = pchild->GetNodeContent();
         if (equ.Len() == 0) {
            hasequ = false;
            break;
         }
      }
      pchild = pchild->GetChildren();
   }
   return hasequ;
}

}  // namespace

