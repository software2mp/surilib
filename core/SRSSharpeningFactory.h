/*! \file SRSSharpeningFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SRSSHARPENINGFACTORY_H_
#define SRSSHARPENINGFACTORY_H_

#include "SRSSharpening.h"
#include "suri/ParameterCollection.h"

#define REGISTER_SHARPENING(M_SName) \
namespace { \
class _M_Register_Sharpening_ { \
public: \
   _M_Register_Sharpening_() { \
      suri::raster::sharpening::SharpeningFactory::GetInstance()->Register(#M_SName, &M_SName::Create); \
   } \
}; \
_M_Register_Sharpening_ _m_register_Sharpening_; \
} \

namespace suri {
namespace raster {
namespace sharpening {

typedef Sharpening* (*CREATESHARPENINGCB)(suri::ParameterCollection& Params);

/**
 *
 */
class SharpeningFactory {
public:
   /**
    * Dtor (no es virtual porque no se deberia heredar de esta clase)
    */
   ~SharpeningFactory();

   /**
    * Devuelve la unica instancia.
    */
   static SharpeningFactory* GetInstance();

   /**
    * Crea una instancia para utilizar algun metodo para fusion de bandas
    * con pancromatica segun los parametros dados.
    */
   Sharpening* Create(const std::string& SName, suri::ParameterCollection& Params);

   /**
    * Registra un nuevo tipo metodo para fusion de bandas con pancromatica.
    */
   void Register(const std::string& SName, CREATESHARPENINGCB pCreateCb);

private:
   /**
    * Ctor (singleton)
    */
   SharpeningFactory();

   std::map<std::string, CREATESHARPENINGCB> factoryCbs_;
};

}  // namespace sharpening
}  // namespace raster
}  // namespace suri

#endif /* SRSSHARPENINGFACTORY_H_ */
