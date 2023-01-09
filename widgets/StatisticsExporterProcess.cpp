/*! \file StatisticsExporterWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2010-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <map>
#include <string>
#include <vector>
#include <utility>

// Includes standard
#include <algorithm>

// Includes Suri
#include "suri/HtmlViewerPart.h"
#include "suri/StatisticsExporterProcess.h"
#include "MovingWindowController.h"
#include "suri/Element.h"
#include "suri/messages.h"
#include "StatsCanvas.h"
#include "StatisticParameterSelectionPart.h"
#include "HtmlReportWidget.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DataViewManager.h"
#include "StatisticsReport.h"
#include "SRHistogramCanvas.h"
#include "CsvMultipleStatisticsReport.h"
#include "wx/file.h"
#include "suri/RasterElement.h"

// Includes wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

namespace {

void GetNoDataValue(suri::Element* pElement, bool& Available, double& NoDataValue) {
   Available = false;
   NoDataValue = 0.0;
   wxXmlNode* pdatanotvalidvalue = pElement->GetNode(
   PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue) {
      pdatanotvalidvalue = pdatanotvalidvalue->GetChildren();
      if (pdatanotvalidvalue->GetType() == wxXML_TEXT_NODE) {
         wxString ctt = pdatanotvalidvalue->GetContent();
         Available = true;
         NoDataValue = suri::StringToNumber<double>(ctt.mb_str());
      }
   }
}

}  // namespace anonimo

// namespace suri
namespace suri {

/** Nombre del proceso **/
const std::string StatisticsExporterProcess::kProcessName = "StatisticsExporterProcess";
/** Clave para atributo de seleccion de estadisticas */
const std::string StatisticsExporterProcess::SelectedStadisticsKeyAttr =
      "StadisticsParams";
/** Clave para atributo de opciones de salida de estadisticas */
const std::string StatisticsExporterProcess::StadisticsOutParamsKeyAttr = "OutputParams";
/** Clave que indica si se trata de una estadistica sobre clases */
const std::string StatisticsExporterProcess::ClassClasificationKeyAttr = "ClassClasif";

const std::string StatisticsExporterProcess::
                  ClassClasificationExporterKeyAttr = "ClassClasifExport";


/**
 * Constructor
 * @param[in] pInputElement puntero al elemento seleccionado de la lista
 * @param[in] ViewerSubset subset sobre el cual se calculan las estadisticas
 * @param[in] pStaticsWidget widget en el cual se debe mostrar el resultado
 */
StatisticsExporterProcess::StatisticsExporterProcess(
      Element *pInputElement, const Subset &ViewerSubset,
      DataViewManager* pDataViewManager, Widget* &pStaticsWidget,
      StatisticsExporterProcess::OutputType output) :
      RasterProcess(pInputElement, ViewerSubset, pDataViewManager), pStatsCanvas_(NULL),
      pStatisticsReport_(NULL), pReportWidget_(pStaticsWidget), pStats_(NULL),
      pHistogram_(NULL), pCsvReport_(NULL), output_(output),
      pDataViewManager_(pDataViewManager) {
   SetProcessName(kProcessName);
   pRenderizationObject_ = new MovingWindowController();
}

/**
 * Destructor
 */
StatisticsExporterProcess::~StatisticsExporterProcess() {
}

/**
 * Configura el proceso para que se calculen las estadisticas configuradas
 * @return siempre true
 */
bool StatisticsExporterProcess::ConfigureProcess() {
   pStatsCanvas_ = new suri::render::StatisticsCanvas(true);

   bool available = false;
   double nodatavalue = 0.0;

   GetNoDataValue(inputElements_[0], available, nodatavalue);

   pStatsCanvas_->SetNoDataValueAvailable(available);
   pStatsCanvas_->SetNoDataValue(nodatavalue);

   pRenderizationObject_->SetOutputCanvas(pStatsCanvas_);

   MovingWindowController* paux = dynamic_cast<MovingWindowController*>(pRenderizationObject_);
   if (paux)
      paux->SetBestBufferSize();

   return true;
}

/**
 * Obtiene las estadisticas del canvas y las muestra por pantalla.
 * @return bool que indica si pudo mostrar el resultado
 */
