/*! \file LandsatEtmTct.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "LandsatEtmTct.h"

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
   double coefs[6][6] = { {  0.3561,  0.3972,  0.3904,  0.6966,  0.2286,  0.1596 },
                          { -0.3344, -0.3544, -0.4556,  0.6966, -0.0242, -0.2630 },
                          {  0.2626,  0.2141,  0.0926,  0.0656, -0.7629, -0.5388 },
                          {  0.0805, -0.0498,  0.1950, -0.1327,  0.5752, -0.7775 },
                          { -0.7252, -0.0202,  0.6683,  0.0631, -0.1494, -0.0274 },
                          {  0.4000, -0.8172,  0.3832,  0.0602, -0.1095,  0.0985 } };

   for (int outix = 0, outlen = OutBands.size(); outix < outlen; ++outix) {
      float* poutput = static_cast<float*>(OutputData[outix]);
      for (int inix = 0; inix < InputDataLength; ++inix) {
         poutput[inix] =
                 coefs[OutBands[outix]][0] * static_cast<T*>(InputData[0])[inix]
               + coefs[OutBands[outix]][1] * static_cast<T*>(InputData[1])[inix]
               + coefs[OutBands[outix]][2] * static_cast<T*>(InputData[2])[inix]
               + coefs[OutBands[outix]][3] * static_cast<T*>(InputData[3])[inix]
               + coefs[OutBands[outix]][4] * static_cast<T*>(InputData[4])[inix]
               + coefs[OutBands[outix]][5] * static_cast<T*>(InputData[5])[inix];
      }
   }
}

}  // namespace anonimo

namespace suri {
namespace raster {
namespace transformation {

REGISTER_TASSELEDCAP_TRANSFORMATION(LandsatEtmTct)

/**
 * Ctor.
 */
LandsatEtmTct::LandsatEtmTct(const std::vector<int>& OutBands) :
      TasseledCapTransformation(OutBands) {

}

/**
 * Dtor.
 */
LandsatEtmTct::~LandsatEtmTct() {

}

/**
 * Metodo para transformar los datos de entrada.
 */
bool LandsatEtmTct::Transform(const std::string& InputDataType, std::vector<void*>& InputData,
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
int LandsatEtmTct::GetRequiredInputBands() {
   return LANDSATETMTCT_REQUIRED_INPUT_BANDS;
}

/**
 * Crea una instancia de la transformacion tasseled-cap usando
 * coeficientes de Landsat ETM.
 */
TasseledCapTransformation* LandsatEtmTct::Create(suri::ParameterCollection& Params) {
   std::vector<int> outputbands;
   Params.GetValue<std::vector<int> >("OUTPUTBANDS", outputbands);

   return new LandsatEtmTct(outputbands);
}

/**
 * Obtiene las bandas de salida con la descripcion para Landsat ETM.
 */
std::map<int, std::string> LandsatEtmTct::GetOutputBands() {
   std::map<int, std::string> bands;

   bands[0] = "TasseledCapBrightness";
   bands[1] = "TasseledCapGreenness";
   bands[2] = "TasseledCapHumidity";
   bands[3] = "TasseledCapFourth";
   bands[4] = "TasseledCapFifth";
   bands[5] = "TasseledCapSixth";

   return bands;
}

}  // namespace transformation
}  // namespace raster
}  // namespace suri
