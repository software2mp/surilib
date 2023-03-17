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

// Includes standard
// Includes Suri
#include "BandEnhancementPart.h"
// Includes Wx,
// Includes App
// Defines

namespace suri {
const std::string BandEnhancementPart::BAND_ENHANCEMENT_XRC_NAME =
      wxT("ID_BAND_ENHANCEMENT_PANEL"); /*! Id del panel */
/** Ctor */
BandEnhancementPart::BandEnhancementPart(RasterElement *pElement) :
      PartCollection(BandEnhancementPart::BAND_ENHANCEMENT_XRC_NAME,
                     wxT(caption_BAND_ENHANCEMENT_PART)) {
   Part *penhancement = new EnhancementSelectionPart(pElement);

   Part *rasteradjustment = new RasterAdjustmentsPart();

   AddPart(penhancement, wxT("ID_BAND_ENHANCEMENT_TABLE_PANEL"));
   AddPart(rasteradjustment, wxT("ID_BAND_ENHANCEMENT_ADJUSTMENTS_PANEL"));
}

/** Dtor */
BandEnhancementPart::~BandEnhancementPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool BandEnhancementPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool BandEnhancementPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool BandEnhancementPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void BandEnhancementPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void BandEnhancementPart::Update() {
}
}  // namespace suri
