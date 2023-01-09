/*! \file IkonosTct.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef IKONOSTCT_H_
#define IKONOSTCT_H_

#include <map>
#include <string>

#include "TasseledCapTransformation.h"
#include"suri/ParameterCollection.h"

#define IKONOSTCT_REQUIRED_INPUT_BANDS    4

namespace suri {
namespace raster {
namespace transformation {

/**
 * Representa una transformacion para IKONOS.
 */
class IkonosTct : public TasseledCapTransformation {
public:
   /**
    * Ctor.
    */
   explicit IkonosTct(const std::vector<int>& OutBands);

   /**
    * Dtor.
    */
   virtual ~IkonosTct();

   /**
    * Metodo para transformar los datos de entrada.
    */
   virtual bool Transform(const std::string& InputDataType, std::vector<void*>& InputData,
                          int InputDataLength, std::vector<void*>& OutputData);

   /**
    * Devuelve la cantidad de bandas requeridas en la entrada.
    */
   virtual int GetRequiredInputBands();

   /**
    * Crea una instancia de la transformacion tasseled-cap usando
    * coeficientes de IKONOS.
    */
   static TasseledCapTransformation* Create(suri::ParameterCollection& Params);

   /**
    * Obtiene las bandas de salida con la descripcion para IKONOS.
    */
   static std::map<int, std::string> GetOutputBands();
};

}  // namespace transformation
}  // namespace raster
}  // namespace suri

#endif  // IKONOSTCT_H_
