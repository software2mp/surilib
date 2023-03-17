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

#include "SubsetCoordinateInputPart.h"

// Includes standard
// Includes Suri
// Includes Wx
// Defines

/** namespace suri */
namespace suri {
const std::string SubsetCoordinateInputPart::SUBSET_COORDINATE_XRC_NAME =
      wxT("ID_SUBSET_COORD_INPUT_PANEL");

SubsetCoordinateInputPart::SubsetCoordinateInputPart() :
      PartCollection(SUBSET_COORDINATE_XRC_NAME) {
   pULDegreeCorner_ = new LatitudeLongitudeInputPart();
   pLRDegreeCorner_ = new LatitudeLongitudeInputPart();
   pULDecimalCorner_ = new DecimalCoordinateInputPart();
   pLRDecimalCorner_ = new DecimalCoordinateInputPart();
   AddPart(pULDegreeCorner_, wxT("ID_UL_DEGREE_COORD_PANEL"));
   AddPart(pLRDegreeCorner_, wxT("ID_LR_DEGREE_COORD_PANEL"));
   AddPart(pULDecimalCorner_, wxT("ID_UL_DECIMAL_COORD_PANEL"));
   AddPart(pLRDecimalCorner_, wxT("ID_LR_DECIMAL_COORD_PANEL"));
}

SubsetCoordinateInputPart::~SubsetCoordinateInputPart() {
}

bool SubsetCoordinateInputPart::SetInternalSubset(const Subset &SubsetIn) {
   // seteo coordenadas en controles.
   if (!pLRDegreeCorner_->SetCoordinate(SubsetIn.lr_)
         || !pULDegreeCorner_->SetCoordinate(SubsetIn.ul_)) {
      return false;
   }

   return true;
}

/** Indica si el Part tiene cambios para salvar. */
bool SubsetCoordinateInputPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool SubsetCoordinateInputPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool SubsetCoordinateInputPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void SubsetCoordinateInputPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void SubsetCoordinateInputPart::Update() {
}

/** Permite a clases que hereden modificar el widget luego de crear la ventana */
bool SubsetCoordinateInputPart::ConfigureWidget() {
   PartCollection::ConfigureWidget();

   return true;
}
}
