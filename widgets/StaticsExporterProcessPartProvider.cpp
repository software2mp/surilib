/*! \file StaticsExporterProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
