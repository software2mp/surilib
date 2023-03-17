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

#ifndef STATISTICPARAMETERSELECTIONPART_H_
#define STATISTICPARAMETERSELECTIONPART_H_

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "suri/SuriObject.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/StatisticsFunctions.h"
#include "suri/Part.h"

namespace suri {
// Forwards
class DatasourceManagerInterface;

/** Permite seleccionar los parametros estadisticos a calcular. */
/** Contiene un panel con checks, con los distintos parametros
 * estadisticos a calcular.
 * Los parametros que se presentan son:
 *  - Parametros  basicos
 *  - Parametros entre bandas
 *  - Histograma
 * Opciones:
 *  - Calcular areas
 */
class StatisticParameterSelectionPart : public suri::Part, public suri::ProcessAtributeProvider {
   /** Nombre del XRC que representa la ventana. */
   static const std::string STATISTIC_PARAMETER_SELECTION_XRC_NAME;

public:
   /** Ctor. Carga el XRC de la pantalla */
   StatisticParameterSelectionPart(DatasourceManagerInterface* pDatasourceManager,
                                           SuriObject::UuidType DatasourceId,
														 bool EnableBasicParams = true);
   /** Dtor. */
   virtual ~StatisticParameterSelectionPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Devuelve si la parte contiene datos validos. */
   bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update();

   /** Retorna las estadisticas seleccionadas en el ultimo commit*/
   Statistics::StatisticsFlag GetSelectedStatistics();
   /** Retorna el raster generado con el url que recibe el constructor */
   std::map<std::string, std::string> GetOutputOptions();

private:
   /** Actualiza los atributos widget con datos en control grafico */
   bool ReadParametersFromWidget();
   /** Actualiza el control grafico con datos en los atributos widget */
   bool UpdateParametersToWidget();

   bool widgetBasicStatistics_; /*! Flag para estadisticas basicas */
   bool widgetCrossBandStatistics_; /*! Flag para estadisticas entre bandas */
   bool widgetHistogram_; /*! Flag para histograma */
   bool widgetShowAsArea_; /*! Estado de flag 'mostrar como area' en pantalla */

   bool basicStatistics_; /*! Flag para estadisticas basicas */
   bool crossBandStatistics_; /*! Flag para estadisticas entre bandas */
   bool histogram_; /*! Flag para histograma */
   bool showAsArea_; /*! Estado de flag 'mostrar como areas' de ultimo commit */

   /** Permite obtener el datasource de la capa que quiero clasificar. */
   DatasourceManagerInterface* pDatasourceManager_;
   /** Id del datasource que quiero clasificar. */
   SuriObject::UuidType datasourceId_;
	bool enableBasicParams_;
};
}

#endif /* STATISTICPARAMETERSELECTIONPART_H_ */
