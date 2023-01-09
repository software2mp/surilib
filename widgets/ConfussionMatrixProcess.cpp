/*! \file ConfussionMatrixProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>
#include <map>

// Includes Suri
#include "ClassInformation.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "ClassMappingPart.h"
#include "ConfusionMatrixReport.h"
#include "ConfusionMatrixTextConverter.h"
#include "EnclosureManipulator.h"
#include "EnclosureValidator.h"
#include "FiltredVectorRenderer.h"
#include "Filter.h"
#include "HtmlReportWidget.h"
#include "resources.h"
#include "StatisticsReport.h"
#include "StatsCanvas.h"
#include "SpatialSelectionPart.h"
#include "suri/HtmlViewerPart.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataViewManager.h"
#include "suri/ConfussionMatrixProcess.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/RasterElement.h"
#include "suri/StatisticsFunctions.h"
#include "suri/VectorElement.h"
#include "suri/VectorStyle.h"
#include "suri/VectorStyleManager.h"

// Includes Wx

// Defines
#define KEYVALUE_SEPARATOR '='
#define ITEM_SEPARATOR '|'

// forwards

namespace suri {
/** Nombre del proceso **/
const std::string ConfussionMatrixProcess::kProcessName = "ConfussionMatrixProcess";
/** Atributo para la seleccion de matriz de confusion */
const std::string ConfussionMatrixProcess::TestResultMapKeyAttr = "TestResultMap";

/** Constructor **/
ConfussionMatrixProcess::ConfussionMatrixProcess(
      Element *pElement, const Subset &WindowSubset, DataViewManager* pDataViewManager,
      Widget* &pStaticsWidget, StatisticsExporterProcess::OutputType output) :
      StatisticsExporterProcess(pElement, WindowSubset, pDataViewManager,
                                pStaticsWidget, output),
      pElement_(pElement),
      pDatasourceManager_(
            pDataViewManager ? pDataViewManager->GetDatasourceManager() : NULL),
      enclosureDatasource_(0),
      pReport_(NULL) {
   SetProcessName(kProcessName);
}

/** Destructor **/
ConfussionMatrixProcess::~ConfussionMatrixProcess() {
}

/** Corre el proceso y genera la salida usando los metodos de configuracion.
 *  Para cada recinto se corre un proceso para obtener el histograma. Luego
 *  se componene los histogramas generando la matriz de confusion
 *  Importante: Se presupone que los ids de los recintos proviene de los elementos
 *  y no de la fuente de datos (la clasificacion trabaja a nivel element y no a
 *  nivel datasource, con lo cual los ids resultantes son ids de elements. */
bool ConfussionMatrixProcess::RunProcess() {
   resultMap_ = std::vector< std::map<std::string, DatasourceInterface*> >(2);
   pAdaptLayer_->GetAttribute<std::map<std::string, DatasourceInterface*> >(
         TestResultMapKeyAttr, resultMap_[0]);

   // Agrego los url de los recintos asociados a las clases.
   EnclosureValidator validator;
   std::vector<DatasourceInterface*> enclosuredatasources;
   for (size_t i = 0; i < resultMap_.size(); ++i) {
      std::map<std::string, DatasourceInterface*>::iterator resultit = resultMap_[i].begin();
      for (; resultit != resultMap_[i].end(); ++resultit) {
         DatasourceInterface* pdatasource = resultit->second;
         // Si es valida como recinto entonces agrego el path del elemento a usar como mask
         if (pdatasource && validator.IsValid(pdatasource))
            enclosuredatasources.push_back(pdatasource);
      }
   }

   showStatistics_ = false;
   std::vector<DatasourceInterface*>::iterator enclosureit =
                                    enclosuredatasources.begin();
   while (enclosureit != enclosuredatasources.end()) {
      enclosureDatasource_ = *enclosureit;
      ++enclosureit;
      showStatistics_ = (enclosureit == enclosuredatasources.end());
      RasterProcess::RunProcess();
   }
   return true;
}

/** Muestra las estadisticas por pantalla */
bool ConfussionMatrixProcess::ConfigureOutput() {
   pStats_ = pStatsCanvas_->GetStatistics();

   // Si no me pidieron que calcule media, varianza, max y min las elimino
   // de estadisticas para que no se muestren en html.
   Statistics::StatisticsFlag statics = Statistics::None;
   pAdaptLayer_->GetAttribute<Statistics::StatisticsFlag>(SelectedStadisticsKeyAttr,
                                                          statics);
   if (pStats_ && (Statistics::Histogram)) {
      /** calculo de histograma **/
      suri::render::HistogramCanvas histcanvas;
      histcanvas.SetNoDataValue(pStatsCanvas_->GetNoDataValue());
      histcanvas.SetNoDataValueAvailable(pStatsCanvas_->IsNoDataValueAvailable());
      histcanvas.SetStatistics(pStats_);
      pRenderizationObject_->SetOutputCanvas(&histcanvas);
      if (pRenderizationObject_->Render()) {
         pHistogram_ = histcanvas.GetHistogram();
      }
      /** fin calculo de histograma **/
   }

   // Configuro opciones con datos de la imagen de entrada
   std::map<std::string, std::string> options;
   pAdaptLayer_->GetAttribute<std::map<std::string, std::string> >(
         StadisticsOutParamsKeyAttr, options);

   ConfigureOutputOptions(&options);

   // Creo ventana que despliega reporte de estadisticas
   if (pStats_)
      CreateHtmlViewerPart(pStats_, pHistogram_, options);

   return true;
}

