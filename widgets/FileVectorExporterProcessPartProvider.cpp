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
#include "FileVectorExporterProcessPartProvider.h"
#include "VectorFormatSelectionPart.h"
#include "TransformationModelParametersPart.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** ctor **/
FileVectorExporterProcessPartProvider::FileVectorExporterProcessPartProvider(
      ProcessAdaptLayer* pPal) :
      VectorProcessPartProvider(pPal) {
}

/** Destructor **/
FileVectorExporterProcessPartProvider::~FileVectorExporterProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool FileVectorExporterProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   bool ok = VectorProcessPartProvider::GetParts(ProcessParts);
   if (ok) {
      DataViewManager* pdvmgr = NULL;
      pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
      bool srslection = false;
      pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeTransformationParametersKeyAttr, srslection);
      if (srslection) {
         std::string spatianreferencein;
         pPal_->GetAttribute<std::string>(suri::TransformationFactory::kParamWktIn, spatianreferencein);
         std::string spatianreferenceout;
         pPal_->GetAttribute<std::string>(suri::TransformationFactory::kParamWktOut, spatianreferenceout);
         ReprojectionProcess::ReprojectionParameters* preprojprocess =
               new ReprojectionProcess::ReprojectionParameters;
         preprojprocess->srWktIn_ = spatianreferencein;
         preprojprocess->srWktOut_ = spatianreferenceout;
         SuriObject::UuidType libraryid = SuriObject::NullClassId;
         pPal_->GetAttribute<SuriObject::UuidType>(ProcessAdaptLayer::LibraryIdKeyAttr, libraryid);
         TransformationModelParametersPart* ptransf = 
                              new TransformationModelParametersPart(libraryid,
                              pdvmgr->GetLibraryManager(), 
                              preprojprocess,
                              TransformationModelParametersPart::Exact);
         ptransf->SetProcessLayer(pPal_);
         ProcessParts.push_back(ptransf);

      }
      VectorFormatSelectionPart* pformat = new VectorFormatSelectionPart();
      pformat->SetProcessLayer(pPal_);
      ProcessParts.push_back(pformat);
   }
   return ok;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* FileVectorExporterProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   FileVectorExporterProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("FileVectorExporterProcess") == 0)
      pprovider = new FileVectorExporterProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
