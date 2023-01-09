/*! \file TasseledCapTransformation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASSELEDCAPTRANSFORMATION_H_
#define TASSELEDCAPTRANSFORMATION_H_

#include <string>
#include <vector>

namespace suri {
namespace raster {
namespace transformation {

class TasseledCapTransformation {
public:
   /**
    * Ctor.
    */
   explicit TasseledCapTransformation(const std::vector<int>& OutBands);

   /**
    * Dtor.
    */
   virtual ~TasseledCapTransformation();

   /**
    * Metodo para transformar los datos de entrada.
    */
   virtual bool Transform(const std::string& InputDataType, std::vector<void*>& InputData,
                          int InputDataLength, std::vector<void*>& OutputData) = 0;

   /**
    * Devuelve la cantidad de bandas requeridas en la entrada.
    */
   virtual int GetRequiredInputBands() = 0;

   /**
    * Devuelve la cantidad de bandas de salida.
    */
   int GetOutputBandCount();

protected:
   std::vector<int> outBands_;
};

}  // namespace transformation
}  // namespace raster
}  // namespace suri

#endif  // TASSELEDCAPTRANSFORMATION_H_
