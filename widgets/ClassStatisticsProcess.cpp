/*! \file ClassStatisticsProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
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
// Includes Wx
#include "suri/ClassStatisticsProcess.h"
#include "HtmlReportWidget.h"
#include "suri/VectorElement.h"
#include "suri/VectorStyleTable.h"
#include "suri/VectorStyleManager.h"
#include "suri/DatasourceInterface.h"
#include "Filter.h"
#include "EnclosureManipulator.h"
#include "FiltredVectorRenderer.h"
#include "StatsCanvas.h"
#include "ClassInformation.h"
#include "wx/file.h"

// Defines
// forwards

namespace suri {

/** Clave para identificar a las clases seleccionadas durante el proceso **/
const std::string ClassStatisticsProcess::SelectedClassesKeyAttr = "SelClasses";
/** numero del paso para areas de entrenamiento **/
const int ClassStatisticsProcess::TranningStatisticsStep = 1;
/** numero del paso para areas de test **/
const int ClassStatisticsProcess::TestStatisticsStep = 2;
/**
 * Constructor
 * @param[in] pElement elemento de entrada al proceso
 * @param[in] WindowSubset subset de entrada al proceso
 * @param[in] pDataViewManager administrador general de la aplicacion
 * @param[out] pStaticsWidget widget para la muestra de las estadisticas generadas
 * por el proceso
 */
ClassStatisticsProcess::ClassStatisticsProcess(Element *pElement,
                                               const Subset &WindowSubset,
                                               DataViewManager* pDataViewManager,
                                               Widget* &pStaticsWidget,
                                               StatisticsExporterProcess::OutputType output) :
      StatisticsExporterProcess(pElement, WindowSubset, pDataViewManager,
                                pStaticsWidget, output),
      statisticsStep_(0), pReport_(NULL), pCurrentEnclosure_(NULL), pCsvReport_(NULL) {
}

/** Destructor **/
ClassStatisticsProcess::~ClassStatisticsProcess() {
}

/**
 * Configura el proceso para que se calculen las estadisticas configuradas
 * @return siempre true
 */
bool ClassStatisticsProcess::ConfigureProcess() {
   pStatsCanvas_ = new suri::render::StatisticsCanvas(true);

   // TODO: Ver si en el caso de que el usuario configure un valor no valido deberia
   // utilizarce
   pStatsCanvas_->SetNoDataValueAvailable(true);
   pStatsCanvas_->SetNoDataValue(ClassInformation::InvalidClassIndex);

   Statistics::StatisticsFlag flags = Statistics::None;
   pAdaptLayer_->GetAttribute<Statistics::StatisticsFlag>(
         StatisticsExporterProcess::SelectedStadisticsKeyAttr, flags);

   pRenderizationObject_->SetOutputCanvas(pStatsCanvas_);

   MovingWindowController* paux = dynamic_cast<MovingWindowController*>(pRenderizationObject_);
   if (paux)
      paux->SetBestBufferSize();

   return true;
}

// ------ Metodos que administran la ejecucion del proceso ------
/** Corre el proceso y genera la salida usando los metodos de configuracion */
bool ClassStatisticsProcess::RunProcess() {
   /** 1. Obtengo las clases seleccionadas en el proceso
    *  2. Armo una lista de mascaras en funcion de las areas de entrenamiento
    *      SELEC * from layer where area = 1 (o algo asi)
    *  3. Corro el proceso de estadisticas
    *  4. Rearmo la lista de mascaras ahora con las areas de test
    *      SELEC * from layer where area = 0 (o algo asi)
    *  5. Vuelvo a correr el proceso de estadisticas
    *  6. Uno las dos estadisticas obtenidas en una sola
    */
   std::vector<DatasourceInterface*> selectedclasses;
   if (!pAdaptLayer_->GetAttribute<std::vector<DatasourceInterface*> >(
         ClassStatisticsProcess::SelectedClassesKeyAttr, selectedclasses))
      return false;
   bool trainresult = false, testresult = false;
   // se corre el proceso por cada clase para las areas de entrenamiento
   // y test
   std::vector<DatasourceInterface*>::iterator it = selectedclasses.begin();
   for (; it != selectedclasses.end(); ++it) {
      statisticsStep_ = 0;
      pCurrentEnclosure_ = (*it);
      Element* pelem = (*it)->GetElement();
      if (pelem) {
         statisticsStep_++;
         LayerList* ptrainlist = GetTrainningMaskList(pelem);
         if (ptrainlist) {
            trainresult = RunClassStatistics(ptrainlist);
         }
         statisticsStep_++;
         LayerList* ptestlist = GetTestMaskList(pelem);
         /** TODO(Gabriel - TCK #6583): Fix para que cuando se solo se calculen
          *  las areas de test en el reporte html
         **/
         if (ptestlist && output_ == StatisticsExporterProcess::HtmlPart) {
            testresult = RunClassStatistics(ptestlist);
         }
         // Si no pudo calcular ninguna estadistica sale
         if (!trainresult && !testresult)
            break;
      }

   }
   return trainresult || testresult;
}

