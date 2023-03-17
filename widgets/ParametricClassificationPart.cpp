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
#include <list>
#include <string>

// Includes Suri
#include "ParametricClassificationPart.h"
#include "ClassSelectionPart.h"
#include "suri/LayerList.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/AuxiliaryFunctions.h"
#include "ParametersSelectionPart.h"
#include "NonSupervisedParameterSelectionPart.h"
#include "suri/VectorElement.h"
#include "MaxLikelihoodPart.h"
#include "ClassificationRenderer.h"
#include "MovingWindowController.h"
#include "StatsCanvas.h"
#include "suri/VectorStyleManager.h"
#include "suri/Progress.h"
#include "suri/XmlFunctions.h"
#include "SpatialOperations.h"
#include "suri/Extent.h"
#include "Clusters.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "EnclosureValidator.h"
#include "EnclosureManipulator.h"
#include "FiltredVectorRenderer.h"
#include "Filter.h"
#include "suri/DataViewManager.h"
#include "suri/ClassificationProcess.h"
#include "suri/ClassStatisticsFileParser.h"
#include "SRStatisticsCanvas.h"
#include "Mask.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/choicebk.h"
// Includes App
// Defines
/** ID a partir del cual se enumeran las clases */
#define NO_CLASS_COUNT 1

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ParametricClassificationPartEvent, ParametricClassificationPart)
#ifdef __WXMSW__
IMPLEMENT_EVENT_CALLBACK(OnSize, OnSize(event); event.Skip(), wxSizeEvent)
IMPLEMENT_EVENT_CALLBACK(OnChoicePageChanged, OnPageChanged(event), wxChoicebookEvent)
#endif
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param[in] pElements lista de elementos
 * @param[in] pInputElement elemento sobre el que se realiza la clasificacion
 */
ParametricClassificationPart::ParametricClassificationPart(
      LayerList* pElements, Element* pInputElement, DataViewManager* pDataViewManager,
      int type) :
      ClassificationPart(), pClassSelectionPart_(NULL), pElements_(pElements),
      pInputElement_(pInputElement), pDataViewManager_(pDataViewManager),
      NEW_EVENT_OBJECT(ParametricClassificationPartEvent) {
   windowTitle_ = wxT(caption_CLASSIFICATION_OPTIONS);
   selectedAlgorithm_ = -1;
   clasificationType_ = type;
}

/**
 * Destructor
 */
ParametricClassificationPart::~ParametricClassificationPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea la ventana de la parte. Carga en el panel interno la ventana
 * del ClassSelectionPart.
 * @return bool que indica si se pudo crear la ventana
 */
bool ParametricClassificationPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
   wxT("ID_CLASSIFICATION_OPTIONS"));

   if (clasificationType_ == Supervised) {
      ParametersSelectionPart::EnumerationType parameterparts =
            SupervisedParameterSelectionPart::Enumerate();
      ParametersSelectionPart::EnumerationType::iterator it = parameterparts.begin();
      while (it != parameterparts.end()) {
         SupervisedParameterSelectionPart* pparameterspart = NULL;
         pparameterspart = SupervisedParameterSelectionPart::Create(*it);
         if (pparameterspart) {
            pParametersSelectioPartList_.push_back(pparameterspart);
         }
         it++;
      }
      pClassSelectionPart_ = new ClassSelectionPart(pDataViewManager_,
                                    ClassSelectionPart::ImportClassSelectionButtonFlag |
                                    ClassSelectionPart::SelectionButtonFlag |
                                    ClassSelectionPart::ClearSelectionButtunFlag |
                                    ClassSelectionPart::TrainClassFilter);
      wxPanel *ppanel = XRCCTRL(*pToolWindow_,
            wxT("ID_CLASS_SELECTION_PART_CONTAINER"), wxPanel);
      AddControl(pClassSelectionPart_, ppanel);
   }

   // Agrego al choicebook las ventanas en pParametersSelectioPartList_.
   wxChoicebook *pchoicebook =
   XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   for (size_t i = 0; i < pParametersSelectioPartList_.size(); i++) {
      wxPanel *ptemppanel = new wxPanel(pchoicebook);
      ptemppanel->SetSizer(new wxBoxSizer(wxVERTICAL));
      pchoicebook->AddPage(ptemppanel,
                           pParametersSelectioPartList_[i]->GetAlgorithmName());
      AddControl(pParametersSelectioPartList_[i]->GetWidget(), ptemppanel);
   }

   // Seteo ventana
   SetInitialValues();

