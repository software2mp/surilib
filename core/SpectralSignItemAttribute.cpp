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
#include "suri/SpectralSignItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor */
SpectralSignItemAttribute::SpectralSignItemAttribute(const std::string& Name,
                                                     const std::string& Value) :
      ComplexItemAttribute(Name, Value) {
}

/** Destructor */
SpectralSignItemAttribute::~SpectralSignItemAttribute() {
}

/** Configura el valor de longitud de onda */
void SpectralSignItemAttribute::SetWavelengthValue(const double Value) {
   attributes_[WavelengthValuePosition]->SetValue(DoubleToString(Value));
}

/** Configura el valor de amplitud */
void SpectralSignItemAttribute::SetAmplitudeValue(const double Value) {
   attributes_[AmplitudeValuePosition]->SetValue(DoubleToString(Value));
}

/** Configura el valor de relevancia */
void SpectralSignItemAttribute::SetReflectanceValue(const double Value) {
   attributes_[RelevanceValuePosition]->SetValue(DoubleToString(Value));
}

/** Configura el tipo de longitud de onda */
void SpectralSignItemAttribute::SetWavelengthType(const std::string Value) {
   attributes_[WavelengthTypePosition]->SetValue(Value);
}

/** Configura el tipo de magnitud */
void SpectralSignItemAttribute::SetMagnitudeType(const std::string Value) {
   attributes_[MagnitudeTypePosition]->SetValue(Value);
}

/** Retorna el valor de longitud de onda */
double SpectralSignItemAttribute::GetWavelengthValue() const {
   return
         (attributes_.size() > 0) ? StringToNumber<double>(
                                          attributes_[WavelengthValuePosition]->GetValue()) :
                                    0;
}

/** Retorna el valor de amplitud */
double SpectralSignItemAttribute::GetAmplitudeValue() const {
   return
         (attributes_.size() > 1) ? StringToNumber<double>(
                                          attributes_[AmplitudeValuePosition]->GetValue()) :
                                    0;
}

/** Retorna el valor de relevancia */
double SpectralSignItemAttribute::GetReflectanceValue() const {
   return
         (attributes_.size() > 2) ? StringToNumber<double>(
                                          attributes_[RelevanceValuePosition]->GetValue()) :
                                    0;
}

/** Retorna el tipo de longitud de onda */
std::string SpectralSignItemAttribute::GetWavelengthType() const {
   return (attributes_.size() > 3) ? attributes_[WavelengthTypePosition]->GetValue() : "";
}

/** Retorna el tipo de magnitud */
std::string SpectralSignItemAttribute::GetMagnitudeType() const {
   return (attributes_.size() > 4) ? attributes_[MagnitudeTypePosition]->GetValue() : "";
}
} /** namespace suri */
