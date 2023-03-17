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
// Includes Suri
#include "VectorOperationProcessPartProvider.h"
#include "suri/DataViewManager.h"
#include "suri/World.h"
#include "VectorOperationPart.h"
#include "BufferPart.h"
#include "MergeLayersPart.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
VectorOperationProcessPartProvider::VectorOperationProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileVectorExporterProcessPartProvider(pPal) {
}

/** Destructor **/
VectorOperationProcessPartProvider::~VectorOperationProcessPartProvider() {
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* VectorOperationProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   VectorOperationProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("VectorOperationProcess") == 0 ||
         ProcessName.compare("VectorBufferProcess") == 0 ||
         ProcessName.compare("MergeLayersProcess") == 0)
      pprovider = new VectorOperationProcessPartProvider(pPal);
   return pprovider;
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool VectorOperationProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<DatasourceInterface*> inputdatasources;
   pPal_->GetAttribute<std::vector<DatasourceInterface*> >(
         ProcessAdaptLayer::VectorOperationDatasourcesKeyAttr, inputdatasources);
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   World* pinputworld = NULL;
   bool worldfound = pPal_->GetAttribute<World*>(
         ProcessAdaptLayer::InputWorldKeyAttr, pinputworld);
   if (!datafound || !worldfound)
      return false;
   bool showvectoroperation = true;
   if (pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeVectorOperationKeyAttr,
                                 showvectoroperation) && showvectoroperation) {
      VectorOperationPart* psubset = new VectorOperationPart();
      psubset->SetProcessLayer(pPal_);
      ProcessParts.push_back(psubset);
   } else if (pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeBufferKeyAttr,
                                 showvectoroperation) && showvectoroperation) {
      BufferPart* pbufferpart = new BufferPart(pdvmgr, pinputworld);
      pbufferpart->SetProcessLayer(pPal_);
      ProcessParts.push_back(pbufferpart);
   } else if (pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeMergeLayersKeyAttr,
                                        showvectoroperation) && showvectoroperation) {
      MergeLayersPart* pmergerpat = new MergeLayersPart(inputdatasources);
      pmergerpat->SetProcessLayer(pPal_);
      ProcessParts.push_back(pmergerpat);
   }

   return  FileVectorExporterProcessPartProvider::GetParts(ProcessParts);
}

} /** namespace suri */