bool StatisticsExporterProcess::ConfigureOutput() {
   pStats_ = pStatsCanvas_->GetStatistics();

   // Si no me pidieron que calcule media, varianza, max y min las elimino
   // de estadisticas para que no se muestren en html.
   Statistics::StatisticsFlag statics = Statistics::None;
   pAdaptLayer_->GetAttribute<Statistics::StatisticsFlag>(SelectedStadisticsKeyAttr,
                                                          statics);
   if (pStats_ && (statics & Statistics::Histogram)) {
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

   std::string strstatics = NumberToString<int>(statics);
   options.insert(std::pair<std::string, std::string>("stats_flags", strstatics));

   // Creo el reporte solicitado que despliega estadisticas
   if (output_ == StatisticsExporterProcess::HtmlPart) {
      CreateHtmlViewerPart(pStats_, pHistogram_, options);
   } else {
      CreateCsvFile(pStats_, pHistogram_, options);
   }
   return true;
}

/** Configura las opciones **/
void StatisticsExporterProcess::ConfigureOutputOptions(
      std::map<std::string, std::string>* pOptions) {
   (*pOptions)["url"] = GetInputElement()->GetUrl().c_str();
   std::vector<int> selectedbands = GetSelectedRasterBands();
#ifdef __UNUSED_CODE__
   // Se cambia el indice de la banda seleccionada por el nombre
   (*pOptions)["selectedbands"] = join<std::vector<int>::iterator>(selectedbands.begin(),
                                                               selectedbands.end());
#endif
   std::string bandnames, name;
   RasterElement* praster = dynamic_cast<RasterElement*>(GetInputElement());
   for (size_t i = 0; i < selectedbands.size(); ++i) {
      praster->GetBandName(name, i);
      if (i > 0)
         bandnames += "|";
      bandnames += name;
   }
   (*pOptions)["selectedbands"] = bandnames;
   std::string extent = "";
   Subset window;
   // TODO(Gabriel - TCK #2315): Las clases hijas no deberian usar el World
   // directamente tendrian que usar el mundo de salida?
//   pInputWorld_->GetWindow(window);
   World* pworld = (pOutputWorld_)? pOutputWorld_ : pInputWorld_;
   pworld->GetWindow(window);
   // TODO(Gabriel - TCK #2315): Ver si se deberia hacer lo mismo que world con
   // el raster model (tener uno de salida y uno de entrada)
   RasterSpatialModel* prastermodel = GetOutputRasterSpatialModel();
   prastermodel->InverseTransform(window.lr_);
   prastermodel->InverseTransform(window.ul_);
   extent = extent + LongToString(SURI_ROUND(long,
         std::min(window.ul_.x_, window.lr_.x_))) + " " + LongToString(SURI_ROUND(long,
         std::min(window.ul_.y_, window.lr_.y_))) + " " + LongToString(SURI_ROUND(long,
         std::max(window.ul_.x_, window.lr_.x_))) + " " + LongToString(SURI_ROUND(long,
         std::max(window.ul_.y_, window.lr_.y_)));
   (*pOptions)["extent"] = extent;
}

/** A partir de la estadistica y las opciones genera el reporte**/
void StatisticsExporterProcess::CreateHtmlViewerPart(
                                 suri::raster::data::StatisticsBase* pStatistics,
                                 suri::raster::data::HistogramBase* pHistogram,
                                 std::map<std::string, std::string>& Options) {
   if (!pStatisticsReport_) {
      pStatisticsReport_ = new HtmlReportWidget(_(caption_STATISTICS_REPORT));
      pReportWidget_ = pStatisticsReport_;
   }

   Statistics::StatisticsFlag flags = Statistics::None;
   pAdaptLayer_->GetAttribute<Statistics::StatisticsFlag>(
         StatisticsExporterProcess::SelectedStadisticsKeyAttr, flags);

   StatisticsReport* preport = new StatisticsReport();
   preport->SetStatistics(pStatistics, pHistogram);
   preport->SetOutputOptions(Options);
   preport->SetFlags(flags);
   pStatisticsReport_->SetReport(preport);
}

/** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
bool StatisticsExporterProcess::ConfigureProcessFromXmlString(const std::string& XmlStr) {
   return false;
}

/** Metodo que obtiene los parametros asociados al proceso en formato XML. 
 * Esto permite que el codigo cliente pueda hacer eso del xml resultante 
 * (ej. para la exportacion del mismo)
 * @return string en formato xml con los parametros asociados al proceso
 * @return string vacio en caso contrario. 
**/
std::string StatisticsExporterProcess::GetParametersAsXmlString() const {
   return "";
}
}  // namespace

