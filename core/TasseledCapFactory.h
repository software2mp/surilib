/*! \file TasseledCapFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASSELEDCAPFACTORY_H_
#define TASSELEDCAPFACTORY_H_

#include <vector>

#include "TasseledCapTransformation.h"
#include "suri/ParameterCollection.h"

#define REGISTER_TASSELEDCAP_TRANSFORMATION(M_TCTName) \
namespace { \
class _M_Register_TasseledCap_Tranformation_ { \
public: \
   _M_Register_TasseledCap_Tranformation_() { \
      suri::raster::transformation::TasseledCapFactory::GetInstance() \
         ->Register(#M_TCTName, &M_TCTName::Create, &M_TCTName::GetOutputBands); \
   } \
}; \
_M_Register_TasseledCap_Tranformation_ _m_register_tasseledcap_tranformation_; \
} \

namespace suri {
namespace raster {
namespace transformation {

typedef TasseledCapTransformation* (*CREATETCTRANSFORMATIONCB)(suri::ParameterCollection& Params);
typedef std::map<int, std::string> (*GETOUTPUTBANDSCB)();

class TasseledCapFactory {
public:
   /**
    * Dtor (no es virtual porque no se deberia heredar de esta clase).
    */
   ~TasseledCapFactory();

   /**
    * Devuelve la unica instancia.
    */
   static TasseledCapFactory* GetInstance();

   /**
    * Crea una instancia para utilizar algun metodo de transformacion tasseled-cap.
    */
   TasseledCapTransformation* Create(const std::string& TCTName, suri::ParameterCollection& Params);

   /**
    * Registra un nuevo tipo metodo para transformacion tasseled-cap.
    */
   void Register(const std::string& TCTName, CREATETCTRANSFORMATIONCB pCreateCb, GETOUTPUTBANDSCB pOutputCb);

   /**
    * Obtiene los nombres de los tipos de transformaciones tasseled-cap disponibles.
    */
   std::vector<std::string> GetAvailableTCTransformations();

   /**
    * Obtiene las bandas de salida para cada transformacion soportada.
    */
   std::map<int, std::string> GetOutputBandsForTCTransformation(const std::string& TCTName);

private:
   /**
    * Ctor (singleton).
    */
   TasseledCapFactory();

   std::map<std::string, CREATETCTRANSFORMATIONCB> factoryCbs_;
   std::map<std::string, GETOUTPUTBANDSCB> outputCbs_;
};

}  // namespace transformation
}  // namespace raster
}  // namespace suri

#endif  // TASSELEDCAPFACTORY_H_
