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

#ifndef CONFUSSIONMATRIXPROCESS_H_
#define CONFUSSIONMATRIXPROCESS_H_

// Includes Estandar
#include <vector>
#include <string>
#include <map>

// Includes Suri
#include "suri/StatisticsExporterProcess.h"
#include "ConfusionMatrixPart.h"
#include "ConfusionMatrixReport.h"

// Includes Wx

// Defines

namespace suri {

// forwards
class DatasourceManagerInterface;
class DataViewManager;

class ConfussionMatrixProcess : public suri::StatisticsExporterProcess {
   ConfussionMatrixProcess(const ConfussionMatrixProcess&);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   ConfussionMatrixProcess(Element *pElement, const Subset &WindowSubset,
                           DataViewManager* pDataViewManager,
                           Widget* &pStaticsWidget, StatisticsExporterProcess::OutputType output);
   virtual ~ConfussionMatrixProcess();
   // ------ Metodos que administran la ejecucion del proceso ------
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess();
   /** Atributo para la seleccion de matriz de confusion */
   static const std::string TestResultMapKeyAttr;

protected:
   /** Metodo que especializa la configuracion del ProcessAdaptLayer para
    *  los procesos especificos
    */
   void DoOnProcessAdaptLayerSet();
   // ------ Metodos que administran la ejecucion del proceso ------
   /** Muestra las estadisticas por pantalla */
   virtual bool ConfigureOutput();
   /** Devuelve una lista con el elemento que se quiere utilizar como mascara.*/
   virtual LayerList *GetMaskList();
   /** Configura las opciones requeridas para mostrar matriz confusion */
   virtual void ConfigureOutputOptions(std::map<std::string, std::string>* pOptions);
   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateHtmlViewerPart(Statistics& Statistics,
                                     std::map<std::string, std::string>& Options);
   /** A partir de la estadistica y las opciones genera el reporte**/
   virtual void CreateHtmlViewerPart(suri::raster::data::StatisticsBase* pStatistics,
                                     suri::raster::data::HistogramBase* pHistogram,
                                     std::map<std::string, std::string>& Options);

private:
   Element* pElement_;
   DatasourceManagerInterface* pDatasourceManager_;
   DatasourceInterface* enclosureDatasource_; /**! datasource del recinto a enmascarar */
   Statistics trainningStatistics_; /**! Estadisticas con areas de entrenamiento */
   Statistics testStatistics_; /**! Estadisticas con areas de test */
   bool showStatistics_;
   /** Reporte que muestra la matriz de confusion en html **/
   ConfusionMatrixReport* pReport_;
   std::vector< std::map<std::string, DatasourceInterface*> > resultMap_;

};

} /** namespace suri */

#endif /* CONFUSSIONMATRIXPROCESS_H_ */