/**
 * Configura las opciones requeridas para mostrar matriz confusion
 * @param[in] pOptions opciones configuradas
 */
void ConfussionMatrixProcess::ConfigureOutputOptions(
                               std::map<std::string, std::string>* pOptions) {
   // Paso el nombre de los enclosures asociados a cada histograma como opcion
   std::vector< std::vector<std::string> >enclosures(resultMap_.size());
   for (size_t i = 0; i < resultMap_.size(); ++i) {
      ConfusionMatrixPart::ResultMap::iterator enclosureit = resultMap_[i].begin();
      for ( ; enclosureit != resultMap_[i].end(); ++enclosureit)
         enclosures[i].push_back(enclosureit->second->GetName());
   }
   /** siempre deberian existir de entrenamiento y test **/
   (*pOptions)[ConfusionMatrixTextConverter::TEST_ENCLOSURES_OP] =
         join(enclosures[0].begin(), enclosures[0].end());

   // Obtengo las clases en la imagen de entrada
   DatasourceInterface* pdatasource = DatasourceInterface::Create(
                                          "RasterDatasource", GetInputElement());
   ClassifiedRasterDatasourceManipulator manipulator;
   std::vector<ClassInformation> classinformation =
              manipulator.GetClassInformationList(pdatasource);
   DatasourceInterface::Destroy(pdatasource);

   // Guardo las clases en opciones
   if (!classinformation.empty()) {
      std::vector<ClassInformation>::iterator classit = classinformation.begin();
      std::string classes = classit->GetName() + KEYVALUE_SEPARATOR + LongToString(classit->GetIndex());
      for (++classit; classit != classinformation.end(); ++classit) {
            if (classit->GetIndex() != ClassInformation::NDVIndex) {
               classes = classes + ITEM_SEPARATOR + classit->GetName() +
                      KEYVALUE_SEPARATOR + LongToString(classit->GetIndex());
            }
      }
      (*pOptions)["classes"] = classes;
   }
}

/** Devuelve una lista con el elemento que se quiere utilizar como mascara.*/
LayerList *ConfussionMatrixProcess::GetMaskList() {
   LayerList* plist = NULL;
   VectorElement *pvector = dynamic_cast<VectorElement*>(Element::Create(
                  "VectorElement", enclosureDatasource_->GetElement()->GetNode("")));
   if (!pvector)
      return NULL;

   // TODO(Javier - TCK #847): Desharcodear el nombre de la tabla de mascaras
   VectorStyleTable* pvectortable = VectorStyleManager::Instance().GetTable(
                                                      "mask", Vector::Polygon);
   // Se utiliza el estilo default de la tabla de mascaras
   std::string style = pvectortable->GetDefaultStyle()->GetWkt();
   pvector->SetStyle(style);

   EnclosureManipulator manipulator;
   EnclosureInformation enclosureinfo = manipulator.
                        GetEnclosureInformation(enclosureDatasource_);
   Filter filter(enclosureinfo.GetQuery());
   filter.RemoveEqualConstraint(0);
   filter.AddEqualConstraint(1);
   FiltredVectorRenderer::SetFilter(&filter, pvector->GetNode());

   plist = new LayerList;
   plist->AddElement(pvector);
   pvector->Activate();

   return plist;
}

/** A partir de la estadistica y las opciones genera el reporte**/
void ConfussionMatrixProcess::CreateHtmlViewerPart(Statistics& Statistics,
                                std::map<std::string, std::string>& Options) {
     if (!pStatisticsReport_) {
        pStatisticsReport_ = new HtmlReportWidget(_(caption_CONFUSION_MATRIX_REPORT));
        pReportWidget_ = pStatisticsReport_;
     }
     ConfusionMatrixReport* preport = new ConfusionMatrixReport();
     preport->SetStatistics(trainningStatistics_, ConfusionMatrixReport::TRAINNING_AREA);
     preport->SetStatistics(testStatistics_, ConfusionMatrixReport::TEST_AREA);
     preport->SetOutputOptions(Options);
     pStatisticsReport_->SetReport(preport);
}

/** A partir de la estadistica y las opciones genera el reporte**/
void ConfussionMatrixProcess::CreateHtmlViewerPart(suri::raster::data::StatisticsBase* pStatistics,
                                  suri::raster::data::HistogramBase* pHistogram,
                                  std::map<std::string, std::string>& Options) {
   if (!pStatisticsReport_) {
      pReport_ = new ConfusionMatrixReport();
      pStatisticsReport_ = new HtmlReportWidget(_(caption_CONFUSION_MATRIX_REPORT));
      pReportWidget_ = pStatisticsReport_;
      pStatisticsReport_->SetReport(pReport_);
      pReport_->SetOutputOptions(Options);
   }
   pReport_->AddStatistics(pStats_, pHistogram_, ConfusionMatrixReport::TEST_AREA);
}

/** Metodo que especializa la configuracion del ProcessAdaptLayer para
 *  los procesos especificos
 */
void ConfussionMatrixProcess::DoOnProcessAdaptLayerSet() {
   bool showspectral = false;
   pAdaptLayer_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr,
                                    showspectral);
   bool includemask = false;
   pAdaptLayer_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, includemask);
}
} /** namespace suri */
