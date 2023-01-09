/*! \file QuickBirdTct.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUICKBIRDTCT_H_
#define QUICKBIRDTCT_H_

#include <map>
#include <string>

#include "TasseledCapTransformation.h"
#include"suri/ParameterCollection.h"

#define QUICKBIRDTCT_REQUIRED_INPUT_BANDS    4

namespace suri {
namespace raster {
namespace transformation {

/**
 * Representa una transformacion para QuickBird-2.
 */
class QuickBirdTct : public TasseledCapTransformation {
public:
   /**
    * Ctor.
    */
   explicit QuickBirdTct(const std::vector<int>& OutBands);

   /**
    * Dtor.
    */
   virtual ~QuickBirdTct();

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
    * coeficientes de QuickBird-2.
    */
   static TasseledCapTransformation* Create(suri::ParameterCollection& Params);

   /**
    * Obtiene las bandas de salida con la descripcion para QuickBird-2.
    */
   static std::map<int, std::string> GetOutputBands();
};

}  // namespace transformation
}  // namespace raster
}  // namespace suri

#endif /* QUICKBIRDTCT_H_ */
