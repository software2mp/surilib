/*! \file StatisticsExporterWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef STATISTICSEXPORTERWIDGET_H_
#define STATISTICSEXPORTERWIDGET_H_

// Includes standard
#include <vector>
#include <string>
#include <map>

// Includes Suri
#include "RasterProcess.h"
#include "suri/StatisticsFunctions.h"
#include "SRStatisticsCanvas.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"

// Includea wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// ! namespace suri
namespace suri {
// forward
class StatsCanvas;
class Element;
class HtmlReportWidget;
class DataViewManager;
class CsvMultipleStatisticsReport;

/** Clase que genera estadisticas a partir de una imagen */
/**
 * Widget que hereda de ExporterWidget y agrega (a las partes de seleccion de
 * subset espacial y espectral ) un part para seleccionar las estadisticas a 
 * realizar.
 * Despues de ejecutar el proceso muestra el resultado por pantalla usando HTML.
 */
class StatisticsExporterProcess : public RasterProcess {
   /** Ctor. de copia */
   StatisticsExporterProcess(const StatisticsExporterProcess &StatisticsExporterWidget);

public:
   /** Enum con los tipos de salida del reporte de estadisticas */
   enum OutputType {HtmlPart, CsvFile};
   /** Ctor. */
   StatisticsExporterProcess(Element *pElement, const Subset &WindowSubset,
                             DataViewManager* pDataViewManager,
                             Widget* &pStaticsWidget,
                             StatisticsExporterProcess::OutputType output);
   /** Dtor. */
   virtual ~StatisticsExporterProcess();
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /** Clave para atributo de seleccion de estadisticas */
   static const std::string SelectedStadisticsKeyAttr;
   /** Clave para atributo de opciones de salida de estadisticas */
   static const std::string StadisticsOutParamsKeyAttr;
   /** Clave que indica si se trata de una estadistica sobre clases */
   static const std::string ClassClasificationKeyAttr;
   static const std::string ClassClasificationExporterKeyAttr;
   /** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
   virtual bool ConfigureProcessFromXmlString(const std::string& XmlStr);

protected:   
   /** Metodo que obtiene los parametros asociados al proceso en formato XML. 
    * Esto permite que el codigo cliente pueda hacer eso del xml resultante 
    * (ej. para la exportacion del mismo)
    * @return string en formato xml con los parametros asociados al proceso
    * @return string vacio en caso contrario. 
   **/
   virtual std::string GetParametersAsXmlString()const;

   // ------ Metodos que administran la ejecucion del proceso ------
   /** Configura el proceso para que se calculen las estadisticas configuradas */
   virtual bool ConfigureProcess();

   /** Muestra las estadisticas por pantalla */
   virtual bool ConfigureOutput();

   /* estadistica */
   suri::render::StatisticsCanvas* pStatsCanvas_;

   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateHtmlViewerPart(suri::raster::data::StatisticsBase* pStatistics,
                                     suri::raster::data::HistogramBase* pHistogram,
                                     std::map<std::string, std::string>& Options);

   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateCsvFile(suri::raster::data::StatisticsBase* pStatistics,
                              suri::raster::data::HistogramBase* pHistogram,
                              std::map<std::string, std::string>& Options){};

   virtual void ConfigureOutputOptions(std::map<std::string, std::string>* pOptions);

   /** Widget que muestra las estadisticas. */
   HtmlReportWidget* pStatisticsReport_;

   /** Puntero al widget que se pasa desde afuera.**/
   Widget* &pReportWidget_;

   /** estadisticas **/
   suri::raster::data::StatisticsBase* pStats_;
   suri::raster::data::HistogramBase* pHistogram_;

   /** Reporte que concatena estadistica en un mismo reporte **/
   CsvMultipleStatisticsReport* pCsvReport_;
   StatisticsExporterProcess::OutputType output_;

private:
   /** Permite obtener la fuente de datos a partir  */
   DataViewManager* pDataViewManager_;
};
}  // suri
#endif /* STATISTICSEXPORTERWIDGET_H_ */
