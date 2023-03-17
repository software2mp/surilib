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
#include <math.h>
#include <string>
#include <vector>

// Includes Suri
#include "ClusterClassificationPart.h"
#include "suri/FactoryHelpers.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"

// Defines


/** Numero Pi */
#define PI 3.14159265

/** namespace suri */
namespace suri {

/**
 * Ctor.
 * \attention el color pasado por parametro es responsabilidad de la clase
 * que llama al ctor.
 */
ClusterClassificationPart::ClusterClassificationPart(
      ClusterClassificationAlgorithm* pClusterClassificationAlgorithm) :
      pAlgorithm_(pClusterClassificationAlgorithm) {
   pAlgorithm_->SetNoClassPixelValue(ClassInformation::NoClassIndex);
   pAlgorithm_->SetNDVPixelValue(ClassInformation::NDVIndex);
}

/** Dtor */
ClusterClassificationPart::~ClusterClassificationPart() {
   delete pAlgorithm_;
}


/**
 * Analiza si el part tiene datos validos.
 * @return bool que indica si los datos ingresados son validos
 */
bool ClusterClassificationPart::HasValidData() {
   return GetThresholdCtrlValue() >= 0;
}

// --- Metodos de Part ---
/**
 * Devuelve el estado del Part.
 * @return true si el part esta habilitado, false en otro caso
 */
bool ClusterClassificationPart::IsEnabled() {
   return enabled_;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return true si los datos de las propiedades son distintos a los del control
 *          wx. false si son iguales.
 */
bool ClusterClassificationPart::HasChanged() {
   // Verifico si se modifico el threshold
   VectorStyle::Color color = GetColourCtrlValue();
   return !FLOAT_COMPARE(pAlgorithm_->GetThreshold(), GetThresholdCtrlValue()) ||
               color_.red_ != color.red_ || color_.green_ != color.green_ ||
               color_.blue_ != color.blue_ || color_.alpha_ != color.alpha_;
}

/**
 * Salva los cambios realizados en el Part.
 * @return true siempre
 */
bool ClusterClassificationPart::CommitChanges() {
   pAlgorithm_->SetThreshold(GetThresholdCtrlValue());
   color_ = GetColourCtrlValue();
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return true siempre
 */
bool ClusterClassificationPart::RollbackChanges() {
   SetColourCtrlValue(color_);
   SetThresholdCtrlValue(pAlgorithm_->GetThreshold());
   return true;
}

/**
 * Actualiza el estado de la parte
 */
void ClusterClassificationPart::Update() {
}

// --- Metodos de ParametersSelectionPart ---
/**
 * Retorna un puntero al clasificador creado por part
 * @return puntero al algoritmo usado para clasificar
 * \attention El puntero que se retorna es propiedad de esta clase, no eliminar
 */
ClassificationAlgorithmInterface* ClusterClassificationPart::GetClassificationManager() {
   return pAlgorithm_;
}

/**
 * Retorna las propiedades de los pixels sin clase.
 * @param[out] Properties propiedades de los pixels no clasificados
 * @return bool que indica resultado de configuracion
 */
bool ClusterClassificationPart::GetNonClassifiedPixelInfo(
      ClassInformation &Properties) {
   ClassInformation classinformation(ClassInformation::NoClassIndex);
   classinformation.SetColor(color_);
   Properties = classinformation;
   return true;
}

}  // namespace


