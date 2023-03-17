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

#include "TasseledCapTransformationPartProvider.h"

#include "ImageFormatSelectionPart.h"
#include "MaskSelectionPart.h"
#include "SpatialSubsetSelectionPart.h"
#include "TasseledCapTransformationPart.h"
#include "suri/RasterElement.h"
#include "suri/World.h"

namespace suri {

/**
 * Ctor.
 */
TasseledCapTransformationPartProvider::TasseledCapTransformationPartProvider(
      ProcessAdaptLayer* pPal) : ProcessPartProviderInterface(pPal) {

}

/**
 * Dtor.
 */
TasseledCapTransformationPartProvider::~TasseledCapTransformationPartProvider() {

}

/**
 * Obtiene los parts necesarios para la configuracion visual del proceso.
 */
bool TasseledCapTransformationPartProvider::GetParts(std::vector<Part*>& ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr,
                                         pdvmgr);

   World* pworld = NULL;
   pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld);

   Element* pelement = NULL;
   pPal_->GetAttribute<Element*>(ProcessAdaptLayer::TasseledCapInputKeyAttr, pelement);

   RasterElement* praster = dynamic_cast<RasterElement*>(pelement);

   TasseledCapTransformationPart* ptasseledcap = new TasseledCapTransformationPart(praster);
   ptasseledcap->SetProcessLayer(pPal_);
   ProcessParts.push_back(ptasseledcap);

   // Seleccion espacial.
   SpatialSubsetSelectionPart* psubset = new SpatialSubsetSelectionPart(pdvmgr, pworld);
   psubset->SetProcessLayer(pPal_);
   ProcessParts.push_back(psubset);

   // Mascara.
   MaskSelectionPart* pmask = new MaskSelectionPart(pdvmgr, pelement);
   pmask->SetProcessLayer(pPal_);
   ProcessParts.push_back(pmask);

   // Archivo de salida.
   ImageFormatSelectionPart* pformat = new ImageFormatSelectionPart();
   pformat->SetEnabled(true);
   pformat->SetProcessLayer(pPal_);
   ProcessParts.push_back(pformat);

   return true;
}

/**
 * Metodo para la creacion de un Part provider.
 *
 * @return instancia del creador correspondiente o NULL en caso contrario.
 */
ProcessPartProviderInterface* TasseledCapTransformationPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   TasseledCapTransformationPartProvider* pprovider = NULL;
   if (ProcessName.compare("TasseledCapProcess") == 0) pprovider =
         new TasseledCapTransformationPartProvider(pPal);
   return pprovider;
}

} /** namespace suri */