/** Corre el proceso enmascarando con la layerlist que se pasa por parametro **/
bool ClassStatisticsProcess::RunClassStatistics(LayerList* pMaskList) {
   pMaskList->SetNoDataValue(ClassInformation::InvalidClassIndex);
   pMaskList->SetNoDataValueAvailable(true);
   pAdaptLayer_->AddAttribute<LayerList*>(ProcessAdaptLayer::MaskListKeyAttr,
                                          pMaskList);
   return RasterProcess::RunProcess();
}

/** A partir de un elemento seleccionados obtiene las areas de
 *  entrenamiento de cada capa y arma una lista con las mascaras correspondientes
 */
LayerList* ClassStatisticsProcess::GetTrainningMaskList(Element* pElement) {
   // TODO(Gabriel - TCK #4040): Crear una lista aplicando el filtro de areas de entrenamiento
   // Genero una lista con la clase seleccionada
   Element* pnewelem = GenerateMask(pElement);
   if (!pnewelem)
      return NULL;
   LayerList* player = new LayerList;
   player->AddElement(pnewelem);
   return player;
}

/** A partir de una elemento obtiene las areas de
 *  test de cada capa y arma una lista con las mascaras correspondientes
 */
LayerList* ClassStatisticsProcess::GetTestMaskList(Element* pElement) {
   // TODO(Gabriel - TCK #4040): Crear una lista aplicando el filtro de areas de test
   Element* pnewelem = GenerateMask(pElement);
   if (!pnewelem)
      return NULL;
   LayerList* player = new LayerList;
   player->AddElement(pnewelem);
   return player;
}


/** Metodo que genera un mascara a partir de la fuente de datos que se pasa por parametro
 *  Precondicion: pElement tiene que ser no nulo
 *  @param[in] pElement fuente de datos sobre la cual se quiere generar una mascara
 *  @return VectorElement* nueva instancia de elemento vectorial de tipo mascara**/
Element* ClassStatisticsProcess::GenerateMask(Element* pElement) {
   EnclosureManipulator manipulator;
   EnclosureInformation enclosureinfo = manipulator.
                        GetEnclosureInformation(pCurrentEnclosure_);
   /** Se verifica que el reciento sea del mismo tipo que se esta buscando **/
   if ((enclosureinfo.GetEnclosureType() == EnclosureInformation::Train &&
      TestStatisticsStep == statisticsStep_) ||
      (enclosureinfo.GetEnclosureType() == EnclosureInformation::Test &&
      TestStatisticsStep != statisticsStep_))
      return NULL;
   VectorElement* pnewelem = dynamic_cast<VectorElement*>(Element::Create(
         "VectorElement", pElement->GetUrl().c_str()));
   if (!pElement)
      return NULL;

   std::string elemname = pElement->GetName().c_str();

   pnewelem->SetName(elemname.c_str());
   Filter filter(enclosureinfo.GetQuery());
   filter.SetFilteredName(elemname);
   if (TestStatisticsStep == statisticsStep_) {
      filter.RemoveEqualConstraint(0);
      filter.AddEqualConstraint(1);
      filter.SetIdFieldName(enclosureinfo.GetTypeAreaName());
   }
   FiltredVectorRenderer::SetFilter(&filter, pnewelem->GetNode());
   VectorStyleTable* pvectortable = NULL;
   pvectortable = VectorStyleManager::Instance().GetTable("mask", Vector::Polygon);
   // Se utiliza el estilo default de la tabla de mascaras
   std::string style = pvectortable->GetDefaultStyle()->GetWkt();
   pnewelem->SetStyle(style);
   pnewelem->Activate();
   return pnewelem;
}
/** A partir de la estadistica y las opciones genera el reporte**/
void ClassStatisticsProcess::CreateHtmlViewerPart(
      Statistics& Statistics, std::map<std::string, std::string>& Options) {
   if (!pStatisticsReport_) {
      pStatisticsReport_ = new HtmlReportWidget(_(caption_STATISTICS_REPORT));
      pReportWidget_ = pStatisticsReport_;
   }
   if (!pReport_) {
      pReport_ = new MultipleStatisticsReport;
      pReport_->SetOutputOptions(Options);
   }
   pStatisticsReport_->SetReport(pReport_);
   std::string title = GetStepTitle(statisticsStep_);
   title.append(" ");
   title.append(pCurrentEnclosure_->GetName().c_str());
   pReport_->AddStatistics(title, Statistics);
}

