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

// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "StaticsExporterProcessPartProvider.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DataViewManager.h"
#include "StatisticParameterSelectionPart.h"
#include "suri/StatisticsExporterProcess.h"
#include "ClassSelectionPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor */
StaticsExporterProcessPartProvider::StaticsExporterProcessPartProvider(
      ProcessAdaptLayer* pPal) :
      RasterProcessPartProvider(pPal) {
}

/** Destructor **/
StaticsExporterProcessPartProvider::~StaticsExporterProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool StaticsExporterProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   if (!datafound)
      return false;
   std::vector<Element*> inputelements;
   bool elemsfound = pPal_->GetAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   DatasourceManagerInterface* pdsmgr = pdvmgr->GetDatasourceManager();
   DatasourceInterface* pdatasource = pdsmgr->GetDatasourceByElementId(
         inputelements[0]->GetUid());
   if (!pdatasource || !elemsfound)
      return false;
	bool classclasif = false;
   pPal_->GetAttribute<bool>(StatisticsExporterProcess::ClassClasificationKeyAttr,
                             classclasif);
   StatisticParameterSelectionPart* pstatic = new StatisticParameterSelectionPart(
         pdsmgr, pdatasource->GetId(), !classclasif);
   pstatic->SetProcessLayer(pPal_);
   ProcessParts.push_back(pstatic);
   bool classstatsexport = false;
   pPal_->GetAttribute<bool>(
         StatisticsExporterProcess::ClassClasificationExporterKeyAttr,
         classstatsexport);
   if (classclasif || classstatsexport) {
         int flags = ClassSelectionPart::SelectionButtonFlag |ClassSelectionPart::ClearSelectionButtunFlag |
                     ClassSelectionPart::TestClassFilter | ClassSelectionPart::TrainClassFilter;
   	   ClassSelectionPart* pclasssel = new ClassSelectionPart(pdvmgr, flags);
	      pclasssel->SetProcessLayer(pPal_);
	      ProcessParts.push_back(pclasssel);
   }
   return RasterProcessPartProvider::GetParts(ProcessParts);
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* StaticsExporterProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   StaticsExporterProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("StatisticsExporterProcess") == 0)
      pprovider = new StaticsExporterProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
