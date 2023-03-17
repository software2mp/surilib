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

#include <string>

#include "SharpeningProcessPartProvider.h"
#include "suri/Part.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/ProcessPartProviderInterface.h"
#include "suri/RasterElement.h"
#include "ImageFormatSelectionPart.h"
#include "SharpeningInputElementsPart.h"
#include "SharpeningInputPanPart.h"
#include "SharpeningInputRGBPart.h"

namespace suri { class Element; }

namespace suri {

/**
 * Ctor.
 */
SharpeningProcessPartProvider::SharpeningProcessPartProvider(ProcessAdaptLayer* pPal) :
      ProcessPartProviderInterface(pPal) {
}

/**
 * Dtor.
 */
SharpeningProcessPartProvider::~SharpeningProcessPartProvider() {

}

/**
 * Obtiene los parts necesarios para la configuracion visual del proceso.
 */
bool SharpeningProcessPartProvider::GetParts(std::vector<Part*>& ProcessParts) {
   SharpeningInputRGBPart* plowrespart = new SharpeningInputRGBPart(NULL);
   plowrespart->SetProcessLayer(pPal_);
   ProcessParts.push_back(plowrespart);

   SharpeningInputPanPart* phighrespart = new SharpeningInputPanPart(NULL);
   phighrespart->SetProcessLayer(pPal_);
   ProcessParts.push_back(phighrespart);

   // Archivo de salida
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
ProcessPartProviderInterface* SharpeningProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   SharpeningProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("SharpeningProcess") == 0) pprovider =
         new SharpeningProcessPartProvider(pPal);
   return pprovider;
}

}  // namespace suri
