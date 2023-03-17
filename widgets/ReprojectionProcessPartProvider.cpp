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
#include <vector>
#include <string>

// Includes Suri
#include "ReprojectionProcessPartProvider.h"
#include "TransformationModelParametersPart.h"
#include "RasterOutputSizePart.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
ReprojectionProcessPartProvider::ReprojectionProcessPartProvider(
      ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
   pReprojectionParameters_ = NULL;

   if (pPal) {
      pPal->GetAttribute<bool>(ReprojectionProcess::ReprojectionKeyAtttr, reprojection_);
      pPal->GetAttribute<bool>(ReprojectionProcess::AskGcpFileAttr, askGcpFile_);
      pPal->GetAttribute<bool>(ReprojectionProcess::ShowResizePartAttr, showResizePart_);
      pPal->GetAttribute<ReprojectionProcess::ReprojectionParameters*>(
            ReprojectionProcess::ReprojectionParamsKeyAttr, pReprojectionParameters_);
   }
}

/** Destructor **/
ReprojectionProcessPartProvider::~ReprojectionProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool ReprojectionProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   SuriObject::UuidType libraryid = SuriObject::NullUuid;
   bool libfound = pPal_->GetAttribute<SuriObject::UuidType>(
         ProcessAdaptLayer::LibraryIdKeyAttr, libraryid);
   World* pworld = NULL;
   bool inworldfound = pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                                   pworld);
   World* poutputworld = NULL;
   bool outworldfound = pPal_->GetAttribute<World*>(
         ProcessAdaptLayer::OutputWorldKeyAttr, poutputworld);
   if (!datafound || !libfound || !inworldfound || !outworldfound)
      return false;

   TransformationModelParametersPart* ptransfpart = NULL;
   if (reprojection_) {
      int flags = !askGcpFile_ ? 
      TransformationModelParametersPart::Polynomial | TransformationModelParametersPart::Exact : 
      TransformationModelParametersPart::GcpFile | TransformationModelParametersPart::Polynomial;
      ptransfpart = new TransformationModelParametersPart(libraryid,
                                                          pdvmgr->GetLibraryManager(),
                                                          pReprojectionParameters_,
                                                          flags);
      ptransfpart->SetProcessLayer(pPal_);
      ProcessParts.push_back(ptransfpart);
   }

   bool bval = false;
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, bval);

   if (!FileExporterProcessPartProvider::GetParts(ProcessParts)) return false;

   for (int ix = 0, lenix = ProcessParts.size(); ix <lenix; ++ix) {
      ProcessAtributeProvider* pprov = dynamic_cast<ProcessAtributeProvider*>(ProcessParts[ix]);
      if (pprov != NULL && pprov->GetProcessLayer() == NULL) {
         pprov->SetProcessLayer(pPal_);
      }
   }
   if (showResizePart_){
	   RasterOutputSizePart* prasteroutpart = new RasterOutputSizePart(
	   		pReprojectionParameters_, pworld, poutputworld);
	   prasteroutpart->SetProcessLayer(pPal_);
	   if (ptransfpart != NULL)
	   	ptransfpart->Attach(prasteroutpart);
	   ProcessParts.insert(ProcessParts.end() - 1, prasteroutpart);
   }
   return true;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* ReprojectionProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   ReprojectionProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("ReprojectionProcess") == 0 || ProcessName.compare("GeoreferenceProcess") == 0)
      pprovider = new ReprojectionProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
