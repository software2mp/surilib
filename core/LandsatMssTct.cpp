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

#include "LandsatMssTct.h"

#include "TasseledCapFactory.h"

#include "suri/DataTypes.h"

namespace {

typedef void (*FTRANSFORM)(std::vector<void*>& InputData, int InputDataLength,
                           std::vector<void*>& OutputData, const std::vector<int>& OutBands);

/**
 * Funcion para transformar los valores.
 */
template<typename T>
void TransformData(std::vector<void*>& InputData, int InputDataLength,
                   std::vector<void*>& OutputData, const std::vector<int>& OutBands) {
   double coefs[4][5] = { {  0.4330,  0.6320,  0.5860,  0.2640, 32.0 },
                          { -0.2900, -0.5620,  0.6000,  0.4910, 32.0 },
                          { -0.8290,  0.5220, -0.0390,  0.1940, 32.0 },
                          {  0.2230,  0.0120, -0.5430,  0.8100, 32.0 } };

   for (int outix = 0, outlen = OutBands.size(); outix < outlen; ++outix) {
      float* poutput = static_cast<float*>(OutputData[outix]);
      for (int inix = 0; inix < InputDataLength; ++inix) {
         poutput[inix] =
                 coefs[OutBands[outix]][0] * static_cast<T*>(InputData[0])[inix]
               + coefs[OutBands[outix]][1] * static_cast<T*>(InputData[1])[inix]
               + coefs[OutBands[outix]][2] * static_cast<T*>(InputData[2])[inix]
               + coefs[OutBands[outix]][3] * static_cast<T*>(InputData[3])[inix]
               + coefs[OutBands[outix]][4];
      }
   }
}

}  // namespace anonimo

namespace suri {
namespace raster {
namespace transformation {

REGISTER_TASSELEDCAP_TRANSFORMATION(LandsatMssTct)

/**
 * Ctor.
 */
LandsatMssTct::LandsatMssTct(const std::vector<int>& OutBands) :
      TasseledCapTransformation(OutBands) {

}

/**
 * Dtor.
 */
LandsatMssTct::~LandsatMssTct() {

}

/**
 * Metodo para transformar los datos de entrada.
 */
bool LandsatMssTct::Transform(const std::string& InputDataType, std::vector<void*>& InputData,
                              int InputDataLength, std::vector<void*>& OutputData) {
   FTRANSFORM ftransform = NULL;

   if (InputDataType.compare(suri::DataInfo<unsigned char>::Name) == 0) {
      ftransform = &TransformData<unsigned char>;
   } else if (InputDataType.compare(suri::DataInfo<unsigned short>::Name) == 0) {
      ftransform = &TransformData<unsigned short>;
   } else if (InputDataType.compare(suri::DataInfo<short>::Name) == 0) {
      ftransform = &TransformData<short>;
   } else if (InputDataType.compare(suri::DataInfo<unsigned int>::Name) == 0) {
      ftransform = &TransformData<unsigned int>;
   } else if (InputDataType.compare(suri::DataInfo<int>::Name) == 0) {
      ftransform = &TransformData<int>;
   } else if (InputDataType.compare(suri::DataInfo<float>::Name) == 0) {
      ftransform = &TransformData<float>;
   } else if (InputDataType.compare(suri::DataInfo<double>::Name) == 0) {
      ftransform = &TransformData<double>;
   }

   if (ftransform != NULL)
      ftransform(InputData, InputDataLength, OutputData, outBands_);

   return true;
}

/**
 * Devuelve la cantidad de bandas requeridas en la entrada.
 */
int LandsatMssTct::GetRequiredInputBands() {
   return LANDSATMSSTCT_REQUIRED_INPUT_BANDS;
}

/**
 * Crea una instancia de la transformacion tasseled-cap usando
 * coeficientes de LandSat-1_3 MSS.
 */
TasseledCapTransformation* LandsatMssTct::Create(suri::ParameterCollection& Params) {
   std::vector<int> outputbands;
   Params.GetValue<std::vector<int> >("OUTPUTBANDS", outputbands);

   return new LandsatMssTct(outputbands);
}

/**
 * Obtiene las bandas de salida con la descripcion para LandSat-1_3 MSS.
 */
std::map<int, std::string> LandsatMssTct::GetOutputBands() {
   std::map<int, std::string> bands;

   bands[0] = "TasseledCapBrightness";
   bands[1] = "TasseledCapGreenness";
   bands[2] = "TasseledCapYellowness";
   bands[3] = "TasseledCapIncomparable";

   return bands;
}

}  // namespace transformation
}  // namespace raster
}  // namespace suri
