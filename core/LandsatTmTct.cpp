/*! \file LandsatTmTct.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "LandsatTmTct.h"

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
   double coefs[6][6] = { {  0.3037,  0.2793,  0.4743,  0.5585,  0.5082,  0.1863 },
                          { -0.2848, -0.2435, -0.5436,  0.7243,  0.0840, -0.1800 },
                          {  0.1509,  0.1973,  0.3279,  0.3406, -0.7112, -0.4572 },
                          {  0.8832, -0.0819, -0.4580, -0.0032, -0.0563,  0.0130 },
                          {  0.0573, -0.0260,  0.0335, -0.1943,  0.4766, -0.8545 },
                          {  0.1238, -0.9038,  0.4041,  0.0573, -0.0261,  0.0240 } };

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

REGISTER_TASSELEDCAP_TRANSFORMATION(LandsatTmTct)

/**
 * Ctor.
 */
LandsatTmTct::LandsatTmTct(const std::vector<int>& OutBands) :
      TasseledCapTransformation(OutBands) {

}

/**
 * Dtor.
 */
LandsatTmTct::~LandsatTmTct() {

}

/**
 * Metodo para transformar los datos de entrada.
 */
bool LandsatTmTct::Transform(const std::string& InputDataType, std::vector<void*>& InputData,
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
int LandsatTmTct::GetRequiredInputBands() {
   return LANDSATTMTCT_REQUIRED_INPUT_BANDS;
}

/**
 * Crea una instancia de la transformacion tasseled-cap usando
 * coeficientes de LandSat-4 TM (ND).
 */
TasseledCapTransformation* LandsatTmTct::Create(suri::ParameterCollection& Params) {
   std::vector<int> outputbands;
   Params.GetValue<std::vector<int> >("OUTPUTBANDS", outputbands);

   return new LandsatTmTct(outputbands);
}

/**
 * Obtiene las bandas de salida con la descripcion para LandSat-4 TM (ND).
 */
std::map<int, std::string> LandsatTmTct::GetOutputBands() {
   std::map<int, std::string> bands;

   bands[0] = "TasseledCapBrightness";
   bands[1] = "TasseledCapGreenness";
   bands[2] = "TasseledCapYellowness";
   bands[3] = "TasseledCapFog";
   bands[4] = "TasseledCapFifth";
   bands[5] = "TasseledCapSixth";

   return bands;
}

}  // namespace transformation
}  // namespace raster
}  // namespace suri
