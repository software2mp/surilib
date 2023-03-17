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
#include "ClassificationProcessPartProvider.h"
#include "ClassificationPart.h"
#include "ParametricClassificationPart.h"
#include "KMeansPart.h"
#include "ClassFussionParametersPart.h"
#include "suri/ClassificationProcess.h"
#include "ClassificationGetter.h"
#include "ThresholdClassSelectionPart.h"
#include "SpectralAngleMapperPart.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor **/
ClassificationProcessPartProvider::ClassificationProcessPartProvider(
      ProcessAdaptLayer* pPal) : FileExporterProcessPartProvider(pPal) {
   if (pPal) {
      pPal->GetAttribute<std::string>(ClassificationProcess::ClassificationType,
                                      classificationType_);
   }
}

/** Dtor **/
ClassificationProcessPartProvider::~ClassificationProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool ClassificationProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   ClassificationPart* pclasifpart = GetClassificationPart();
   if (!pclasifpart)
      return false;
   ClassificationGetter* pgetter = pclasifpart;
   pPal_->AddAttribute<ClassificationGetter*>(
         ClassificationProcess::ClassGetterAttrKey, pgetter);
   pclasifpart->SetProcessLayer(pPal_);
   if (!FileExporterProcessPartProvider::GetParts(ProcessParts))
      return false;
   std::vector<Part*>::iterator it = ProcessParts.begin();
   ProcessParts.insert(it, pclasifpart);
   return true;
}

/** En funcion de los parametros de entrada selecciona el part de clasificacion acorde**/
ClassificationPart* ClassificationProcessPartProvider::GetClassificationPart() {
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   std::vector<Element*> inputelements;
   bool elemsfound = pPal_->GetAttribute< std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   LayerList* playerlist = NULL;
   bool listfound = pPal_->GetAttribute<LayerList*>(
         ProcessAdaptLayer::InputElementsKeyAttr, playerlist);

   if (!datafound || !elemsfound || !listfound)
      return NULL;
   ClassificationPart* pclasifpart = NULL;
   if (classificationType_ == "ParametricClassificationPart") {
      pclasifpart = new ParametricClassificationPart(
            playerlist, inputelements[0], pdvmgr,
            ParametricClassificationPart::Supervised);
   } else if (classificationType_ == "KMeansPart") {
      // Clasificacion no supervisada correspondiente a la version 1.1.0
      pclasifpart = new KMeansPart(inputelements[0]);
   } else if (classificationType_ == "ClassFussionParametersPart") {
      pclasifpart = new ClassFussionParametersPart(inputelements[0]);
   } else if (classificationType_ == "ThresholdClassification") {
      pclasifpart = new ThresholdClassSelectionPart(pdvmgr, inputelements[0]);
   } else if (classificationType_ == "SpectralAngleMapper") {
      pclasifpart = new SpectralAngleMapperPart(pdvmgr->GetLibraryManager());
   }
   return pclasifpart;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* ClassificationProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   ClassificationProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("ClassificationProcess") == 0 ||
         ProcessName.compare("KMeansClassificationProcess") == 0)
      pprovider = new ClassificationProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