#ifdef __WXMSW__
   /**
    * El algoritmo de layout de wx falla en windows si existe una jerarquia de
    * ventanas muy profunda(empeora en 64 bits). Es un problema reconocido en
    * wx, ver TCK #2572 para una descripcion mas completa.
    * En sopi esto producia que no se actualizaran los viewers al hacer un resize
    * de la ventana. En OnSize se ejecuta codigo que parchea el bug de
    * windows/wx
    */
   if (wxPlatformInfo::Get().GetArchitecture() == wxARCH_64) {
      XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_OPTIONS_PANEL"), wxPanel)->Connect(
            wxEVT_SIZE, wxSizeEventHandler(ParametricClassificationPartEvent::OnSize),
            NULL, pEventHandler_);
      XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_OPTIONS_PANEL"), wxPanel)->Connect(
            wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED,
            wxChoicebookEventHandler(ParametricClassificationPartEvent::OnChoicePageChanged), NULL,
            pEventHandler_);
   }
#endif
   return true;
}

/**
 * Devuelve el estado del Part.
 * @return true si esta habilitado
 */
bool ParametricClassificationPart::IsEnabled() {
   return enabled_;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return true si el part tiene cambios para salvar, false en otro caso
 */
bool ParametricClassificationPart::HasChanged() {
   bool haschanged = false;
   if (pClassSelectionPart_) {
      haschanged = pClassSelectionPart_->HasChanged();
   }

   // Analizo si se modifico el part activo
   wxChoicebook *pchoicebook =
   XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   int activepart = pchoicebook->GetSelection();
   if (selectedAlgorithm_ != activepart) {
      haschanged = true;
   } else {
      haschanged = haschanged || pParametersSelectioPartList_[activepart]->HasChanged();
   }

   return haschanged;
}

/**
 * Salva los cambios realizados en el Part. Al aplicar solo actualiza los
 * datos de los parametros desplegados.
 * @return informa si tuvo exito
 */
bool ParametricClassificationPart::CommitChanges() {
   if (!HasValidData()) {
      return false;
   }

   bool validvectors = true;
// Analizo si todos los vectores seleccionados estan dentro del raster
   pClassSelectionPart_->CommitChanges();
   std::list<ClassificationClass*> selectedelements =
         pClassSelectionPart_->GetSelectedClasses();
   std::list<ClassificationClass*>::iterator it = selectedelements.begin();
   for (; it != selectedelements.end(); ++it) {
      if (!(*it)->GetClassAsElement())
         continue;
      VectorElement* pvectorelement =
            dynamic_cast<VectorElement*>((*it)->GetClassAsElement());
      if (pvectorelement) {
         bool isinside = false;
         // Si el elemento es invalido o no esta dentro del raster no se puede usar
         // para clasificar
         if (!pvectorelement->IsValid()
               || !AnalizeIfVectorInsideRaster(pInputElement_, pvectorelement,
                                               isinside)) {
            SHOW_ERROR(_(message_INVALID_CLASS_s), pvectorelement->GetName().c_str());
            validvectors = false;
         } else if (!isinside) {
            SHOW_ERROR(_(message_INVALID_VECTOR_EXTEND_s),
                       pvectorelement->GetName().c_str());
            validvectors = false;
         }
      }
   }

   if (!validvectors) {
      return false;
   }

   // Aplico los cambios al part
   wxChoicebook *pchoicebook =
   XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   int activepart = pchoicebook->GetSelection();
   if (activepart >= 0) {
      selectedAlgorithm_ = activepart;
      pClassSelectionPart_->CommitChanges();
      pParametersSelectioPartList_[activepart]->CommitChanges();
   }
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return true si tuvo exito (en este caso siempre retorna true).
 */
bool ParametricClassificationPart::RollbackChanges() {
   // Analizo si se modifico el part activo
   wxChoicebook *pchoicebook = XRCCTRL(*pToolWindow_,
         wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   if (selectedAlgorithm_ >= 0) {
      pchoicebook->SetSelection(selectedAlgorithm_);
      pParametersSelectioPartList_[selectedAlgorithm_]->RollbackChanges();
   }
   pClassSelectionPart_->RollbackChanges();
   return true;
}

/**
 * Inicializa los datos mostrados
 */
void ParametricClassificationPart::SetInitialValues() {
   wxChoicebook *pchoicebook =
   XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   pchoicebook->SetSelection(0);
   selectedAlgorithm_ = 0;
   for (size_t i = 0; i < pParametersSelectioPartList_.size(); i++)
      pParametersSelectioPartList_[i]->SetInitialValues();
   pClassSelectionPart_->SetInitialValues();
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void ParametricClassificationPart::GetWindowIcon(wxBitmap& ToolBitmap, int X,
                                                 int Y) const {
   GET_BITMAP_RESOURCE(icon_CLASSIFICATION_PARAMETERS, ToolBitmap);
}

/**
 * Informa si las partes que componen el part tienen datos validos (seleccion de
 * clases, y seleccion de parametros relacionados al algoritmo).
 * @return true si tiene datos validos, false en otro caso
 */
bool ParametricClassificationPart::HasValidData() {
   bool validdata = false;
   wxChoicebook *pchoicebook =
   XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   int activepart = pchoicebook->GetSelection();
   if (activepart >= 0) {
      validdata = true;
      validdata = validdata && pClassSelectionPart_->HasValidData();
      validdata = validdata && pParametersSelectioPartList_[activepart]->HasValidData();
   }
   return validdata;
}

/**
 * Retorna la cantidad de clases
 * @return cantidad de clases
 * \pre Se debe llamar a GetClassificationRendererNode
 */
int ParametricClassificationPart::GetClassCount() {
   if (selectedAlgorithm_ >= 0) {
      return pClassSelectionPart_->GetSelectedClasses().size();
   }
   return 0;
}

/**
 * Carga en vector las propiedades de las clases cargadas.
 * @param[out] Properties vector donde se cargan las porpiedades de las
 *             clases que se usan para clasificar.
 * @return true si la cantidad de clases es mayor a cero, false en otro caso
 * \pre Se debe llamar a GetClassificationRendererNode
 */
bool ParametricClassificationPart::GetClassProperties(
      std::vector<ClassInformation> &Properties) {
   int classcount = GetClassCount();
   if (classcount > 0) {
      // Clase SIN CLASE.
      ClassInformation classproperties(ClassInformation::NoClassIndex);
      pParametersSelectioPartList_[selectedAlgorithm_]->GetNonClassifiedPixelInfo(
            classproperties);
      Properties.push_back(classproperties);

      // Clase valor no valido.
      ClassInformation ndvclassproperties(ClassInformation::NDVIndex, ClassInformation::NDVName);
      Properties.push_back(ndvclassproperties);

      // Agrego propiedades de los elementos en el
      std::list<ClassificationClass*> selectedelements =
            pClassSelectionPart_->GetSelectedClasses();
      std::list<ClassificationClass*>::iterator itelements = selectedelements.begin();
      for (int i = 1; itelements != selectedelements.end() && i < classcount + 1;
            ++i, ++itelements) {
         // Completo con color de vector de la clase
         // Obtengo color del elemento activo
         std::string name, id;
         VectorStyle::Color color;
         if ((*itelements)->GetClassAsElement()) {
            Element* pdselem = (*itelements)->GetClassAsElement();
             /**
             * Como la LayerList recibida tiene las capas del visualizador
             * activo, es necesario obtener el id del datasource asociado.
             * Se pasa LayerList de visualizador para que se tomen cambios
             * hechos por el usuario a estilo de vectores.
             */
            LayerInterface* player =
                  pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext()->
                  GetLayerByElementId(pdselem->GetUid());
            VectorElement* pvelem = dynamic_cast<VectorElement*>(pdselem);
            /** 
            * TCK #7264: Para que tome realmente la configuracion visual se debe utilizar la capa
            * y no la fuente de datos
            **/
           if (player) {
               id = player->GetAssociatedDatasourceId();
               name = player->GetElement()->GetName().c_str();
               pvelem = dynamic_cast<VectorElement*>(player->GetElement());
            }
            std::string style = pvelem->GetStyle(pvelem->GetActiveLayer());
            VectorStyle * pvectorstyle = VectorStyle::Create(style);
            color = pvectorstyle->GetSymbol()->color_;
            if (pvectorstyle->GetBrush()
                  && pvectorstyle->GetBrush()->id_ != VectorStyle::Brush::Transparent
                  && (pvectorstyle->GetBrush()->color_.red_ != 0
                        || pvectorstyle->GetBrush()->color_.blue_ != 0
                        || pvectorstyle->GetBrush()->color_.green_ != 0)) {
               color = pvectorstyle->GetBrush()->color_;
            } else if (pvectorstyle->GetPen()
                  && (pvectorstyle->GetPen()->color_.red_ != 0
                        || pvectorstyle->GetPen()->color_.blue_ != 0
                        || pvectorstyle->GetPen()->color_.green_ != 0)) {
               color = pvectorstyle->GetPen()->color_;
            }
            VectorStyle::Destroy(pvectorstyle);
            std::string id = pvelem->GetUid().c_str();
         } else {
            ClassStatisticsFileParser::ClassStatisticsInfo* pclassinfo = (*itelements)->GetClass();
            name = pclassinfo->name_;
            id = SuriObject::CreateId();
            color = VectorStyle::Color(rand() % 255, rand() % 255, rand() % 255);
         }

         ClassInformation classproperties(i, name, color, id);

         Properties.push_back(classproperties);
      }
   }
   return classcount > 0;
}

/**
 * Retorna el nodo que se debe agregar al elemento para clasificar.
 * Crea un ClassificationRenderer::Parameters y le asigna el
 * ClasificationAlgorithm que retorna el part seleccionado. Utiliza
 * GetXmlNode para generar nodo.
 * @param[in] pRasterElement raster que se desea clasificar
 * @return Nodo de clasificador.
 */
wxXmlNode* ParametricClassificationPart::GetClassificationRendererNode(
      RasterElement* pRasterElement) {
   wxChoicebook *pchoicebook =
   XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   int activepart = pchoicebook->GetSelection();

   /*
    INCIO CODIGO PARCHE:
    Es necesario castear el algoritmo a ClusterClassificationAlgorithm* para
    que se puedan configurar los recintos seleccionados en pClassSelectionPart_
    Ver TCK #856
    */
   // Obtengo cluster del algoritmo seleccionado
   ClusterClassificationAlgorithm* pclusteralgorithm =
         dynamic_cast<ClusterClassificationAlgorithm*>(
                        pParametersSelectioPartList_[activepart]->GetClassificationManager());
   if (!pclusteralgorithm) {
      // Genero nodo de algoritmo
      ClassificationRenderer::Parameters params;
      params.pAlgorithm_ =
            pParametersSelectioPartList_[activepart]->GetClassificationManager();

      return ClassificationRenderer::GetXmlNode(params);
   }
   Clusters* pclusters = pclusteralgorithm->GetClusters();
   pclusters->Clean();

   /* FIN CODIGO PARCHE */

   // Cargo clases
   std::list<ClassificationClass*> selectedelements =
         pClassSelectionPart_->GetSelectedClasses();
   std::list<ClassificationClass*>::iterator it = selectedelements.begin();

   // Despliego barra de progreso
   Progress progression(selectedelements.size(), wxT(message_RENDERING_PROGRESS));

   // Recorro vectores y calculo las estadisticas para cada una
   /** Como indice inicial se utiliza el consecutivo luego de el indice de sin clase **/
   int classid = 1;
   while (it != selectedelements.end()) {
      if ((*it)->GetClassAsElement()) {
         VectorElement* pvectorelement =
               dynamic_cast<VectorElement*>((*it)->GetClassAsElement());
         bool isinside = false;
         if (pvectorelement
               && AnalizeIfVectorInsideRaster(pRasterElement, pvectorelement, isinside)
               && isinside) {
            bool isinside = false;
            if (!AnalizeIfVectorInsideRaster(pRasterElement, pvectorelement,
                                             isinside)) {
               SHOW_ERROR(_(message_INVALID_CLASS_s));
            }
            // Calculo estadisticas
            suri::raster::data::StatisticsBase* pstats = NULL;
            if (CalculateStatistics(pRasterElement, pvectorelement, pstats)) {
               // agrego clase
               pclusters->AddCluster(classid, pstats);
               classid++;
            } else {
               pvectorelement->Activate(false);
            }
         } else {
            REPORT_DEBUG(
                  "D: Error. Elemento en lista que no es VectorElement o fuera de la imagen");
         }
      } else {
         // TODO: Actualizar a estadisticas nuevas
         ClassStatisticsFileParser::ClassStatisticsInfo* pclassinfo = (*it)->GetClass();
         pclusters->AddCluster(classid, pclassinfo->pStatistics_);
         ++classid;
      }

      progression.Update();
      it++;
   }

   // Genero nodo de algoritmo
   ClassificationRenderer::Parameters params;
   params.pAlgorithm_ =
         pParametersSelectioPartList_[activepart]->GetClassificationManager();

   return ClassificationRenderer::GetXmlNode(params);
}

/**
 * Calcula las estadisticas del poligono.
 * @param[in] pRasterElement raster con el que se van a calcular las estadisticas
 * @param[in] pPolygon vector que se usara como mascara
 * @param[out] Statistics estadisticas calculadas
 * @return informa is la operacion se realizo exitosamente
 */
bool ParametricClassificationPart::CalculateStatistics(RasterElement* pRasterElement,
                                                       VectorElement* pPolygon,
                                                       Statistics& Statistics) {
   // Valido que el vector corresponda a un recinto
   LayerInterface* player =
         pDataViewManager_->GetViewcontextManager()->GetAssociatedLayerByElementId(
               pPolygon->GetUid());
   if (!player)
      return false;
   DatasourceInterface* pdatasource =
         pDataViewManager_->GetViewcontextManager()->GetAssociatedDatasource(
               player->GetId());
   EnclosureValidator validator;
   EnclosureManipulator manipulator;
   if (!validator.IsValid(pdatasource))
      return false;
   EnclosureInformation enclosureinfo = manipulator.GetEnclosureInformation(
         pdatasource);

   // Busco nodo combinacion de bandas en Raster
   wxXmlNode* pbandcombinationnode = GetNodeByName(
   wxT(RENDERIZATION_NODE "|" BAND_COMBINATION_NODE),
                                                   pRasterElement->GetNode(""));
   if (!pbandcombinationnode)
      return false;

#ifdef __UNUSED_CODE__
   // NOTA: Analizar si tiene sentido usar interpolacion en estadisticas
   // Busco nodo de interpolado en Raster
   wxXmlNode* pinterpolationnode = GetNodeByName(
         wxT(RENDERIZATION_NODE "|" INTERPOLATION_NODE), pRasterElement->GetNode(""));
#endif

   // Cargo el vector verificando que sea valido.
   VectorElement *pvectorelement = dynamic_cast<VectorElement*>(Element::Create(
         "VectorElement", pPolygon->GetNode("")));
   if (!pvectorelement) {
      return false;
   }

   // Creo raster que se usara para clasificar
   Element *prasterelement = Element::Create("RasterElement",
                                             pRasterElement->GetNode(""));
   prasterelement->Activate();

   // Limpio el nodo renderizacion del nuevo raster
   wxXmlNode* pnewrenderizationnode = GetNodeByName(wxT(RENDERIZATION_NODE),
                                                    prasterelement->GetNode(""));
   wxXmlNode* pchildren = pnewrenderizationnode->GetChildren();
   while (pchildren) {
      pnewrenderizationnode->RemoveChild(pchildren);
      pchildren = pnewrenderizationnode->GetChildren();
   }

   // Agrego nodo combinacion de bandas
   pnewrenderizationnode->AddChild(new wxXmlNode(*pbandcombinationnode));
   if (!pnewrenderizationnode->GetChildren()) {
      Element::Destroy(prasterelement);
      return false;
   }

#ifdef __UNUSED_CODE__
   // NOTA: Analizar si tiene sentido usar interpolacion en estadisticas
   // Agrego nodo de interpolado
   if (pinterpolationnode)
   pnewrenderizationnode->GetChildren()->SetNext(new wxXmlNode(*pinterpolationnode));
#endif

   // Creo canvas y MovingWindowController
   StatsCanvas *poutcanvas = new StatsCanvas;
   // Las estadisticas necesarias con la media y la matriz de covarianza
   poutcanvas->SetRequiredStats(
         Statistics::Mean | Statistics::CovarianceMatrix | Statistics::Variance
               | Statistics::MaxValue | Statistics::MinValue);

   MovingWindowController * pcontroller = new MovingWindowController();

   // Cargo lista a renderizar
   LayerList* pelementlist = new LayerList();
   pelementlist->AddElement(prasterelement);
   // Configuro el poligono con mascara
   LayerList* pmasklist = new LayerList();

   // Cargo estilo mascara al vector
   pvectorelement->SetStyle(
         VectorStyleManager::Instance().GetDefaultStyle("mask", Vector::Polygon)->GetWkt());
   Filter filter(enclosureinfo.GetQuery());
   FiltredVectorRenderer::SetFilter(&filter, pvectorelement->GetNode());
   pvectorelement->Activate();
   pmasklist->AddElement(pvectorelement);

   // Le paso la imagen completa como mundo
   World* pworld = new World;
   Subset worldextent;
   prasterelement->GetElementExtent(worldextent);
   int width, height;
   pRasterElement->GetRasterSize(width, height);
   pworld->SetSpatialReference(prasterelement->GetSpatialReference().c_str());
   pworld->SetWorld(worldextent);
   pworld->SetWindow(worldextent);
   pworld->SetViewport(width, height);

   // Seteo canvas que calcula estadisticas.
   pcontroller->SetRenderizationList(pelementlist);
   pcontroller->SetMaskList(pmasklist);
   pcontroller->SetWorld(pworld);
   pcontroller->SetOutputCanvas(poutcanvas);
   pcontroller->SetBestBufferSize();

   // Renderizo y retorno las estadisticas.
   bool renderizationstatus = pcontroller->Render();
   if (renderizationstatus) {
      Statistics = poutcanvas->GetStatistics();
   }

   // Elimino objetos usados para calcular estadisticas
   pcontroller->SetRenderizationList(NULL);
   pcontroller->SetMaskList(NULL);
   pcontroller->SetWorld(NULL);
   pcontroller->SetOutputCanvas(NULL);

   delete pworld;
   delete pelementlist;
   delete pmasklist;
   delete pcontroller;
   delete poutcanvas;
   Element* pelement = pvectorelement;
   Element::Destroy(prasterelement);
   Element::Destroy(pelement);

   return renderizationstatus;
}

/** Calcula las estadisticas del poligono */
bool ParametricClassificationPart::CalculateStatistics(RasterElement* pRasterElement,
                                 VectorElement* pPolygon,
      suri::raster::data::StatisticsBase* &pStatistics) {
   // Valido que el vector corresponda a un recinto
   LayerInterface* player =
         pDataViewManager_->GetViewcontextManager()->GetAssociatedLayerByElementId(
               pPolygon->GetUid());
   if (!player)
      return false;
      
   DatasourceInterface* pdatasource =
         pDataViewManager_->GetViewcontextManager()->GetAssociatedDatasource(
               player->GetId());
   EnclosureValidator validator;
   EnclosureManipulator manipulator;
   if (!validator.IsValid(pdatasource))
      return false;

   // Busco nodo combinacion de bandas en Raster
   wxXmlNode* pbandcombinationnode = GetNodeByName(
   wxT(RENDERIZATION_NODE "|" BAND_COMBINATION_NODE),
                                                   pRasterElement->GetNode(""));
   if (!pbandcombinationnode)
      return false;

#ifdef __UNUSED_CODE__
   // NOTA: Analizar si tiene sentido usar interpolacion en estadisticas
   // Busco nodo de interpolado en Raster
   wxXmlNode* pinterpolationnode = GetNodeByName(
         wxT(RENDERIZATION_NODE "|" INTERPOLATION_NODE), pRasterElement->GetNode(""));
#endif


   // Creo raster que se usara para clasificar
   Element *prasterelement = Element::Create("RasterElement",
                                             pRasterElement->GetNode(""));
   prasterelement->Activate();

   // Limpio el nodo renderizacion del nuevo raster
   wxXmlNode* pnewrenderizationnode = GetNodeByName(wxT(RENDERIZATION_NODE),
                                                    prasterelement->GetNode(""));
   wxXmlNode* pchildren = pnewrenderizationnode->GetChildren();
   while (pchildren) {
      pnewrenderizationnode->RemoveChild(pchildren);
      pchildren = pnewrenderizationnode->GetChildren();
   }

   // Agrego nodo combinacion de bandas
   pnewrenderizationnode->AddChild(new wxXmlNode(*pbandcombinationnode));
   if (!pnewrenderizationnode->GetChildren()) {
      Element::Destroy(prasterelement);
      return false;
   }

#ifdef __UNUSED_CODE__
   // NOTA: Analizar si tiene sentido usar interpolacion en estadisticas
   // Agrego nodo de interpolado
   if (pinterpolationnode)
   pnewrenderizationnode->GetChildren()->SetNext(new wxXmlNode(*pinterpolationnode));
#endif

   // Creo canvas y MovingWindowController
   suri::render::StatisticsCanvas* poutcanvas = new suri::render::StatisticsCanvas(true);
   poutcanvas->SetNoDataValue(Mask::Transparent);
   poutcanvas->SetNoDataValueAvailable(true);
   MovingWindowController * pcontroller = new MovingWindowController();

   // Cargo lista a renderizar
   LayerList* pelementlist = new LayerList();
   pelementlist->AddElement(prasterelement);
   // Configuro el poligono con mascara
   LayerList* pmasklist = new LayerList();
   // Cargo el vector verificando que sea valido.
   VectorElement *pvectorelement = dynamic_cast<VectorElement*>(Element::Create(
         "VectorElement", pPolygon->GetUrl().c_str()));
   if (!pvectorelement) {
      return false;
   }
   EnclosureInformation enclosureinfo = manipulator.GetEnclosureInformation(
         pdatasource);
   Filter filter(enclosureinfo.GetQuery());
   FiltredVectorRenderer::SetFilter(&filter, pvectorelement->GetNode());
   // Cargo estilo mascara al vector
   pvectorelement->SetStyle(
         VectorStyleManager::Instance().GetDefaultStyle("mask", Vector::Polygon)->GetWkt());
   pvectorelement->Activate();
   pmasklist->AddElement(pvectorelement);

   // Le paso la imagen completa como mundo
   World* pworld = new World;
   Subset worldextent;
   prasterelement->GetElementExtent(worldextent);
   int width, height;
   pRasterElement->GetRasterSize(width, height);
   pworld->SetSpatialReference(prasterelement->GetSpatialReference().c_str());
   pworld->SetWorld(worldextent);
   pworld->SetWindow(worldextent);
   pworld->SetViewport(width, height);

   // Seteo canvas que calcula estadisticas.
   pcontroller->SetRenderizationList(pelementlist);
   pcontroller->SetMaskList(pmasklist);
   pcontroller->SetWorld(pworld);
   pcontroller->SetOutputCanvas(poutcanvas);
   pcontroller->SetBestBufferSize();

   // Renderizo y retorno las estadisticas.
   bool renderizationstatus = pcontroller->Render();
   if (renderizationstatus) {
      pStatistics = poutcanvas->GetStatistics();
   }

   // Elimino objetos usados para calcular estadisticas
   pcontroller->SetRenderizationList(NULL);
   pcontroller->SetMaskList(NULL);
   pcontroller->SetWorld(NULL);
   pcontroller->SetOutputCanvas(NULL);

   delete pworld;
   delete pelementlist;
   delete pmasklist;
   delete pcontroller;
   delete poutcanvas;
   Element* pelement = pvectorelement;
   Element::Destroy(prasterelement);
   Element::Destroy(pelement);

   return renderizationstatus;
}

/**
 * Verifica si un vector esta completamente dentro del raster. Intersecta
 * extends de vector y raster y lo compara contra el extend del vector
 * en sistema de coordenadas del mundo.
 * @param[in] pRasterElement imagen que se quiere analizar
 * @param[in] pVectorElement vector que se quiere analizar
 * @param[out] InsideRaster informa si el vector esta completamente dentro
 * del raster
 * @return Informa si pudo realizar la comparcion
 */
bool ParametricClassificationPart::AnalizeIfVectorInsideRaster(
      Element* pRasterElement, VectorElement* pVectorElement, bool &InsideRaster) {
   // Encuentro el extent del vector en sistema de referencia del raster
   Subset vectorextent;
   pVectorElement->GetElementExtent(vectorextent);
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pVectorElement->GetSpatialReference().c_str());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pRasterElement->GetSpatialReference().c_str());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (pct == NULL) {
      REPORT_DEBUG("D: No se pudo crear coordinates transform.");
      return false;
   }
   pct->Transform(vectorextent, _SPATIAL_OPERATIONS_EXTENT_POINTS_);
   delete pct;

   // Caluclo interseccion
   Extent intersection(0, 0, 0, 0);
   if (!IntersectExtents(pRasterElement, pVectorElement, intersection)) {
      REPORT_DEBUG("D: No se pudo crear coordinates transform.");
      return false;
   }

   InsideRaster = (intersection == Extent(vectorextent));
   return true;
}

#ifdef __WXMSW__
/** evento OnSize */
void ParametricClassificationPart::OnSize(wxSizeEvent& Event) {
   wxWindow* pwindow = XRCCTRL(*pToolWindow_, wxT("ID_CLASSIFICATION_OPTIONS_PANEL"), wxPanel);
   pwindow->Layout();

   // FixSizers no funciona bien si hay un Choicebook, este codigo corrige el error
   wxChoicebook *pchoicebook = XRCCTRL(*pToolWindow_,
         wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   wxSize windowsize = pchoicebook->GetClientSize();
   pchoicebook->SetPageSize(windowsize);

   FixSizers(pwindow, "");
}

/** Evento que se dispara cuando se cambio de pagina del book */
void ParametricClassificationPart::OnPageChanged(wxBookCtrlBaseEvent &Event) {
   wxChoicebook *pchoicebook = XRCCTRL(*pToolWindow_,
         wxT("ID_CLASSIFICATION_PARAMETERS"), wxChoicebook);
   FixSizers(pchoicebook, "");
}

#endif

}  // namespace suri
