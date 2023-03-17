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

#ifndef CLASSSTATISTICSPROCESS_H_
#define CLASSSTATISTICSPROCESS_H_

// Includes Estandar
// Includes Suri
#include "suri/StatisticsExporterProcess.h"
#include "MultipleStatisticsReport.h"
#include "CsvMultipleStatisticsReport.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase encargada de realizar el calculo de estadisticas
 *  utilizando las clases seleccionadas durante el proceso
 */
class ClassStatisticsProcess : public suri::StatisticsExporterProcess {
   /** ctor copia**/
   ClassStatisticsProcess(const ClassStatisticsProcess&);

public:
   /** Clave para identificar a las clases seleccionadas durante el proceso **/
   static const std::string SelectedClassesKeyAttr;
   /** ctor **/
   ClassStatisticsProcess(Element *pElement, const Subset &WindowSubset,
                          DataViewManager* pDataViewManager, Widget* &pStaticsWidget,
                          StatisticsExporterProcess::OutputType output);
   /** dtor **/
   virtual ~ClassStatisticsProcess();
   // ------ Metodos que administran la ejecucion del proceso ------
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess();

   
private:
   /** numero del paso para areas de entrenamiento **/
   static const int TranningStatisticsStep;
   /** numero del paso para areas de test **/
   static const int TestStatisticsStep;
   /** Configura el proceso para que se calculen las estadisticas configuradas */
   virtual bool ConfigureProcess();
   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateHtmlViewerPart(Statistics& Statistics,
                             std::map<std::string, std::string>& Options);
   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateHtmlViewerPart(suri::raster::data::StatisticsBase* pStatistics,
                                     suri::raster::data::HistogramBase* pHistogram,
                                     std::map<std::string, std::string>& Options);
   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateCsvFile(Statistics& Statistics,
                              std::map<std::string, std::string>& Options);
   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateCsvFile(suri::raster::data::StatisticsBase* pStatistics,
                              suri::raster::data::HistogramBase* pHistogram,
                              std::map<std::string, std::string>& Options);
   /** A partir de un elemento seleccionados obtiene las areas de
    *  entrenamiento de cada capa y arma una lista con las mascaras correspondientes
    */
   LayerList* GetTrainningMaskList(Element* pElement);
   /** A partir de una elemento obtiene las areas de
    *  test de cada capa y arma una lista con las mascaras correspondientes
    */
   LayerList* GetTestMaskList(Element* pElement);
   /** Devuelve el titulo que corresponde al paso de calculo de estatdistica **/
   std::string GetStepTitle(int Step);
   /** Metodo que genera un mascara a partir de la fuente de datos que se pasa por parametro
    *  Precondicion: pElement tiene que ser no nulo
    *  @param[in] pElement fuente de datos sobre la cual se quiere generar una mascara
    *  @return VectorElement* nueva instancia de elemento vectorial de tipo mascara**/
   Element* GenerateMask(Element* pElement);
   /** Corre el proceso enmascarando con la layerlist que se pasa por parametro **/
   bool RunClassStatistics(LayerList* pMaskList);
   /** entero que indica en que paso del calculo de estadisticas se encuentra **/
   int statisticsStep_;
   /** Reporte que concatena estadistica en un mismo reporte **/
   MultipleStatisticsReport* pReport_;
   /** Clase sobre la cual se encuentra calculando estadistica **/
   DatasourceInterface* pCurrentEnclosure_;
   /** Reporte que concatena estadistica en un mismo reporte **/
   CsvMultipleStatisticsReport* pCsvReport_;
};

} /** namespace suri */

#endif /* CLASSSTATISTICSPROCESS_H_ */
