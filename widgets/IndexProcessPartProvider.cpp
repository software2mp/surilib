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
#include "IndexProcessPartProvider.h"
#include "suri/RasterElement.h"
#include "IndexSelectionPart.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor **/
IndexProcessPartProvider::IndexProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Dtor **/
IndexProcessPartProvider::~IndexProcessPartProvider() {
}
/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool IndexProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<Element*> inputelements;
   bool includespectral = false;
   // Se indica que no es necesario el part de seleccion espectral
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, includespectral);
   bool elemsfound = pPal_->GetAttribute< std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   if (!elemsfound)
      return false;
   // Crea un elemento con el Filename de elemento recibido
   RasterElement *praster = RasterElement::Create(inputelements[0]->GetUrl().c_str(),
                                                  inputelements[0]->GetMetadataMap());
   if (!praster)
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
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   IndexSelectionPart* pindex = new IndexSelectionPart(praster, pdvmgr ? pdvmgr->GetLibraryManager() : NULL);
   pindex->SetProcessLayer(pPal_);
   ProcessParts.push_back(pindex);
   delete praster;
   return FileExporterProcessPartProvider::GetParts(ProcessParts);
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* IndexProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   IndexProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("IndexProcess") == 0)
      pprovider = new IndexProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
