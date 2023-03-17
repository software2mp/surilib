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
#include "BandMathProcessPartProvider.h"
#include "suri/RasterElement.h"
#include "BandMathPart.h"
#include "SpectralSelectionPart.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
BandMathProcessPartProvider::BandMathProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Destructor **/
BandMathProcessPartProvider::~BandMathProcessPartProvider() {
}


/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool BandMathProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<std::string> bandnames;
   std::vector<Element*> inputelements;
      bool elemsfound = pPal_->GetAttribute< std::vector<Element*> >(
            ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   // Obtiene los nombres de las bandas de entrada
   RasterElement *praster = RasterElement::Create(inputelements[0]->GetUrl().c_str(), inputelements[0]->GetMetadataMap());
   if (!praster || !elemsfound)
      return false;     
   RasterElement* pinputraster = dynamic_cast<RasterElement*>(inputelements[0]);
   for (int i = 0; i < pinputraster->GetBandCount(); ++i) {
      std::string bname;
      pinputraster->GetBandName(bname, i);
      praster->SetBandName(bname, i);
      int valid = 1;
      pinputraster->GetBandValidity(valid, i);
      praster->SetBandValidity(valid, i);
   }

   int bandcount = praster->GetBandCount();
   std::string bandname;
   for (int i = 0; i < bandcount; i++) {
      praster->GetBandName(bandname, i);
      int valid = 1;
      praster->GetBandValidity(valid, i);
      if (valid != 0)
         bandnames.push_back(bandname);
   }
   delete praster;
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   BandMathPart* pband = new BandMathPart(bandnames, pdvmgr->GetLibraryManager());
   pband->SetProcessLayer(pPal_);
   ProcessParts.push_back(pband);
   bool includespectral = false;
   // Se indica que no es necesario el part de seleccion espectral
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, includespectral);

   bool ok = FileExporterProcessPartProvider::GetParts(ProcessParts);

   return ok;
}
/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* BandMathProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   BandMathProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("BandMathProcess") == 0)
      pprovider = new BandMathProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