/** A partir de la estadistica y las opciones genera el reporte**/
void ClassStatisticsProcess::CreateHtmlViewerPart(
      suri::raster::data::StatisticsBase* pStatistics,
      suri::raster::data::HistogramBase* pHistogram,
      std::map<std::string, std::string>& Options) {
   if (!pStatisticsReport_) {
      pStatisticsReport_ = new HtmlReportWidget(_(caption_STATISTICS_REPORT));
      pReportWidget_ = pStatisticsReport_;
   }
   if (!pReport_) {
      pReport_ = new MultipleStatisticsReport;
      pReport_->SetOutputOptions(Options);
   }
   pStatisticsReport_->SetReport(pReport_);
   std::string title = GetStepTitle(statisticsStep_);
   title.append(" ");
   title.append(pCurrentEnclosure_->GetName().c_str());

   pReport_->AddStatistics(title, pStatistics, pHistogram);
}

/** A partir de la estadistica y las opciones genera el reporte**/
void ClassStatisticsProcess::CreateCsvFile(
      Statistics& Statistics, std::map<std::string, std::string>& Options) {
   if (!pCsvReport_) {
      pCsvReport_ = new CsvMultipleStatisticsReport();
      pCsvReport_->SetOutputOptions(Options);
   }
   std::string title = "{";
   title += pCurrentEnclosure_->GetName().c_str();
   title += "}\n";
   pCsvReport_->AddStatistics(title, Statistics);

   std::string filename;
   if (pAdaptLayer_) {
      pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::OutputFileNameKeyAttr,
                                              filename);
   }

   wxFile wxfile;
   wxfile.Create(filename.c_str(), true);
   if (!wxfile.Open(filename.c_str(), wxFile::write))
      return;
   wxfile.Write(pCsvReport_->GetContentAsText("csv").c_str());
}

/** A partir de la estadistica y las opciones genera el reporte**/
void ClassStatisticsProcess::CreateCsvFile(
      suri::raster::data::StatisticsBase* pStatistics,
      suri::raster::data::HistogramBase* pHistogram,
      std::map<std::string, std::string>& Options) {
   if (!pCsvReport_) {
      pCsvReport_ = new CsvMultipleStatisticsReport;
      pCsvReport_->SetOutputOptions(Options);
   }

   std::string title = "{";
   title += pCurrentEnclosure_->GetName().c_str();
   title += "}\n";
   pCsvReport_->AddStatistics(title, pStatistics, pHistogram);

   std::string filename;
   if (pAdaptLayer_) {
      pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::OutputFileNameKeyAttr,
                                              filename);
   }

   wxFile wxfile;
   wxfile.Create(filename.c_str(), true);
   if (!wxfile.Open(filename.c_str(), wxFile::write))
      return;
   wxfile.Write(pCsvReport_->GetContentAsText("csv").c_str());
}

/** Devuelve el titulo que corresponde al paso de calculo de estatdistica **/
std::string ClassStatisticsProcess::GetStepTitle(int Step) {
   std::string title;
   switch (Step) {
      case 1:
         title = html_TRAINNING_STATISTICS;
         break;
      case 2:
         title = html_TEST_STATISTICS;
         break;
      default:
         title = "UNKNOWN";
         break;
   }
   return title;
}
} /** namespace suri */
